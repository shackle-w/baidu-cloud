/*
 * 文件名：putsfile.h
 * 创建时间：24.6.6
 * 作者：shackle
 * 描述：该文件的作用是处理向服务器上传文件的接口调用
*/

#ifndef __PUTSFILEH__
#define __PUTSFILEH__

#include "my_header.h"
#include "log.h"
#include "config.h"
#include "tools.c"
#include "command.h"

/*
 * 函数名：putsFile
 * 函数作用：从数据库下载文件到服务器
 * 函数参数：
 *  -int net_fd：通信的socket
 *  -user_t *user：用户的信息
 *  -MYSQL* conn：数据库的指针
 * 返回值：下载成功返回0
*/
int putsFile(int net_fd, user_t *user, MYSQL *conn);


#endif

