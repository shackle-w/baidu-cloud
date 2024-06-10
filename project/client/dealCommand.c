#include "head.h"

// 命令处理
int distributeCommand(run_status_t *runstatus){
    printf("%s",runstatus->user_name); 
    printf("@:");
    printf("%s",runstatus->path); 
    printf("$");
    fflush(stdout);

    char buf[1024] = {0};
    read(STDIN_FILENO, buf, sizeof(buf));
    // 处理命令最后的换行
    buf[strlen(buf)-1] = 0;
    // 处理命令最前面的空格
    for(int i=0; i<strlen(buf); i++){
        if(buf[i] == ' '){
            for(int j=i; j<strlen(buf); j++){
              buf[j] = buf[j+1];
            }
            i--;
        }else{
            break;
        }
    }
    // 处理命令最后的空格
    for(int i=strlen(buf); i>0; i--){
        if(buf[i-1] == ' '){
            buf[i-1] = 0;
        }else{
            break;
        }
    }
    // 处理命令和参数中间的多余空格
    for(int i=0; i<strlen(buf); i++){
        if(buf[i]==' ' && buf[i+1]==' '){
            for(int j=i; j<strlen(buf); j++){
              buf[j] = buf[j+1];
            }
            i--;
        }
    }
    //printf("len: %ld ; buf:%s \n", strlen(buf), buf);
    
    // 判断命令类型, 不同命令转到不同的处理逻辑
    if (strncmp(buf, "ls", 2) == 0){
        lsCommand(buf, runstatus);
    } else if (strncmp(buf, "cd", 2) == 0){
        cdCommand(buf, runstatus);
    } else if(strncmp(buf, "pwd", 3) == 0){
        pwdCommand(buf, runstatus);
    } else if(strncmp(buf, "rm", 2) == 0){
        rmCommand(buf, runstatus);
    } else if(strncmp(buf, "mkdir", 5) == 0){
        mkdirCommand(buf, runstatus);
    } else if(strncmp(buf, "puts", 4) == 0){
        putsCommand(buf, runstatus);
    } else if(strncmp(buf, "gets", 4) == 0){
        getsCommand(buf, runstatus);
    } else {
        printf("非法命令, 请重新输入 \n");
    } 

    return 0;
}
