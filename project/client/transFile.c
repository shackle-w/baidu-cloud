#include "head.h"

int downloadFile(int net_fd, char *fileName){

    char* dir_name = "download";
    struct stat st = {0};
    // 检查目录是否存在
    if (stat(dir_name, &st) == -1) {
        // 如果不存在则创建目录
        mkdir(dir_name, 0777);
    }

    char bufName[100] = {0};
    sprintf(bufName, "./download/%s", fileName);

    printf("%s \n", bufName );
    int file_fd = open(bufName, O_RDWR|O_CREAT, 0666); 

    while(1){
        char buf[4096] = {0};
        ssize_t res = recv(net_fd, buf, sizeof(buf), MSG_WAITALL);
        if(res == 0){
            // 下载完成
            break;
        }
        write(file_fd, buf, res);
    }

    close(file_fd);
    return  0;
}


// 向服务端传文件
int sendFile(int  net_fd, int file_fd){

    struct stat st;
    fstat(file_fd, &st);

    // 使用sendfile发送文件
    sendfile(net_fd, file_fd, 0, st.st_size);

    return 0;
}    

// 续传文件
int sendFileBreakPoint(int net_fd, int file_fd, int currentServeFileSize){

    struct stat st;
    fstat(file_fd, &st);

    off_t offset = currentServeFileSize;
    // 使用sendfile发送文件
    sendfile(net_fd, file_fd, &offset, st.st_size-offset);

    return 0;
}
