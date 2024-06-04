/*
 * 函数名：main,c
 * 创建时间：5.29
 * 作者：shackle
 * 描述：该文件是服务器的工作逻辑
*/

#include "head.h"

int pipe_fd[2];

void func(int num){
    // 随便发一个信号给子进程
    int res_write = write(pipe_fd[1], "1", 1);
    ERROR_CHECK(res_write, -1, "write");
}

int main(int argc, char *argv[]){

    LOG(WARNING, "开启服务器");
    ARGS_CHECK(argc, 2);

    // 2. 获取配置文件的内容
    char ip[SMALL_BUF], port[SMALL_BUF], username[SMALL_BUF], password[SMALL_BUF], host[SMALL_BUF], database[SMALL_BUF], prefix_path[SMALL_BUF];
    char* parameters[] = {"ip", "port", "username", "password", "host", "database", "prefix_path"};
    char* values[] = {ip, port, username, password, host, database, prefix_path};

    for (int i = 0; i < 7; ++i) {
        int flag = getparameter(argv[1], parameters[i], values[i]);
        ERROR_CHECK(flag, -1,"getparameter");
    }

    // 3. 连接到数据库
    MYSQL *conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, host, username, password, database, 0, NULL, 0)){
        LOG(ERROR, "%s\n", mysql_error(conn));
        printf("%s\n", mysql_error(conn));
        return -1;
    }
    LOG(INFO, "连接到数据库");

    // 4. 创建子进程
    pipe(pipe_fd);// 用于主进程和子进程通信
    if(fork() != 0){
        // 主进程逻辑，接收退出信号，给子进程退出信号
        signal(2, func);
        wait(NULL);

        close(pipe_fd[0]);
        close(pipe_fd[1]);
        // 关闭数据库
        mysql_close(conn);
        
        return 0;
    }

    // 子进程逻辑
    setpgid(0, 0);
    close(pipe_fd[1]);
    

    // 5 初始化两个线程池，一个处理长命令， 一个处理短命令
    pool_t poolLong;
    int res = initPool(&poolLong, 5);
    ERROR_LOG(res, -1, "initPool");
    ERROR_CHECK(res, -1, "initPool");
    poolLong.sql = conn;
    
    pool_t poolShort;
    res = initPool(&poolShort, 3);
    ERROR_LOG(res, -1, "initPool");
    ERROR_CHECK(res, -1, "initPool");
    poolShort.sql = conn;
    // 6. 创建socket连接
    int net_fd;
    res = initSocket(&net_fd, ip, port);
    ERROR_LOG(res, -1, "initSocket");
    ERROR_CHECK(res, -1, "initSocket");

    // 7. 创建epoll监听
    int epoll_fd = epoll_create(1);
    ERROR_LOG(epoll_fd, -1, "epoll_create");
    ERROR_CHECK(epoll_fd, -1, "epoll_create");

    // 8. 加入监听信号
    res = epollAddfd(epoll_fd, net_fd);
    ERROR_LOG(res, -1, "epollAddfd");
    ERROR_CHECK(res, -1, "epollAddfd");
    res = epollAddfd(epoll_fd, pipe_fd[0]);
    ERROR_LOG(res, -1, "epollAddfd");
    ERROR_CHECK(res, -1, "epollAddfd");

    // 创建服务端的信息
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    // 8. 监听信号

    while(1){
        // 创建监听就绪结构体
        struct epoll_event list[1024];
        int epoll_num = epoll_wait(epoll_fd, list, 1024, -1);

        for(int i = 0; i < epoll_num; i++){
            if(list[i].data.fd == net_fd){
                //printf("有连接到来\n");
                // 有连接进来
                int socket_fd = accept(net_fd, (struct sockaddr *)&client_address, &client_address_len);
                ERROR_LOG(socket_fd, -1, "accept");
                ERROR_CHECK(socket_fd, -1, "accept");

                logSocketConnection(inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
               
                int tag = -1;
                if(recv(socket_fd, &tag, sizeof(int), MSG_WAITALL) == -1 ){
                    LOG(ERROR, "未接收到信号标识");
                    break;
                }
                // TODO:注册信号1， 登录信号2， 短命令3，长命令4，文件5
                if(tag == 1 || tag == 2){
                    pthread_mutex_lock(&poolShort.pool_lock);
                    enQueue(&poolShort.queue, socket_fd);
                    poolShort.commandTag = tag;
                    pthread_cond_signal(&poolShort.cond);
                    pthread_mutex_unlock(&poolShort.pool_lock);
                }else if(tag == 4){
                   pthread_mutex_lock(&poolLong.pool_lock);
                    enQueue(&poolLong.queue, socket_fd);
                    poolLong.commandTag = tag;
                    pthread_cond_broadcast(&poolLong.cond);
                    pthread_mutex_unlock(&poolLong.pool_lock);
                }else{
                    LOG(WARNING, "客户端退出：");
                }
            }


            if(list[i].data.fd == pipe_fd[0]){
            //printf("终止信号到来\n");
            // 终止信号到来
            char buf[60] = {0};
            read(pipe_fd[0], buf, sizeof(buf));

            // 修改标志位
            pthread_mutex_lock(&poolShort.pool_lock);
            poolShort.exitFlag = 1;
            pthread_cond_broadcast(&poolShort.cond);
            pthread_mutex_unlock(&poolShort.pool_lock);

            for(int i = 0; i < poolShort.threadNum; i++){
                int res_func = pthread_join(poolShort.threadIds[i], NULL);
                if(res_func != 0){
                    LOG(ERROR, "Thread Error: %ld 引发", poolShort.threadIds[i]);
                }
            }
            // 请空线程池创建的遗留
            free(poolShort.threadIds);
            pthread_mutex_destroy(&poolShort.pool_lock);
            pthread_cond_destroy(&poolShort.cond);
                
                
            // 修改标志位
            pthread_mutex_lock(&poolLong.pool_lock);
            poolLong.exitFlag = 1;
            pthread_cond_broadcast(&poolLong.cond);
            pthread_mutex_unlock(&poolLong.pool_lock);

            for(int i = 0; i < poolLong.threadNum; i++){
                int res_func = pthread_join(poolLong.threadIds[i], NULL);
                if(res_func != 0){
                    LOG(ERROR, "Thread Error: %ld 引发", poolLong.threadIds[i]);
                }
            }
            // 请空线程池创建的遗留
            free(poolLong.threadIds);
            pthread_mutex_destroy(&poolLong.pool_lock);
            pthread_cond_destroy(&poolLong.cond);
            exit(EXIT_SUCCESS);
                
        }
    }
}

    // TODO: 错误情况下也需要清理资源
}
