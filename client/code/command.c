/*
 * 文件名：command.c
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件实现命令接口文档内函数
*/

#include "command.h"

/*
 * 函数名：shardCommand
 * 函数作用：对命令进行简单的处理
 * 函数参数：
 *  -char *buf：传入的命令
 * 返回值：3代表短命令，4代表长命令-1，代表错误
*/
int shardCommand(char *buf){


    char *command  = strtok(buf, " ");
    // 解析命令和参数
    char *file_buf = strtok(NULL, "\n");

    // 处理命令
    if(command == NULL){
        printf("命令为空，请重新输入\n");
        return -1;
    }

    if(strcmp(command, "puts") == 0){
        if(file_buf == NULL){
            printf("命令格式有误，请重新输入\n");
            return -1;
        }
        FILE *file = fopen(file_buf, "r");
        if(file == NULL){
            LOG(ERROR, "打开文件失败，文件不存在");
            printf("打开文件失败, 请重新输入\n");
            return -1;
        }
        return 4;
    }else if(strcmp(command, "gets") == 0){
        if(file_buf == NULL){
            printf("命令格式有误：请重新输入\n");
            return -1;
        }
        return 4;
    }else if(strcmp(command, "remove") == 0 ||strcmp(command, "rm") == 0 || strcmp(command, "mkdir") == 0 || strcmp(command, "touch") == 0){
        // 这些命令需要有参数
        if(file_buf == NULL){
            printf("命令格式有误：请重新输入\n");
            return -1;
        }
        return 3;
    }else if(strcmp(command, "cd") == 0 ){
        // 该命令最多有两个参数
        file_buf = strtok(NULL, " ");
        if(file_buf != NULL){
            printf("cd参数过多\n");
            return -1;
        }
        return 3;
    }else if(strcmp(command, "ls") != 0 && strcmp(command, "pwd") != 0){
        printf("命令错误，请输入正确的命令\n");
        return -1;
    }
}

/*
 * 函数名：shortCommand
 * 函数作用：处理短命令
 * 函数参数：
 *  -int net_fd：进行通信的soacket
 *  -char *command：需要处理的命令
 *  -char *user：携带的用户信息
 * 返回值：0代表成功，-1代表失败
*/
int shortCommand(int net_fd, user_t *user){
    // 1. 将封装后的信息发送给服务器
    if(SEND_SHORT(net_fd, *user) != 0){
        return -1;
    }
    return 0;
}
/*
 * 函数名：longCommand
 * 函数作用：处理短命令
 * 函数参数：
 *  -int net_fd：进行通信的soacket
 *  -char *command：需要处理的命令
 *  -char *user：携带的用户信息
 * 返回值：0代表成功，-1代表失败
*/
int longCommand(int net_fd, user_t *user);




