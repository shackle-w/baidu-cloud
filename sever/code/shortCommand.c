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
    char *userID;
    resolve(userID, user->token);

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
        SEND_INFORMATION(net_fd, *user);
        SEND_OVER(net_fd);

        return 0;
    }else if(strcmp(path_buf, "..") == 0){
        // 1. 根据用户id和当前路径，获取当前路径的父路径id
        // 若当前路径的父路径id = -1,不用更改路径返回
        bzero(str, sizeof(str));
        sprintf(str, "select * from file where file_path = '%s' && user_id = %d && file_type = 0 ", user->path, atoi(userID));
        LOG(INFO, "操作数据库的语句：%s", str);

        if(mysql_query(conn, str) == -1){
            bzero(information, sizeof(information));
            sprintf(information, "查询数据库失败");
            memcpy(user->command, information, sizeof(buf));
            SEND_ERROR(net_fd, *user);
            SEND_OVER(net_fd);

            return -1;
        }
        result = mysql_store_result(conn);

        MYSQL_ROW row = mysql_fetch_row(result);
        mysql_free_result(result);
        
        if(row == NULL){
            // 路径查找失败，返回错误信息
            bzero(information, sizeof(information));
            sprintf(information, "查找路径失败");
            memcpy(user->command, information, sizeof(buf));
            SEND_ERROR(net_fd, *user);
            SEND_OVER(net_fd);

            return -1;
        }

        int fid = atoi(row[3]);
        bzero(str, sizeof(str));
        if(fid == -1){
            // 说明已经是父目录
            SEND_INFORMATION(net_fd, *user);
            SEND_OVER(net_fd);

            return 0;
        }
        sprintf(str, "select file_path from file where id = %d", fid);
        LOG(INFO, "操作数据库的语句：%s", str);

        if(mysql_query(conn, str) == -1){
            bzero(information, sizeof(information));
            sprintf(information, "查找父目录失败");
            LOG(ERROR, "%s", information);
            memcpy(user->command, information, sizeof(buf));
            SEND_ERROR(net_fd, *user);
            SEND_OVER(net_fd);

            return -1;
        }
        result = mysql_store_result(conn);

        row = mysql_fetch_row(result);
        mysql_free_result(result);
        
        if(row == NULL){
            // 路径查找失败，返回错误信息
            bzero(information, sizeof(information));
            sprintf(information, "查找父路径失败");
            LOG(ERROR, "%s", information);

            memcpy(user->command, information, sizeof(buf));
            SEND_ERROR(net_fd, *user);
            SEND_OVER(net_fd);

            return -1;
        }

        strcpy(user->path, row[0]);
        SEND_INFORMATION(net_fd, *user);
        SEND_OVER(net_fd);

        return 0;
    }else{
        bzero(str, sizeof(str));
        sprintf(str, "select * from file where file_path = '%s' && user_id = %d && file_type = 0",user->path, atoi(userID));
        LOG(INFO, "操作数据库的语句：%s", str);

        if(mysql_query(conn, str) == -1){
            bzero(information, sizeof(information));
            sprintf(information, "查询数据库失败");
            LOG(ERROR, "%s", information);

            memcpy(user->command, information, sizeof(buf));
            SEND_ERROR(net_fd, *user);
            SEND_OVER(net_fd);

            return -1;
        }
        result = mysql_store_result(conn);

        MYSQL_ROW row = mysql_fetch_row(result);
        mysql_free_result(result);
        
        if(row == NULL){
            // 路径查找失败，返回错误信息
            bzero(information, sizeof(information));
            sprintf(information, "查找路径失败");
            LOG(ERROR, "%s", information);

            memcpy(user->command, information, sizeof(buf));
            SEND_ERROR(net_fd, *user);
            SEND_OVER(net_fd);

            return -1;
        }

        strcpy(user->path, row[4]);
        SEND_INFORMATION(net_fd, *user);
        SEND_OVER(net_fd);

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
    char *userID;
    resolve(userID, user->token);

    // 2. 对命令进行切分
    char buf[BUF_SIZE] = {0};
    memcpy(buf, user->command, sizeof(buf));
    LOG(INFO, "要处理的命令：%s", buf);
    int len = strlen(buf);
    buf[len] = '\0';
    
    char *command = strtok(buf, " ");
    char *path_buf = strtok(NULL, "\n");
    while(path_buf != NULL){   
        char information[BUF_SIZE] = {0};
        MYSQL_RES *result = NULL;
        char str[PAGE_SIZE] = {0};
        if(path_buf == NULL || strcmp(path_buf, ".") == 0){
            // 打印当前的路径下的文件
            
            // 1. 找到当前路径的id
            bzero(str, sizeof(str));
            sprintf(str, "select id from file where file_path = '%s' && user_id = %d ", user->path, atoi(userID));
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查询数据库失败");
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);

                return -1;
            }
            result = mysql_store_result(conn);

            MYSQL_ROW row = mysql_fetch_row(result);
            mysql_free_result(result);
        
            // 此时绝对可以找到
            if(row == NULL){
                // 路径查找失败，返回错误信息
                bzero(information, sizeof(information));
                sprintf(information, "查找路径失败");
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);

                return -1;
            }

            int id = atoi(row[0]);
            bzero(str, sizeof(str));
            sprintf(str, "select filename from file where father_id = %d && file_status = 1", id);
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查找目录下的文件失败");
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);

                return -1;
            }
            result = mysql_store_result(conn);

            row = mysql_fetch_row(result);
            mysql_free_result(result);
        
            if(row == NULL){
                // 该目录下没有文件
                bzero(information, sizeof(information));
                snprintf(information, BUF_SIZE, "%s",path_buf);
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_INFORMATION(net_fd, *user);
                SEND_OVER(net_fd);

                return 0;
            }
            // 打印路径 
            bzero(information, sizeof(information)); 
            snprintf(information,BUF_SIZE, "%s", path_buf);
            LOG(ERROR, "%s", information);

            memcpy(user->command, information, sizeof(buf));
            SEND_INFORMATION(net_fd, *user);

            while(row != NULL){
                bzero(information, sizeof(information));
                snprintf(information,BUF_SIZE,"%s", row[0]);
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_INFORMATION(net_fd, *user);
                row = mysql_fetch_row(result);  
            }
            SEND_OVER(net_fd);

            return 0;
        }else if(strcmp(path_buf ,"..") == 0){
            // 1. 根据用户id和当前路径，获取当前路径的父路径id
            // 若当前路径的父路径id = -1,打印当前工作路径下的文件
            bzero(str, sizeof(str));
            sprintf(str, "select father_id from file where file_path = '%s' && user_id = %d && file_type = 0 ", user->path, atoi(userID));
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查询数据库失败");
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);

                return -1;
            }
            result = mysql_store_result(conn);

            MYSQL_ROW row = mysql_fetch_row(result);
            mysql_free_result(result);
        
            if(row == NULL){
                // 路径查找失败，返回错误信息
                bzero(information, sizeof(information));
                sprintf(information, "查找路径失败");
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);

                return -1;
            }

            int fid = atoi(row[0]);
            bzero(str, sizeof(str));
            if(fid == -1){
                // 说明已经是父目录，打印此目录下的文件即可
                fid = atoi(userID);
            }
            sprintf(str, "select filename from file where id = %d && file_status = 1", fid);
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查找文件失败");
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);

                return -1;
            }
            result = mysql_store_result(conn);

            row = mysql_fetch_row(result);
            mysql_free_result(result);
        
            if(row == NULL){
                // 该目录下没有文件
                bzero(information, sizeof(information));
                snprintf(information,BUF_SIZE, "%s", path_buf);
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_INFORMATION(net_fd, *user);
                SEND_OVER(net_fd);
                return 0;
            }
            // 打印路径 
            bzero(information, sizeof(information)); 
            snprintf(information,BUF_SIZE,"%s", path_buf);
            LOG(ERROR, "%s", information);

            memcpy(user->command, information, sizeof(buf));
            SEND_INFORMATION(net_fd, *user);

            while(row != NULL){
                bzero(information, sizeof(information));
                snprintf(information,BUF_SIZE, "%s", row[0]);
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_INFORMATION(net_fd, *user);
                row = mysql_fetch_row(result);  
            }
            SEND_OVER(net_fd);
            return 0;
        }else{
            bzero(str, sizeof(str));
            sprintf(str, "select id from file where file_path = '%s' && user_id = %d && file_type = 0",path_buf, atoi(userID));
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "查询数据库失败");
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);
                return -1;
            }
            result = mysql_store_result(conn);

            MYSQL_ROW row = mysql_fetch_row(result);
            mysql_free_result(result);
        
            if(row == NULL){
                // 路径查找失败，返回错误信息
                bzero(information, sizeof(information));
                sprintf(information, "查找路径失败");
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);
                return -1;
            }
            // 打印路径 
            bzero(information, sizeof(information)); 
            snprintf(information,BUF_SIZE, "%s" ,path_buf);
            LOG(ERROR, "%s", information);

            memcpy(user->command, information, sizeof(buf));
            SEND_INFORMATION(net_fd, *user);

            while(row != NULL){
                bzero(information, sizeof(information));
                snprintf(information,BUF_SIZE,"%s", row[0]);
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_INFORMATION(net_fd, *user);
                row = mysql_fetch_row(result);  
            }
            SEND_OVER(net_fd);
            return 0;
        }
    path_buf = strtok(NULL, " ");
    }
    SEND_OVER(net_fd);
    return 0;
}


