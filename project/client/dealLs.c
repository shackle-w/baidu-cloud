#include "head.h"

int lsCommand(char *buf, run_status_t *runstatus){
    // eg: 非"ls" 或者 非"ls /a/b"命令 -> "lss"
    if(buf[2] != 0 && buf[2] != ' '){
        printf("命令非法 \n");
        return 0;

    }
    if(strlen(buf) >2 ){
        // 暂时不允许"ls  /路径/路径命令" 
        printf("不识别的参数 \n");
        return 0;
    }

    // 构建传输协议
    protocol_client protocol;
    bzero(&protocol, sizeof(protocol));

    protocol.command_flag = LS;
    protocol.parameter_flag = 1;
    stringCat(protocol.buf, "token", runstatus->token);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "path", runstatus->path);

    protocol.parameter_len = strlen(protocol.buf);
    protocol.parameter_num = 2;
    utilSend(runstatus->net_fd, &protocol, runstatus);

    // 接收对端数据
    protocol_serve protocol_serve;
    bzero(&protocol_serve, sizeof(protocol_serve));

    int recv_len = 0;
    recv(runstatus->net_fd, &recv_len, sizeof(int), MSG_WAITALL);
    recv(runstatus->net_fd, &protocol_serve, recv_len, MSG_WAITALL);

    if(protocol_serve.status == 0){
        // 打印服务器的返回: 当前路径下的文件列表
        HashMap *map = createHashMap();
        getParameterMap(protocol_serve, map);
        char *fileList = get(map, "fileList");

        if(fileList != NULL){
            printf("%s \n", fileList);
        }
    }else{
        // 根据返回值, 判断是否有同名文件夹存在
        printf("ls失败  \n");
    }

    return 0;
}
