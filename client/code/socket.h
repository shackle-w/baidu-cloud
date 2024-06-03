/*
 * 文件名：socket.h
 * 创建时间：5.29
 * 作者：shackle
 * 描述：该文件的作用是创建客户端的socket
*/

#ifndef __SOCKETH__
#define __SOCKETH__

#include "my_header.h"
#include "log.h"

/*
 * 函数名：initTcp
 * 函数作用：初始化客户端socket
 * 参数：
 *  -int *net_fd：创建的连接socket
 *  -char *ip：服务器的ip地址
 *  -char *port：服务器的端口号
 * 返回值：成功返回0，失败返回-1
*/
int initTcp(int *net_fd, char *ip, char *port);
#endif
