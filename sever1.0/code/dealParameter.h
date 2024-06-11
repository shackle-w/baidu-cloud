/*
 * 文件名：dealParameter.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件是处理客户端发送来的信息
*/

#ifndef __DEALPARAMETERH__
#define __DEALPARAMETERH__

#include "my_header.h"
#include "severStruct.h"
#include "map.h"

/*
 * 函数名：getParameterMap
 * 函数作用：对服务端发送过来的数据报进行处理
 * 函数参数：
 *  protocol_client protocol：服务器发送来的数据报
 *  HashMap *map：存放用户信息的结构体
 * 函数返回值：返回插入的信息数
*/
int getParameterMap(protocol_client protocol, HashMap *map);

/*
 * 函数名：stringCat
 * 函数作用：拼接key和value的值
 * 函数参数：
 *  char *buf：拼接后存放的位置
 *  char *key：拼接的key值
 *  char *value：拼接的value值
 * 返回值：成功返回0
*/
int stringCat(char *buf, char *key, char *value);

#endif
