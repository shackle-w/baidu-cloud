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
 * 函数作用：向服务器发送文件
 * 函数参数：
 *  int net_fd：用于通信的socket
 *  int file_fd：要发送的文件描述符
 * 函数返回值：成功返回0
*/
int sendFile(int net_fd, int file_fd);

/*
 * 函数名：sendFileBreakPoint
 * 函数作用：从断点继续发送文件
 * 函数参数：
 *  int net_fd：用于通信的socket
 *  int file_fd：要发送的文件描述符
 *  int currentServeFileSize：偏移量
 *  函数返回值：成功返回0
*/
int sendFileBreakPoint(int net_fd, int file_fd, int currentServeFileSize);

/*
 * 函数名：downloadFile
 * 函数作用：从服务器下载文件
 * 函数参数：
 *  int net_fd：用于通信的socket
 *  char *fileName：要接受文件的文件名
 * 函数返回值：成功返回0
*/
int downloadFile(int net_fd, char *fileName);

#endif
