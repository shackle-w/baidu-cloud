/*
 * 文件名：initTcp.c
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件的作用是实现建立socket连接
*/

#include "initTcp.h"


int initTcp(int *netFd, char *ip, char *port){

    *netFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(port));
    sockaddr.sin_addr.s_addr = inet_addr(ip);
    connect(*netFd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));

    return 0;
}
