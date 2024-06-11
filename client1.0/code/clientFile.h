/*
 * 文件名：clientFile.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现了对文件的处理
*/

#ifndef __CLIENTFILEH_
#define __CLIENTFILEH__

#include "my_header.h"
#include "clientStruct.h"
#include "dealParameter.h"
#include "send.h"
#include "pathList.h"
#include "transFile.h"
#include "sonThread.h"


/*
 * 函数名：putsCommand
 * 函数作用：发送上传文件的命令
 * 函数参数：
 *  char *buf：输入的命令
 *  run_status_t *runstatus：客户端的状态信息
 * 函数返回值：成功返回0
 */
int putsCommand(char *buf, run_status_t *runstatus);

/*
 * 函数名：getssCommand
 * 函数作用：发送下载文件的命令
 * 函数参数：
 *  char *buf：输入的命令
 *  run_status_t *runstatus：客户端的状态信息
 * 函数返回值：成功返回0
 */
int getsCommand(char *buf, run_status_t *runstatus);

/*
 * 函数名：syncPuts
 * 函数作用：用于上传文件的同步信息的命令
 * 函数参数：
 *  int net_fd：通信的socket
 * 函数返回值：成功返回0
 */
int syncPuts(int net_fd);

/*
 * 函数名：filePuts
 * 函数作用：用于发送上传文件信息
 * 函数参数：
 *  int net_fdi：用于通信的socket
 *  PutsData *data：文件的相关信息
 * 函数返回值：成功返回0
*/
int filePuts(int net_fdi, PutsData *data);

/*
 * 函数名：syncGets
 * 函数作用：用于下载文件的同步信息的命令
 * 函数参数：
 *  int net_fd：通信的socket
 * 函数返回值：成功返回0
*/
int syncGets(int net_fd);

/*
 * 函数名：fileGets
 * 函数作用：用于发送下载文件信息
 * 函数参数：
 *  int net_fdi：用于通信的socket
 *  PutsData *data：文件的相关信息
 * 函数返回值：成功返回0
*/
int fileGets(int net_fdi, PutsData *data);


#endif
