/*
 * 文件名：register.c
 * 创建时间：5.28
 * 作者：shackle
 * 描述：该文件是实现客户端登录的逻辑
*/

#include "register.h"
#include "log.h"
#include <l8w8jwt/encode.h>

/*
 * 函数名：getToken
 * 函数作用：根据用于id和用户名生成token
 * 参数：
 *  -char *id：用户的id
 *  -char *username：用户的用户名
 *  -char *jwt：用于获取生成的token
 * 返回值：成功返回0，失败返回-1
*/
int getToken(char *id, char *username, char **jwt){

  // JWT字符串的长度
  size_t jwt_length;  

  // 初始化编码参数结构体
  struct l8w8jwt_encoding_params params;
  l8w8jwt_encoding_params_init(&params); 

  // 设置JWT使用的算法，这里使用HS512
  params.alg = L8W8JWT_ALG_HS512;  

  // 设置JWT的主要荷载部分内容:
  // 主题
  params.sub = id;  
  // 签发者
  params.iss = "shackle";  
  // 接收方
  params.aud = username;  
  // token过期/有效时间(0x7fffffff:最大值-> 2038年1月19日)
  params.exp = 0x7fffffff;  
  // token签发时间
  params.iat = 0;  

  // 设置加密密钥
  params.secret_key = (unsigned char*)"shackle string token key";
  params.secret_key_length = strlen((char *)params.secret_key);

  // 输出变量
  params.out = jwt;
  params.out_length = &jwt_length;

  // 加密
  l8w8jwt_encode(&params);

  LOG(INFO, "生成的token: %s", jwt);

  return 0;
}

/*
 * 函数名：generate_salt
 * 函数作用：基于时间生成一个随机的盐值
 * 函数参数：
 *  -
 * 返回值：返回一个盐值的字符串
*/
char *generate_salt(){
    // +1用于存储字符串结束符'\0'
    char *salt = (char *)malloc(SALT_LENGTH + 1);
    ERROR_CHECK(salt, NULL, "malloc");
    ERROR_LOG(salt, NULL, "malloc");

    const char charset[] = "0123456789"
                            "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand(time(NULL));

    for(int i = 0; i < SALT_LENGTH; ++i){
        salt[i] = charset[rand() % (sizeof(charset) - 1)];
    }

    salt[SALT_LENGTH] = '\0';

    return salt;
}

/*
 * 函数名：registerAccount
 * 函数作用：用于和客户端交互，注册一个账号
 * 参数：
 *  -int net_fd：连接的sockt
 *  -user_t *user：封装的用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：0：注册成功，1：注册失败
*/
int registerAccount(int net_fd, user_t *user, MYSQL *conn){
    // 1. 接收客户端的账号
    int len = 0;
    char username[BUF_SIZE] = {0};
    char buf[SMALL_BUF] = {0};
    char str[PAGE_SIZE] = {0};
    //printf("进入到注册函数内\n");
    recv(net_fd, &len, sizeof(int), MSG_WAITALL);
    recv(net_fd, username, len, MSG_WAITALL);
    LOG(INFO, "客户端传来的账号信息：%s", username);
    

    // TODO：判断账号名是否重复，重复返回信息
    snprintf(buf,sizeof(buf), "this is a error");
    
    memcpy(user->token, buf, strlen(buf) + 1);
    memcpy(user->path, buf, strlen(buf) + 1);
    
    //printf("数据库目前的错误%s\n", mysql_error(conn));
    // 查询数据库中是否存在此用户
    MYSQL_RES* result = NULL;
    sprintf(str, "select * from user where user_name = '%s'", username);
    LOG(INFO, "操作数据库的语言：%s", str);
    
    if(mysql_query(conn, str)) {
        printf("%s\n", mysql_error(conn));
        sprintf(buf, "查询数据库失败");
        memcpy(user->command, buf, sizeof(buf));
        sendError(net_fd, user);
        return -1;
    }
    //printf("查询数据库成功\n");
    result = mysql_store_result(conn);
    MYSQL_ROW row;
    //printf("after query...\n");
    if(result != NULL){
        row = mysql_fetch_row(result);
        mysql_free_result(result);
        if(row != NULL){
            // 存在，则发送-1
            LOG(ERROR, "数据库中存在该账号");
            sprintf(buf, "数据库中存在该账号");
            memcpy(user->command, buf, sizeof(buf));
            sendError(net_fd, user);
            mysql_free_result(result);
            return -1;
        }
    }
   
    // 2. 生成盐值发送给客户端
    char *salt = generate_salt();
    sendRegister(net_fd, salt);

    // 3. 接收客户端发来的处理后的密码
    int tag = 0;
    recv(net_fd, &tag, sizeof(int), MSG_WAITALL);

    recv(net_fd, &len, sizeof(int), MSG_WAITALL);
    recv(net_fd, buf, len, MSG_WAITALL);
    LOG(INFO, "客户端发来的密码信息：%s", buf);

    // TODO：将账号和客户端发来的密码存到数据库（注意，账号已被覆盖）
    // 往数据库中注册user信息
    sprintf(str, "insert into user (user_name, slat, crypt_password) values ( '%s', '%s', '%s' )" , username, salt, buf);
    if(mysql_query(conn, str)) {
        printf("%s\n", mysql_error(conn));
        LOG(ERROR, "向数据库添加用户失败");
        sprintf(buf, "向数据库添加用户失败");
        memcpy(user->command, buf, sizeof(buf));
        sendError(net_fd, user);
        return -1;
    }

    // 从数据库获得其id，生成token发送给客户端
    sprintf(str, "select id from user where user_name = '%s'", username);
    LOG(INFO, "操作数据库的语句：%s", str);
    
    
    if(mysql_query(conn, str)) {
        printf("%s\n", mysql_error(conn));
        LOG(ERROR, "获取用户id失败");
        sprintf(buf, "获取用户id失败");
        memcpy(user->command, buf, sizeof(buf));
        sendError(net_fd, user);
        return -1;
    }

    result = mysql_store_result(conn);
    row = mysql_fetch_row(result);
    mysql_free_result(result);
    int user_id = atoi(row[0]);
    
    // 创建新用户的根目录
    sprintf(str, "insert into file (user_id, path, father_path_id)  values ( '%d', '/', -1 )", user_id);
    LOG(INFO, "操作数据库的语句：%s", str);
    if(mysql_query(conn, str)) {
        printf("%s\n", mysql_error(conn));
        sprintf(buf, "创建用户失败");
        LOG(ERROR, "创建用户文件信息失败");
        memcpy(user->command, buf, sizeof(buf));
        sendError(net_fd, user);
        return -1;
    }

    char *token;
    getToken(row[0], username, &token);

    //printf("token = %s\n", token);
    // 返回根据用户id生成的token
    sendRegister(net_fd, token);
    return 0;
}

