/*
 * 文件名：send.c
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件的作用是实现发送客户端信息的文件
*/

#include "send.h"

// 发送信息函数
int utilSend(int netFd, protocol_client *protocol, run_status_t *runstatus){


    // 发送创建文件夹行为
    int send_len = sizeof(int)*4 + protocol->parameter_len;
    ssize_t ret1 = send(runstatus->net_fd, &send_len, sizeof(int), MSG_NOSIGNAL);
    ssize_t ret2 = send(runstatus->net_fd, protocol, send_len, MSG_NOSIGNAL);
    if(ret1 == -1 || ret2 == -1){
        // 对方断开
        // 重新链接
        initTcp(&runstatus->net_fd, get(runstatus->map , "ip"), get(runstatus->map , "port"));

        // 重新发送
        send(runstatus->net_fd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(runstatus->net_fd, protocol, send_len, MSG_NOSIGNAL);
    }

    return 0;
}
