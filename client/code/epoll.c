/*
 * 文件名：epoll.c
 * 创建时间：2024.5.30
 * 作者：shackle
 * 描述：该文件的作用是在epoll内加入监听
 */

#include "epoll.h"

/*
 * 函数名：epoll_addfd
 * 描述：在epoll中添加要监听的文件描述符
 * 参数
 *  -int epollfd：epollfd的文件描述符
 *  -int filefd：要监听的文件描述符
 * 返回值：若成功，返回0，失败，返回-1
 */
int epollAddfd(int epollfd, int filefd){
    struct epoll_event event;
    event.data.fd = filefd;
    event.events = EPOLLIN|EPOLLET;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, filefd, &event)!= 0){
        LOG(ERROR, "向监听树中加入文件描述符失败");
        return -1;
    }

    return 0;
}

/*
 * 函数名：hadleEpollEvents
 * 描述：对epoll监听进行封装
 * 参数：
 *  -int epoll_fd：epoll的文件描述符
 *  -int socket_fd：连接的socket
 *  -user_t *user：用户的信息
 * 返回值：若成功，返回0，失败，返回-1
*/
int handleEpollEvents(int epoll_fd, int socket_fd, user_t *user){
    struct epoll_event list[1024];
    int nums = epoll_wait(epoll_fd, list, 1024, -1);
    for(int i = 0; i < nums; i++){
        if(list[i].data.fd == STDIN_FILENO){
            char tmp[BUF_SIZE] = {0};
            char command[BUF_SIZE] = {0};
            fgets(command, sizeof(command), stdin);
            command[strcspn(command, "\n")] = '\0';
            memcpy(tmp, command, sizeof(tmp));
            
            int tag = shardCommand(command);
            LOG(INFO, "信号的类型：%d", tag);
            memcpy(user->command, tmp, sizeof(tmp));
            LOG(INFO, "客户端需要处理的命令：%s", user->command);
            
            if(tag == 3){
                int res = shortCommand(socket_fd, user);
                ERROR_CHECK(res, -1, "shortCommand");
            } else if(tag == 4){
                // 切分出是上传命令还是下载命令
                if(strcmp(command, "puts") == 0){
                    // TODO:上传命令
                    pthread_t tid;
                    int *result;
                    if(pthread_create(&tid, NULL, threadMain, user) != 0){
                        ERROR_CHECK(tag, 4, "pthread_create");
                    }
                    if(pthread_join(tid, (void **)&result) != 0){
                        fprintf(stderr, "线程执行失败");
                    }
                }else{
                    // TODO：下载命令
                    pthread_t tid1;
                    int *result;
                    if(pthread_create(&tid1, NULL, threadMain, user) != 0){
                        ERROR_CHECK(tag, 4, "pthread_create");
                    }
                    if(pthread_join(tid1, (void **)&result) != 0){
                        fprintf(stderr, "线程执行失败");
                    }
                }

            }
        }
        if(list[i].data.fd == socket_fd){
            printf("短命令socket响应到来：\n");
            int res_receive = dealReceive(socket_fd, user);
            ERROR_CHECK(res_receive, -1, "receiver");
        }
    }
}

