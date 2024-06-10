#include "head.h"

// 向客户端传输文件
int sendFile(int net_fd, int file_fd){

    struct stat st;
    fstat(file_fd, &st);
    sendfile(net_fd, file_fd, 0, st.st_size);

    return 0;
}


// 接收文件存储到file_fd
int uploadFile(int net_fd, int file_fd){

    while(1){
        char buf[4096] = {0};
        ssize_t res = recv(net_fd,buf, sizeof(buf),MSG_WAITALL);
        if(res == 0){
            // 上传完成
            break;
        }
        write(file_fd, buf, res);
    }
    return 0;
}

// 断点续传
int uploadFileBreakPoint(int net_fd, FILE * file){

    while(1){
        char buf[4096] = {0};
        ssize_t res = recv(net_fd,buf, sizeof(buf),MSG_WAITALL);
        if(res == 0){
            // 上传完成
            break;
        }
        fwrite(buf, 1, res, file);
    }

    return 0;
}
