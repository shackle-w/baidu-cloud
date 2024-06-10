#include "head.h"

int mkdirRequest(int netFd, protocol_client protocol, pool_t *pool){
    HashMap *map = createHashMap();
    getParameterMap(protocol, map);

    char *token = get(map, "token");
    char *path = get(map, "path");
    char *dirName = get(map, "dirName");

    int userId = 0;
    // 从token中获得userId
    userId = getUserId(token);
    if(userId == -1){
        // token不对, 导致token解析userId出错
        // 客户端提供的token不对, 暂时不着急处理, 正常来讲不可能出现这种情况
        return 0;
    }

    // 拼接这个要创建的文件夹的路径
    char dirPath[100] = {0};
    sprintf(dirPath, "%s%s/", path, dirName);


    // 获取一个数据库连接: 从数据库连接池
    sql_connect_t sqlConnect;
    memset(&sqlConnect, 0, sizeof(sqlConnect));
    getConnect(&sqlConnect, pool);

    // 拼接查询sql
    // 获取当前用户在这个路径下是否已经存在这个路径文件夹
    char bufSql[200] = {0};
    sprintf(bufSql, 
            "select * from file where delete_flag=0 and file_name = '%s' " 
            " and father_path_id = (select id from file where delete_flag=0 and user_id=%d and path='%s')"
            ,dirName, userId, path);
    // 查询
    mysql_query(&sqlConnect.connect, bufSql);

    MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);
    my_ulonglong row_num = mysql_num_rows(result);
    if(row_num != 0){
        // 这个路径文件夹已经存在
        protocol_serve  protocol_res;
        memset(&protocol_res, 0, sizeof(protocol_res));
        protocol_res.status = -1;
        protocol_res.parameter_flag = 0;
        protocol_res.parameter_len = 0;

        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

        // 放回连接
        setConnect(sqlConnect, pool);
        mysql_free_result(result);
        return 0;
    }
    // 释放上次查询结果
    mysql_free_result(result);

    // 获得父级路径的id
    int currentPathId = 0;
    bzero(bufSql, sizeof(bufSql));
    sprintf(bufSql, 
            "select * from file where delete_flag=0 and user_id=%d and path='%s' ",
            userId, path);
    mysql_query(&sqlConnect.connect, bufSql);
    result = mysql_store_result(&sqlConnect.connect);
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(result))) {
        currentPathId = atoi(row[0]);
    }
    mysql_free_result(result);

    // 没有这个文件夹, 就创建这个文件夹
    bzero(bufSql, sizeof(bufSql));
    sprintf(bufSql, 
            "insert into file (user_id, path, father_path_id, file_name)  values ( %d, '%s', %d, '%s')",
            userId, dirPath, currentPathId, dirName);

    mysql_query(&sqlConnect.connect, bufSql);
    result = mysql_store_result(&sqlConnect.connect);
    mysql_free_result(result);

    // 使用完毕之后, 把数据库连接还回去
    setConnect(sqlConnect, pool);

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
