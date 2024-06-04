/*
 * 文件名：shortCommand2.c
 * 创建时间：5.31
 * 作者：shackle
 * 描述：该文件是一些逻辑相对简单的命令
*/

#include "command.h"


/*
 * 函数名：realizePWD
 * 函数作用：处理PWD命令
  * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0， 失败返回-1
*/
int realizePWD(int net_fd, user_t *user, MYSQL *conn){
    // 1. 因为在user内保存有pwd所有接收后返回即可
    LOG(INFO, "当前工作路径：%s",user->path);

    sendInformation(net_fd, user);
    SEND_OVER(net_fd);

    return 0;
}

/*
 * 函数名：realizeMKDIR
 * 函数作用：处理MKDOR命令
  * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0， 失败返回-1
*/

// 暂时仅考虑在当前目录下常见文件夹
int realizeMKDIR(int net_fd, user_t *user, MYSQL *conn){
    // 1. 获取用户char *userID
    int userID = resolve(user->token);

    printf("userID：%d\n", userID);
    // 2. 对命令进行切分
    char tmp[BUF_SIZE] = {0};
    memcpy(tmp, user->command, sizeof(tmp));
    int len = strlen(tmp);
    tmp[len] = '\0';
    
    char buf[BUF_SIZE] = {0};
    char *command = strtok(tmp, " ");
    char *path_buf = strtok(NULL, " ");   
    char information[BUF_SIZE] = {0};
    MYSQL_RES *result = NULL;
    char str[PAGE_SIZE] = {0};
    char father_buf[SMALL_BUF] = {0};
    memcpy(father_buf, user->path, sizeof(father_buf));
    
    // TODO 拼接相对路径
    while(path_buf != NULL){
        // 1. 拼接要创建的文件夹的路径
        char dirPath[BUF_SIZE] = {0};
        sprintf(dirPath, "%s%s/", father_buf, path_buf);

        // 2. 查找该路径是否存在
        sprintf(str, 
            "select * from file where delete_flag=0 and file_name = '%s' " 
            " and father_path_id = (select id from file where delete_flag=0 and user_id=%d and path='%s')"
        ,path_buf, userID, user->path);
        LOG(INFO, "操作数据库的语言：%s", str);

        mysql_query(conn, str);
        
        result = mysql_store_result(conn);
        my_ulonglong row_num = mysql_num_rows(result);

        if(row_num != 0){
            // 路径已存在
            bzero(buf, sizeof(buf));
            sprintf(buf, "文件夹%s已存在", path_buf);
            LOG(ERROR, "文件夹%s已存在", path_buf);
            memcpy(user->receive, buf, sizeof(buf));
            sendError(net_fd, user);
            mysql_free_result(result);
            path_buf = strtok(NULL, " ");
            continue;
        }
        mysql_free_result(result);
     
        // 3. 获得父级路径的id
        int currentPathId = 0;
        bzero(str, sizeof(str));
        sprintf(str, 
        "select * from file where delete_flag=0 and user_id=%d and path='%s' ",
        userID, user->path);
        
        LOG(INFO, "操作数据库的语言：%s", str);
        mysql_query(conn, str);
        result = mysql_store_result(conn);
        MYSQL_ROW row;
        if((row = mysql_fetch_row(result))){
            currentPathId = atoi(row[0]);
        }
        mysql_free_result(result);

        // 4. 向数据库内插入数据
        bzero(str, sizeof(str));
        sprintf(str, 
        "insert into file (user_id, path, father_path_id, file_name)  values ( %d, '%s', %d, '%s')",
        userID, dirPath, currentPathId, path_buf);

        LOG(INFO, "操作数据库的语句：%s", str);
        if(mysql_query(conn, str) == -1) {
            bzero(buf, sizeof(buf));
            sprintf(buf, "创建文件信息失败");
            LOG(ERROR, "%s", buf);
            memcpy(user->receive, buf, sizeof(buf));
            SEND_ERROR(net_fd, *user);
            path_buf = strtok(NULL, " ");
            continue;
        }
        result = mysql_store_result(conn);
        mysql_free_result(result);
        
        path_buf = strtok(NULL, " ");
    }
    SEND_OVER(net_fd);
    return 0;
}

/*
 * 函数名：realizeRM
 * 函数作用：处理RM命令
  * 函数参数：
 *  -int net_fd：连接的socket
 *  -user_t *user：用户信息
 *  -MYSQL *conn：连接的数据库
 * 返回值：成功返回0， 失败返回-1
*/
int realizeRM(int net_fd, user_t *user, MYSQL *conn){
    // 1. 获取用户char *userID
    int userID = resolve(user->token);

    // 2. 对命令进行切分
    char buf[BUF_SIZE] = {0};
    memcpy(buf, user->command, sizeof(buf));
    LOG(INFO, "要处理的命令：%s", buf);
    int len = strlen(buf);
    buf[len] = '\0';
    
    char *command = strtok(buf, " ");
    char *path_buf = strtok(NULL, " "); 
    char information[BUF_SIZE] = {0};
    MYSQL_RES *result = NULL;
    char str[PAGE_SIZE] = {0};
    
    if(strcmp(path_buf, "-r") == 0){
        // 删除的是目录
        path_buf = strtok(NULL, " ");
        if(strcmp(path_buf, ".") == 0 || strcmp(path_buf, "..") == 0){
            bzero(information, sizeof(information));
            printf(information, "rm：拒绝删除'.'或'..' 目录：跳过'.'",sizeof(information));
            LOG(ERROR, "information");
            memcpy(user->command, information, sizeof(information));
            SEND_ERROR(net_fd, *user);
            SEND_OVER(net_fd);
            return -1;
        } 
        if(path_buf == NULL){
            bzero(information, sizeof(information));
            printf(information, "没有删除的对象",sizeof(information));
            LOG(ERROR, "information");
            memcpy(user->command, information, sizeof(information));
            SEND_ERROR(net_fd, *user);
            SEND_OVER(net_fd);
            return -1;
        }
        while(path_buf != NULL){
            bzero(str, sizeof(str));
            sprintf(str, "select id from file where file_path = '%s' && user_id = %d", user->path, userID);
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
                sprintf(information, "查找id失败");
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);
                return -1;
            }

            int id = atoi(row[0]);
            
            bzero(str, sizeof(str));
            sprintf(str, "update file file_status = 0 where father_id = '%d' ", id);
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "修改数据库信息失败");
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);
                return -1;
            }
            path_buf = strtok(NULL, " ");
        }
        SEND_OVER(net_fd);
        return 0;
    }else{
        while(path_buf != NULL){
            bzero(str, sizeof(str));
            sprintf(str, "updata file file_status = 0 where filename = '%s' && user_id = %d", path_buf, userID);
            LOG(INFO, "操作数据库的语句：%s", str);

            if(mysql_query(conn, str) == -1){
                bzero(information, sizeof(information));
                sprintf(information, "删除文件%s失败", path_buf);
                LOG(ERROR, "%s", information);

                memcpy(user->command, information, sizeof(buf));
                SEND_ERROR(net_fd, *user);
                SEND_OVER(net_fd);
                return -1;
            }
            path_buf = strtok(NULL, " ");
        }
        SEND_OVER(net_fd);
        return 0;
    }
    SEND_OVER(net_fd);
    return 0;
}

