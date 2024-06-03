/*
 * 文件名：command.h
 * 创建时间：5.31
 * 作者：shackle
 * 描述：该文件是处理命令的文件
*/

#ifndef __COMMANDH__
#define __COMMANDH__

#include "my_header.h"
#include "log.h"
#include "tools.h"
#include <l8w8jwt/encode.h>
#include <l8w8jwt/decode.h>

/* 函数名：resolve
 * 函数作用：根据用户传来的token，分解出用户id
 * 参数：
 *  -char *userId：用户id
 *  -char *token：用户传入的token
 * 返回值：成功返回0，失败返回-1
*/
int resolve(char *userId, char *token);

/*
 * 函数名：dealShort
 * 函数作用：处理短命令
 * 参数：
 *  -int net_fd：文件描述符
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0，失败返回-1
*/
int dealShort(int net_fd, user_t *user, MYSQL *conn);

/*
 * 函数名：realizeCD
 * 函数作用：处理CD命令
 * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0， 失败返回-1
*/
int realizeCD(int net_fd, user_t *user, MYSQL *conn);

/*
 * 函数名：realizeLS
 * 函数作用：处理LS命令
  * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0， 失败返回-1
*/
int realizeLS(int net_fd, user_t *user, MYSQL *conn);

/*
 * 函数名：realizePWD
 * 函数作用：处理PWD命令
  * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0， 失败返回-1
*/
int realizePWD(int net_fd, user_t *user, MYSQL *conn);

/*
 * 函数名：realizeMKDIR
 * 函数作用：处理MKDOR命令
  * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0， 失败返回-1
*/
int realizeMKDIR(int net_fd, user_t *user, MYSQL *conn);

/*
 * 函数名：realizeRM
 * 函数作用：处理RM命令
  * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0， 失败返回-1
*/
int realizeRM(int net_fd, user_t *user, MYSQL *conn);

#endif
