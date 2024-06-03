/*
 * 文件名：socket.h
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件用于创建socket连接
*/

#ifndef __SOCKETH__
#define __SOCKETH__

#include "my_header.h"
#include "log.h"

/*
 * 函数名：initSocket
 * 描述：根据端口和IP构建服务端的网络监听
 * 参数：
 *  -int *socketfd：socket的文件描述符，用于服务器和客户端通信
 *  -char *ip：服务器端的ip地址
 *  -char *port：服务器端的端口号
 * 返回值：socketfd
 */
int initSocket(int *socketfd, char *ip, char *port);

#endif
