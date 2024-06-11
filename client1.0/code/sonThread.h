/*
 * 文件名：sonThread.h
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件的作用是实现子线程运行的逻辑接口函数
*/

#ifndef __SONTHREADH__
#define __SONTHREADH__

#include "my_header.h"
#include "clientStruct.h"
#include "clientFile.h"

/*
 * 函数名：threadMain
 * 函数作用：子线程的运行逻辑，用于发送和接受文件
 * 函数参数：
 *  void *p：任意的指针类型
 * 函数返回值：void *：任意的指针类型
*/
void *threadMain(void *p);

#endif
