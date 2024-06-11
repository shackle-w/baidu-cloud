/*
 * 文件名：dealPwd.c
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件实现pwd命令
*/

#include "dealCommand.h"

int pwdCommand(char *buf, run_status_t *runstatus){
    printf("%s\n", runstatus->path);
    return  0;
}
