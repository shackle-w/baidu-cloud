/*
 * 文件名：queue.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件的作用是实现队列有关的接口函数
*/

#ifndef __QUEUEH__
#define __QUEUEH__

#include "my_header.h"
#include "severStruct.h"

/*
 * 函数名：enQueue
 * 函数作用：将一个socket链接加入到队列
 * 函数参数：
 *  queue_t *queue：queue结构体，用于存放socket
 *  int net_fd：需要加入队列的结点
 *  int flag: 是长传文件还是下载文件
 * 函数返回值：成功返回0
*/
int enQueue(queue_t *queue, int net_fd, int flag);

/*
 * 函数名：deQueue
 * 函数作用：删除队列的头节点
 * 函数参数：
 *  queue_t *queue：存放socket结点的队列
 * 函数返回值：成功返回0
*/
int deQueue(queue_t *queue);

#endif
