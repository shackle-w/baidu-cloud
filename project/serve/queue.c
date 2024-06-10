#include "head.h"

// 把main监听获取到的
// 和客户端连接的socket对象的文件描述符
// 放入队列
int enQueue(queue_t *pQueue, int net_fd, int flag){

    // 创建结点
    node_t *pNode = (node_t *)calloc(1, sizeof(node_t));
    pNode->net_fd = net_fd;
    pNode->flag = flag;

    // 加入队列
    if(pQueue->size == 0){
        // 新结点既是队头又是队尾
        pQueue->head = pNode;
        pQueue->end = pNode;
    }else{
        // 非空
        pQueue->end->pNext = pNode;
        pQueue->end = pNode;
    }
    // size++
    pQueue->size ++;

    return 0;
}

// 出队列
int deQueue(queue_t *pQueue){

    if(pQueue->size == 0){
        return -1;
    }

    // 有内容
    node_t *p = pQueue->head;

    if(pQueue->size == 1){
        pQueue->head = NULL;
        pQueue->end = NULL;
    }else{
        pQueue->head = pQueue->head->pNext ;
    }

    // size
    pQueue->size --;

    free(p);
    return 0;
}
