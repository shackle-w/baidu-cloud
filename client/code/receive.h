/*
 * 文件名：receiver.h
 * 创建时间：5.31
 * 作者：shackle
 * 描述：该文件是对服务器返回信息的处理
*/

#ifndef __RECEIVEH__
#define __RECEIVEH__

#include "my_header.h"
#include "tools.h"

/*
 * 函数名：dealReceive
 * 函数作用：处理服务端返回的信息
 * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户的信息
 * 返回值：成功返回0.失败返回-1
*/
int dealReceive(int net_fd, user_t *user);


#endif
