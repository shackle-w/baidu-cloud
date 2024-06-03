/*
 * 文件名：epoll.h
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件是处理epoll监听的接口文件
*/
#ifndef __EPOLLH__
#define __EPOLLH__

#include "my_header.h"
#include "log.h"
#include "tools.h"
#include "command.h"
#include "pool.h"
#include "receive.h"

/*
 * 函数名：epoll_addfd
 * 描述：在epoll中添加要监听的文件描述符
 * 参数
 *  -int epollfd：epollfd的文件描述符
 *  -int filefd：要监听的文件描述符
 * 返回值：若成功，返回0，失败，返回-1
 */
int epollAddfd(int epollfd, int filefd);

/*
 * 函数名：hadleEpollEvents
 * 描述：对epoll监听进行封装
 * 参数：
 *  -int epoll_fd：epoll的文件描述符
 *  -int socket_fd：连接的socket
 *  -user_t *user：用户的信息
 * 返回值：若成功，返回0，失败，返回-1
*/
int handleEpollEvents(int epoll_fd, int socket_fd, user_t *user);



#endif
