/*
 * 文件名：command.c
 * 创建时间：5.31
 * 作者：shackle
 * 描述：该文件是处理命令的逻辑
*/

#include "command.h"

/* 函数名：resolve
 * 函数作用：根据用户传来的token，分解出用户id
 * 参数：
 *  -char *token：用户传入的token
 * 返回值：成功返回用户id，失败返回-1
*/
int resolve(char *token){
    int userId;
    // 初始化解码参数结构体
    struct l8w8jwt_decoding_params params;
    l8w8jwt_decoding_params_init(&params);

	// 设置JWT使用的算法，这里使用HS512
    params.alg = L8W8JWT_ALG_HS512;

    // token值
    char *str = token;
    // 填入token字符串和长度
    params.jwt = (char*)str;
    params.jwt_length = strlen(str);

    // 设置密钥
    char *key="shackle string token key";
    params.verification_key = (unsigned char*)key;
    params.verification_key_length = strlen(key);

    // 定义: 接收结果的指针和变量
    struct l8w8jwt_claim *claims = NULL;
    size_t claim_count = 0;
    enum l8w8jwt_validation_result validation_result;

    // 解码: 获得token中存储的信息
    int decode_result = l8w8jwt_decode(&params, &validation_result, &claims, &claim_count);

    // 判断是否是正确的token
    if (decode_result == L8W8JWT_SUCCESS && validation_result == L8W8JWT_VALID) {
        LOG(INFO,"Token validation successful!");

        // 打印token中解码出来的荷载信息
        for (size_t i = 0; i < claim_count; i++) {
            LOG(INFO,"Claim [%zu]: %s = %s\n", i, claims[i].key, claims[i].value);
        }
        userId = atoi(claims[3].value);
    } else {
        LOG(ERROR,"Token validation failed!");
        userId =  -1;
    }

    l8w8jwt_free_claims(claims, claim_count);

    return userId;
}
/*
 * 函数名：dealShort
 * 函数作用：处理短命令
 * 参数：
 *  -int net_fd：文件描述符
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0，失败返回-1
*/
int dealShort(int net_fd, user_t *user, MYSQL *conn){
    // 对命令进行简单的处理
    
    int len = 0;
    int res_recv = recv(net_fd, &len, sizeof(int), MSG_WAITALL);

    recv(net_fd, user, len, MSG_WAITALL);
    char buf[BUF_SIZE] = {0};
    memcpy(buf, user->command, sizeof(buf));
    
    buf[len] = '\0';
    logReceivedMessage(buf);


    // 解析命令和参数
    char *command = strtok(buf, " ");
    char *file_buf = strtok(NULL, "\n");

    int res = -1;    
    if(strcmp(command, "cd") == 0){
        // 调用处理cd命令的函数
        res = realizeCD(net_fd, user, conn);
        if(res == -1){
            LOG(ERROR, "处理CD命令失败");
            return -1;
        }
        return 0;
    }

    if(strcmp(command, "ls") == 0){
        // 调用处理ls命令的函数
        res = realizeLS(net_fd, user, conn);
        if(res == -1){
            LOG(ERROR, "处理LS命令失败");
            return -1;
        }
        return 0;
    }

    if((strcmp(command,"remove") == 0) || strcmp(command, "rm") == 0){
        // 调用处理rm命令的函数
        res = realizeRM(net_fd, user, conn);
        if(res == -1){
            LOG(ERROR, "处理RM命令失败");
            return -1;
        }
        return 0;
    }

    if(strcmp(command, "pwd") == 0){
        // 调用处理pwd命令的函数
        res = realizePWD(net_fd, user, conn);
        if(res == -1){
            LOG(ERROR, "处理PWD命令失败");
            return -1;
        }
        return 0;
    }

    if(strcmp(command, "mkdir") == 0){
        // 调用处理mkdir命令的函数
        res = realizeMKDIR(net_fd, user, conn);
        if(res == -1){
            LOG(ERROR, "处理MKDIR命令失败");
            return -1;
        }
        return 0;
    }
    return 0;
}

