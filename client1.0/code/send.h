/*
 * 文件名：send.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件的作用是实现向服务器发送信息的接口
*/

#ifndef __CLIENTSENDH__
#define __CLIENTSENDH__

#include "my_header.h"
#include "clientStruct.h"
#include "map.h"
#include "initTcp.h"

/*
 * 函数名：utilSend
 * 函数作用：向服务端发送信息
 * 函数参数：
 *  int netFd：通讯的socket
 *  protocol_client *protocol：客户端封装的信息
 *  run_status_t *runstatus：客户端的状态信息
 * 函数返回值：成功返回0
*/
int utilSend(int netFd, protocol_client *protocol, run_status_t *runstatus);

#endif
