/*
 * 文件名：dealCommand.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现了对命令处理的接口
*/

#ifndef __DEALCOMMANH_
#define __DEALCOMMANH_

#include "my_header.h"
#include "clientStruct.h"
#include "dealParameter.h"
#include "send.h"
#include "pathList.h"
#include "clientFile.h"

/*
 * 函数名：distributeCommand
 * 函数作用：对输入的命令进行处理
 * 函数参数：
 *  run_status_t *runstatus：客户端的状态信息
 * 函数返回值：成功返回0
*/
int distributeCommand(run_status_t *runstatus);

/*
 * 函数名：mkdirCommand
 * 函数作用：处理mkdir命令
 * 函数参数：
 *  char *buf：输入的命令
 *  run_status_t *runstatus：客户端的状态信息
 * 函数返回值：成功返回0
*/
int mkdirCommand(char *buf, run_status_t *runstatus);

/*
 * 函数名：lsCommand
 * 函数作用：处理ls命令
 * 函数参数：
 *  char *buf：输入的命令
 *  run_status_t *runstatus：客户端的状态信息
 * 函数返回值：成功返回0
*/
int lsCommand(char *buf, run_status_t *runstatus);

/*
 * 函数名：cdCommand
 * 函数作用：处理mkdir命令
 * 函数参数：
 *  char *buf：输入的命令
 *  run_status_t *runstatus：客户端的状态信息
 * 函数返回值：成功返回0
*/
int cdCommand(char *buf, run_status_t *runstatus);

/*
 * 函数名：pwdCommand
 * 函数作用：处理pwd命令
 * 函数参数：
 *  char *buf：输入的命令
 *  run_status_t *runstatus：客户端的状态信息
 * 函数返回值：成功返回0
*/ 
int pwdCommand(char *buf, run_status_t *runstatus);

/*
 * 函数名：rmCommand
 * 函数作用：处理rm命令
 * 函数参数：
 *  char *buf：输入的命令
 *  run_status_t *runstatus：客户端的状态信息
 * 函数返回值：成功返回0
*/
int rmCommand(char *buf, run_status_t *runstatus);


#endif
