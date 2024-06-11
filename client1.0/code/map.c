/*
 * 文件名：map.c
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现和map相关的函数
*/

#include "map.h"


HashMap* createHashMap() {
    HashMap *map = (HashMap*)malloc(sizeof(HashMap));
    map->capacity = INIT_CAPACITY;
    map->size = 0;
    map->table = (Node**)malloc(sizeof(Node*) *INIT_CAPACITY);
    memset(map->table, 0, sizeof(Node*) * INIT_CAPACITY);
    return map;
}

void freeHashMap(HashMap *map) {
    for (int i = 0; i < map->capacity; i++) {
        Node *node = map->table[i];
        while (node) {
            Node *temp = node;
            node = node->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(map->table);
    free(map);
}

int hash(char *key, int capacity) {
    unsigned long index = 0;
    // 一个简单的哈希计算，计算该键值对存放在哈希表的那个位置
    for (int i = 0; key[i] != '\0'; i++) {
        index = index * 31 + key[i];
    }
    return index % capacity;
}

void insert(HashMap **table, char *key, char *value) {

    int index = hash(key, (*table)->capacity);
    Node *node = (*table)->table[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;  // 更新value
            return;
        }
        node = node->next;
    }
    // 添加key-value
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->value = strdup(value);

    // 头插法插入结点
    newNode->next = (*table)->table[index];
    (*table)->table[index] = newNode;
    (*table)->size++;
}

char * get(HashMap *map, char *key) {
    int index = hash(key, map->capacity);
    Node *node = map->table[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

// TODO: 后面再改
char* toString(HashMap *map) {
    int length = 0;
    char *result = (char *)malloc(sizeof(char) * BUF_SIZE);
    result[0] = '\0';

    for (int i = 0; i < map->capacity; i++) {
        Node *node = map->table[i];
        while (node) {
            int needed = snprintf(NULL, 0, "%s%s=%s, ", result, node->key, node->value);
            if (length + needed >= BUF_SIZE) {
                char *new_result = (char *)realloc(result, length + needed + 1);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            sprintf(result + length, "%s=%s, ", node->key, node->value);
            length += needed - 2;  // 更新长度，减去多余的", "
            node = node->next;
        }
    }
    if (length > 0) result[length - 2] = '\0';  // 移除最后一个逗号和空格
    return result;
}
