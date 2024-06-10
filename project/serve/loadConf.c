#include "head.h"

void initMap(HashMap *map){

    FILE * file = fopen("config.ini", "r");

    while(1){
        char line[100];
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
