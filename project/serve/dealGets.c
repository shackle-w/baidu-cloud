#include "head.h"

int dealGets(int netFd, pool_t *pool){

    // --------------------接收客户端上传的文件信息-----------------
    int msgLen = 0;
    recv(netFd, &msgLen, sizeof(int), MSG_WAITALL);
    protocol_client  protocol;
    bzero(&protocol, sizeof(protocol));
    recv(netFd, &protocol, msgLen, MSG_WAITALL);

    // ---------------解析参数-------------------
    HashMap *map = createHashMap();
    getParameterMap(protocol, map);
    char *token = get(map, "token");
    char *path = get(map, "path"); 
    char *fileName = get(map, "fileName"); 

    int userId = getUserId(token);

    // --------------- 获得这个用户下的当前路径:是否有这个文件信息 -------------------
    // --------------有则回传文件, 没有回传失败--------------------------
    sql_connect_t sqlConnect;
    memset(&sqlConnect, 0, sizeof(sqlConnect));
    getConnect(&sqlConnect, pool);

    // 拼接查询sql
    // 根据用户id, path, fileName -> 文件类型, 文件sha1值, 加载文件
    char bufSql[200] = {0};
    sprintf(bufSql,
            "select * from file where upload_status=1 and delete_flag=0 and file_name = '%s' and father_path_id = "
            "(select id from file where delete_flag=0 and user_id=%d and path='%s' )",
            fileName,userId, path);

    mysql_query(&sqlConnect.connect, bufSql);
    MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);
    my_ulonglong row_num = mysql_num_rows(result);

    if(row_num > 0){
        MYSQL_ROW row = mysql_fetch_row(result);
        // 有这个文件, 获取hash值
        char sha1[100] = {0};
        memcpy(sha1, row[7], sizeof(sha1));
        
        //  当前路径下存在该文件   
        protocol_serve  protocol_res;
        memset(&protocol_res, 0, sizeof(protocol_res));
        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

        // 使用sha1_hash值, 在配置文件中指明的base_path下创建一个文件
        // 打开获得file_fd
        char hashFilePathName[1024] = {0};
        char * basePath = get(pool->map , "base_path");
        sprintf(hashFilePathName, "%s%s", basePath, sha1);

        int file_fd = open(hashFilePathName, O_RDWR);
        sendFile(netFd, file_fd);
        close(file_fd);
    }else{
        //  当前路径下不存在该文件   
        protocol_serve  protocol_res;
        memset(&protocol_res, 0, sizeof(protocol_res));
        protocol_res.status = -1;
        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);
    }

        mysql_free_result(result);
    // 使用完毕之后, 把数据库连接还回去
    setConnect(sqlConnect, pool);

    return 0;
}


// 回复给客户端同步文件下载的回复
int syncGets(int net_fd){

    protocol_serve  protocol_res;
    memset(&protocol_res, 0, sizeof(protocol_res));
    int send_len = sizeof(int)*4 + protocol_res.parameter_len;
    send(net_fd, &send_len, sizeof(int), MSG_NOSIGNAL);
    send(net_fd, &protocol_res, send_len, MSG_NOSIGNAL);

    return 0;
}
