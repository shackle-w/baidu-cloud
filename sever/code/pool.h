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
#include "queue.h"
#include "tools.h"
#include "register.h"
#include "command.h"
#include "putsFile.h"
#include "getsfile.h"


// 声明一个线程池所需要的结构提
typedef struct pool_s{
    pthread_t *threadIds; // 所有线程池的id
    int threadNum; // 线程的数量
    queue_t queue; // 就绪队列
    pthread_mutex_t pool_lock; // 队列的锁
    pthread_cond_t cond; // 条件变量
    int exitFlag; // 退出标记
    int commandTag; // 信号的标识，根据不同的标识选择不同的处理逻辑
    MYSQL *sql; //为线程服务的数据库
}pool_t;

/*
 * 函数名：initPool
 * 函数作用：初始化线程池
 * 参数：
 *  -pool_t *pool：有关线程池的结构体
 *  -int nums：线程的个数
 * 返回值：成功0，失败-1
*/
int initPool(pool_t *pool, int nums);


/*
 * 函数名：threadMain(void *p);
 * 描述：定义线程的入口函数
 * 参数：
 *  -void *p：一个指针类型，用于
 * 返回值：
 */
void *threadMain(void *p);


#endif
