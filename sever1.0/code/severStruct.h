/*
 * 文件名：clientStruct.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件存放项目同一需要的结构体信息
*/

#ifndef __SEVERSTRUCTH__
#define __SEVERSTRUCTH__

#include "my_header.h"

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
    char buf[1460];// 参数数组
}protocol_client;

// 构建服务器响应协议格式
typedef struct serve_protocol_s{
    int status; // 状态码: 0正常; -1异常 ...
    int parameter_flag; // 1:携带参数, 0:无参数
    int parameter_len; // 参数长度
    int parameter_num; // 参数个数
    char buf[1460]; // 参数数组
}protocol_serve;

// 队列中一个结点
typedef struct node_s{
    // 客户端连接的文件描述符
    int net_fd;
    // 0:上传文件, 1:下载文件
    int flag;
    // 指向下一个结点的来凝结
    struct node_s *pNext;

}node_t;

// 队列
typedef struct queue_s{
    // 队列的头
    node_t *head;
    // 队列尾
    node_t *end;
    // 队列中存储了多少数据
    int size;
}queue_t;

typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

typedef struct HashMap {
    Node **table;  // 指针数组
    int capacity;
    int size;
} HashMap;

// 连接记录:用于超时踢出
typedef struct ConnectRecord  {
    int netFd;
    time_t lastTime;
    struct ConnectRecord *next;
}ConnectRecord;

// 所有连接状态: 用于超时踢出
typedef struct ConnectsStatus {
    // 循环时间队列
    ConnectRecord *table[30];

    // 记录每一个连接在循环时间队列中的下标位置: 假设用户同时在线上限为1024
    int index[1024]; 

} ConnectsStatus;

// 数据库连接类型
typedef struct sql_connect_s{
    MYSQL connect; // 数据库连接
    int useFlag; // 是否被使用:0空闲,1使用
    int connsIndex; // 连接编号 
} sql_connect_t;

typedef struct use_msg_s{
    int userId;
    char userName[255];
    char salt[255];
    char cryptPassword[255];
}use_msg_t;

typedef struct file_msg_s{
    int id;
    int userId;
    char path[255];
    int fatherPathId;
    int fileType;
    char fileName[255];
    char md5[50];
    char sha1[50];
    int uploadStatus;

}file_msg_t;

// 整个服务器状态记录
typedef struct pool_s{
    pthread_t *poolThreadIds;
    int poolThreadNum;

    queue_t  taskQueue;

    pthread_mutex_t threadLock;
    pthread_cond_t cond;


    // 数据库连接数组
    sql_connect_t *conns;
    // 维护的数据库连接个数
    int connNum;
    pthread_mutex_t connLock;
    pthread_cond_t connCond;

    HashMap *map;
    int exitTag;


    // 客户端连接时间记录
    ConnectsStatus connsStatus; 
}pool_t;

#endif
