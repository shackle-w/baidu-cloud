/*
 * 文件名：queue.h
 * 创间时间：5.26
 * 作者：shackle
 * 描述：该文件为使用队列的接口文件
*/

#ifndef __QUEUEHEAD__
#define __QUEUEHEAD__

#include <my_header.h>

// 创建队列结点的的结构体他
typedef struct node_s{
    int net_fd; // 用于存放全连接队列的socket结点
    struct node_s *next;  // 用于连接下一个结点
}node_t;

// 创建队列的结点
typedef struct queue_s{
    node_t *head; // 用于指向队列的头结点
    node_t *tail; // 用于执行队列的尾结点
    int size; // 用于存放队列存放了几个结点
}queue_t;

/*
 * 函数名：enQueue
 * 函数作用：想队列中加入一个元素
 * 参数：
 *  -queue_t *pQueue：队列
 *  -int net_fd：需要插入队列的文件描述符
 * 返回值：
*/
int enQueue(queue_t *pQueue, int net_fd);

/*
 * 函数名：deQueue
 * 函数作用：从队列中删除一个元素
 * 参数：
 *  -queue_t *pQueue：队列
 * 返回值：
*/
int deQueue(queue_t *pQueue);

/*
 * 函数名：destroyQueue
 * 函数作用：销毁一个队列
 * 参数：
 *  -queue_t *pQueue：队列
 * 返回值：
*/
int destoryQueue(queue_t *pQueue);

#endif
