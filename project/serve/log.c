#include <57header.h>

#define ERROR 1
#define INFO 2
#define MESSAGE 3

#define LOGX(flag, msg, file, function, line){ \
    if(flag == ERROR){ \
        int file_fd = open("log_error.log", O_RDWR | O_APPEND);  \
        char buf[1024] = {0}; \
        time_t now = time(NULL); \
        struct tm *local_time = localtime(&now); \
        strftime(buf, sizeof(buf), "[ %Y-%m-%d %H:%M:%S ]", local_time); \
        sprintf(buf + strlen(buf), " [%s :: %s :: %d]: %s  \n", file, function, line, msg) ; \
        write(file_fd, buf, strlen(buf));\
        close(file_fd);\
    }else if(flag == INFO){\
        int file_fd = open("log_info.log", O_RDWR | O_APPEND);  \
        char buf[1024] = {0}; \
        time_t now = time(NULL); \
        struct tm *local_time = localtime(&now); \
        strftime(buf, sizeof(buf), "[ %Y-%m-%d %H:%M:%S ]", local_time); \
        sprintf(buf + strlen(buf), " [%s :: %s :: %d]: %s  \n", file, function, line, msg) ; \
        write(file_fd, buf, strlen(buf));\
        close(file_fd);\
    }else if(flag == MESSAGE){\
        int file_fd = open("log_message.log", O_RDWR | O_APPEND);  \
        char buf[1024] = {0}; \
        time_t now = time(NULL); \
        struct tm *local_time = localtime(&now); \
        strftime(buf, sizeof(buf), "[ %Y-%m-%d %H:%M:%S ]", local_time); \
        sprintf(buf + strlen(buf), " [%s :: %s :: %d]: %s  \n", file, function, line, msg) ; \
        write(file_fd, buf, strlen(buf));\
        close(file_fd);\
    } \
    return 0;\
}
