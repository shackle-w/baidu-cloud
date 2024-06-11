/*
 * 文件名：timeout.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现超时重传的接口函数
*/

#ifndef __TIMEOUTH__
#define __TIMEOUTH__

#include "my_header.h"
#include "severStruct.h"
#include "log.h"
#include "epoll.h"

/*
 * 函数名：insetConnect
 * 函数作用：新链接到来，加入记录
 * 函数参数：
 *  int netFd：连接的socket
 *  ConnectsStatus *connsStatus：维护的连接结构体
 * 函数返回值：成功返回0
*/
int insetConnect(int netFd,  ConnectsStatus * connsStatus);

/*
 * 函数名：updataConnect
 * 函数作用：更新链接的信息
 * 函数参数：
 *  int netFd：连接的socket
 *  ConnectsStatus *connsStatus：维护的连接结构体
 * 函数返回值：成功返回0
*/
int updateConnect(int netFd, ConnectsStatus * connsStatus);

/*
 * 函数名：deleteConnect
 * 函数作用：移除指定链接的记录
 * 函数参数：
 *  int netFd：连接的socket
 *  ConnectsStatus *connsStatus：维护的连接结构体
 * 函数返回值：成功返回0
*/
int deleteConnect(int netFd, ConnectsStatus *connsStatus);

/*
 * 函数名：checkConnect
 * 函数作用：检查链接状态
 * 函数参数：
 *  int netFd：连接的socket
 *  ConnectsStatus *connsStatus：维护的连接结构体
 * 函数返回值：成功返回0
*/
int checkConnect(int epollFd, ConnectsStatus * connsStatus);

#endif
