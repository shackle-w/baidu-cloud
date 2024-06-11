/*
 * 文件名：confLoad.c
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件的作用是实现将配置文件中的信息加入到结构体HashMap中
*/

#include "confLoad.h"

void initMap(HashMap *map){

    FILE * file = fopen("config.ini", "r");

    while(1){
        char line[SMALL_BUF];
        bzero(line, sizeof(line));
        // 读一行数据
        char *res = fgets(line, sizeof(line), file);
        if(res == NULL){
            fclose(file);
            return ;
        }
        // 处理数据
        char *lineKey = strtok(line, "=");
        char *lineValue = strtok(NULL, "\n");
        insert(&map, lineKey, lineValue);
    }
}

HashMap* parameterMap(){

    HashMap *map = createHashMap();
    initMap(map);

    return map;
}
