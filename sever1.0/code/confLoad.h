/*
 * 文件名：confLoad.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现从配置文件中获取信息，并封装到HashMap结构体
*/

#ifndef __CONFLOADH__
#define __CONFLOADH__

#include "my_header.h"
#include "severStruct.h"
#include "map.h"

/*
 * 函数名：initMap
 * 函数作用：获取配置文件的内容，保存到结构体中
 * 函数参数：
 *  HashMap *map：结构体：HashMap
 * 函数返回值：void
*/
void initMap(HashMap *map);

/*
 * 函数名：parameterMap
 * 函数作用：从配置文件的获取信息，保存到HashMap结构体中
 * 函数参数：
 *  void
 * 函数返回值：HashMap* 封装好后的的结构体
*/
HashMap* parameterMap();

#endif
