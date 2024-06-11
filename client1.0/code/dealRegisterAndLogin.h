/*
 * 文件名：dealRegisterAndLogin.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现登录以及注册的接口函数
*/

#ifndef __DEALREGISTERANDLOGINH__
#define __DEALREGISTERANDLOGINH__

#include "my_header.h"
#include "clientStruct.h"
#include "termio.h"
#include "dealParameter.h"
#include "send.h"

/*
 * 函数名：disableEcho
 * 函数作用：用于禁止终端回显的函数
 * 函数参数：
 *  int fd：文件描述符，是终端的文件描述符
 *  struct termios *old_termios：指向结构体的指针，用于保存当前终端的设置，以便以后恢复
 * 函数返回值：void
*/
void disableEcho(int fd, struct termios *old_termios);

/*
 * 函数名：restoreEcho
 * 函数作用：用于恢复终端回显的函数
 * 函数参数：
 *  int fd：指向终端的文件描述符
 *  struct termios *old_termios：指向先前保存的”termios“结构体的指针
 * 返回值：void
*/
void restoreEcho(int fd, struct termios *old_termios);

/*
 * 函数名：distributeLoginOrRegister
 * 函数作用：用于选择要进行的操作
 * 函数参数：
 *  run_status_t *runstatus：保存客户端运行状态的信息
 * 函数返回值：成功返回0
*/
int distributeLoginOrRegister(run_status_t *runstatus);

/*
 * 函数名：doLogin
 * 函数作用：处理登录的逻辑
 * 函数参数：
 *  run_status_t *runstatus：保存客户端运行状态的信息
 * 函数返回值：成功返回0
*/
int doLogin(run_status_t *runstatus);

/*
 * 函数名：doRegister
 * 函数作用：处理注册的逻辑
 * 函数参数：
 *  run_status_t *runstatus：保存客户端运行状态的信息
 * 函数返回值：成功返回0
*/
int doRegister(run_status_t *runstatus);

#endif
