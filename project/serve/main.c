#include "head.h"

int pipeFd[2];
void func(int num){
    write(pipeFd[1], "1", 1);
}

int main(int argc,char*argv[])
{
    // -----日志调用示例
    LOG(LOG_TYPE_ERROR, "error 日志测试");
    LOG(LOG_TYPE_WARNING, "warning 日志测试");
    LOG(LOG_TYPE_INFO, "info 日志测试");
    // -----


    LOG(LOG_TYPE_INFO, "服务启动");
    pipe(pipeFd);
    if(fork() != 0){
        signal(SIGINT, func);
        wait(NULL);
        LOG(LOG_TYPE_INFO, "程序停止运行");
        exit(0);
    }
    setpgid(0, 0);

    pool_t pool;
    initPool(&pool);

    int socketFd;
    initTcp(&socketFd, get(pool.map, "ip"), get(pool.map, "port"));

    int epollFd = epoll_create(1);
    epollAdd(epollFd, socketFd);
    epollAdd(epollFd, pipeFd[0]);


    while(1){
        struct epoll_event redy[10];
        int epollNum = epoll_wait(epollFd, redy, 10, 1000);

        for(int i=0; i<epollNum; i++){
            if(redy[i].data.fd == pipeFd[0]){
                // 说明: 有信号产生 -> 程序退出
                char buf[60] = {0};
                read(pipeFd[0], buf, sizeof(buf));

                pthread_mutex_lock(&pool.threadLock);
                pool.exitTag = 1;
                pthread_cond_broadcast(&pool.cond );
                pthread_mutex_unlock(&pool.threadLock);

                for(int k=0; k<pool.poolThreadNum; k++){
                    pthread_join(pool.poolThreadIds[k], NULL);
                }
                exit(0);

            }else if(redy[i].data.fd == socketFd){
                // 有新连接进来
                int netFd = accept(socketFd, NULL, NULL);
                epollAdd(epollFd, netFd);
                // 加入循环的时间队列
                insetConnect(netFd, &pool.connsStatus);
                LOG(LOG_TYPE_INFO, "新连接进来");
            }else{
                // 客户端请求进来
                int res = doRequest(redy[i].data.fd, &pool); 
                if(res == PUTS_SYNC || res == GETS_SYNC){
                    // 如果是文件上传, 会交给子进程处理, 没有必要在main中继续监听这个文件上传的链接了
                    epollRemove(epollFd, redy[i].data.fd);
                    // 把此net_fd移除循环时间队列
                    deleteConnect(redy[i].data.fd, &pool.connsStatus );
                    continue;
                }else if(res == -1){
                    LOG(LOG_TYPE_INFO, "客户端断开");
                    epollRemove(epollFd, redy[i].data.fd);
                    close(redy[i].data.fd);
                    // 把此net_fd移除循环时间队列
                    deleteConnect(redy[i].data.fd, &pool.connsStatus );
                    continue;
                }
                updateConnect(redy[i].data.fd, &pool.connsStatus );
            }
        }
        // 判断超时踢出
        checkConnect(epollFd, &pool.connsStatus );
    }
    return 0;
}

