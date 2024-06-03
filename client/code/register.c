/*
 * 文件名：register.c
 * 创建时间：5.28
 * 作者：shackle
 * 描述：该文件是注册登录文件的实现
*/
#include "register.h"

/*
 * 函数名：handleLoginRegister
 * 函数作用：注册登录账号
 * 参数：
 *  -int socket_fd：连接到socket
 *  -user_t *user：封装的用户信息
 *  -char *account：用户的账号名
 * 返回值：0：成功， -1：失败
*/
int handleLoginRegister(int socket_fd, user_t *user, char *account){
    int flag = 0;
    char c;
    char username[SMALL_BUF] = {0};
    char password[SMALL_BUF] = {0};
    while(flag == 0){
        printf("请选择您想进行的操作(0-登录，1-注册)：");
        int select = 4;
        scanf("%d", &select);
        while((c = getchar()) != '\n');
        printf("username：");
        scanf("%s", username);
        while((c = getchar()) != '\n');
        printf("password：");
        scanf("%s", password);
        while((c = getchar()) != '\n');

        if(select == 1){
            int res = registerAccount(socket_fd, username, password, user);
            ERROR_LOG(res, -1, "registerAccount");
            if(res == -1){
                printf("注册失败\n");
            } else {
                flag = 1;
            }
        } else if(select == 0){
            int res = loginID(socket_fd, username, password, user);
            ERROR_LOG(res, -1, "loginID");
            if(res == -1){
                printf("登录失败\n");
            } else {
                flag = 1;
            }
        } else {
            printf("输入有误\n");
        }
    }
    memcpy(account, username, sizeof(account));
}

/*
 * 函数名：registerAccount
 * 函数作用：用于注册一个账号
 * 参数：
 *  -int net_fd：连接的socket
 *  -char *account：用户的账号名
 *  -char *password：用户输入的密码
 *  -user_t *user：用户的信息
 * 返回值：0：注册成功，-1：注册失败
*/
int registerAccount(int net_fd, const char *account, const char *password, user_t *user){
    // 1. 将账号发送给客户端
    // 1. 分装用户的信息，密码
    int len = strlen(account) + 1;
    char buf[BUF_SIZE] = {0};
    memcpy(buf, account, len);
    if(sendRegister(net_fd, buf) ==  -1){
        LOG(ERROR, "发送注册信息失败");
        return -1;
    }
    LOG(INFO, "客户端发送给客户端的账号信息长度：%d,信息：%s",len, buf);
    
    int tag = 0;
    char information[SMALL_BUF] = {0};
    if(recv(net_fd, &tag, sizeof(int), MSG_WAITALL) == -1){
        LOG(ERROR, "接收信息失败");
        return -1;
    }
    if(tag == 4){
        // 收到错误信息
        recv(net_fd, &len, sizeof(int), MSG_WAITALL);
        recv(net_fd, user, len, MSG_NOSIGNAL);
        printf("%s\n", user->command);
        return -1;
    }

    // 2. 接收服务器传来的盐值
    char salt[BUF_SIZE];
    if(recv(net_fd, &len, sizeof(int), MSG_WAITALL) == -1 || recv(net_fd, salt, len, MSG_WAITALL) == -1){
        LOG(ERROR, "接收盐值失败");
        return -1;
    }
    LOG(INFO, "服务器发来的盐值：%s", salt);

    // 3. 封装盐值和密码
    char *encrypted = crypt(password, salt);
    if(encrypted == NULL){
        LOG(ERROR, "密码加密失败");
        return -1;
    }

    // 4. 将封装后的信息发送给服务器
    memcpy(buf, encrypted, len);
    if(sendRegister(net_fd, buf) == -1){
        LOG(ERROR, "发送加密信息失败");
        return -1;
    }
     
    if(recv(net_fd, &tag, sizeof(int), MSG_WAITALL) == -1){
        LOG(ERROR, "接收信息失败");
        return -1;
    }

    if(tag == 4){
        // 收到错误信息
        recv(net_fd, &len, sizeof(int), MSG_WAITALL);
        recv(net_fd, user, len, MSG_NOSIGNAL);
        printf("%s\n", user->command);
        return -1;
    }

    // 5. 接收服务器发送来的token，初始化user
    if(recv(net_fd, &len, sizeof(int), MSG_WAITALL) == -1 || recv(net_fd, information, len, MSG_WAITALL) == -1){
        LOG(ERROR, "接收token失败");
        return -1;
    }
    LOG(INFO, "注册成功");
    memcpy(user->token, information, len);
    strcpy(user->path, "/");
    return 0;
}

/*
 * 函数名：loginID
 * 函数作用：用于登录一个账号
 * 参数：
 *  -int net_fd：连接的socket
 *  -char *account：用户的账号名
 *  -char *password：用户输入的密码
 *  -user_t *user：封装的用户信息
 * 返回值：0：登录成功，-1：登录失败
*/
int loginID(int net_fd, const char *account, const char *password, user_t *user){
    // 1. 将账号发送给客户端
    int len = strlen(account) + 1;
    char buf[BUF_SIZE] = {0};
    memcpy(buf, account, len);
    if(sendLogin(net_fd, buf) == -1){
        LOG(ERROR, "发送登录信息失败");
        return -1;
    }
    LOG(INFO, "客户端发送给客户端的账号信息长度：%d,信息：%s",len, buf);
    
    int tag = 0;
    char information[SMALL_BUF] = {0};
    if(recv(net_fd, &tag, sizeof(int), MSG_WAITALL) == -1){
        LOG(ERROR, "接收信息失败");
        return -1;
    }
    if(tag == 4){
        // 收到错误信息
        recv(net_fd, &len, sizeof(int), MSG_WAITALL);
        recv(net_fd, user, len, MSG_NOSIGNAL);
        printf("%s\n", user->command);
        return -1;
    }

    // 2. 接收服务器传来的盐值
    char salt[BUF_SIZE];
    if(recv(net_fd, &len, sizeof(int), MSG_WAITALL) == -1 || recv(net_fd, salt, len, MSG_WAITALL) == -1){
        LOG(ERROR, "接收盐值失败");
        return -1;
    }
    LOG(INFO, "服务器发来的盐值：%s", salt);

    // 3. 封装盐值和密码
    char *encrypted = crypt(password, salt);
    if(encrypted == NULL){
        LOG(ERROR, "密码加密失败");
        return -1;
    }

    // 4. 将封装后的信息发送给服务器
    memcpy(buf, encrypted, len);
    if(sendLogin(net_fd, buf) == -1){
        LOG(ERROR, "发送加密信息失败");
        return -1;
    }
    
    // 5. 接收服务器结束的信息
    if(recv(net_fd, &tag, sizeof(int), MSG_WAITALL) == -1){
       LOG(ERROR, "接收信息失败");
       return -1;
    }

    if(tag == 4){
        // 收到错误信息
        recv(net_fd, &len, sizeof(int), MSG_WAITALL);
        recv(net_fd, user, len, MSG_NOSIGNAL);
        printf("%s\n", user->command);
        return -1;
    }

    // 5. 接收服务器发送来的token，初始化user
    if(recv(net_fd, &len, sizeof(int), MSG_WAITALL) == -1 || recv(net_fd, information, len, MSG_WAITALL)== -1){
        LOG(ERROR, "接收token失败");
        return -1;
    }
    LOG(INFO, "登录成功");
    memcpy(user->token, information, len);
    strcpy(user->path, "/");

    return 0;
}
