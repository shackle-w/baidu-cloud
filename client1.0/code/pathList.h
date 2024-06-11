/*
 * 文件名：pathLish.h
 * 创建时间24.6.11
 * 作者：shackle
 * 描述：该文件的作用是提供路径队列相关的接口文档
*/
#ifndef __PATHLISTH__
#define __PATHLISTH__

#include "my_header.h"

// 队列相关
typedef struct PathItems {
    int size;
    char* items[100]; // 指针数组
} PathItems;

/*
 * 函数名：isEmpty
 * 函数作用：判断队列是否为空
 * 函数参数：
 *  PathItems *items
 * 函数返回值：true or false
*/
int isEmpty(PathItems* items);

/*
 * 函数名：addItem
 * 函数作用：向队列中加入一条信息
 * 函数参数：
 *  PathItems *items:指向队列的指针
 *  char *items：要插入的队列的路径
 * 函数返回值：void
*/
void addItem(PathItems* items, char* item);

/*
 * 函数名：removeFirstItem
 * 函数作用：删除队列的第一个元素
 * 函数参数：
 *  PathItems *items：指向队列的指针
 * 函数返回值：第一个路径
*/
char* removeFirstItem(PathItems* items);

/*
 * 函数名：getFirstItem
 * 函数作用：得到队列的第一个元素
 * 函数参数：
 *  PathItems：指向队列的指针
 * 函数返回值：:第一个路径
*/
char* getFirstItem(PathItems* items);

/*
 * 函数名：createItems
 * 函数作用：创建一个不含路径的队列
 * 函数参数：
 *  void
 * 函数返回值：PathItems *：指向队列的指针
*/
PathItems* createItems();

/*
 * 函数名：createItemsPath
 * 函数作用：创建一个含有一条路径的队列
 * 函数参数：
 *  char *path：要加入队列的路径
 * 函数返回值：PathItems *：指向队列的指针
*/
PathItems* createItemsPath(char *path);

/*
 * 函数名：moveForwardOne
 * 函数作用：队列所有的路径向前覆盖一个路径
 * 函数参数：
 *  PathItems *items：指向队列的指针
 *  int index：从那个位置开始向前覆盖一个路径
 * 函数返回值：成功返回零
*/
int moveForwardOne(PathItems *items, int index);
/*
 * 函数名：moveForwardTwo
 * 函数作用：队列所有的路径向前覆盖两个路径
 * 函数参数：
 *  PathItems *items：指向队列的指针
 *  int index：从那个位置开始向前覆盖两个路径
 * 函数返回值：成功返回零
*/
int moveForwardTwo(PathItems *items, int index);


#endif
