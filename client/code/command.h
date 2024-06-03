/*
 * 文件名：command.h
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件是对输入的命令进行处理的函数
*/

#ifndef __COMMANDH__
#define __COMMANDH__

#include "my_header.h"
#include "log.h"
#include "tools.h"


/*
 * 函数名：shardCommand
 * 函数作用：对命令进行简单的处理
 * 函数参数：
 *  -char *buf：命令  
 * 返回值：3代表短命令，4代表长命令-1，代表错误
*/
int shardCommand(char *buf);

/*
 * 函数名：shortCommand
 * 函数作用：处理短命令
 * 函数参数：
 *  -int net_fd：进行通信的soacket
 *  -char *command：需要处理的命令
 *  -char *user：携带的用户信息
 * 返回值：0代表成功，-1代表失败
*/
int shortCommand(int net_fd, user_t *user);

/*
 * 函数名：longCommand
 * 函数作用：处理短命令
 * 函数参数：
 *  -int net_fd：进行通信的soacket
 *  -char *command：需要处理的命令
 *  -char *user：携带的用户信息
 * 返回值：0代表成功，-1代表失败
*/
int longCommand(int net_fd, user_t *user);


#endif

