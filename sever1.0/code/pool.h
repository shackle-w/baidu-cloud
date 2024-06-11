/*
 * 文件名：pool.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件是实现线程的接口函数
*/

#ifndef __POOLH__
#define __POOLH__

#include "my_header.h"
#include "severStruct.h"
#include "confLoad.h"
#include "sonThread.h"

/*
 * 函数名：initPool
 * 函数作用：初始化线程池的信息
 * 函数参数：
 *  pool_t *pPool：结构体pool_t：启动记录了整个服务器状态记录
 * 返回值：
 *  成功返回0
*/
int initPool(pool_t *pPool);

#endif
