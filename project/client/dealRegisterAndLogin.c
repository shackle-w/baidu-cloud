#include "head.h"
#include "termio.h"

// 用于禁用终端回显的函数
void disableEcho(int fd, struct termios *old_termios) {
    struct termios new_termios;
    tcgetattr(fd, old_termios);
    new_termios = *old_termios;
    new_termios.c_lflag &= ~(ECHO);
    tcsetattr(fd, TCSANOW, &new_termios);
}
// 用于恢复终端回显的函数
void restoreEcho(int fd, struct termios *old_termios) {
    tcsetattr(fd, TCSANOW, old_termios);
}

// 分发登陆注册
int distributeLoginOrRegister(run_status_t *runstatus){
    printf("请选择 (Y)登陆, (R)注册, (E)退出:  ");
    fflush(stdout);

    char choose[10];
    scanf("%s", choose);

    switch(choose[0]){
    case 'Y':
        doLogin(runstatus);
        //printf("client: distributeLoginOrRegister: token: %s \n", runstatus->token);
        break;
    case 'R':
        doRegister(runstatus);
        break;
    case 'E':
        exit(0);   
        break;
    default:
        printf("错误字符,请重新输入 \n");
    }
    return 0;
}

// 处理登陆
int doLogin(run_status_t *runstatus){

    // 接收账号
    printf("user name:");
    fflush(stdout);
    char name[100] = {0};
    scanf("%s", name);

    // 读掉换行
    char c;
    scanf("%c", &c);

    // 准备接收密码
    printf("pass word:");
    fflush(stdout);

    char password[100] = {0};
    struct termios old_termios;
    disableEcho(STDIN_FILENO, &old_termios);
    // 读取密码
    fgets(password, 100, stdin);
    restoreEcho(STDIN_FILENO, &old_termios);
    password[strcspn(password, "\n")] = 0;

    // 换行
    printf("\n");

    // 构建传输协议内容
    protocol_client protocol;
    bzero(&protocol, sizeof(protocol));

    protocol.command_flag = LOGIN;
    protocol.parameter_flag = 1;
    stringCat(protocol.buf, "username", name);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "password", password);
    protocol.parameter_len = strlen(protocol.buf);
    protocol.parameter_num = 2;

    // 发送login行为
    utilSend(runstatus->net_fd, &protocol, runstatus);

    // 接收对端数据
    protocol_serve protocol_serve;
    bzero(&protocol_serve, sizeof(protocol_serve));

    int recv_len = 0;
    recv(runstatus->net_fd, &recv_len, sizeof(int), MSG_WAITALL);
    recv(runstatus->net_fd, &protocol_serve, recv_len, MSG_WAITALL);

    //printf("stat: %d \n", protocol_serve.status);
    if(protocol_serve.status == -1){
        //  服务器返回数据, 状态码为异常
        printf("账号不存在或者密码错误 \n");
        return -1;
    }
    
    // 获得token
    HashMap *map = createHashMap();
    getParameterMap(protocol_serve, map);

    char *token = get(map, "token");
    // printf("token: %s \n", token);
    strcpy(runstatus->token, token);
    runstatus->token_len = strlen(token);
    runstatus->login_status = 1;
    //printf("dealRegisterAndLogin: doLogin: name: %s \n", name);
    strcpy(runstatus->user_name, name);
    //printf("dealRegisterAndLogin: doLogin: user_name: %s \n", runstatus->user_name);
    
    free(token);
    return 0;
}

// 处理注册
int doRegister(run_status_t *runstatus){

    char name[100] = {0};
    while(1){
        // 接收账号
        printf("user name:");
        fflush(stdout);
        scanf("%s", name);

        // 读掉换行
        char c;
        scanf("%c", &c);

        // 验证注册账号
        // 构建传输数据协议报文
        protocol_client protocol;
        bzero(&protocol, sizeof(protocol));

        protocol.command_flag = REGISTER_NAME;
        protocol.parameter_flag = 1;
        stringCat(protocol.buf, "username", name);
        protocol.parameter_len = strlen(protocol.buf);
        protocol.parameter_num = 1;

        // 发送注册的账号行为
        utilSend(runstatus->net_fd, &protocol, runstatus);

        // 读取服务端的返回信息, 判断是否是有这个注册的用户名字
        int msgLen = 0;
        recv(runstatus->net_fd, &msgLen, sizeof(int), MSG_WAITALL);

        protocol_serve  protocol_serve;
        bzero(&protocol_serve, sizeof(protocol_serve));
        recv(runstatus->net_fd, &protocol_serve, msgLen, MSG_WAITALL);

        if(protocol_serve.status == 0){
            break;
        }else{
            printf("用户名存在, 请更换 \n");
        }
    }
    // 读取注册密码
    char password[100];
    while(1){
        bzero(password, sizeof(password));
        printf("pass word:");
        fflush(stdout);

        struct termios old_termios;
        disableEcho(STDIN_FILENO, &old_termios);
        // 读取密码
        fgets(password, 100, stdin);
        restoreEcho(STDIN_FILENO, &old_termios);
        password[strcspn(password, "\n")] = 0;

        // 换行
        printf("\n");

        // 读取确认密码密码
        printf("reinput pass word:");
        fflush(stdout);

        char passwordAgain[100];
        bzero(passwordAgain, sizeof(passwordAgain));
        disableEcho(STDIN_FILENO, &old_termios);
        // 读取密码
        fgets(passwordAgain, 100, stdin);
        restoreEcho(STDIN_FILENO, &old_termios);
        passwordAgain[strcspn(password, "\n")] = 0;

        // 换行
        printf("\n");

        if(strcmp(password, passwordAgain) != 0){
            printf("密码两次输入不一致 \n");
        }else{
            // 密码一致
            break;
        }
    }

    // 构建传输协议内容
    protocol_client protocol;
    bzero(&protocol, sizeof(protocol));

    protocol.command_flag = REGISTER_PASSWORD;
    protocol.parameter_flag = 1;
    stringCat(protocol.buf, "username", name);
    strcat(protocol.buf, "&");
    stringCat(protocol.buf, "password", password);
    protocol.parameter_len = strlen(protocol.buf);
    protocol.parameter_num = 2;

    // 发送login行为
    utilSend(runstatus->net_fd, &protocol, runstatus);

    // 读取服务端的返回信息, 判断是否是有这个注册的用户名字
    int msgLen = 0;
    recv(runstatus->net_fd, &msgLen, sizeof(int), MSG_WAITALL);

    protocol_serve  protocol_serve;
    bzero(&protocol_serve, sizeof(protocol_serve));
    recv(runstatus->net_fd, &protocol_serve, msgLen, MSG_WAITALL);

    if(protocol_serve.status == 0){
        printf("用户注册成功 \n");
        return 0;
    }else{
        printf("发生未知错误 \n");
        return -1;
    }

}
