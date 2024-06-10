#include "head.h"

int mkdirCommand(char *buf, run_status_t *runstatus){
    if(buf[5] == 0){
        printf("mkdir 命令需要参数 \n");
        return 0;
    }
    if(buf[5] != ' '){
        printf("命令非法 \n");
        return 0;
    }

    strtok(buf, " ");
    char *dirName = strtok(NULL, "=");
    
    char *p = dirName;
    while(*p){
        if(!isalnum(*p)){
            printf("文件夹名字不允许非字母和数字的特殊字符 \n");
            return 0;
        }
        p++;
    }

    // 构建传输协议内容
    protocol_client protocol;
    bzero(&protocol, sizeof(protocol));

    protocol.command_flag = MKDIR;
    protocol.parameter_flag = 1;
    stringCat(protocol.buf, "token", runstatus->token);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "path", runstatus->path);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "dirName", dirName);

    protocol.parameter_len = strlen(protocol.buf);
    protocol.parameter_num = 3;

    // 发送创建文件夹行为
    int send_len = sizeof(int)*4 + protocol.parameter_len;
    send(runstatus->net_fd, &send_len, sizeof(int), MSG_NOSIGNAL);
    send(runstatus->net_fd, &protocol, send_len, MSG_NOSIGNAL);    

    // 接收对端数据
    protocol_serve protocol_serve;
    bzero(&protocol_serve, sizeof(protocol_serve));

    int recv_len = 0;
    recv(runstatus->net_fd, &recv_len, sizeof(int), MSG_WAITALL);
    recv(runstatus->net_fd, &protocol_serve, recv_len, MSG_WAITALL);

    if(protocol_serve.status == 0){
        // 创建成功
    }else{
        // 根据返回值, 判断是否有同名文件夹存在
        printf("创建文件夹失败, 检查是否是同名文件  \n");
    }
    return 0;
}
