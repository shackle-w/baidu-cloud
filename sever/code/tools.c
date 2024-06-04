/*
 * 函数名：tools
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件的作用是实现发送信息的工具类
*/

#include "tools.h"

#define SEND_SUCCESS 0
#define SEND_FAILURE -1
#define REGISTER_TAG 1
#define LOGIN_TAG 2
#define INFORMATION_TAG 3
#define ERROR_TAG 4
#define FILE_TAG 5
#define OVER_TAG 6

/*
 * 函数名：sendData
 * 函数作用：向客户端发送数据信息
 * 参数
 *  -int net_fd：通信使用的socket
 *  -int tag：信号类型
 *  -void *info：要发送的信息
 *  -size_t info_len：要发送信息的长度
 * 返回值：0成功， 1失败
*/
int sendData(int net_fd, int tag, void *info, size_t info_len){

    if(send(net_fd, &tag, sizeof(int), MSG_NOSIGNAL) == -1){
        LOG(ERROR, "标识发送失败");
        return SEND_FAILURE;
    }
    LOG(INFO, "服务器发送的标识：%d\n", tag);
    if(send(net_fd, &info_len, sizeof(int), MSG_NOSIGNAL) == -1){
        LOG(ERROR, "信息长度发送失败");
        return SEND_FAILURE;
    }
    LOG(INFO, "服务器发送的信息长度：%ld\n", info_len);
    if(send(net_fd, (char *)info, info_len, MSG_NOSIGNAL) == -1){
        LOG(ERROR, "信息发送失败");
        return SEND_FAILURE;
    }
    LOG(INFO, "服务器发送的信息:%s\n", (char*) info);      
    return SEND_SUCCESS;
}


/*
 * 函数名：sendRegister
 * 函数作用：向客户端发送注册信息
 * 参数：
 *  -int net_fd：连接的socket
 *  -char *information：注册需要的信息
 * 返回值：成功返回0，失败返回-1
*/
int sendRegister(int net_fd, char *information){
    return sendData(net_fd, REGISTER_TAG, information, strlen(information) + 1);
}

/*
 * 函数名：sendLogin
 * 函数作用：向客户端发送登录信息
 * 参数：
 *  -int net_fd：连接的socket
 *  -char *information：登录需要的信息
 * 返回值：成功返回0，失败返回-1
*/
int sendLogin(int net_fd, char *information){
    return sendData(net_fd, LOGIN_TAG, information, strlen(information) + 1);
}

/*
 * 函数名：sendInformation
 * 函数作用：向客户端发送返回信息
 * 参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：封装的user信息
 * 返回值：成功返回0，失败返回-1
*/
int sendInformation(int net_fd, user_t *user){
    return sendData(net_fd, INFORMATION_TAG, user, sizeof(user_t));
}
/*
 * 函数名：sendError
 * 函数作用：向客户端发送返回错误信息
 * 参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：封装的user信息
 * 返回值：成功返回0，失败返回-1
*/
int sendError(int net_fd, user_t *user){
    return sendData(net_fd, ERROR_TAG, user, sizeof(user_t));
}

/*
 * 函数名：sendFile
 * 函数作用：向服务器发送文件
 * 参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：封装的user信息
 * 返回值：成功返回0，失败返回-1；
*/
int sendFile(int net_fd, user_t *user){
    return sendData(net_fd, FILE_TAG, user, sizeof(user_t));
}

/*
 * 函数名：sendOver
 * 函数作用：向服务器发送结束信息
 * 参数：
 *  -int net_fd：连接的socket
 * 返回值：成功返回0，失败返回-1；
*/
int sendOver(int net_fd){
    int tag = OVER_TAG;
    if(send(net_fd, &tag, sizeof(int), MSG_NOSIGNAL) == -1){
        LOG(ERROR, "发送结束信息失败");
        return -1;
    }
    return 0;
}

