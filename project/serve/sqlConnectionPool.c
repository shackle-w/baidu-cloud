#include "head.h"

int getConnect(sql_connect_t *sqlConnect, pool_t* pool){
    
    sqlConnect->connsIndex = -1;
    // 获得一个数据库连接
    while(1){
        pthread_mutex_lock(&pool->connLock);
        for(int i=0; i<pool->connNum; i++){
            if(pool->conns[i].useFlag == 0){
                // 未使用连接
                pool->conns[i].useFlag = 1;
                sqlConnect->connect = pool->conns[0].connect;

                sqlConnect->connsIndex = i;
                break;
            }
        }

        if(sqlConnect->connsIndex == -1){
            // TODO: 发生数据库连接用完, 不够用, 可能导致"主线程"阻塞到这个位置
            // 要加日志警告, 数据库连接不够, 要调大, 
            // 或者实现为代码上的数据库连接自动扩大
            pthread_cond_wait(&pool->connCond, &pool->connLock);
            pthread_mutex_unlock(&pool->connLock);
        }else{
            pthread_mutex_unlock(&pool->connLock);
            break;
        }
    }

    return 0;
}

int setConnect(sql_connect_t sqlConnect, pool_t* pool){

    pthread_mutex_lock(&pool->connLock);
    pool->conns[sqlConnect.connsIndex].useFlag = 0;
    pthread_cond_broadcast(&pool->connCond);
    pthread_mutex_unlock(&pool->connLock);

    return 0;
}
