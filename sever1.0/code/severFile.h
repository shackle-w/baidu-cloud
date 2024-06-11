/*
 * 文件名：clientFile.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现了对文件的处理
*/

#ifndef __CLIENTFILEH_
#define __CLIENTFILEH__

#include "my_header.h"
#include "severStruct.h"
#include "transFile.h"
#include "map.h"
#include "token.h"
#include "dealParameter.h"
#include "sqlConnectionPool.h"

/*
 * 函数名syncPuts
 * 函数作用：回复给客户端同步文件上传的回复
 * 函数参数：
 *  int net_fd：通信用的socket
 * 函数返回值：成功返回0
*/
int syncPuts(int net_fd);

/*
 * 函数名：dealPuts
 * 函数作用：处理puts命令
 * 函数参数：
 *  int net_fd：通信用的socket
 *  pool_t *pool：服务器的运行信息
 * 函数返回值：成功返回0
*/
int dealPuts(int net_fd, pool_t *pool);

/*
 * 函数名syncGest
 * 函数作用：回复给客户端同步文件下载的回复
 * 函数参数：
 *  int net_fd：通信用的socket
 * 函数返回值：成功返回0
*/
int syncGets(int net_fd);

/*
 * 函数名：dealGets
 * 函数作用：处理gets命令
 * 函数参数：
 *  int net_fd：通信用的socket
 *  pool_t *pool：服务器的运行信息
 * 函数返回值：成功返回0
*/
int dealGets(int net_fd, pool_t *pool);



#endif
