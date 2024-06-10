#include "head.h"

int lsRequest(int netFd, protocol_client protocol, pool_t *pool){

    // 解析请求协议中的参数-> 存到map中
    HashMap *map = createHashMap();
    getParameterMap(protocol, map);

    // 获得解析的token 和当前用户所在的路径
    char *token = get(map, "token");
    char *path = get(map, "path");

    // 从token中获得userId
    int userId = 0;
    userId = getUserId(token);
    if(userId == -1){
        return 0;
    }

    // 获取一个数据库连接: 从数据库连接池
    sql_connect_t sqlConnect;
    memset(&sqlConnect, 0, sizeof(sqlConnect));
    getConnect(&sqlConnect, pool);

    // 拼接查询sql
    char bufSql[200] = {0};
    sprintf(bufSql, 
            "select file_name from file where delete_flag=0 and father_path_id= "
            "(select id from file where user_id=%d and path='%s' and delete_flag=0)",
            userId, path);
    // 查询
    mysql_query(&sqlConnect.connect, bufSql);

    MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);
    my_ulonglong row_num = mysql_num_rows(result);

    char fileList[1024] = {0};
    if(row_num > 0){
        // 当前路径下有文件或者文件夹-> 把查出的文件名字, 都存在fileList中
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            strcat(fileList, row[0]);
            strcat(fileList, "  ");
        }
    }else{
        // 当前路径下没有文件或者文件夹
    }
    mysql_free_result(result);
    // 使用完毕之后, 把数据库连接还回去
    setConnect(sqlConnect, pool);


    protocol_serve  protocol_res;
    memset(&protocol_res, 0, sizeof(protocol_res));

    protocol_res.status = 0;
    if(strlen(fileList)> 0){
        protocol_res.parameter_flag = 1;
        protocol_res.parameter_num = 1;

        stringCat(protocol_res.buf, "fileList", fileList);
        protocol_res.parameter_len = strlen(protocol_res.buf);
    }else{
        protocol_res.parameter_flag = 0;
        protocol_res.parameter_num = 0;

        protocol_res.parameter_len = 0;
    }

    int send_len = sizeof(int)*4 + protocol_res.parameter_len;
    send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
    send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

    return 0;
}


