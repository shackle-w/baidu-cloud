/*
 * 文件名：socket.c
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件的作用是初始化socket连接
*/

#include "socket.h"

/*
 * 函数名：initSocket
 * 描述：根据端口和IP构建服务端的网络监听
 * 参数：
 *  -int *socketfd：socket的文件描述符，用于服务器和客户端通信
 *  -char *ip：服务器端的ip地址
 *  -char *port：服务器端的端口号
 * 返回值：socketfd
 */
int initSocket(int *socketfd, char *ip, char *port){
    // 封装socketfd
    *socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(*socketfd == -1){
        LOG(ERROR, "创建socket失败");
        return -1;
    }
    // 接触TIME_WAIT等待时
    int reuse = 1;
    if(setsockopt(*socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1){
        LOG(ERROR, "取消等待时间失败");
        return -1;
    }
    

    // 构建结构体sockadd_in
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

    // 绑定端口
    int res_bind = bind(*socketfd, (struct sockaddr*)&addr, sizeof(addr));
    if(res_bind == -1){
        LOG(ERROR, "bind绑定端口出错");
        return -1;
    }

    // 监听端口
    int res_listen = listen(*socketfd, 10);
    if(res_listen == -1){
        LOG(ERROR, "listen监听端口出错");
        return -1;
    }

    return 0;
}
