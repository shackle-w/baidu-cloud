/*
 * 文件名：salt.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件是实现密码加密的接口函数
*/

#ifndef __SALTH__
#define __SALTH__

#include "my_header.h"
#define SALT_LENGTH 8

/*
 * 函数名：salt
 * 函数作用：生成随机的盐值
 * 函数参数
 *  void
 * 函数返回值：生成的随机盐值
*/
char* salt();

/*
 * 函数名：cryptPassword
 * 函数作用：根据输入的密码和盐值，对密码进行加密
 * 函数参数：
 *  char *password：输入的密码
 *  char *salt：随机生成的盐值
 * 函数返回值：加密后的密码
*/
char* cryptPassword(char *password, char *salt);

#endif
