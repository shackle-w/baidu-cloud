#include "head.h"

int loginRequest(int netFd, protocol_client protocol, pool_t* pool){

    HashMap *map = createHashMap();
    getParameterMap(protocol, map);

    char *username = get(map, "username");
    char *password = get(map, "password");

    // 获取一个数据库连接: 从数据库连接池
    sql_connect_t sqlConnect;
    memset(&sqlConnect, 0, sizeof(sqlConnect));
    getConnect(&sqlConnect, pool);

    // 根据用户名获得盐值和hash密码
    char bufSql[200] = {0};
    char *str = "select * from user where user_name = ";
    sprintf(bufSql, "%s", str);
    sprintf(bufSql + strlen(str), "'%s'", username);
    mysql_query(&sqlConnect.connect, bufSql);

    MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);
    my_ulonglong row_num = mysql_num_rows(result);
    if(row_num == 0){
        // 不存在该用户
        protocol_serve  protocol_res;
        memset(&protocol_res, 0, sizeof(protocol_res));
        protocol_res.status = -1;
        protocol_res.parameter_flag = 0;
        protocol_res.parameter_len = 0;

        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

        setConnect(sqlConnect, pool);
        mysql_free_result(result);
        return 0;
    }

    // 保存数据库查询结果
    use_msg_t sqlUser;
    memset(&sqlUser, 0, sizeof(sqlUser));

    MYSQL_ROW row;
    if ((row = mysql_fetch_row(result))) {
        sqlUser.userId = atoi(row[0]);
        strcpy(sqlUser.userName, row[1]);
        strcpy(sqlUser.salt, row[2]);
        strcpy(sqlUser.cryptPassword, row[3]);
    }

    // 使用完毕之后, 把数据库连接还回去
    setConnect(sqlConnect, pool);
    mysql_free_result(result);

    // 根据盐值和输入密码, 计算新hash密码
    char *cryptPasswordStr = cryptPassword(password, sqlUser.salt);
    // 比较计算结果 和 数据库查询出来的密码
    if(strcmp(cryptPasswordStr, sqlUser.cryptPassword) != 0){
        // 密码不一致
        protocol_serve  protocol_res;
        memset(&protocol_res, 0, sizeof(protocol_res));
        protocol_res.status = -1;
        protocol_res.parameter_flag = 0;
        protocol_res.parameter_len = 0;

        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);
        return 0;
    } else{
        // hash后密码一致
    }
    free(cryptPasswordStr);

    // 生产token
    char bufToken[1024] = {0};
    getToken(sqlUser.userId, bufToken);

    // 登陆成功: 返回给客户端token
    protocol_serve  protocol_res;
    memset(&protocol_res, 0, sizeof(protocol_res));
    protocol_res.status = 0;
    protocol_res.parameter_flag = 1;
    protocol_res.parameter_num = 1;

    stringCat(protocol_res.buf, "token", bufToken);
    protocol_res.parameter_len = strlen(protocol_res.buf);

    int send_len = sizeof(int)*4 + protocol_res.parameter_len;
    send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
    send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

    return 0;
}
int registerNameRequest(int netFd, protocol_client protocol, pool_t* pool){

    HashMap *map = createHashMap();
    getParameterMap(protocol, map);
    char *username = get(map, "username");

    // 获取一个数据库连接: 从数据库连接池
    sql_connect_t sqlConnect;
    memset(&sqlConnect, 0, sizeof(sqlConnect));
    getConnect(&sqlConnect, pool);

    char bufSql[200] = {0};
    char *str = "select * from user where user_name = ";
    sprintf(bufSql, "%s", str);
    sprintf(bufSql + strlen(str), "'%s'", username);
    mysql_query(&sqlConnect.connect, bufSql);

    MYSQL_RES *res = mysql_store_result(&sqlConnect.connect);

    my_ulonglong row_num = mysql_num_rows(res);

    // 使用完毕之后, 把数据库连接还回去
    setConnect(sqlConnect, pool);

    protocol_serve  protocol_res;
    memset(&protocol_res, 0, sizeof(protocol_res));

    if(row_num > 0){
        // 存在该用户
        protocol_res.status = -1;
        protocol_res.parameter_flag = 0;
        protocol_res.parameter_len = 0;

        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

    }else{
        // 不存在该用户
        protocol_res.status = 0;
        protocol_res.parameter_flag = 0;
        protocol_res.parameter_len = 0;

        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);
    }

    mysql_free_result(res);
    return 0;
}

int registerPasswordRequest(int netFd, protocol_client protocol, pool_t* pool){
    HashMap *map = createHashMap();
    getParameterMap(protocol, map);

    char *username = get(map, "username");
    char *password = get(map, "password");

    char *saltStr = salt();
    char *cryptPasswordStr = cryptPassword(password, saltStr);

    // 获取一个数据库连接: 从数据库连接池
    sql_connect_t sqlConnect;
    memset(&sqlConnect, 0, sizeof(sqlConnect));
    getConnect(&sqlConnect, pool);

    char bufSql[200] = {0};
    sprintf(bufSql, "insert into user (user_name, slat, crypt_password) values ( '%s', '%s', '%s' )", username, saltStr, cryptPasswordStr);

    int resQuery = mysql_query(&sqlConnect.connect, bufSql);
    if(resQuery != 0){
        printf("注册用户失败 \n");
        return 0;
    }

    MYSQL_RES *res = mysql_store_result(&sqlConnect.connect);
    mysql_free_result(res);

    // 先获得用户id
    int userId = 0;
    bzero(bufSql, sizeof(bufSql));
    sprintf(bufSql, "select * from user where user_name = '%s'", username);
    mysql_query(&sqlConnect.connect, bufSql);
    res = mysql_store_result(&sqlConnect.connect);
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(res))) {
        userId = atoi(row[0]);
    }
    mysql_free_result(res);
    
    // 给这个用户创建一个根目录
    bzero(bufSql, sizeof(bufSql));
    sprintf(bufSql, "insert into file (user_id, path, father_path_id)  values ( '%d', '/', -1 )", userId);
    mysql_query(&sqlConnect.connect, bufSql);
    res = mysql_store_result(&sqlConnect.connect);
    mysql_free_result(res);

    // 使用完毕之后, 把数据库连接还回去
    setConnect(sqlConnect, pool);

    free(cryptPasswordStr);
    free(saltStr);

    // 构建响应信息
    protocol_serve  protocol_res;
    memset(&protocol_res, 0, sizeof(protocol_res));

    protocol_res.status = 0;
    protocol_res.parameter_flag = 0;
    protocol_res.parameter_len = 0;

    int send_len = sizeof(int)*4 + protocol_res.parameter_len;
    send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
    send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

    return 0;
}
