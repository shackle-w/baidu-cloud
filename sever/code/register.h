/*
 * 文件名：register.h
 * 创建时间：5.28
 * 作者；shackle
 * 描述：该文件的作用是处理连接数据的登录所需的文件
*/
#ifndef __REGISTERH__
#define __REGISTERH__

#include "my_header.h"
#include "tools.h"

#define SALT_LENGTH 16 // 盐值的长度

/*
 * 函数名：generate_salt
 * 函数作用：基于时间生成一个随机的盐值
 * 函数参数：
 *  -
 * 返回值：返回一个盐值的字符串
*/
char *generate_salt();

/*
 * 函数名：getToken
 * 函数作用：根据用于id和用户名生成token
 * 参数：
 *  -char *id：用户的id
 *  -char *username：用户的用户名
 *  -char *jwt：用于获取生成的token
 * 返回值：成功返回0，失败返回-1
*/
int getToken(char *id, char *username, char **jwt);

/*
 * 函数名：registerAccount
 * 函数作用：用于和客户端交互，注册一个账号
 * 参数：
 *  -int net_fd：连接的sockt
 *  -user_t *user：封装的用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：0：注册成功，1：注册失败
*/
int registerAccount(int net_fd, user_t *user, MYSQL *conn);

/*
 * 函数名：loginID
 * 函数作用：用于和客户端交互，注册一个账号
 * 参数：
 *  -int net_fd：连接的sockt
 *  -user_t *user：封装的用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：0：注册成功，1：注册失败
*/
int loginID(int net_fd, user_t *user, MYSQL *conn);

#endif