/*
 * 函数名：loginID
 * 函数作用：用于和客户端交互，注册一个账号
 * 参数：
 *  -int net_fd：连接的sockt
 *  -user_t *user：封装的用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：0：注册成功，1：注册失败
*/
int loginID(int net_fd, user_t *user, MYSQL *conn){
    // 1. 接收客户端的账号
    // 存在获取盐值，不存在返回信息
    int len = 0;
    char username[BUF_SIZE] = {0};
    char buf[BUF_SIZE] = {0};
    char password[SMALL_BUF] = {0};

    printf("启动登录函数\n");
    recv(net_fd, &len, sizeof(int), MSG_WAITALL);
    recv(net_fd, username, len, MSG_WAITALL);
    LOG(INFO, "客户端传来的账号信息：%s", username);
    // TODO：判断账号名是否重复，重复返回信息

    sprintf(buf, "this is a error");
    memcpy(user->token, buf, sizeof(buf));
    memcpy(user->path, buf, sizeof(buf));
    
    // 查询数据库中是否存在此用户
    MYSQL_RES* result = NULL;
    char str[PAGE_SIZE] = { 0 };
    sprintf(str, "select * from user where user_name = '%s'", username);
    LOG(INFO, "操作数据库的语言：%s", str);
    if(mysql_query(conn, str) == -1) {
        printf("%s\n", mysql_error(conn));
        sprintf(buf, "查询数据库失败");
        LOG(ERROR, "查询数据库失败");
        memcpy(user->command, buf, sizeof(buf));
        sendError(net_fd, user);
        return -1;
    }
    result = mysql_store_result(conn);
    
    if(result == NULL){
        sprintf(buf, "数据库中不存在该账号");
        LOG(ERROR, "数据库中不存在该账号");
        memcpy(user->command, buf, sizeof(buf));
        sendError(net_fd, user);
        return -1;
    }
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);

    // 如果找到了，发送盐值
    sendRegister(net_fd, row[3]);

    int tag = 0;
    recv(net_fd, &tag, sizeof(int), MSG_WAITALL);

    // 4. 接收客户端发来的处理后的密码

    // 客户端发送hash后的密码
    recv(net_fd, &len, sizeof(len), MSG_WAITALL);
    recv(net_fd, password, len, MSG_WAITALL);
    LOG(INFO, "从客户端获得slat：%s", password);
 
    // 判断与数据库中存储的值是否一致
    if(strcmp(row[3], password) != 0) {
        // 不一致返回-1
        sprintf(buf, "账号密码不匹配");
        LOG(ERROR, "账号密码不匹配");
        memcpy(user->command, buf, sizeof(buf));
        sendError(net_fd, user);
        return -1;
    }
    
    // 5. 生成token发送给客户端
    char *token;
    getToken(row[0], username, &token);

    // 返回根据用户id生成的token
    sendRegister(net_fd, token);
    LOG(INFO, "服务器发送登录结束信息");
}

