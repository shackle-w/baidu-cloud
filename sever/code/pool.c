/*
 * 文件名：pool.h
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件是有关线程池实现的文件
*/

#include "pool.h"

/*
 * 函数名：initPool
 * 函数作用：初始化线程池
 * 参数：
 *  -pool_t *pool：有关线程池的结构体
 *  -int nums：线程的个数
 * 返回值：成功0，失败-1
*/
int initPool(pool_t *pool, int nums){
    // 给子线程id开辟空间
    pool->threadIds = (pthread_t *)calloc(nums, sizeof(pthread_t));
    if(pool->threadIds == NULL){
        LOG(ERROR, "calloc为子线程开辟空间失败");
        return -1;
    }

    pool->threadNum = nums;

    bzero(&pool->queue, sizeof(queue_t));

    if(pthread_mutex_init(&pool->pool_lock, NULL) != 0){
        LOG(ERROR, "初始化线程锁失败");
        return -1;
    }
    if(pthread_cond_init(&pool->cond, NULL) != 0){
        LOG(ERROR, "初始化线程条件变量失败");
        return -1;
    }

    pool->exitFlag = 0;
    pool->commandTag = 0;
    // 创建线程：
    for(int i = 0; i < nums; i++){
        if(pthread_create(&pool->threadIds[i], NULL, threadMain, pool) != 0){
            LOG(ERROR, "创建子线程失败");
            return -1;
        }

        //LOG(INFO, "创建线程：%ld", pool->threadIds[i]);
    }

    return 0;
}


/*
 * 函数名：threadMain(void *p);
 * 描述：定义线程的入口函数
 * 参数：
 *  -void *p：一个指针类型，用于
 * 返回值：
 */
void *threadMain(void *p){
    pool_t *pPool = (pool_t *)p;

    while(1){
        int net_fd;
        pthread_mutex_lock(&pPool->pool_lock);

        while(pPool->queue.size <= 0 && pPool->exitFlag == 0){
            pthread_cond_wait(&pPool->cond, &pPool->pool_lock);
        }

        //pthread_t thread_id = pthread_self();
        //LOG(INFO, "本次为您服务的进程id：%ld", thread_id);
        //printf("Thread ID：%ld\n", thread_id);
        if(pPool->exitFlag == 1){
            
            printf("线程主动退出：\n");
            pthread_mutex_unlock(&pPool->pool_lock);
            pthread_exit(NULL);
        }

        net_fd = pPool->queue.head->net_fd;
        
        deQueue(&pPool->queue);

        pthread_mutex_unlock(&pPool->pool_lock);
        
        while(1){  // 该层循环为客户端提供多次登录注册服务
            int tag = 0;
            user_t user;
            if(pPool->commandTag == 1){
                // 提供注册服务
                int res = registerAccount(net_fd, &user, pPool->sql);               
                if(res == -1){
                    LOG(ERROR, "注册失败");
                    recv(net_fd, &tag, sizeof(int), MSG_WAITALL);
                    pPool->commandTag = tag;
                    break;
                }
                // 接受下一条指令类型
                if(recv(net_fd, &tag, sizeof(int), MSG_WAITALL) == -1){
                    LOG(WARNING, "客户端退出");
                    goto end;
                }
                pPool->commandTag = tag;
            }else if(pPool->commandTag == 2){
                // 提供登录服务
                int res = loginID(net_fd, &user, pPool->sql);
                if(res == -1){
                    LOG(ERROR, "登录失败");
                    recv(net_fd, &tag, sizeof(int), MSG_WAITALL);
                    pPool->commandTag = tag;
                    break;
                }
                // 接受下一条指令类型
                if(recv(net_fd, &tag, sizeof(int), MSG_WAITALL) == -1){
                    LOG(WARNING, "客户端退出");
                    goto end;
                }
                pPool->commandTag = tag;
            }else if(pPool->commandTag == 3){
                // 为客户端提供短命令的服务
                int res = dealShort(net_fd, &user, pPool->sql);
                if(res == -1){
                    LOG(ERROR, "处理命令失败");
                    recv(net_fd, &tag, sizeof(int), MSG_WAITALL);
                    pPool->commandTag = tag;
                    break;
                }
                // 接受下一条指令类型
                if(recv(net_fd, &tag, sizeof(int), MSG_WAITALL) == -1){
                    // 客户端没有信息发来，退出
                    LOG(WARNING, "客户端退出");
                    goto end;
                }
                pPool->commandTag = tag;
            }else{    
                // TODO；为线程提供上传或下载服务
                // 1. 接受命令切分出是上传还是下载服务
                int len = 0;
                recv(net_fd, &len, sizeof(int), MSG_WAITALL);
                recv(net_fd, &user, len, MSG_WAITALL);
                char command[SMALL_BUF] = {0};
                memcpy(command, user.command, SMALL_BUF);
                
                // 2.切分命令，选择不同的逻辑为客户端服务
                char *tmp = strtok(command, " ");
                if(strcmp(tmp, "gets") == 0){
                    // TODO：给客户端传文件
                    int res = getsFile(net_fd, &user, pPool->sql);
                    if(res == -1){
                        LOG(ERROR, "客户端下载文件失败or中断");
                        goto end;
                    }
                    goto end;
                }else{
                    // TODO：接受客户端传来的文件
                    int res = putsFile(net_fd, &user, pPool->sql);
                    if(res == -1){
                        LOG(ERROR, "客户端上传文件失败or中断");
                        goto end;
                    }
                    goto end;
                }

                // 退出条件：上传以及下载文件成功，或者客户端断开连接
            }
        }
        // 何时关闭这个net_fd，只有客户端主动退出，以及文件上传成功，下载文件成功
end:
        close(net_fd);
        continue;
    }

    return NULL;
}
