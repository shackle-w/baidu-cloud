#include "head.h"

// 新链接到来, 加入记录
int insetConnect(int netFd,  ConnectsStatus * connsStatus){

    // 初始化信息
    ConnectRecord *connResord = (ConnectRecord *) calloc(1, sizeof(ConnectRecord));
    time( &connResord->lastTime );
    connResord->netFd = netFd;

    // 计算时间余数
    int timeIndex = connResord->lastTime%30;

    // 记录余数所代表的下标位置
    connsStatus->index[netFd] = timeIndex;

    if(connsStatus->table[timeIndex] == NULL){
        // 当前时间散列位置为空
        connsStatus->table[timeIndex] = connResord;
    }else{
        // 当前位置不空: 头插
        connResord->next = connsStatus->table[timeIndex]; 
        connsStatus->table[timeIndex] = connResord;
    }

    return 0;
}

// 更新连接的说话时间记录
int updateConnect(int netFd, ConnectsStatus * connsStatus){

    // 获得次netFd之前在循环时间队列的下标位置
    int oldIndex = connsStatus->index[netFd];

    ConnectRecord *update;

    // 遍历这个下标位置, 从这个位置移除此netFd
    ConnectRecord * temp = connsStatus->table[oldIndex];

    // 判断是不是头记录
    if(temp->netFd == netFd){
        // 保存记录
        update = temp;
        // 移掉头
        connsStatus->table[oldIndex] = temp->next;
    }else{
        // 不是头位置: 遍历找
        while(temp->next != NULL){
            if(temp->next->netFd != netFd){
                temp = temp->next;
            }else{
                // 记录
                update = temp->next;
                // 移除记录
                temp->next = temp->next->next;
                break;
            }
        }
    }

    // 更新时间
    time( &update->lastTime );

    // 计算时间余数
    int newIndex = update->lastTime%30;

    // 记录余数所代表的下标位置
    connsStatus->index[netFd] = newIndex;

    if(connsStatus->table[newIndex] == NULL){
        // 当前时间散列位置为空
        connsStatus->table[newIndex] = update;
    }else{
        // 当前位置不空: 头插
        update->next = connsStatus->table[newIndex]; 
        connsStatus->table[newIndex] = update;
    }

    return 0;
}

// 移除指定链接的记录
int deleteConnect(int netFd, ConnectsStatus *connsStatus){

    // 获得次netFd之前在循环时间队列的下标位置
    int oldIndex = connsStatus->index[netFd];

    ConnectRecord * conn;
    // 遍历这个下标位置, 从这个位置移除此netFd
    ConnectRecord * temp = connsStatus->table[oldIndex];

    // 判断是不是头记录
    if(temp->netFd == netFd){
        // 保存记录
        conn = temp;
        // 移掉头
        connsStatus->table[oldIndex] = temp->next;
    }else{
        // 不是头位置: 遍历找
        while(temp->next != NULL){
            if(temp->next->netFd != netFd){
                temp = temp->next;
            }else{
                // 记录
                conn = temp->next;
                // 移除记录
                temp->next = temp->next->next;
                break;
            }
        }
    }

    connsStatus->index[netFd] = 0;
    free(conn);
    return 0;
}

// 检查连接状态: 并进行超时踢出
int checkConnect(int epoll_fd, ConnectsStatus * connsStatus){

    // 获得当前时间
    time_t currTime;
    time(&currTime);

    // 获得次当前时间在队列的下标位置
    int index =  currTime % 30;

    ConnectRecord * conn;
    // 遍历这个下标位置
    ConnectRecord * temp = connsStatus->table[index];

    // 先处理非头结点
    while(temp != NULL && temp->next != NULL){

        if( currTime - temp->next->lastTime  >= 30){
            // temp->next超时
            conn = temp->next ;

            // 移除记录
            temp->next = temp->next->next;

            // 释放连接, 移除epoll监听, 释放内存
            epollRemove(epoll_fd, conn->netFd);
            close(conn->netFd);

            connsStatus->index[conn->netFd] = 0;
            free(conn);

        }else{
            // 未超时, 向后遍历
            temp = temp->next;
        }
    }

    // 对头进行判断
    if(connsStatus->table[index] != NULL && currTime - connsStatus->table[index]->lastTime  >=30 ){
        // 头超时
        conn = connsStatus->table[index] ;

        // 移除头部
        connsStatus->table[index] = conn->next;

        // 释放连接, 移除epoll监听, 释放内存
        epollRemove(epoll_fd, conn->netFd);
        close(conn->netFd);
        connsStatus->index[conn->netFd] = 0;
        free(conn);
        
        LOG(LOG_TYPE_INFO, "客户端被超时剔除");
    }

    return 0;
}
