#include "head.h"


// 文件下载操作
int fileGets(int net_fd, PutsData *data){
    char fileName[200] = {0};
    memcpy(fileName, data->fileName, strlen(data->fileName));

    // 把下列信息发送给服务端:
    // token
    // path路径
    // 文件名字

    // 构建传输协议
    protocol_client protocol;
    bzero(&protocol, sizeof(protocol));

    protocol.command_flag = PUTS;
    protocol.parameter_flag = 1;
    stringCat(protocol.buf, "token", data->token);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "path", data->path);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "fileName", fileName);

    protocol.parameter_len = strlen(protocol.buf);
    protocol.parameter_num = 3;
    int send_len = sizeof(int)*4 + protocol.parameter_len;
    send(net_fd, &send_len, sizeof(int), MSG_NOSIGNAL);
    send(net_fd, &protocol, send_len, MSG_NOSIGNAL); 

    // 接收对端数据
    protocol_serve protocol_serve;
    bzero(&protocol_serve, sizeof(protocol_serve));

    int recv_len = 0;
    recv(net_fd, &recv_len, sizeof(int), MSG_WAITALL);
    recv(net_fd, &protocol_serve, recv_len, MSG_WAITALL);

    if(protocol_serve.status == 0){
        // 开始接收文件
        downloadFile(net_fd, fileName);

    }else{
        printf("当前路径下可能不存在该文件, 检查参数 \n");
    }
    return 0;
}

// 文件下载同步步骤
int syncGets(int net_fd){

    // 发送文件下载的"预备"请求, 让服务端创建子进程
    protocol_client protocol;
    bzero(&protocol, sizeof(protocol));
    protocol.command_flag = GETS_SYNC;

    int send_len = sizeof(int)*4 + protocol.parameter_len;
    send(net_fd, &send_len, sizeof(int), MSG_NOSIGNAL);
    send(net_fd, &protocol, send_len, MSG_NOSIGNAL); 

    // 接收对端数据
    protocol_serve protocol_serve;
    bzero(&protocol_serve, sizeof(protocol_serve));

    int recv_len = 0;
    recv(net_fd, &recv_len, sizeof(int), MSG_WAITALL);
    recv(net_fd, &protocol_serve, recv_len, MSG_WAITALL);

    if(protocol_serve.status == 0){
        // 文件下载同步成功
    }else{
        // 文件下载同步失败
        printf("无法上传文件 \n");
        pthread_exit(0);
    }

    return 0;
}

int getsCommand(char *buf, run_status_t *runstatus){

    // gets命令后面没有跟内容
    if(buf[4] == 0){
        printf("请输入要下载文件的名子 \n");
        return 0;
    }
    // 判断: getsr, getsx....
    if(buf[4] != ' '){
        printf("非法命令 \n");
        return 0;
    }

    // 获得文件名字
    char fileName[1024] = {0};
    for(int i=0; i<strlen(buf); i++ ){
        if(buf[i+5] == 0){
            break;
        }
        fileName[i] = buf[i+5]; 
    }

    // 构建交给子线程进行下载文件的数据
    PutsData data;
    memset(&data, 0, sizeof(data));
    data.flag = 1;
    memcpy(data.token, runstatus->token, strlen(runstatus->token));
    memcpy(data.path, runstatus->path, strlen(runstatus->path));
    memcpy(data.fileName, fileName, strlen(fileName));
    char *ip = get(runstatus->map, "ip");
    memcpy(data.ip, ip, strlen(ip));
    char *port = get(runstatus->map, "port");
    memcpy(data.port, port, strlen(port));

    pthread_t son_id;
    pthread_create(&son_id, NULL, threadMain, &data);
    usleep(1000);

    return  0;
}

