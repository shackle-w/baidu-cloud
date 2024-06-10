#include "head.h"

// 文件上传操作
int filePuts(int net_fd, PutsData *data){

    char fileName[200] = {0};
    memcpy(fileName, data->fileName, strlen(data->fileName));

    // 打开文件, 并获得一些文件信息
    int file_fd = open(fileName, O_RDONLY);
    if(file_fd == -1){
        printf("文件不存在, 或者打开权限不足 \n");
        pthread_exit(0);
    }

    // 获得文件信息:文件长度
    struct stat stat_file;
    fstat(file_fd, &stat_file);
    off_t fileSize = stat_file.st_size;

    // 获得文件的hash值
    char md5_hash[1024] = {0}; 
    char sha1_hash[1024] = {0}; 

    char command[1024] = {0};
    sprintf(command, "%s %s", "sha1sum", fileName);
    FILE *pipe = popen(command, "r");
    fread(sha1_hash, 1, sizeof(sha1_hash), pipe);

    bzero(command, sizeof(command));
    sprintf(command, "%s %s", "md5sum", fileName);
    pipe = popen(command, "r");
    fread(md5_hash, 1, sizeof(md5_hash), pipe);

    // 截掉后面的文件名字
    int i=0;
    while(md5_hash[i] != 0){
        if(md5_hash[i] == ' '){
            md5_hash[i] = 0;
            break;
        }
        i++;
    }
    i=0;
    while(sha1_hash[i] != 0){
        if(sha1_hash[i] == ' '){
            sha1_hash[i] = 0;
            break;
        }
        i++;
    }

    // 把下列信息发送给服务端:
    // token
    // path路径
    // 文件名字
    // 文件大小
    // sha1值
    // md5值

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
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "sha1", sha1_hash);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "md5", md5_hash);

    char strFileSize[20];
    sprintf(strFileSize, "%ld", fileSize);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "fileSize", strFileSize);

    protocol.parameter_len = strlen(protocol.buf);
    protocol.parameter_num = 6;

    int send_len = sizeof(int)*4 + protocol.parameter_len;
    send(net_fd, &send_len, sizeof(int), MSG_NOSIGNAL);
    send(net_fd, &protocol, send_len, MSG_NOSIGNAL);    

    // 接收对端数据
    protocol_serve protocol_serve;
    bzero(&protocol_serve, sizeof(protocol_serve));

    int recv_len = 0;
    recv(net_fd, &recv_len, sizeof(int), MSG_WAITALL);
    recv(net_fd, &protocol_serve, recv_len, MSG_WAITALL);

    // 接收返回值,  根据不同返回状态
    if(protocol_serve.status == 0){
        // 当前路径新传逻辑, 上传文件
        sendFile(net_fd, file_fd);
        close(file_fd);

    }else if(protocol_serve.status == 11){
        printf("当前路径下可能存在同名文件夹 \n");
    }else if(protocol_serve.status == 12){
        printf("当前路径下可能存在同名文件 \n");
    }else if(protocol_serve.status == 20){
        // 当前路径下断点续传
        bzero(&protocol_serve, sizeof(protocol_serve));

        int recv_len = 0;
        recv(net_fd, &recv_len, sizeof(int), MSG_WAITALL);
        recv(net_fd, &protocol_serve, recv_len, MSG_WAITALL);
        HashMap *map = createHashMap();
        getParameterMap(protocol_serve, map);
        int currentServeFileSize = atoi(get(map, "currentServeFileSize"));

        // 续传文件
        sendFileBreakPoint(net_fd, file_fd, currentServeFileSize);

        close(file_fd);
    }else if(protocol_serve.status == 21){
        // 不同路径下断点续传
        // 先收大小
        // 再做续传
        bzero(&protocol_serve, sizeof(protocol_serve));

        int recv_len = 0;
        recv(net_fd, &recv_len, sizeof(int), MSG_WAITALL);
        recv(net_fd, &protocol_serve, recv_len, MSG_WAITALL);
        HashMap *map = createHashMap();
        getParameterMap(protocol_serve, map);
        int currentServeFileSize = atoi(get(map, "currentServeFileSize"));

        // 续传文件
        sendFileBreakPoint(net_fd, file_fd, currentServeFileSize);

        close(file_fd);
        
    }else if(protocol_serve.status == 30){
        printf("文件秒传成功 \n");
    }
    return 0;
}

// 文件上传同步步骤
int syncPuts(int net_fd){

    // 发送文件上传的"预备"请求, 让服务端创建子进程
    protocol_client protocol;
    bzero(&protocol, sizeof(protocol));
    protocol.command_flag = PUTS_SYNC;

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
        // 文件上传同步成功
    }else{
        // 文件上传同步失败
        printf("无法上传文件 \n");
        pthread_exit(0);
    }

    return 0;
}
int putsCommand(char *buf, run_status_t *runstatus){

    // puts命令后面没有跟内容
    if(buf[4] == 0){
        printf("请输入要上传文件的名子 \n");
        return 0;
    }
    // 判断: putsr, putsx....
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

    // 构建交给子线程进行上传文件的数据
    PutsData data;
    memset(&data, 0, sizeof(data));
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

