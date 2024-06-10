#include <55header.h>
#include <ctype.h>

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
    int command_flag;// 参考上面enum设置
    int parameter_flag;//1:携带参数; 0:没有参数
    int parameter_len;//参数字符长度
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

// 客户端运行状态信息
typedef struct run_status_s{
    int login_status;
    char user_name[100];
    int token_len;
    char token[1024];
    int path_len;
    char path[1000];
    int net_fd;
    struct HashMap *map;
}run_status_t;

typedef struct PutsData{
    char path[1000];
    char token[1024];
    char fileName[1024];
    char ip[100];
    char port[100];
    int flag; // 0: 上传文件, 1:下载文件
}PutsData;

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

// map相关操作
HashMap* createHashMap();
void freeHashMap(HashMap *table);
void insert(HashMap **table, char *key, char *value);
int hash(char *key, int capacity);
void resize(HashMap **table);
char * get(HashMap *map, char *key);
char* toString(HashMap *map);

// 队列相关
typedef struct PathItems {
    int size;
    char* items[100];
} PathItems;

int isEmpty(PathItems* items);
void addItem(PathItems* items, char* item);
char* removeFirstItem(PathItems* items);
char* getFirstItem(PathItems* items);
PathItems* createItems();
PathItems* createItemsPath(char *path);
int moveForwardOne(PathItems *items, int index);
int moveForwardTwo(PathItems *items, int index);

int initTcp(int *netFd, char *ip, char *port);

// 获得配置文件map
HashMap *parameterMap();

// 子线程的入口函数
void *threadMain(void *p);

// 拼接key=value数据
int stringCat(char *buf, char *key, char *value);

// 分发登陆注册
int distributeLoginOrRegister(run_status_t *runstatus);

// 分发命令
int distributeCommand(run_status_t *runstatus);

// 处理登陆
int doLogin(run_status_t *runstatus);
// 处理注册
int doRegister(run_status_t *runstatus);

// 数据报的参数处理
int getParameterMap(protocol_serve protocol, HashMap *map);

// 处理mkdir命令
int mkdirCommand(char *buf, run_status_t *runstatus);

// ls命令
int lsCommand(char *buf, run_status_t *runstatus);

// cd命令
int cdCommand(char *buf, run_status_t *runstatus);

// pwd命令
int pwdCommand(char *buf, run_status_t *runstatus);

// 删除文件夹
int rmCommand(char *buf, run_status_t *runstatus);

// 文件上传命令处理
int putsCommand(char *buf, run_status_t *runstatus);

// 文件下载命令处理
int getsCommand(char *buf, run_status_t *runstatus);

// pust的同步命令
int syncPuts(int net_fd);
// 文件上传逻辑处理
int filePuts(int net_fdi, PutsData *data);


// gest的同步命令
int syncGets(int net_fd);
// 文件下载逻辑处理
int fileGets(int net_fdi, PutsData *data);
// 上传文件

int sendFile(int net_fd, int file_fd);
// 续传文件
int sendFileBreakPoint(int net_fd, int file_fd, int currentServeFileSize);

// 下载文件
int downloadFile(int net_fd, char *fileName);

// 发送信息函数
int utilSend(int netFd, protocol_client *protocol, run_status_t *runstatus);


