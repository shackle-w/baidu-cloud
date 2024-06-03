/*
 * 文件名：receiver.c
 * 创建时间：5.31
 * 作者：shackle
 * 描述：该文件是对服务器返回信息进行具体处理
*/

#include "receive.h"

/*
 * 函数名：dealReceive
 * 函数作用：处理服务端返回的信息
 * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户的信息
 * 返回值：成功返回0.失败返回-1
*/
int dealReceive(int net_fd, user_t *user){
    while(1){
        int tag = 0;
        int len = 0;
        char information[BUF_SIZE] = {0};
        // 1. 接收tag
        recv(net_fd, &tag, sizeof(int), MSG_WAITALL);
        if(tag == 5){
            // 1. 退出信号
            break;
        }
        if(tag == 4){
            // 2. 打印错误信息
            recv(net_fd, &len, sizeof(int), MSG_WAITALL);
            recv(net_fd, user, len, MSG_WAITALL);
            printf("information:%s",user->command);
            break;
        }
        if(tag == 3){
            // 3. 打印返回信息
            recv(net_fd, &len, sizeof(int), MSG_WAITALL);
            recv(net_fd, user, len, MSG_WAITALL);
            char *command = user->command;
            command = strtok(command, " ");
            if(strcmp(command, "cd") == 0){
                LOG(INFO, "cd后当前的路径：%s", user->path);
                break;
            }
            if(strcmp(command, "makdir") == 0 && strcmp(command, "rm") == 0){
                LOG(INFO, "此时的工作路径：%s", user->path);
                break;
            }
            if(strcmp(command, "pwd") == 0){
                printf("%s\n", user->path);
            }
            printf("%s\n", user->command);
            LOG(INFO, "此时的工作路径：%s", user->path);
            break;
        }
    }
}
