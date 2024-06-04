/*
 * 文件名：main.c
 * 创建时间：5.29
 * 作者：shackle
 * 描述：该文件是客户端的运行逻辑
*/

#include "head.h"

int main(int argc, char *argv[]){
    // 1. 打开日志文件

    LOG(WARNING, "打开客户端");
    ARGS_CHECK(argc, 3);

    // 2. 读取命令传来的ip和端口号
    char *ip = argv[1];
    char *port = argv[2];

    // 3. 封装socket
    int socket_fd = 0;
    int res_func = initTcp(&socket_fd, ip, port);
    ERROR_LOG(res_func, -1, "initTcp");
    ERROR_CHECK(res_func, -1, "initTcp");   
    
    // 结构体user_t，用于保存token等信息
    user_t user;
    //4. 进行注册登录操作
    char username[SMALL_BUF];
    char password[SMALL_BUF];

    // 4. 进行注册登录操作
    res_func = handleLoginRegister(socket_fd, &user, username);
    ERROR_LOG(res_func, -1, "handleLoginRegister");
    ERROR_CHECK(res_func, -1, "handleLoginRegister");

    // 5. 创建epoll监听
    int epoll_fd = epoll_create(1);
    ERROR_LOG(epoll_fd, -1, "epoll_create");
    ERROR_CHECK(epoll_fd, -1, "epoll_create");
    
    int res = epollAddfd(epoll_fd, STDIN_FILENO);
    ERROR_LOG(res, -1, "epollAddfd");
    ERROR_CHECK(res, -1, "epollAddfd");
    res = epollAddfd(epoll_fd, socket_fd);
    ERROR_LOG(res, -1, "epollAddfd");
    ERROR_CHECK(res, -1, "epollAddfd");
    
    printfHint(username, user.path);
    fflush(stdout);

    while(1){
        
        res = handleEpollEvents(epoll_fd, socket_fd, &user);
        ERROR_LOG(res, -1, "handleEpollEvents");
        ERROR_CHECK(res, -1, "handleEpollEvents");
        
        printfHint(username, user.path);
        fflush(stdout);
    }

    // 关闭日志文件
    return 0;
}
