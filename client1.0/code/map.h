/*
 * 文件名：map.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件的作用是创建和键值对有关的接口
*/

#ifndef __CLIENTMAPH__
#define __CLIENTMAPH__

#include "my_header.h"
#include "clientStruct.h"

#define INIT_CAPACITY 16


/*
 * 函数名：createHashMap
 * 函数作用：创建一个HashMap结构体
 * 函数参数：
 *  -void
 * 函数返回值：一个HashMap结构体
*/
HashMap* createHashMap();

/*
 * 函数名：freeHashMap
 * 函数作用：释放一个HashMap结构体
 * 函数参数：
 *  HashMap *table：一个指向HashMap结构体的指针
 * 函数返回值：void
*/
void freeHashMap(HashMap *table);

/*
 * 函数名：insert
 * 函数作用：向结构体HashMap中插入一个元素
 * 函数参数：
 *  HashMap **table：HashMap的数组
 *  char *key：要插入结构体中的key值
 *  char *value：要插入结构体中的value值
 * 函数返回值：void
*/
void insert(HashMap **table, char *key, char *value);

/*
 * 函数名：hash
 * 函数作用：通过要插入结构体的key值以及hash表的容量来计算插入哈希表的位置
 * 函数参数：
 *  char *key：要插入hash表中的数据的key值
 *  int capacity：hash表的容量
 * 函数返回值：index：该键值对在哈希表的位置
*/
int hash(char *key, int capacity);

/*
 * 函数名：resize
 * 函数作用：扩大哈希表的长度
 * 函数参数：
 *  HashMap **table：传入二级指针，因为要修改结构体的值
 * 函数返回值：void
*/
void resize(HashMap **table); // TODO: 暂时未实现

/*
 * 函数名：get
 * 函数作用：根据key的值，得到相应的value的值
 * 函数参数：
 *  HashMap *map：指向结构体的map值
 *  char *key：需要value值的key值
 * 函数返回值：char * 相应的value值
*/
char *get(HashMap *map, char *key);

/*
 * 函数名：toString
 * 函数作用：将key_value值封装为key=value,key=value...的形式
 * 函数参数：
 *  HashMap *map：指向结构体map的指针
 * 函数函数值：封装好后的字符串
*/
char *toString(HashMap *map);

#endif
