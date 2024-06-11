/*
 * 文件名：initTcp.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现建立socket连接的接口函数
*/

#ifndef __INITTCPH__
#define __INITTCPH__

#include "my_header.h"

/*
 * 函数名：initTcp
 * 函数作用：建立服务端和客户端的socket连接
 * 函数参数：
 *  int *netFd：socket连接用到的文件描述符
 *  char *ip：服务器的ip号
 *  char *port：服务器的端口号
 * 返回值：成功返回0
*/
int initTcp(int *netFd, char *ip, char *port);

#endif
