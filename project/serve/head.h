#include <55header.h>
#include <mysql/mysql.h>


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

// -----------超时退出的相关操作
// 新链接到来, 加入记录
int insetConnect(int netFd,  ConnectsStatus * connsStatus);
// 更新连接的说话时间记录
int updateConnect(int netFd, ConnectsStatus * connsStatus);
// 移除指定链接的记录
int deleteConnect(int netFd, ConnectsStatus *connsStatus);
// 检查连接状态
int checkConnect(int epollFd, ConnectsStatus * connsStatus);


// ----------队列相关操作
// 入队列
int enQueue(queue_t *queue, int net_fd, int flag);
// 出队列
int deQueue(queue_t *queue);

// ---------map相关操作
HashMap* createHashMap();
void freeHashMap(HashMap *table);
void insert(HashMap **table, char *key, char *value);
int hash(char *key, int capacity);
void resize(HashMap **table);
char * get(HashMap *map, char *key);
char * toString(HashMap *map);

// map加载数据
HashMap *parameterMap();

// 初始化子线程
int initPool(pool_t *pPool);

// 子线程的入口函数
void * threadFun(void *p);

// 初始化socket连接
int initTcp(int *socketFd, char *ip, char *port);

// 添加epoll监听
int epollAdd(int epollFd, int netFd);
// 删除epoll监听
int epollRemove(int epollFd, int netFd);

// 向指定客户端传输文件
int transFile(int netFd);

// 处理客户端发过来的命令
int doRequest(int netFd, pool_t* pool);

// 从字符串中切割出参数,的到参数map
int getParameterMap(protocol_client protocol, HashMap *map);

// 获得随机盐值
char* salt();
// 根据盐值获得散列密码
char* cryptPassword(char *password, char *salt);

// 处理登陆请求
int loginRequest(int netFd, protocol_client protocol, pool_t* pool);
// 处理注册请求: 验证账号
int registerNameRequest(int netFd, protocol_client protocol, pool_t* pool);
// 处理注册请求: 携带账号密码
int registerPasswordRequest(int netFd, protocol_client protocol, pool_t* pool);

// 获得一个数据库连接
int getConnect(sql_connect_t *sqlConnect, pool_t* pool);
// 使用完毕之后, 把数据库连接还回去
int setConnect(sql_connect_t sqlConnect, pool_t* pool);

// 使用用户的id产生一个token 
int getToken(int userId, char *buf );

// 拼接key=value数据
int stringCat(char *buf, char *key, char *value);

// 处理mkdir命令
int mkdirRequest(int netFd, protocol_client protocol, pool_t* pool);
// 处理ls命令
int lsRequest(int netFd, protocol_client protocol, pool_t* pool);

// 根据token, 对token解析获得userId
int getUserId(char * token);

// 检查路径
int pathCheckRequest(int netFd, protocol_client protocol, pool_t* pool);

// 回复给客户端同步文件上传的回复
int syncPuts(int net_fd);
// 处理puts命令
int dealPuts(int net_fd, pool_t *pool);
// 接收文件
int uploadFile(int net_fd, int file_fd);
// 断点续传
int uploadFileBreakPoint(int netFd, FILE * file);

// 回复给客户端同步文件下载的回复
int syncGets(int net_fd);
// 处理gets命令
int dealGets(int net_fd, pool_t *pool);

// 传输文件
int sendFile(int net_fd, int  file_fd);

// 删除命令
int rmRequest(int netFd, protocol_client protocol, pool_t * pool);
int rmRRequest(int netFd, protocol_client protocol, pool_t * pool);


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
// -----------------------
