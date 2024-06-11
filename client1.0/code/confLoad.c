/*
 * 文件名：confLoad.c
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件的作用是实现将配置文件中的信息加入到结构体HashMap中
*/

#include "confLoad.h"

char * getParameter(char *key){

    FILE * file = fopen("config.ini", "r");
    while(1){
        char line[100];
        bzero(line, sizeof(line));
        // 读一行数据
        char *res = fgets(line, sizeof(line), file);
        if(res == NULL){
            char buf[] = "没有要找的内容 \n";
            return NULL;
        }
        // 处理数据
        char *line_key = strtok(line, "=");
        if(strcmp(key, line_key) == 0){
            // 要找的内容
            char *line_value = strtok(NULL, "=");
            char buf[200] = {0};
            memcpy(buf, line_value, strlen(line_value)-1);
            return strdup(buf);
        }
    }
    return 0;
}

HashMap* parameterMap(){

    HashMap *map = createHashMap();
    insert(&map, "ip", getParameter("ip"));
    insert(&map, "port", getParameter("port"));
    return map;
}
