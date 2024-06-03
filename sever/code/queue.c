/*
 * 文件名：queue.c
 * 创建时间：24.5.26
 * 作者：shackle
 * 描述：该文件的作用是对队列进行具体操作的实现
*/

#include "queue.h"

/*
 * 函数名：enQueue
 * 函数作用：想队列中加入一个元素
 * 参数：
 *  -queue_t *pQueue：队列
 *  -int net_fd：需要插入队列的文件描述符
 * 返回值：
*/
int enQueue(queue_t *pQueue, int net_fd){
    // 1. 创建一个队列结点
    node_t *pnew = (node_t *)calloc(1, sizeof(node_t));

    pnew->net_fd = net_fd;

    if(pQueue->size == 0){
        // 队列没有元素
        pQueue->head = pnew;
        pQueue->tail = pnew;
    }else{
        pQueue->tail->next = pnew;
        pQueue->tail = pnew;
    }

    pQueue->size++;

    return 0;
}

/*
 * 函数名：deQueue
 * 函数作用：从队列中删除一个元素
 * 参数：
 *  -queue_t *pQueue：队列
 * 返回值：
*/
int deQueue(queue_t *pQueue){
    if(pQueue->size == 0){
        printf("没有就绪的socket连接\n");
        return -1;
    }

    node_t *p = pQueue->head;
    pQueue->head = p->next;
    
    if(pQueue->size == 1){
        pQueue->tail == NULL;
    }

    pQueue->size--;

    free(p);

    return 0;
}
/*
 * 函数名：destroyQueue
 * 函数作用：销毁一个队列
 * 参数：
 *  -queue_t *pQueue：队列
 * 返回值：
*/
int destoryQueue(queue_t *pQueue){
    if(pQueue->size == 0){
        free(pQueue);
        return 0;
    }
    
    node_t *p = pQueue->head;
    node_t *tmp = p->next;
    while(p->next != NULL){
        free(p);
        p = tmp;
        tmp = tmp->next;
    }

    free(pQueue);

    return 0;
}

