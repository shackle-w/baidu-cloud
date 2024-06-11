/*
 * 文件名：epoll.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件的作用是实现epoll监听的接口函数
*/

#ifndef __EPOLLH__
#define __EPOLLH__

#include "my_header.h"

/*
 * 函数名：epollAdd
 * 函数作用：加入一个epoll监听
 * 函数参数：
 *  int epollFd：监听的epollFd集合
 *  int netFd：要加入监听的文件描述符
 * 函数返回值：成功返回0
*/
int epollAdd(int epollFd, int netFd);

/*
 * 函数名：epollRemove
 * 函数作用：删除一个epoll监听
 * 函数参数：
 *  int epollFd：监听的epollFd集合
 *  int netFd：要删除监听的文件描述符
 * 函数返回值：成功返回0
*/
int epollRemove(int epollFd, int netFd);
#endif
