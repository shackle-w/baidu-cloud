/*
 * 文件名：epoll.c
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件的作用是实现epoll监听
*/

#include "epoll.h"

int epollAdd(int epollFd, int netFd){

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = netFd;

    epoll_ctl(epollFd, EPOLL_CTL_ADD, netFd, &event);

    return 0;
}


int epollRemove(int epollFd, int netFd){

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = netFd;

    epoll_ctl(epollFd, EPOLL_CTL_DEL, netFd, &event);

    return 0;
}
