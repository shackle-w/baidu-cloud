#include "head.h"

// 删除文件或者文件夹
int rmRequest(int netFd, protocol_client protocol, pool_t * pool){
    HashMap *map = createHashMap();
    getParameterMap(protocol, map);

    char *token = get(map, "token");
    char *path = get(map, "path");
    char *fileOrDirName = get(map, "fileOrDirName");

    int userId = 0;
    // 从token中获得userId
    userId = getUserId(token);

    // 获取一个数据库连接: 从数据库连接池
    sql_connect_t sqlConnect;
    memset(&sqlConnect, 0, sizeof(sqlConnect));
    getConnect(&sqlConnect, pool);

    // 拼接查询sql
    // 获取当前用户在这个路径下是否已经存在这个路径文件夹
    char bufSql[200] = {0};
    sprintf(bufSql, 
            "select * from file where delete_flag=0 and file_name='%s' and "
            "father_path_id=(select id from file where delete_flag=0 and user_id=%d and path='%s')"
            , fileOrDirName, userId, path);
    // 查询
    mysql_query(&sqlConnect.connect, bufSql);

    MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);
    my_ulonglong row_num = mysql_num_rows(result);
    if(row_num <= 0){
        // 该路径下不存在这个文件或者目录
        protocol_serve  protocol_res;
        memset(&protocol_res, 0, sizeof(protocol_res));
        protocol_res.status = -1;
        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

        // 放回连接
        setConnect(sqlConnect, pool);
        mysql_free_result(result);

        return 0;
    }else{
        // 该路径下存在这个文件目录或者状态
        // 获得文件状态
        MYSQL_ROW row = mysql_fetch_row(result);
        int fileType = atoi(row[4]);

        if(fileType == 1){
            // 是个文件, 删除这个文件
            int fileId = atoi(row[0]);            

            mysql_free_result(result);

            bzero(bufSql, sizeof(bufSql));
            sprintf(bufSql, 
                    "update file set delete_flag=1 where id = %d "
                    , fileId);
            // 查询
            mysql_query(&sqlConnect.connect, bufSql);

            MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);

            protocol_serve  protocol_res;
            memset(&protocol_res, 0, sizeof(protocol_res));
            int send_len = sizeof(int)*4 + protocol_res.parameter_len;
            send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
            send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

            // 放回连接
            setConnect(sqlConnect, pool);
            mysql_free_result(result);
            return 0;

        }else{
            // 是个文件夹
            int fileId = atoi(row[0]);            
            mysql_free_result(result);

            // 判断如果是个文件夹下, 是否有更多文件
            bzero(bufSql, sizeof(bufSql));            
            sprintf(bufSql, 
                    "select * from file where delete_flag=0 and father_path_id=%d  "
                    , fileId);
            // 查询
            mysql_query(&sqlConnect.connect, bufSql);

            MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);
            my_ulonglong row_num = mysql_num_rows(result);
            mysql_free_result(result);

            if(row_num <= 0){
                // 如果不存在, 删除
                bzero(bufSql, sizeof(bufSql));
                sprintf(bufSql, 
                        "update file set delete_flag=1 where id = %d "
                        , fileId);
                // 查询
                mysql_query(&sqlConnect.connect, bufSql);
                MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);
                mysql_free_result(result);
                // 放回连接
                setConnect(sqlConnect, pool);

                protocol_serve  protocol_res;
                memset(&protocol_res, 0, sizeof(protocol_res));
                int send_len = sizeof(int)*4 + protocol_res.parameter_len;
                send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
                send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

                return 0;
            }else{
                // 如果存在, 返回错误
                protocol_serve  protocol_res;
                memset(&protocol_res, 0, sizeof(protocol_res));
                protocol_res.status = -1;
                int send_len = sizeof(int)*4 + protocol_res.parameter_len;
                send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
                send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

                // 放回连接
                setConnect(sqlConnect, pool);
                return 0;
            }
        }
    }

    return 0;
}


// 递归删除
int rmRRequest(int netFd, protocol_client protocol, pool_t * pool){
    HashMap *map = createHashMap();
    getParameterMap(protocol, map);
    char *token = get(map, "token");
    char *path = get(map, "path");
    char *fileOrDirName = get(map, "fileOrDirName");

    int userId = 0;
    // 从token中获得userId
    userId = getUserId(token);

    // 获取一个数据库连接: 从数据库连接池
    sql_connect_t sqlConnect;
    memset(&sqlConnect, 0, sizeof(sqlConnect));
    getConnect(&sqlConnect, pool);

    // 拼接查询sql
    // 获取当前用户在这个路径下是否已经存在这个路径文件夹
    char bufSql[200] = {0};
    sprintf(bufSql, 
            "select * from file where delete_flag=0 and file_name='%s' and "
            "father_path_id=(select id from file where delete_flag=0 and user_id=%d and path='%s')"
            , fileOrDirName, userId, path);
    // 查询
    mysql_query(&sqlConnect.connect, bufSql);

    MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);
    my_ulonglong row_num = mysql_num_rows(result);
    mysql_free_result(result);
    if(row_num <= 0){
        // 该路径下不存在这个目录
        protocol_serve  protocol_res;
        memset(&protocol_res, 0, sizeof(protocol_res));
        protocol_res.status = -1;
        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

        // 放回连接
        setConnect(sqlConnect, pool);
        mysql_free_result(result);
        return 0;
    }else {
        // 该路径下存在这个目录 or 文件
        // 全部标记为删除

        bzero(bufSql, sizeof(bufSql));
        sprintf(bufSql, 
                "update file set delete_flag=1 where user_id=%d and path like '%s%s/%s' "
                ,userId,  path, fileOrDirName, "%");
        // 查询
        mysql_query(&sqlConnect.connect, bufSql);
        MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);
        mysql_free_result(result);
        // 放回连接
        setConnect(sqlConnect, pool);

        protocol_serve  protocol_res;
        memset(&protocol_res, 0, sizeof(protocol_res));
        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

        return 0;
    }
    return 0;
}
