/*
 * 文件名：config.h
 * 创建时间：5.29
 * 作者：shackle
 * 描述：该文件是获取配置文件内的信息
*/

#ifndef __CONFIGH__
#define __CONFIGH__

#include "my_header.h"
#include "log.h"

/*
 * 函数名：getparameter
 * 函数作用：从配置文件中获取信息
 * 参数：
 *  char *file：配置文件名
 *  char *key: 需要获取的标识
 *  char *value：获取的内容
 * 返回值：0成功，-1失败
*/
int getparameter(char *file, char *key, char *value);
#endif
