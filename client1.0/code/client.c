/*
 * 文件名：client.c
 * 创建时间：24.6.11
 * 作者：shackle
 * 描述：该文件为客户端的启动逻辑文件
*/
#include "head.h"

int main(int argc, char *argv[])
{
    run_status_t runstatus;
    memset(&runstatus, 0, sizeof(runstatus));
    runstatus.map = parameterMap();
    strcpy(runstatus.path, "/");

    initTcp(&runstatus.net_fd, get(runstatus.map, "ip"), get(runstatus.map, "port"));

    while(1){
        if(runstatus.login_status == 0){
            // 未登录状态
            distributeLoginOrRegister(&runstatus);
        }else{
            // 命令状态
            distributeCommand(&runstatus);
        }
    }
    return 0;
}
