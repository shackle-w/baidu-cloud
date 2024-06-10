#include "head.h"

// 初始化tcp监听
int initTcp(int *socketFd, char *ip, char *port){

    *socketFd = socket(AF_INET, SOCK_STREAM, 0);

    int reuse = 1;
    setsockopt(*socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(port));
    sockaddr.sin_addr.s_addr = inet_addr(ip);

    bind(*socketFd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));

    listen(*socketFd, 10); 

    return 0;
}
