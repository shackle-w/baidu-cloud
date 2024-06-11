/*
 * 文件名：dealCommand.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现了对命令处理的接口
*/

#ifndef __DEALCOMMANH_
#define __DEALCOMMANH_

#include "my_header.h"
#include "severStruct.h"
#include "queue.h"
#include "dealParameter.h"
#include "dealRegisterAndLogin.h"

/*
 * 函数名：doRequest
 * 函数作用：对输入的命令进行处理
 * 函数参数：
 *  int netFd：通信用的socket连接
 *  pool_t *pool：客户端的状态信息
 * 函数返回值：成功返回0
*/
int doRequest(int netFd, pool_t* pool);

/*
 * 函数名：mkdirRequest
 * 函数作用：处理mkdir命令
 * 函数参数：
 *  int netFd：通信用的socket
 *  protocol_client protocol：客户端传来的信息
 *  pool_t *pool：服务器的运行状态
 * 函数返回值：成功返回0
*/
int mkdirRequest(int netFd, protocol_client protocol, pool_t* pool);

/*
 * 函数名：lsRequest
 * 函数作用：处理ls命令
 * 函数参数：
 *  int netFd：通信用的socket
 *  protocol_client protocol：客户端传来的信息
 *  pool_t *pool：服务器的运行状态
 * 函数返回值：成功返回0
*/
int lsRequest(int netFd, protocol_client protocol, pool_t* pool);

/*
 * 函数名：rmRequest
 * 函数作用：处理rm命令
 * 函数参数：
 *  int netFd：通信用的socket
 *  protocol_client protocol：客户端传来的信息
 *  pool_t *pool：服务器的运行状态
 * 函数返回值：成功返回0
*/
int rmRequest(int netFd, protocol_client protocol, pool_t * pool);

/*
 * 函数名：rmRRequest
 * 函数作用：处理rm -r 命令
 * 函数参数：
 *  int netFd：通信用的socket
 *  protocol_client protocol：客户端传来的信息
 *  pool_t *pool：服务器的运行状态
 * 函数返回值：成功返回0
*/
int rmRRequest(int netFd, protocol_client protocol, pool_t * pool);

/*
 * 函数名：pathCheckRequest
 * 函数作用：处理检查路径的信息
 * 函数参数：
 *  int netFd：通信用的socket
 *  protocol_client protocol：客户端传来的信息
 *  pool_t *pool：服务器的运行状态
 * 函数返回值：成功返回0
*/
// TODO：客户端发送了此信息吗
int pathCheckRequest(int netFd, protocol_client protocol, pool_t* pool);

#endif
