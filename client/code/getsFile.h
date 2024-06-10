/*
 * 文件名：getsfile.h
 * 创建时间：24.6.6
 * 作者：shackle
 * 描述：该文件的作用是处理从服务器下载文件的接口调用
*/

#ifndef __GETSFILEH__
#define __GETSFILEH__

#include "my_header.h"
#include "log.h"
#include "tools.c"

/*
 * 函数名：getsFile
 * 函数作用：从数据库下载文件到服务器
 * 函数参数：
 *  -int net_fd：通信的socket
 *  -user_t *user：用户的信息
 * 返回值：下载成功返回0
*/
int getsFile(int net_fd, user_t *user);


#endif

