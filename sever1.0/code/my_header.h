#ifndef MY_HEADER_H
#define MY_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <time.h>
#include <sys/wait.h>
#include <dirent.h>
#include <syslog.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <mysql/mysql.h>
#include <ctype.h>

#define BUF_SIZE 1024
#define PAGE_SIZE 4096
#define SMALL_BUF 100

// 检查命令行参数数量是否符合预期
#define ARGS_CHECK(argc, expected) \
    do { \
        if ((argc) != (expected)) { \
            fprintf(stderr, "args num error!\n"); \
            exit(1); \
        } \
    } while (0)                      

// 检查返回值是否是错误标记,若是则打印msg和错误信息
#define ERROR_CHECK(ret, error_flag, msg) \
    do { \
        if ((ret) == (error_flag)) { \
            perror(msg); \
            exit(1); \
        } \
    } while (0)

// 检查线程返回值是否是错误标记，若是则打印msg和错误信息
#define THREAD_ERROR_CHECK(ret, msg)\
	{if(ret!=0)\
		{fprintf(stderr, "%s:%s \n", msg,strerror(ret));\
		}\
	}
#endif
