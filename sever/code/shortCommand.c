/*
 * 文件名：shortCommand.c
 * 创建时间：5.31
 * 作者：shackle
 * 描述：该文件是实现短命令的具体逻辑
*/

#include "command.h"

/*
 * 函数名：realizeCD
 * 函数作用：处理CD命令
 * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0， 失败返回-1
*/
int realizeCD(int net_fd, user_t *user, MYSQL *conn){
    // 1. 获取用户char *userID
    int userID = resolve(user->token);

    // 2. 对命令进行切分
    char buf[BUF_SIZE] = {0};
    memcpy(buf, user->command, sizeof(buf));
    LOG(INFO, "要处理的命令：%s", buf);
    int len = strlen(buf);
    buf[len] = '\0';
    
    char *command = strtok(buf, " ");
    char *path_buf = strtok(NULL, "\n");
    
    char information[BUF_SIZE] = {0};
    MYSQL_RES *result = NULL;
    char str[PAGE_SIZE] = {0};
    if(path_buf == NULL || strcmp(path_buf, ".") == 0){
        // 不用更改返回结果即可
        memcpy(user->receive, user->path, BUF_SIZE);
        sendInformation(net_fd, user);
        sendOver(net_fd);

        return 0;
    }else if(strcmp(path_buf, "..") == 0){
        // 1. 根据用户id和当前路径，获取当前路径的父路径id
        // 若当前路径的父路径id = -1,不用更改路径返回
        bzero(str, sizeof(str));
        sprintf(str, "select father_path_id from file where path = '%s' && user_id = %d", user->path, userID);
        LOG(INFO, "操作数据库的语句：%s", str);

        if(mysql_query(conn, str) == -1){
            LOG(ERROR, "%s", mysql_error(conn));
            bzero(information, sizeof(information));
            sprintf(information, "查询数据库失败");
            memcpy(user->receive, information, sizeof(buf));
            sendError(net_fd, user);
            sendOver(net_fd);

            return -1;
        }
        result = mysql_store_result(conn);

        MYSQL_ROW row = mysql_fetch_row(result);
        mysql_free_result(result);
        
        int fid = atoi(row[0]);
        bzero(str, sizeof(str));
        if(fid == -1){
            // 说明已经是父目录
            memcpy(user->receive, user->path, BUF_SIZE);
            sendInformation(net_fd, user);
            sendOver(net_fd);

            return 0;
        }
        sprintf(str, "select path from file where id = %d", fid);
        LOG(INFO, "操作数据库的语句：%s", str);

        if(mysql_query(conn, str) == -1){
            LOG(ERROR, "%s", mysql_error(conn));
            bzero(information, sizeof(information));
            sprintf(information, "查找父目录失败");
            memcpy(user->receive, information, sizeof(buf));
            sendError(net_fd, user);
            sendOver(net_fd);

            return -1;
        }
        result = mysql_store_result(conn);

        row = mysql_fetch_row(result);
        mysql_free_result(result);
        

        strcpy(user->receive, row[0]);
        sendInformation(net_fd, user);
        sendOver(net_fd);

        return 0;
    }else{
        // 封装路径
        char tmp[BUF_SIZE] = {0};
         // 绝对路径，不做处理，相对路径，封装
        if(tmp[0] == '/'){
            // 绝对路径不做处理
        }else{
            strcat(tmp, user->path);
        }

        strcat(tmp, path_buf);
        int path_len = strlen(tmp);
        if(tmp[path_len - 1] == '/'){
            // 结尾是/不做处理
        }else{
            tmp[path_len] = '/';
            tmp[path_len + 1] = '\0';
        }


        bzero(str, sizeof(str));
        sprintf(str, "select id from file where user_id = %d && path = '%s' && delete_flag = 0",userID, tmp);
        LOG(INFO, "操作数据库的语句：%s", str);

        if(mysql_query(conn, str) == -1){
            LOG(ERROR, "%s", mysql_error(conn));
            bzero(information, sizeof(information));
            sprintf(information, "查询数据库失败");

            memcpy(user->receive, information, sizeof(buf));
            sendError(net_fd, user);
            sendOver(net_fd);

            return -1;
        }
 
        result = mysql_store_result(conn);
        my_ulonglong row_num = mysql_num_rows(result);
        if(row_num != 0){
            // 说明存在该路径，将该路径封装返回
            memcpy(user->receive, tmp, BUF_SIZE);
            sendInformation(net_fd, user);
            sendOver(net_fd);
        }else{
            // 当前路径下没有该文件夹
            bzero(information, sizeof(information));
            sprintf(information, "该文件夹：%s不存在", path_buf);
            memcpy(user->receive, information, BUF_SIZE);
            sendError(net_fd, user);
            sendOver(net_fd);
        }
        mysql_free_result(result);
            
        return 0;
    }
}

