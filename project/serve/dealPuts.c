#include "head.h"

int dealPuts(int netFd, pool_t *pool){

    // --------------------接收客户端上传的文件信息-----------------
    time_t first_time;
    time(&first_time);

    // 当前子进程不能一直recv等待
    // (有可能客户端要上传文件, 但是自己只发了同步请求, 然后文件名字出错, 没有发文件上传)
    // 如果超过一定时间, 客户端不发信息, 退出去
    // 使用不阻塞的recv读取数据, 这样可以一边读, 一边计时, 超时则断开
    char *p = (char *)malloc(5);
    int len = 0;
    while(len < 4){
        ssize_t sret = recv(netFd,p+len,4-len, MSG_DONTWAIT);
        if(sret == 0){
            return 0;
        }
        if(sret == -1){
            // 无数据到来, 检测时间, 给5秒等待,
            // 如果5秒都没来, 基本上意味着客户端在准备文件信息的时候出错
            // 那就结束这次上传任务
            time_t now_time;
            time(&now_time);
            if(now_time - first_time > 5){
                // 等待客户端的文件信息时超时  
                return 0;
            }
            continue;
        }
        len += sret;
    }
    int* msgLen = (int *)p;

    protocol_client  protocol;
    bzero(&protocol, sizeof(protocol));
    recv(netFd, &protocol, *msgLen, MSG_WAITALL);

    // ---------------解析参数-------------------
    HashMap *map = createHashMap();
    getParameterMap(protocol, map);

    char *token = get(map, "token");
    char *path = get(map, "path"); 
    char *fileName = get(map, "fileName"); 
    char *sha1 = get(map, "sha1"); 
    char *md5 = get(map, "md5"); 
    int fileSize = atoi(get(map, "fileSize")); 

    int userId = 0;
    // 从token中获得userId
    userId = getUserId(token);
    if(userId == -1){
        // token不对, 导致token解析userId出错
        // 客户端提供的token不对, 暂时不着急处理, 正常来讲不可能出现这种情况
        return 0;
    }

    // --------------- 获得这个用户下的当前路径:是否有这个文件信息 -------------------
    // --------------- 判断路径重名,续传,秒传,新传 -------------------
    sql_connect_t sqlConnect;
    memset(&sqlConnect, 0, sizeof(sqlConnect));
    getConnect(&sqlConnect, pool);

    // 拼接查询sql
    // 根据用户id, path, fileName -> 文件类型, 文件md5, sha1, 上传状态
    char bufSql[200] = {0};
    sprintf(bufSql,
            "select * from file where delete_flag=0 and file_name = '%s' and father_path_id = "
            "(select id from file where delete_flag=0 and user_id=%d and path='%s' )",
            fileName,userId, path);

    mysql_query(&sqlConnect.connect, bufSql);
    MYSQL_RES *result = mysql_store_result(&sqlConnect.connect);
    my_ulonglong row_num = mysql_num_rows(result);

    // 构建相应报文
    protocol_serve  protocol_res;
    memset(&protocol_res, 0, sizeof(protocol_res));

    if(row_num >0){
        // 这个用户的这个路径下, 已经存在这个同名的文件或者文件夹了
        // 获得这个文件或者文件夹的信息
        file_msg_t file;
        memset(&file, 0, sizeof(file));

        MYSQL_ROW row = mysql_fetch_row(result);
        file.fileType = atoi(row[4]);
        memcpy(file.md5, row[6], strlen(row[6]));
        memcpy(file.sha1, row[7], strlen(row[7]));
        file.uploadStatus = atoi(row[8]);

        if(file.fileType == 0){
            // 是个文件夹: 相同路径下,不允许文件文件夹同名
            // 状态码:11
            protocol_res.status = 11;
        }else if(file.uploadStatus ==0 && strcmp(file.md5, md5) ==0 && strcmp(file.sha1,sha1) == 0){
            // 一个相同的未上传成功的文件
            // 需要断点续传
            // 状态码:20(需要续传)
            protocol_res.status = 20;
        }else{
            // 这个路径下: 已经存在一个同名文件
            // 而且, 这个文件要么上传已经成功, 要么和当前文件不是同一个文件(只是名字相同而已)
            // 状态码:12 (文件名重复)
            protocol_res.status = 12;
        }

    }else{
        mysql_free_result(result);

        // 这个用户的这个路径下没有这个文件, 查看服务器上是否拥有这个文件
        // 根据md5 和 sha1值查找
        // 拼接查询sql
        bzero(bufSql, sizeof(bufSql));
        sprintf(bufSql,
                "select * from file where  " 
                " file_hash_md5 = '%s' and file_hash_sha1 = '%s' ",
                md5, sha1);
        mysql_query(&sqlConnect.connect, bufSql);
        result = mysql_store_result(&sqlConnect.connect);
        my_ulonglong row_num = mysql_num_rows(result);

        if(row_num <= 0){
            // 没有这个文件-> 新传逻辑: 状态码0
            protocol_res.status = 0;
        }else{
            file_msg_t file;
            memset(&file, 0, sizeof(file));

            MYSQL_ROW row = mysql_fetch_row(result);

            file.uploadStatus = atoi(row[8]);

            if(file.uploadStatus == 1){
                // 有这个文件, 成功上传状态 -> 秒传: 30
                protocol_res.status = 30;
            }else{
                // 有这个文件, 未成功上传状态 -> 续传: 状态码21(先帮别人续传)
                protocol_res.status = 21;
            }
        }
    }
    
    mysql_free_result(result);

    // 会报文给客户端当前逻辑状态
    int send_len = sizeof(int)*4 + protocol_res.parameter_len;
    send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
    send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

    // --------------接下来: 处理新传/秒传/续传-------------------------
    // --------------先获得当前路径------------------------------------
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
    // --------------根据 protocol.status 判断--------------------------

    if(protocol_res.status == 0){
        // 服务器中从来没有过这个文件, 全新上传
        char * basePath = get(pool->map , "base_path");

        // 使用sha1_hash值, 在配置文件中指明的base_path下创建一个文件
        // 打开获得file_fd
        char hashFilePathName[1024] = {0};
        sprintf(hashFilePathName, "%s%s", basePath, sha1);
        int file_fd = open(hashFilePathName, O_RDWR|O_CREAT, 0600);

        // 在数据库中增加这个记录
        char filePath[1024] = {0};
        sprintf(filePath, "%s%s", path, fileName);

        // 先去数据库加数据,但是文件的状态, 要是未上传成功状态, 
        // 等文件上传成功, 再修改状态
        bzero(bufSql, sizeof(bufSql));
        sprintf(bufSql,
                "insert into file" 
                "(user_id,path,father_path_id,file_type,file_name,file_hash_md5,file_hash_sha1,upload_status)"
                "values ( %d,  '%s', %d , 1, '%s', '%s', '%s', 0)"
                ,userId, filePath,currentPathId, fileName, md5, sha1);
        mysql_query(&sqlConnect.connect, bufSql);
        result = mysql_store_result(&sqlConnect.connect);
        mysql_free_result(result);

        // 调用函数接收上传文件
        uploadFile(netFd, file_fd);

        // 再修改上传为完成 
        bzero(bufSql, sizeof(bufSql));
        sprintf(bufSql,
                "update file set upload_status=1  "
                "where path = '%s' and user_id = %d and  delete_flag=0"
                ,filePath,userId);

        mysql_query(&sqlConnect.connect, bufSql);
        result = mysql_store_result(&sqlConnect.connect);
        mysql_free_result(result);

        close(file_fd);
    }else if(protocol_res.status == 20){
        
        // 当前路径下已经有这个文件, 只不过属于未上传完成状态
        char * basePath = get(pool->map , "base_path");

        // 使用sha1_hash值, 在配置文件中指明的base_path下创建一个文件
        // 打开获得file_fd
        char hashFilePathName[1024] = {0};
        sprintf(hashFilePathName, "%s%s", basePath, sha1);
        // 在当前路径下已经存在这个文件, 但是需要续传
        // 读取当前文件大小: 
        int file_fd = open(hashFilePathName, O_RDWR);
        struct stat stat_file;
        fstat(file_fd, &stat_file);

        // 把文件大小转为字符串
        char currentServeFileSize[20] = {0};
        sprintf(currentServeFileSize, "%ld", stat_file.st_size); 

        close(file_fd);

        // 回复文件大小给客户端
        memset(&protocol_res, 0, sizeof(protocol_res));
        protocol_res.status = 0;
        protocol_res.parameter_flag = 1;
        protocol_res.parameter_num = 1;
        stringCat(protocol_res.buf, "currentServeFileSize", currentServeFileSize);

        protocol_res.parameter_len = strlen(protocol_res.buf);
        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

        // 开始续传,
        FILE *file = fopen(hashFilePathName, "a+");
        uploadFileBreakPoint(netFd, file);
        fclose(file);

        // 再修改上传为完成 
        char filePath[1024] = {0};
        sprintf(filePath, "%s%s", path, fileName);

        bzero(bufSql, sizeof(bufSql));
        sprintf(bufSql,
                "update file set upload_status=1  "
                "where path = '%s' and user_id = %d and  delete_flag=0"
                ,filePath,userId);

        mysql_query(&sqlConnect.connect, bufSql);
        result = mysql_store_result(&sqlConnect.connect);
        mysql_free_result(result);


    }else if(protocol_res.status == 21){
        // 再非当前路径下, 有个相同文件, 但是这个文件是未上传成功状态, 需要跨用户和路径续传
        
        // 获得文件大小:告诉客户端
        char * basePath = get(pool->map , "base_path");

        // 使用sha1_hash值, 在配置文件中指明的base_path下创建一个文件
        // 打开获得file_fd
        char hashFilePathName[1024] = {0};
        sprintf(hashFilePathName, "%s%s", basePath, sha1);
        // 在当前路径下已经存在这个文件, 但是需要续传
        // 读取当前文件大小: 
        int file_fd = open(hashFilePathName, O_RDWR);
        struct stat stat_file;
        fstat(file_fd, &stat_file);

        // 把文件大小转为字符串
        char currentServeFileSize[20] = {0};
        sprintf(currentServeFileSize, "%ld", stat_file.st_size); 

        close(file_fd);

        // 回复文件大小给客户端
        memset(&protocol_res, 0, sizeof(protocol_res));
        protocol_res.status = 0;
        protocol_res.parameter_flag = 1;
        protocol_res.parameter_num = 1;
        stringCat(protocol_res.buf, "currentServeFileSize", currentServeFileSize);

        protocol_res.parameter_len = strlen(protocol_res.buf);
        int send_len = sizeof(int)*4 + protocol_res.parameter_len;
        send(netFd, &send_len, sizeof(int), MSG_NOSIGNAL);
        send(netFd, &protocol_res, send_len, MSG_NOSIGNAL);

        // 开始续传,
        FILE *file = fopen(hashFilePathName, "a+");
        uploadFileBreakPoint(netFd, file);
        fclose(file);

        // 传完修改源文件上传状态 
        bzero(bufSql, sizeof(bufSql));
        sprintf(bufSql,
                "update file set upload_status=1  "
                "where  file_hash_md5 = '%s'  and file_hash_sha1 = '%s' and upload_status = 0 "
                ,md5,sha1);
        mysql_query(&sqlConnect.connect, bufSql);
        result = mysql_store_result(&sqlConnect.connect);
        mysql_free_result(result);
        
        // 再向数据库添加数据
        char filePath[1024] = {0};
        sprintf(filePath, "%s%s", path, fileName);

        bzero(bufSql, sizeof(bufSql));
        sprintf(bufSql,
                "insert into file" 
                "(user_id,path,father_path_id,file_type,file_name,file_hash_md5,file_hash_sha1,upload_status)"
                "values ( %d,  '%s', %d , 1, '%s', '%s', '%s', 1)"
                ,userId, filePath,currentPathId, fileName, md5, sha1);
        mysql_query(&sqlConnect.connect, bufSql);
        result = mysql_store_result(&sqlConnect.connect);
        mysql_free_result(result);

    }else if(protocol_res.status == 30){
        // 秒传逻辑: 直接向数据库加数据
        // 需要参数: user_id, path(文件不以/结尾), father_path_id, file_name, md5, sha1
        char filePath[1024] = {0};
        sprintf(filePath, "%s%s", path, fileName);

        bzero(bufSql, sizeof(bufSql));
        sprintf(bufSql,
                "insert into file" 
                "(user_id,path,father_path_id,file_type,file_name,file_hash_md5,file_hash_sha1,upload_status)"
                "values ( %d,  '%s', %d , 1, '%s', '%s', '%s', 1)"
                ,userId, filePath,currentPathId, fileName, md5, sha1);
        mysql_query(&sqlConnect.connect, bufSql);
        result = mysql_store_result(&sqlConnect.connect);
        mysql_free_result(result);
    }

    // 使用完毕之后, 把数据库连接还回去
    setConnect(sqlConnect, pool);

    return 0;
}


// 回复给客户端同步文件上传的回复
int syncPuts(int net_fd){

    protocol_serve  protocol_res;
    memset(&protocol_res, 0, sizeof(protocol_res));
    int send_len = sizeof(int)*4 + protocol_res.parameter_len;
    send(net_fd, &send_len, sizeof(int), MSG_NOSIGNAL);
    send(net_fd, &protocol_res, send_len, MSG_NOSIGNAL);

    return 0;
}
