#include "head.h"

//初始化线程池
int initPool(pool_t *pPool){

    bzero(&pPool->connsStatus, sizeof(ConnectsStatus));

    pPool->map = parameterMap();
    pPool->poolThreadNum = atoi(get(pPool->map, "thread_num"));

    //初始化队列和锁
    bzero(&pPool->taskQueue, sizeof(queue_t));

    // 初始化锁和条件遍历
    pthread_mutex_init(&pPool->threadLock, NULL);
    pthread_cond_init(&pPool->cond, NULL);

    // 退出标记位0, 不退出,  1是退出
    pPool->exitTag = 0;

    // 初始化数组:
    pPool->poolThreadIds = (pthread_t *) calloc(pPool->poolThreadNum, sizeof(pthread_t));
    // 创建对应的子线程
    for(int i=0; i<pPool->poolThreadNum; i++){
        // 创建子线程
        pthread_create(&pPool->poolThreadIds[i], NULL,threadFun, pPool);
    }

    // 初始化数据库连接池相关内容
    pthread_mutex_init(&pPool->connLock, NULL);
    pthread_cond_init(&pPool->connCond, NULL);
    pPool->connNum = atoi(get(pPool->map, "conn_num")); 
    pPool->conns = (sql_connect_t *)calloc(pPool->connNum , sizeof(sql_connect_t));
    for(int i=0; i<pPool->connNum; i++){
        pthread_mutex_lock(&pPool->connLock);
        mysql_init(&pPool->conns[i].connect);
        mysql_real_connect(&pPool->conns[i].connect,
                           get(pPool->map, "mysql_location"),
                           get(pPool->map, "mysql_user"),
                           get(pPool->map, "mysql_password"),
                           get(pPool->map, "mysql_db"),
                           0,
                           NULL,
                           0);
        pPool->conns[i].useFlag = 0; 
        pPool->conns[i].connsIndex = i;

        pthread_mutex_unlock(&pPool->connLock);
    }

    return 0;
}

