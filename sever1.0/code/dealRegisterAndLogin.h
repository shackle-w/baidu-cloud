/*
 * 文件名：dealRegisterAndLogin.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现登录以及注册的接口函数
*/

#ifndef __DEALREGISTERANDLOGINH__
#define __DEALREGISTERANDLOGINH__

#include "my_header.h"
#include "severStruct.h"
#include "dealParameter.h"
#include "salt.h"
#include "sqlConnectionPool.h"
#include "token.h"

/*
 * 函数名：loginRequest
 * 函数作用：处理登录请求
 * 函数参数：
 *  int netFd：通信的socket
 *  protocol_clinet protocol：客户端传来的信息
 *  pool_t *pool：服务器的状态信息
 * 函数返回值：
 *  成功返回0
*/
int loginRequest(int netFd, protocol_client protocol, pool_t* pool);

/*
 * 函数名：registerNameRequest
 * 函数作用：处理注册请求：验证账号
 * 函数参数：
 *  int netFd：通信的socket
 *  protocol_clinet protocol：客户端传来的信息
 *  pool_t *pool：服务器的状态信息
 * 函数返回值：
 *  成功返回0
*/
int registerNameRequest(int netFd, protocol_client protocol, pool_t* pool);

/*
 * 函数名：registerPasswordRequest
 * 函数作用：处理注册请求：卸载账号密码
 * 函数参数：
 *  int netFd：通信的socket
 *  protocol_clinet protocol：客户端传来的信息
 *  pool_t *pool：服务器的状态信息
 * 函数返回值：
 *  成功返回0
*/
int registerPasswordRequest(int netFd, protocol_client protocol, pool_t* pool);

#endif
