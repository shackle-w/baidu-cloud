/*
 * 文件名：register.h
 * 创建时间：5.28
 * 作者：shackle
 * 描述：该文件的作用是用于注册登录
*/

#ifndef __REGISTERH__
#define __REGISTERH__

#include "my_header.h"
#include "log.h"
#include "tools.h"

/*
 * 函数名：registerAccount
 * 函数作用：用于注册一个账号
 * 参数：
 *  -int net_fd：连接的socket
 *  -char *account：用户的账号名
 *  -char *password：用户输入的密码
 *  -user_t *user：封装的用户信息
 * 返回值：0：注册成功，1：注册失败
*/
int registerAccount(int net_fd, const char *account, const char *password, user_t *user);

/*
 * 函数名：loginID
 * 函数作用：用于注册一个账号
 * 参数：
 *  -int net_fd：连接的socket
 *  -char *account：用户的账号名
 *  -char *password：用户输入的密码
 *  -user_t *user：封装的用户信息
 * 返回值：0：登录成功，1：登录失败
*/
int loginID(int net_fd, const char *account, const char *password, user_t *user);

/*
 * 函数名：handleLoginRegister
 * 函数作用：注册登录账号
 * 参数：
 *  -int socket_fd：连接到socket
 *  -user_t *user：封装的用户信息
 *  -char *account：用户的账号名
 * 返回值：0：成功， -1：失败
*/
int handleLoginRegister(int socket_fd, user_t *user, char *account);


#endif
