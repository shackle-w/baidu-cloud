#include "head.h"

int getParameterMap(protocol_client protocol, HashMap *map){

    int paraNum = protocol.parameter_num;
    char *array[10] = {0};

    char *temp = strtok(protocol.buf, "&");
    for(int i=0; i<paraNum; i++){
        array[i] = temp;

        temp = strtok(NULL, "&");
    }
        
    for(int j=0; j<paraNum; j++){
        temp = array[j];
        char *key = strtok(temp, "=");
        char *value = strtok(NULL, "=");

        insert(&map, key, value);
    }

    return paraNum;
}

// 拼接key=value数据
int stringCat(char *buf, char *key, char *value){
    strcat(buf, key);
    strcat(buf, "=");
    strcat(buf, value);

    return 0;
}
