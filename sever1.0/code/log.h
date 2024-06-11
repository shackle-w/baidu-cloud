/*
 * 文件名：log.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件是实现日志的宏
*/

#ifndef __LOGH__
#define __LOGH__

#include "my_header.h"
#include "map.h"

// ------log-----相关-----
enum{
    LOG_TYPE_ERROR = 1,
    LOG_TYPE_WARNING = 2,
    LOG_TYPE_INFO = 3,
    // 可以加更多
};

#define LOG(flag, msg){\
    HashMap *map = parameterMap();\
    char* base_log_path = get(map, "log_path");\
    char* log_level = get(map, "log_level"); \
    if(flag == LOG_TYPE_ERROR && (strncmp(log_level, "error", 5)== 0 ||strncmp(log_level, "warning", 7)== 0 || strncmp(log_level, "info", 4) == 0) ){ \
        char logName[200] = {0}; \
        sprintf(logName, "%s/log_error.log", base_log_path); \
        int file_fd = open(logName, O_RDWR |O_CREAT|O_APPEND, 0600);  \
        char buf[1024] = {0}; \
        time_t now = time(NULL); \
        struct tm *local_time = localtime(&now); \
        strftime(buf, sizeof(buf), "[ %Y-%m-%d %H:%M:%S ]", local_time); \
        sprintf(buf + strlen(buf), " [pid : %d]", getpid()) ; \
        sprintf(buf + strlen(buf), " [ ERROR ]" ); \
        sprintf(buf + strlen(buf), " [%s :: %s :: %d]: %s \n", __FILE__, __FUNCTION__, __LINE__, msg) ; \
        write(file_fd, buf, strlen(buf));\
        close(file_fd);\
        bzero(logName, 200);\
        sprintf(logName, "%s/log_all.log", base_log_path); \
        file_fd = open(logName, O_RDWR |O_CREAT|O_APPEND, 0600);  \
        write(file_fd, buf, strlen(buf));\
        close(file_fd);\
    }else if(flag == LOG_TYPE_WARNING && (strncmp(log_level, "warning", 7)==0 || strncmp(log_level, "info", 4)==0)){\
        char logName[200] = {0}; \
        sprintf(logName, "%s/log_waring.log", base_log_path); \
        int file_fd = open(logName, O_RDWR |O_CREAT|O_APPEND, 0600);  \
        char buf[1024] = {0}; \
        time_t now = time(NULL); \
        struct tm *local_time = localtime(&now); \
        strftime(buf, sizeof(buf), "[ %Y-%m-%d %H:%M:%S ]", local_time); \
        sprintf(buf + strlen(buf), " [pid : %d]", getpid()) ; \
        sprintf(buf + strlen(buf), " [ WARNING ]") ; \
        sprintf(buf + strlen(buf), " [%s :: %s :: %d]: %s \n", __FILE__, __FUNCTION__, __LINE__, msg) ; \
        write(file_fd, buf, strlen(buf));\
        close(file_fd);\
        bzero(logName, 200);\
        sprintf(logName, "%s/log_all.log", base_log_path); \
        file_fd = open(logName, O_RDWR |O_CREAT|O_APPEND, 0600);  \
        write(file_fd, buf, strlen(buf));\
        close(file_fd);\
    }else if(flag == LOG_TYPE_INFO && (strncmp(log_level, "info", 4)==0)){\
        char logName[200] = {0}; \
        sprintf(logName, "%s/log_info.log", base_log_path); \
        int file_fd = open(logName, O_RDWR |O_CREAT|O_APPEND, 0600);  \
        char buf[1024] = {0}; \
        time_t now = time(NULL); \
        struct tm *local_time = localtime(&now); \
        strftime(buf, sizeof(buf), "[ %Y-%m-%d %H:%M:%S ]", local_time); \
        sprintf(buf + strlen(buf), " [pid : %d]", getpid()) ; \
        sprintf(buf + strlen(buf), " [ INFO ]") ; \
        sprintf(buf + strlen(buf), " [%s :: %s :: %d]: %s \n", __FILE__, __FUNCTION__, __LINE__, msg) ; \
        write(file_fd, buf, strlen(buf));\
        close(file_fd);\
        bzero(logName, 200);\
        sprintf(logName, "%s/log_all.log", base_log_path); \
        file_fd = open(logName, O_RDWR |O_CREAT|O_APPEND, 0600);  \
        write(file_fd, buf, strlen(buf));\
        close(file_fd);\
    } \
};
#endif
