/*
 * 文件名：clientStruct.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件存放项目同一需要的结构体信息
*/

#ifndef __CLIENTSTRUCTH__
#define __CLIENTSTRUCTH__

#include "my_header.h"

struct HashMap;
enum {
    LOGIN = 100,
    REGISTER_NAME = 101,
    REGISTER_PASSWORD = 102,
    LS = 110,
    CD = 120,
    PATHCHECK = 121,
    PWD = 130,
    RM = 140,
    RMR = 141,
    MKDIR = 150,
    PUTS = 200,
    PUTS_SYNC = 201,
    GETS = 300,
    GETS_SYNC = 301,
};
// 构建客户端传输协议
typedef  struct client_protocol_s{
    int command_flag; // 参考上面enum设置
    int parameter_flag; // 1:携带参数; 0:没有参数
    int parameter_len; // 参数字符长度
    int parameter_num; // 参数个数
    char buf[BUF_SIZE]; // 参数数组
}protocol_client;

// 构建服务器响应协议格式
typedef struct serve_protocol_s{
    int status; // 状态码: 0正常; -1异常 ...
    int parameter_flag; // 1:携带参数, 0:无参数
    int parameter_len; // 参数长度
    int parameter_num; // 参数个数
    char buf[BUF_SIZE]; // 参数数组
}protocol_serve;

// 客户端运行状态信息
typedef struct run_status_s{
    int login_status; // 是否是登录状态
    char user_name[SMALL_BUF]; // 用户名
    int token_len; // 服务器返回的token长度
    char token[BUF_SIZE]; // 服务器返回的token
    int path_len; // 当前所处路径的长度
    char path[BUF_SIZE]; // 当前所处的路径
    int net_fd; // 用于通信的socket结点
    struct HashMap *map; // 客户端的一些信息
}run_status_t;

// 文件相关的结构体
typedef struct PutsData{
    char path[BUF_SIZE]; // 当前所处的路径
    char token[BUF_SIZE]; // 用户的token信息
    char fileName[BUF_SIZE]; // 文件名
    char ip[SMALL_BUF]; // 服务器的IP地址
    char port[SMALL_BUF]; // 服务器通信的端口号
    int flag; // 0: 上传文件, 1:下载文件
}PutsData;

// 构建用户信息的结点
typedef struct Node {
    char *key; // 用户信息的key值
    char *value; // 用户信息的value值
    struct Node *next; // 指向下一个结点
} Node;

typedef struct HashMap {
    Node **table;  // 指针数组
    int capacity; // HashMap可以存放的键值对个数
    int size; // HahMap存放的键值对个数
} HashMap;

#endif
