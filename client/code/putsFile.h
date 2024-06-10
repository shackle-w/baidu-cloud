/*
 * 文件名：putsfile.h
 * 创建时间：24.6.6
 * 作者：shackle
 * 描述：该文件的作用是处理向服务器上传文件的接口调用
*/

#ifndef __PUTSFILEH__
#define __PUTSFILEH__

#include "my_header.h"
#include "log.h"
#include "tools.c"
#include <openssl/evp.h>

/*
 * 函数名：computeHash
 * 函数作用：计算文件的Hash值
 * 函数参数：
 *  -const EVP_MD *md_type：传入的不同hash类型
 *  int fd：文件描述符
 *  unsigned char *result：计算hash的结果
 *  unsigned int *result_len：计算hash的长度
 * 返回值：int
*/
int compute_hash(const EVP_MD *md_type, int fd, unsigned char *result, unsigned int *result_len);


/*
 * 函数名：putsFile
 * 函数作用：从数据库下载文件到服务器
 * 函数参数：
 *  -int net_fd：通信的socket
 *  -user_t *user：用户的信息
 * 返回值：下载成功返回0
*/
int putsFile(int net_fd, user_t *user);


#endif

