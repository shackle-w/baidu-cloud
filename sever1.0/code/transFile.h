/*
 * 文件名：transFile.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现发送文件的一些接口
*/

#ifndef __TRANSFILEH__
#define __TRANSFILEH__

#include "my_header.h"

/*
 * 函数名：sendFile
 * 函数作用：用于传输文件
 * 函数参数：
 *  int net_fd：通信用的socket
 *  int file_fd：文件描述符
 * 函数返回值：成功返回0
*/
int sendFile(int net_fd, int  file_fd);

/*
 * 函数名：uploadFile
 * 函数作用：用于传输文件
 * 函数参数：
 *  int net_fd：通信用的socket
 *  int file_fd：文件描述符
 * 函数返回值：成功返回0
*/
int uploadFile(int net_fd, int file_fd);

/*
 * 函数名：uploadFileBreakPoint
 * 函数作用：实现断点续传
 * 函数参数：
 *  int netFd：通信用的socket
 *  FILE *file：文件指针
 * 函数返回值：成功返回0
*/
int uploadFileBreakPoint(int netFd, FILE * file);

#endif
