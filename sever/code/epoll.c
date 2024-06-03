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
