/*
 * 文件名：confLoad.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现从配置文件中获取信息，并封装到HashMap结构体
*/

#ifndef __CONFLOADH__
#define __CONFLOADH__

#include "my_header.h"
#include "clientStruct.h"
#include "map.h"

/*
 * 函数名：getParameter
 * 函数作用：从配置文件中获取想要的信息
 * 函数参数：
 *  char *key：要获取的参数
 * 函数返回值：根据key值获取到的value值
*/
char * getParameter(char *key);

/*
 * 函数名：parameterMap
 * 函数作用：将配置文件中的信息封装到HashMap结构体中
 * 函数参数：
 *  void
 * 函数返回值：指向结构体hashMap的指针
*/
HashMap* parameterMap();

#endif
