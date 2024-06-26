/*
 * 文件名：pathList.c
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现和路径队列有关的函数
*/

#include "pathList.h"

int isEmpty(PathItems* items) {
    return items->size == 0;
}

void addItem(PathItems* items, char* item) {
    items->items[items->size++] = strdup(item); // strdup复制字符串
}

char* removeFirstItem(PathItems* items) {
    if (isEmpty(items)) {
        return NULL;
    }
    char *str = items->items[0];
    for(int i=0; i<items->size; i++){
        items->items[i] = items->items[i+1];
    }
    items->size--;
    return str;
}

int moveForwardOne(PathItems *items, int index){
    for(int i=0; i<items->size; i++){
        items->items[i] = items->items[i+1];
    }
    items->size = items->size -1;
    return 0;
}

int moveForwardTwo(PathItems *items, int index){
    for(int i=index; i<=items->size; i++){
        items->items[i-1] = items->items[i+1];
    }
    items->size = items->size -2;
    return 0;
}

char* getFirstItem(PathItems* items) {
    if (isEmpty(items)) {
        return NULL;
    }
    return items->items[0];
}

PathItems* createItems() {
    PathItems* items = (PathItems*)malloc(sizeof(PathItems));
    items->size = 0;
    return items;
}

PathItems* createItemsPath(char *path) {
    PathItems* items = (PathItems*)malloc(sizeof(PathItems));
    items->size = 0;

    char buf[BUF_SIZE] = {0};
    memcpy(buf, path, strlen(path));

    char temp[BUF_SIZE] = {0};
    int tempIndex = 0;

    // TODO：理解该段代码的含义
    for(int i=1; i<=strlen(buf); i++){

        if(buf[i] != '/' && buf[i] != 0){
            temp[tempIndex++] = buf[i];
        }else if(buf[i] != 0){
            addItem(items, temp);
            bzero(temp, sizeof(temp));
            tempIndex = 0;
        }
    }
    return items;
}
