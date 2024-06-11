/*
 * 文件名：sqlConnectionPool.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现和数据库链接的接口
*/

#ifndef __SQLCONNECTIONPOOLH__
#define __SQLCONNECTIONPOOLH__

#include "my_header.h"
#include "severStruct.h"


/*
 * 函数名：getConnect
 * 函数作用：获得一个数据库连接
 * 函数参数：
 *  sql_connect_t *sqlConnect：一个数据库连接
 *  pool_t *pool：服务器的运行状态
 * 函数返回值：成功返回0
*/
int getConnect(sql_connect_t *sqlConnect, pool_t* pool);

/*
 * 函数名：setConnect
 * 函数作用：使用完毕之后，把数据库连接还回去
 * 函数参数：
 *  sql_connect_t *sqlConnect：一个数据库连接
 *  pool_t *pool：服务器的运行状态
 * 函数返回值：成功返回0
*/
int setConnect(sql_connect_t sqlConnect, pool_t* pool);

#endif
