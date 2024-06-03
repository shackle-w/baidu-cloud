/*
 * 函数名：tools.h
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件的作用是提供客户端的一系列的工具
*/

#ifndef __TOOLH__
#define __TOOLH__

#include "my_header.h"
#include "log.h"

// 定义结构体，封装发送命令是的信息
typedef struct user_s{
    char token[BUF_SIZE]; // token信息
    char command[BUF_SIZE]; // 输入的命令
    char path[BUF_SIZE]; // 当前的路径
}user_t;

/*
 * 函数名：sendRegister
 * 函数作用：向服务器发送注册信息
 * 参数：
 *  -int net_fd：连接的socket
 *  -char *information：注册需要的信息
 * 返回值：成功返回0，失败返回-1
*/
int sendRegister(int net_fd, char *information);

/*
 * 函数名：sendLogin
 * 函数作用：向服务器发送登录信息
 * 参数：
 *  -int net_fd：连接的socket
 *  -char *information：登录需要的信息
 * 返回值：成功返回0，失败返回-1
*/
int sendLogin(int net_fd, char *information);

/*
 * 函数名：sendShort
 * 函数作用：向服务器发送短命令
 * 参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：封装的user信息
 * 返回值：成功返回0，失败返回-1
*/
int sendShort(int net_fd, user_t *user);

/*
 * 函数名：sendLong
 * 函数作用：向服务器发送长命令
 * 参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：封装的user信息
 * 返回值：成功返回0，失败返回-1
*/
int sendLong(int net_fd, user_t *user);

/*
 * 函数名：sendFile
 * 函数作用：向服务器发送文件
 * 参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：封装的user信息
 * 返回值：成功返回0，失败返回-1；
*/
int sendFile(int net_fd, user_t *user);

/*
 * 函数名：sendOver
 * 函数作用：向服务器发送结束信号
 * 参数：
 *  -int net_fd：连接的socket
 * 返回值：成功返回0，失败返回-1；
*/
int sendOver(int net_fd);

// 封装发送注册信息的函数宏
#define SEND_REGISTER(net_fd, information) sendRegister(net_fd, &information);

// 封装发送登录信息的函数宏
#define SEND_LOGIN(net_fd, information) sendLogin(net_fd, &information);

// 封装发送短命令的函数宏
#define SEND_SHORT(net_fd, user) sendShort(net_fd, &user)

// 封装发送长命令的函数宏
#define SEND_LONG(net_fd, user) sendLong(net_fd, &user)

// 封装发送文件的函数宏
#define SEND_FILE(net_fd, user) sendFile(net_fd, &user)

// 封装发送错误信号的函数宏
#define SEND_OVER(net_fd) sendOver(net_fd)

/*
 * 函数名：printfHint
 * 函数作用：打印一个提示符
 * 参数：
 *  -char *username：用户名
 *  -char *path：工作路径
 * 返回值：void类型
*/
void printfHint(char *username, char *path);

#endif
