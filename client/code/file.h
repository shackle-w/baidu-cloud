/*
 * 文件名：file.h
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件的作用是处理上传和下载文件
*/

#ifndef __FILEH__
#define __FILEH__

#include "my_header.h"
#include "log.h"
#include "tools.h"

/*
 * 函数名：putsFile
 * 函数作用：向服务端推送文件
 * 参数：
 *  -int net_fd：用于通信的socket
 * 返回值：成功返回0，失败返回-1
*/
int putsFile(int net_fd);


#endif