/*
 * 函数名：realizeLS
 * 函数作用：处理LS命令
  * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0， 失败返回-1
*/
int realizeLS(int net_fd, user_t *user, MYSQL *conn){
    // 1. 获取用户char *userID
    int userID = resolve(user->token);

    // 2. 对命令进行切分
    char tmp[BUF_SIZE] = {0};
    memcpy(tmp, user->command, sizeof(tmp));
    LOG(INFO, "要处理的命令：%s", tmp);
    int len = strlen(tmp);
    tmp[len] = '\0';
    
    char buf[BUF_SIZE] = {0};
    char *command = strtok(tmp, " ");
    char *path_buf = strtok(NULL, " ");
    do{ 
        LOG(INFO, "当前处理的路径:%s", path_buf);
        char information[BUF_SIZE] = {0};
        MYSQL_RES *result = NULL;
        char str[PAGE_SIZE] = {0};
        if(path_buf == NULL || strcmp(path_buf, ".") == 0){
            // 打印当前的路径下的文件
            
            // 1. 找到当前路径的id
            bzero(str, sizeof(str));
            sprintf(str, "select id from file where path = '%s' && user_id = %d && delete_flag = 0", user->path,userID);
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查询数据库失败");
                LOG(ERROR, "%s", information);

                memcpy(user->receive, information, sizeof(buf));
                sendError(net_fd, user);
                path_buf = strtok(NULL, " ");
                continue;
            }
            result = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(result);
            mysql_free_result(result);
        
            // 根据父目录id查找符合条件的id
            int id = atoi(row[0]);
            bzero(str, sizeof(str));
            sprintf(str, "select file_name from file where father_path_id = %d && delete_flag = 0", id);
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查找父目录下的文件失败");

                memcpy(user->receive, information, sizeof(buf));
                sendError(net_fd, user);
                path_buf = strtok(NULL, " ");
                continue;
            }

            char fileList[BUF_SIZE] = {0};
            // 拼接信息
            if(path_buf != NULL){
                strcat(fileList, path_buf);
                strcat(fileList, ": ");
            }
            result = mysql_store_result(conn);
            my_ulonglong row_num = mysql_num_rows(result);
            if(row_num > 0){
                // 当前路径下有文件或文件夹
                while((row = mysql_fetch_row(result))){
                    strcat(fileList, row[0]);
                    strcat(fileList, " ");
                }
            }else{
                // 当前路径下没有文件或文件夹
            }
            mysql_free_result(result);
            memcpy(user->receive, fileList, BUF_SIZE);
            sendInformation(net_fd, user);
            path_buf = strtok(NULL, " ");
            continue;
        }else if(strcmp(path_buf ,"..") == 0){
            // 1. 根据用户id和当前路径，获取当前路径的父路径id
            // 若当前路径的父路径id = -1,打印当前工作路径下的文件
            bzero(str, sizeof(str));
            sprintf(str, "select father_path_id from file where path = '%s' && user_id = %d && delete_flag = 0 ", user->path, userID);
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查询数据库失败");

                memcpy(user->receive, information, sizeof(buf));
                sendError(net_fd, user);
                path_buf = strtok(NULL, " ");
                continue;
            }
            result = mysql_store_result(conn);

            MYSQL_ROW row = mysql_fetch_row(result);
            mysql_free_result(result);
            

            int fid = atoi(row[0]);
            bzero(str, sizeof(str));
            if(fid == -1){
                // 说明已经是父目录，打印此目录下的文件即可
                bzero(str, sizeof(str));
                sprintf(str, "select id from file where path = '%s' && user_id = %d && delete_flag = 0", user->path,userID);
                LOG(INFO, "操作数据库的语句：%s", str);

                if(mysql_query(conn, str) == -1){
                    bzero(information, sizeof(information));
                    sprintf(information, "查询数据库失败");

                    memcpy(user->receive, information, sizeof(buf));
                    sendError(net_fd, user);
                    path_buf = strtok(NULL, " ");
                    continue;
                }
                result = mysql_store_result(conn);
                MYSQL_ROW row = mysql_fetch_row(result);
                mysql_free_result(result);
        
                // 根据父目录id查找符合条件的id
                fid = atoi(row[0]);
            }
            bzero(str, sizeof(str));
            sprintf(str, "select file_name from file where father_path_id = %d && delete_flag = 0", fid);
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查找文件失败");

                memcpy(user->receive, information, sizeof(buf));
                sendError(net_fd, user);
                path_buf = strtok(NULL, " ");
                continue;
            }   
            char fileList[BUF_SIZE] = {0};
            // 拼接信息
            strcat(fileList, path_buf);
            strcat(fileList, ": ");

            result = mysql_store_result(conn);
            my_ulonglong row_num = mysql_num_rows(result);
            if(row_num > 0){
                // 当前路径下有文件或文件夹
                while((row = mysql_fetch_row(result))){
                    strcat(fileList, row[0]);
                    strcat(fileList, " ");
                }
            }else{
                // 当前路径下没有文件或文件夹
            }
            mysql_free_result(result);
            memcpy(user->receive, fileList, BUF_SIZE);
            sendInformation(net_fd, user);
            path_buf = strtok(NULL, " ");
            continue;
        }else{
            bzero(str, sizeof(str));
            sprintf(str, "select id from file where path = '%s' && user_id = %d && delete_flag = 0",path_buf, userID);
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查询数据库失败");

                memcpy(user->receive, information, sizeof(buf));
                sendError(net_fd, user);
                path_buf = strtok(NULL, " ");
                continue;
            }
            result = mysql_store_result(conn);

            MYSQL_ROW row = mysql_fetch_row(result);
            mysql_free_result(result);
            
            if(row == NULL){
                // 路径查找失败，返回错误信息
                bzero(information, sizeof(information));
                sprintf(information, "没有该文件夹:%s", path_buf);

                memcpy(user->receive, information, sizeof(buf));
                sendError(net_fd, user);
                path_buf = strtok(NULL, " ");
                continue;
            }
            
            bzero(str, sizeof(str));
            sprintf(str, "select file_name from file where father_path_id = %d && delete_flag = 0", atoi(row[0]));
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查找文件失败");

                memcpy(user->receive, information, sizeof(buf));
                sendError(net_fd, user);
                path_buf = strtok(NULL, " ");
                continue;
            }
            char fileList[BUF_SIZE] = {0};
            // 拼接信息
            strcat(fileList, path_buf);
            strcat(fileList, ": ");

            result = mysql_store_result(conn);
            my_ulonglong row_num = mysql_num_rows(result);
            if(row_num > 0){
                // 当前路径下有文件或文件夹
                while((row = mysql_fetch_row(result))){
                    strcat(fileList, row[0]);
                    strcat(fileList, " ");
                }
            }else{
                // 当前路径下没有文件或文件夹
            }
            mysql_free_result(result);
            memcpy(user->receive, fileList, BUF_SIZE);
            sendInformation(net_fd, user);
            path_buf = strtok(NULL, " ");
            continue;  
        }
    }while(path_buf != NULL);
    sendOver(net_fd);
    return 0;
}


