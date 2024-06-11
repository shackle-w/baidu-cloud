/*
 * 文件名：salt.c
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件是实现和密码加密有关的函数
*/

#include "salt.h"

char* salt() {
    char *salt = (char*)malloc((SALT_LENGTH + 4) * sizeof(char)); // 分配内存空间，额外加上 "$6$" 和 "$"

    struct timeval nowTime;
    gettimeofday(&nowTime, NULL);
    srand((unsigned int) nowTime.tv_usec );

    strcpy(salt, "$6$");
    for (int i = 0; i < SALT_LENGTH; ++i) {
        sprintf(&salt[i + 3], "%x", rand() % 16);
    }
    strcat(salt, "$");

    return salt;
}

char* cryptPassword(char *password, char *salt) {

    char *crypted = crypt(password, salt);

    char *res = (char *)malloc(strlen(crypted)+1);
    memset(res, 0, strlen(crypted)+1);
    strcpy(res, crypted);

    return res;
}
