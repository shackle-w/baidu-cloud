#include "head.h"

// 子线程的入口
void * threadFun(void *p){
    pool_t *pPool = (pool_t *) p;
    while(1){

        int net_fd = 0;
        int flag = 0;
        pthread_mutex_lock(&pPool->threadLock );
        // 去队列中拿任务
        while(pPool->taskQueue.size <= 0  && pPool->exitTag == 0){
            pthread_cond_wait(&pPool->cond , &pPool->threadLock);
        }
        // 检测是否要退出
        if(pPool->exitTag == 1){
            pthread_mutex_unlock(&pPool->threadLock);
            return 0;
        }
        net_fd = pPool->taskQueue.head->net_fd;
        flag = pPool->taskQueue.head->flag;
        // 删除队列中的任务
        deQueue(&pPool->taskQueue );
        pthread_mutex_unlock(&pPool->threadLock );

        if(flag == 0){
            // 回复给客户端同步文件上传的回复
            syncPuts(net_fd);
            // 具体处理puts命令
            dealPuts(net_fd, pPool);
        }else{
            // 回复给客户端同步文件下载的回复
            syncGets(net_fd);
            // 具体处理下载命令
            dealGets(net_fd, pPool);
        }

        close(net_fd);
    }
    return NULL;
}
