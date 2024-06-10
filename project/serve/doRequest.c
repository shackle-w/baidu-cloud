#include "head.h"

int putsRequest(int netFd, protocol_client protocol, pool_t * pool){
    // 把net_fd交给子进程-> 放入队列
    pthread_mutex_lock(&pool->threadLock );
    // 用来上传文件的链接对象 ->net_fd放入队列, 让子线程处理
    enQueue(&pool->taskQueue , netFd, 0);
    pthread_cond_broadcast(&pool->cond );
    pthread_mutex_unlock(&pool->threadLock );
    return 0;
}
int getsRequest(int netFd, protocol_client protocol, pool_t * pool){
    // 把net_fd交给子进程-> 放入队列
    pthread_mutex_lock(&pool->threadLock );
    // 用来下载文件的链接对象 ->net_fd放入队列, 让子线程处理
    enQueue(&pool->taskQueue , netFd, 1);
    pthread_cond_broadcast(&pool->cond );
    pthread_mutex_unlock(&pool->threadLock );
    return 0;
}
int otherRequest(int netFd, protocol_client protocol, pool_t * pool){
    return 0;
}

// 数据传输格式: (客户端向服务器)
//              先传protocol的长度
//              再传protocol内容
int doRequest(int netFd, pool_t * pool){

    int msgLen = 0;
    int recv_res = recv(netFd, &msgLen, sizeof(int), MSG_WAITALL);
    if(recv_res == 0){
        // 对端断开连接
        return -1;
    }

    protocol_client  protocol;
    bzero(&protocol, sizeof(protocol));
    recv_res = recv(netFd, &protocol, msgLen, MSG_WAITALL);
    if(recv_res == 0){
        // 对端断开连接
        return -1;
    }

    int commandTag = protocol.command_flag;

    int res = 0;
    switch(commandTag){
    case LOGIN:
        res = loginRequest(netFd, protocol, pool);
        return res;
    case REGISTER_NAME:
        res = registerNameRequest(netFd, protocol, pool);
        return res;
    case REGISTER_PASSWORD:
        res = registerPasswordRequest(netFd, protocol, pool);
        return res;
    case LS:
        res = lsRequest(netFd, protocol, pool);
        return res;
    case PATHCHECK:
        res = pathCheckRequest(netFd, protocol, pool);
        return res;
    case RM:
        res = rmRequest(netFd, protocol, pool);
        return res;
    case RMR:
        res = rmRRequest(netFd, protocol, pool);
        return res;
    case MKDIR:
        res = mkdirRequest(netFd, protocol, pool);
        return res;
    case PUTS_SYNC:
        // 调用子进程处理文件上传
        putsRequest(netFd, protocol, pool);
        // 返回命令状态
        return PUTS_SYNC;
    case GETS_SYNC:
        getsRequest(netFd, protocol, pool);
        return GETS_SYNC;
    default:
        res = otherRequest(netFd, protocol, pool);
        return res;
    }
    return 0;
}
