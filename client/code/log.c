#include "log.h"


// 打开日志文件
FILE *logFile;

/*
 * 函数名：initLogger
 * 函数作用：打开日志文件
 * 参数：
 * 返回值
*/
void initLogger(const char *filename) {
    logFile = fopen(filename, "a");
    if (logFile == NULL) {
        fprintf(stderr, "Error: Unable to open log file.\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * 函数名：closeLogger
 * 函数作用：关闭日志文件
 * 参数：
 * 返回值：
*/
void closeLogger() {
    if (logFile != NULL) {
        fclose(logFile);
    }
}

/*
 * 函数名: getCurrentTime
 * 函数作用：获取当前的时间
 * 参数：
 *  
 * 返回值：
*/
char *getCurrentTime() {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}

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
void writeLog(LogLevel level, const char *file, int line, const char *func, const char *format, ...) {
    initLogger("logfile.txt");
    va_list args;
    va_start(args, format);

    char *levelStr;
    char *color;

    switch (level) {
        case INFO:
            levelStr = "INFO";
            color = ANSI_COLOR_CYAN;
            break;
        case WARNING:
            levelStr = "WARNING";
            color = ANSI_COLOR_YELLOW;
            break;
        case ERROR:
            levelStr = "ERROR";
            color = ANSI_COLOR_RED;
            break;
        default:
            levelStr = "UNKNOWN";
            color = ANSI_COLOR_RESET;
            break;
    }

    fprintf(logFile, "%s[%s] %s[%s:%d:%s]: %s", color, levelStr, getCurrentTime(), file, line, func, ANSI_COLOR_RESET);
    vfprintf(logFile, format, args);
    fprintf(logFile, "%s\n", ANSI_COLOR_RESET);

    va_end(args);
    closeLogger();
}


/*
 * 函数名：logSocketConnection
 * 函数作用：将客户端的连接信息记录到日志中
 * 参数：
 *  const char *address：客户端的IP地址
 *  int port：客户端用于通信的端口
*/

void logSocketConnection(const char *address, int port) {
    LOG(INFO, "Established connection to %s:%d", address, port);
}


/*
 * 函数名：logReceivedMessage
 * 函数作用：将客户端发送的命令记录到日志文件中
 * 参数：
 *  -const char *message：客户端发送的命令
 * 返回值
*/
void logReceivedMessage(const char *message) {
    LOG(INFO, "Received message: %s", message);
}


