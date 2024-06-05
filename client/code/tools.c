/*
 * 函数名：tools
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件的作用是实现发送信息的工具类
*/

#include "tools.h"

/*
 * 函数名：sendData
 * 函数作用：封装发送信息的函数
 * 参数：
 *  -int net_fd：连接的socket
 *  -int tag：发送信号的标签
 *  -void *data：要发送的信息
 *  -int data_len：要发送的信息长度
 * 返回值：成功0，失败-1
*/
int sendData(int net_fd, int tag, void *data, int data_len){
    // 发送信息标签
    if(send(net_fd, &tag, sizeof(int), MSG_NOSIGNAL) == -1){
        LOG(ERROR, "发送标签失败");
        return -1;
    }
    LOG(INFO, "客户端发送的标签：%d\n", tag);
    // 发送数据长度
    if(send(net_fd, &data_len, sizeof(int), MSG_NOSIGNAL) == -1){
        LOG(ERROR, "发送数据长度失败");
        return -1;
    }
    LOG(INFO,"客户端发送信息的长度：%d\n", data_len);
    // 发送数据
    if(send(net_fd, (char *)data, data_len, MSG_NOSIGNAL) == -1){
        LOG(ERROR, "发送数据失败");
        return -1;
    }

    return 0;
}



/*
 * 函数名：sendRegister
 * 函数作用：向服务器发送注册信息
 * 参数：
 *  -int net_fd：连接的socket
 *  -char *information：注册需要的信息
 * 返回值：成功返回0，失败返回-1
*/
int sendRegister(int net_fd, char *information){
    int len = strlen(information) + 1;
    return sendData(net_fd, 1, information, len);

}

/*
 * 函数名：sendLogin
 * 函数作用：向服务器发送登录信息
 * 参数：
 *  -int net_fd：连接的socket
 *  -char *information：登录需要的信息
 * 返回值：成功返回0，失败返回-1
*/
int sendLogin(int net_fd, char *information){
    int len = strlen(information) + 1;
    return sendData(net_fd, 2, information, len);
}

/*
 * 函数名：sendShort
 * 函数作用：向服务器发送短命令
 * 参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：封装的user信息
 * 返回值：成功返回0，失败返回-1
*/
int sendShort(int net_fd, user_t *user){
    int len = sizeof(*user);
    LOG(INFO, "发送的命令：", user->command);
    return sendData(net_fd, 3, user, len);
}
/*
 * 函数名：sendLong
 * 函数作用：向服务器发送长命令
 * 参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：封装的user信息
 * 返回值：成功返回0，失败返回-1
*/
int sendLong(int net_fd, user_t *user){
    int len = sizeof(*user);
    LOG(INFO, "发送的命令：", user->command);
    return sendData(net_fd, 4, user, len);
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
    int len = sizeof(*user);
    return sendData(net_fd, 5, user, len);
}

/*
 * 函数名：sendOver
 * 函数作用：向服务器发送结束信号
 * 参数：
 *  -int net_fd：连接的socket
 * 返回值：成功返回0，失败返回-1；
*/
int sendOver(int net_fd){
    int tag = 6;
    return sendData(net_fd, tag, NULL, 0);
}


/*
 * 函数名：printfHint
 * 函数作用：打印一个提示符
 * 参数：
 *  -char *username：用户名
 *  -char *path：工作路径
 * 返回值：void类型
*/
void printfHint(char *username, char *path){
    char result[SMALL_BUF];
    if(strcmp(path, "/") == 0){
        strcpy(result, "/");
    }else{
        const char *last_slash = strrchr(path, '/');
        if (last_slash != NULL) {
            // Copy the part after the last '/'
            strcpy(result, last_slash + 1);
        } else {
            // If '/' is not found, copy the entire string
            strcpy(result, path);
        }
    }

    printf("%s:%s$ ", username, result);

}

