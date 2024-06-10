#include "head.h"

int cdCommand(char *buf, run_status_t *runstatus){
    // cd后没有输入要去往的路径
    if(buf[2] == 0){
        printf("请输入要cd到的路径 \n");
        return 0;
    }
    // 判断: cdr, cdx....
    if(buf[2] != ' '){
        printf("非法命令 \n");
        return 0;
    }

    // 从标准输入中， 获得要cd的路径参数 
    strtok(buf, " ");
    char dirName[1024] = {0};
    memcpy(dirName, strtok(NULL, "="), sizeof(dirName));

    if(dirName[0] == '/' && strlen(dirName) ==1){
        bzero(runstatus->path, strlen(runstatus->path));
        // 走到这意味着验证路径没有问题, 可以修改路径等
        memcpy(runstatus->path, "/", 1);

        return 0;
    }

    // 切割参数存储到栈中
    PathItems *items = createItems();
    if(dirName[0] == '/'){
        items =createItems();
    }else{
        items = createItemsPath(runstatus->path);
    }

    // 走到这， 意味着从当前目录为起始出发点， 开始切换目录
    char temp[1024] = {0};
    int tempIndex = 0;
    for(int i=0; i<=strlen(dirName); i++){
        if(!isalnum(dirName[i]) && dirName[i] != '.' && dirName[i] != '/' && dirName[i] != 0){
            printf("文件夹名字不允许非字母和数字的特殊字符 \n");
            return 0;
        }
        if(i==0 && dirName[i] == '/' ){
            continue;
        }

        if(dirName[i] != '/' && dirName[i] != 0){
            temp[tempIndex++] = dirName[i];
        }else if(dirName[i] != 0){
            addItem(items, temp);
            bzero(temp, sizeof(temp));      
            tempIndex = 0;
        }else if(i != 0){
            addItem(items, temp);
            break;
        }
    }

    // 目录处理
    for(int i=0; i < items->size; i++ ){
        char item[1024] = {0};
        memcpy(item, items->items[i], sizeof(items->items[i]));

        if(strlen(item)==2 && strncmp(item, "..", 2) == 0){
            // 向上跳一级, 
            if(i == 0){
                moveForwardOne(items, i);
            }else{
                moveForwardTwo(items, i);
            }
            i=-1;
            continue;
        }
        if(strlen(item)==0){
            continue;
        }

        char *p = item;
        while(*p){
            if(!isalnum(*p)){
                printf("文件夹名字不允许非字母和数字的特殊字符 \n");
                return 0;
            }
            p++;
        }
    }

    // 构建一个新的字符串, 存储构建的新的路径
    char newPath[1024] = {0};
    strcpy(newPath, "/");

    while(getFirstItem(items) != NULL){

        char *item = removeFirstItem(items);
        if(strlen(item)==0){
            continue;
        }

        // 构建传输协议
        protocol_client protocol;
        bzero(&protocol, sizeof(protocol));

        protocol.command_flag = PATHCHECK;
        protocol.parameter_flag = 1;
        stringCat(protocol.buf, "token", runstatus->token);
        strcat(protocol.buf, "&");
        stringCat(protocol.buf, "path", newPath);
        strcat(protocol.buf, "&");
        stringCat(protocol.buf, "subPath", item);

        protocol.parameter_len = strlen(protocol.buf);
        protocol.parameter_num = 3;

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
            // 存在这个路径: 路径没有问题
            strcat(newPath, item);
            strcat(newPath, "/");
        }else{
            // 不存在这个路径: 路径有问题
            printf("跳转失败,请检查路径是否正确 \n");
            return 0;
        }
    }

    bzero(runstatus->path, strlen(runstatus->path));
    // 走到这意味着验证路径没有问题, 可以修改路径等
    memcpy(runstatus->path, newPath, strlen(newPath));

    return 0;
}
