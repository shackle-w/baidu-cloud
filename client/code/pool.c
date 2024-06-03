/*
 * 文件名：pool.h
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件是有关线程池实现的文件
*/

#include "pool.h"

/*
 * 函数名：threadMain(void *p);
 * 描述：定义线程的入口函数
 * 参数：
 *  -void *p：一个指针类型，用于
 * 返回值：
 */
void *threadMain(void *p){
    int *ret_val = (int *)malloc(sizeof(int));
    ERROR_CHECK(ret_val, NULL, "malloc");
    *ret_val = 0;
    
    // 1. 创建socket连接
    int net_fd = 0;
    int res = initTcp(&net_fd, "192.168.85.128", "9000");
    if(res == -1){
        LOG(ERROR, "创建传输文件连接失败");
        *ret_val = -1;
    }


    user_t *user = (user_t*)p;
    
    // 2. 对命令进行切分，分别处理上传和下载文件
    char *command = user->command;
    command = strtok(command, " ");

    if(strcmp(command, "gest") == 0){
        // TODO：下载文件的逻辑
    }else{
        // TODO：上传文件的逻辑
    }
    
    pthread_exit((void*)ret_val);
}
