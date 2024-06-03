/*
 * 文件名：log.h
 * 创建时间：5.26
 * 作者：shackle
 * 描述：该文件是日志的接口文件
*/

#ifndef __LOGHEAD__
#define __LOGHEAD__

#include "my_header.h"
#include <stdarg.h>

// ANSI颜色码
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

// 定义日志级别
typedef enum {
    INFO,
    WARNING,
    ERROR
} LogLevel;

// 打开日志文件
extern FILE *logFile;

/*
 * 函数名：initLogger
 * 函数作用：打开日志文件
 * 参数：
 * 返回值
*/
void initLogger(const char *filename);

/*
 * 函数名：closeLogger
 * 函数作用：关闭日志文件
 * 参数：
 * 返回值：
*/
void closeLogger();

/*
 * 函数名: getCurrentTime
 * 函数作用：获取当前的时间
 * 参数：
 *  
 * 返回值：
*/
char *getCurrentTime();

/*
 * 函数名：wrireLog
 * 函数作用：向日志文件内写入内容
 * 参数：
 *  -LogLevel level：记录的级别
 *  -const char *file：记录来的文件
 *  -int len：记录来自的行号
 *  const char *func：记录来自的函数
 *  const char *format：记录的其他信息
 * 返回值：
*/
void writeLog(LogLevel level, const char *file, int line, const char *func, const char *format, ...);


// 宏，用于调用日志（参数：记录级别，以及和scanf一样的输入
#define LOG(level, ...) writeLog(level, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define ERROR_LOG(ret, error_flag, msg)\
{\
    if((ret) == (error_flag)){\
        LOG(ERROR, "ERROR INFORMATION :%s", msg);\
    }\
}

#define INFO_LOG(msg) LOG(INFO, "INFO INFORMATION: %s", msg);

/*
 * 函数名：logSocketConnection
 * 函数作用：将客户端的连接信息记录到日志中
 * 参数：
 *  const char *address：客户端的IP地址
 *  int port：客户端用于通信的端口
*/

void logSocketConnection(const char *address, int port);


/*
 * 函数名：logReceivedMessage
 * 函数作用：将客户端发送的命令记录到日志文件中
 * 参数：
 *  -const char *message：客户端发送的命令
 * 返回值
*/
void logReceivedMessage(const char *message);

#endif
