#include "head.h"

// 删除一级文件或者文件夹
int rmFileOrDir(char *fileOrDirName, run_status_t * runstatus){

    // 构建传输协议内容
    protocol_client protocol;
    bzero(&protocol, sizeof(protocol));

    protocol.command_flag = RM;
    protocol.parameter_flag = 1;
    stringCat(protocol.buf, "token", runstatus->token);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "path", runstatus->path);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "fileOrDirName", fileOrDirName);

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
        // 删除成功
    }else{
        // 删除失败
        printf("删除失败请检查参数是否正确  \n");
    }
    return 0;
}

// 递归删除文件夹
int rmRFileOrDir(char *fileOrDirName, run_status_t * runstatus){
    
    // 构建传输协议内容
    protocol_client protocol;
    bzero(&protocol, sizeof(protocol));

    protocol.command_flag = RMR;
    protocol.parameter_flag = 1;
    stringCat(protocol.buf, "token", runstatus->token);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "path", runstatus->path);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "fileOrDirName", fileOrDirName);

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
        // 删除成功
    }else{
        // 删除失败
        printf("删除失败请检查参数是否正确  \n");
    }

    return 0;
}

// 处理rm命令和参数
int rmCommand(char *buf, run_status_t *runstatus){
    // 可以接受的命令
    // rm fileName
    // rm dirName
    // rm -r fileName
    // rm -r dirName

    // rm后没有输入要删除文件或者文件夹
    if(buf[2] == 0){
        printf("请输入要删除的内容 \n");
        return 0;
    }
    // 判断: rmr, rmx....
    if(buf[2] != ' '){
        printf("非法命令 \n");
        return 0;
    }
    
    if(strncmp(buf, "rm -r ", 6) == 0){
        // rm -r 参数命令
        
        // 判断参数是否合法
        //char *p = buf;
        //p = p+6;
        //while(*p){
        //    if(!isalnum(*p)){
        //        printf("参数非法 \n");
        //        return 0;
        //    }
        //    p++;
        //}
        
        // 获取参数
        strtok(buf, " ");
        strtok(NULL, " ");
        char fileOrDirName[100] = {0};
        memcpy(fileOrDirName, strtok(NULL, "="), sizeof(fileOrDirName));

        // printf("rm -r : fileOrDirName: %s \n", fileOrDirName);

        rmRFileOrDir(fileOrDirName, runstatus);
    }else{
        // rm 参数命令
        // 判断参数是否合法
        //char *p = buf;
        //p = p+3;
        //while(*p){
        //    if(!isalnum(*p)){
        //        printf("参数非法 \n");
        //        return 0;
        //    }
        //    p++;
        //}
        
        // 获取参数
        strtok(buf, " ");
        char fileOrDirName[100] = {0};
        memcpy(fileOrDirName, strtok(NULL, "="), sizeof(fileOrDirName));

        // printf("rm : fileOrDirName: %s \n", fileOrDirName);
        
        rmFileOrDir(fileOrDirName, runstatus);
    }

    return  0;
}

