/*
 * 文件名：putsFile.c
 * 创建时间：24.6.6
 * 作者：shackle
 * 描述：该文件实现从客户端接受文件
*/

#include "putsFile.h"

int putsFile(int net_fd, user_t *user, MYSQL *conn){
    // 1. 接受user用户信息
    int len = 0;
    recv(net_fd, &len, sizeof(int), MSG_WAITALL);
    recv(net_fd, user, len, MSG_WAITALL);
    int userID = resolve(user->token);

    // 2. 切分出命令和文件和hash值
    char tmp[SMALL_BUF] = {0};
    memcpy(tmp, user->command, sizeof(tmp));
    char *command = strtok(tmp, " ");
    char *file = strtok(NULL, " ");
    char hash[SMALL_BUF] = {0};
    memcpy(hash, user->receive, SMALL_BUF);

    // 3. 根据用户名和文件名以及当前的路径
    // 3.1 判断文件名是否和文件夹重名
    // 3.1.1 若重名，返回失败
    // 3.1.2 若不重名，往下执行
    // 3.2 判断在当前路径下是否有该文件，
    // 3.2.1 若无该文件，则根据文件的hash值判断数据库是否有该文件
    // 3.2.1.1 若找到一样的hash值，将该条信息加入到数据库中
    // 3.2.1.2 若没有找到一样的hash值，发送偏移量为0
    // 3.2.2 若有该文件，则根据文件的hash值判断时候为同一个文件
    // 3.2.2.1 若不是同一个文件，则返回失败信息
    // 3.2.2.2 若是同一个文件，查看文件是否上传完整
    // 3.2.2.2.1 若上传的是完成的，返回发送完成
    // 3.2.2.2.2 若长传的是为完成的，返回文件的大小，即偏移量
    char str[PAGE_SIZE] = {0};
    MYSQL_RES *result = NULL;
    char information[BUF_SIZE] = {0};
    
    // 3.1 判断文件名是否和文件夹重名
    bzero(str, sizeof(str));
    sprintf(str, "select file_name from file where path = '%s' && user_id = %d", user->path, userID);
    LOG(INFO, "操作数据库的语句：%s", str);

    if(mysql_query(conn, str) == -1){
        bzero(information, sizeof(information));
        sprintf(information, "查询数据库失败");
        LOG(ERROR, "%s", information);

        memcpy(user->receive, information, sizeof(information));
        sendError(net_fd, user);
        return -1;
    }

    result = mysql_store_result(conn);
    my_ulonglong row_num = mysql_num_rows(result);
    mysql_free_result(conn);
    if(row_num > 0){
        // 3.1.1文件和文件夹同名
        bzero(information, sizeof(information));
        sprintf(information, "文件和文件夹同名，不允许上传此文件");
        LOG(ERROR, "%s", information);

        memcpy(user->receive, information, sizeof(information));
        sendError(net_fd, user);
        return -1;
    }
    



    // 3.2 判断当前路径下是否有该文件
    bzero(str, sizeof(str));
    sprintf(str, "select file_hash_sha1 from file where path = '%s' && user_id = %d && file_name = %s", user->path, userID, file);
    LOG(INFO, "操作数据库的语句：%s", str);

    if(mysql_query(conn, str) == -1){
        bzero(information, sizeof(information));
        sprintf(information, "查询数据库失败");
        LOG(ERROR, "%s", information);

        memcpy(user->receive, information, sizeof(information));
        sendError(net_fd, user);
        return -1;
    }

    result = mysql_store_result(conn);
    row_num = mysql_num_rows(result);
    MYSQL_ROW row = NULL;
    if(row_num == 0){
        // 3.2.1 当前路径下没有该文件
        // 根据文件hash值判断数据库是否存在该文件
        mysql_free_result(result);
        bzero(str, sizeof(str));
        sprintf(str, "select file_hash_sha1 from file where file_hash_sha1 = '%s'", hash);
        
        if(mysql_query(conn, str) == -1){
            bzero(information, sizeof(information));
            sprintf(information, "查询数据库失败");
            LOG(ERROR, "%s", information);

            memcpy(user->receive, information, sizeof(information));
            sendError(net_fd, user);
            return -1;
        }

        result = mysql_store_result(conn);
        row_num = mysql_num_rows(result);
        if(row_num > 0){
            // 3.2.1.1 数据库内有该文件
            // 将该条数据加入到加入到用户的表中

        }else{
            // 3.2.1.2 数据库内没有该文件
            // 发送偏移量为0的信息

        }
        mysql_free_result(result);
   


 // 3.2.2当前路径下有该文件
        row = mysql_fetch_row(result);
        if(strcmp(row[0], hash) == 0){
            // 是同一个文件
        }else{
            // 不是同一个文件，发送错误信息
            bzero(information, sizeof(information));
            sprintf(information, "文件:%s已存在", file);
            LOG(ERROR, "%s", information);

            memcpy(user->receive, information, sizeof(information));
            sendError(net_fd, user);
            return -1;
        }
    }else{
        // 3.2.1 当前路径下没有该文件
        // 根据文件hash值判断数据库是否存在该文件
        mysql_free_result(result);
        bzero(str, sizeof(str));
        sprintf(str, "select file_hash_sha1 from file where file_hash_sha1 = '%s'", hash);
        
        if(mysql_query(conn, str) == -1){
            bzero(information, sizeof(information));
            sprintf(information, "查询数据库失败");
            LOG(ERROR, "%s", information);

            memcpy(user->receive, information, sizeof(information));
            sendError(net_fd, user);
            return -1;
        }

        result = mysql_store_result(conn);
        row_num = mysql_num_rows(result);
        if(row_num > 0){
            // 3.2.1.1 数据库内有该文件
        }else{
            // 3.2.1.2 数据库内没有该文件
        }
        mysql_free_result(result);
   

    }
    mysql_free_result(result);
    

}
