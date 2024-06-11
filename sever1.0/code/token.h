/*
 * 文件名：token.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件用于生成Token的接口函数
*/

#ifndef __TOKENH__
#define __TOKENH__

#include "my_header.h"
#include <l8w8jwt/encode.h>
#include <l8w8jwt/encode.h>
#include <l8w8jwt/decode.h>

/*
 * 函数名：getToken
 * 函数作用：根据用户的Id，生成一个token
 * 函数参数：
 *  int userId：用户的id
 *  char *buf：生成的token保存的位置
 * 函数返回值：成功返回0
*/
int getToken(int userId, char *buf );

/*
 * 函数名：getUserId
 * 函数作用：根据token，解析出用户id
 * 函数参数：
 *  char *token：客户端传来的token
 * 函数返回值：用户的id;
*/
int getUserId(char * token);

#endif
