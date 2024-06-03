/*
 * 文件名：socket.c
 * 创建时间：5.29
 * 作者：shackle
 * 描述：该文件实现连接socket的初始化
*/
#include "socket.h"

/*
 * 函数名：initTcp
 * 函数作用：初始化客户端socket
 * 参数：
 *  -int *net_fd：创建的连接socket
 *  -char *ip：服务器的ip地址
 *  -char *port：服务器的端口号
 * 返回值：成功返回0，失败返回-1
*/
int initTcp(int *net_fd, char *ip, char *port){
    // 1. 封装ip和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));
    

    // 2. 初始化socket
    *net_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(*net_fd == -1){
        LOG(ERROR, "创建socket失败");
        return -1;
    }

    
    // 解除TIME_WAIT等待时
    int reuse = 1;
    int res_setsockope = setsockopt(*net_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    // 3. 连接到服务器
    int res = connect(*net_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(res, -1, "connect");
    if(res == -1){
        LOG(ERROR, "连接到服务器失败");
        return -1;
    }
}
