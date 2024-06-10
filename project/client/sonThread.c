#include "head.h"


void *threadMain(void *p){
    PutsData *data = (PutsData *)p;

    // 建立链接
    int net_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(data->port));
    sockaddr.sin_addr.s_addr = inet_addr(data->ip);
    connect(net_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));

    if(data->flag == 0){
        // 文件上传同步请求
        syncPuts(net_fd);
        // 文件上传操作
        filePuts(net_fd, data);
    }else{
        // 文件下载同步请求
        syncGets(net_fd);
        // 文件下载操作
        fileGets(net_fd, data);
    }

    close(net_fd);
    return NULL;
}

