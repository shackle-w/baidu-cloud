/*
 * 文件名：pool.h
 * 创建时间：5.30
 * 作者：shackle
 * 描述：该文件是线程池的头文件，用于处理线程池
*/

#ifndef __POOLH__
#define __POOLH__

#include "my_header.h"
#include "log.h"
#include "tools.h"
#include "socket.h"
#include "file.h"


/*
 * 函数名：threadMain(void *p);
 * 描述：定义线程的入口函数
 * 参数：
 *  -void *p：一个指针类型，用于
 * 返回值：
 */
void *threadMain(void *p);

#endif
