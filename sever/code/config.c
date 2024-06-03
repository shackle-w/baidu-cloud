/*
 * 文件名：config.c
 * 创建时间5.29
 * 作者：shackle
 * 描述：该文件的作用是从配置文件中获得想要的信息
*/

#include "config.h"
#define SMALL_BUF 100
/*
 * 函数名：getparameter
 * 函数作用：从配置文件中获取信息
 * 参数：
 *  char *file：配置文件名
 *  char *key: 需要获取的标识
 *  char *value：获取的内容
 * 返回值：0成功，-1失败
*/
int getparameter(char *file, char *key, char *value){
    FILE * file_fd = fopen(file, "r");
    ERROR_CHECK(file_fd, NULL, "fopen");
    while(1){
        char line[SMALL_BUF];
        bzero(line, sizeof(line));
        // 读一行数据
        char *res = fgets(line, sizeof(line), file_fd);
        if(res == NULL){
            fclose(file_fd);
            sprintf(value, "没有找到该内容");
            return -1;
        }

        // 去掉行尾的换行符
        line[strcspn(line, "\n")] = 0;
        
        // 处理数据
        char *line_key = strtok(line, "=");
        if(line_key == NULL){
            continue;
        }
        if(strcmp(key, line_key) == 0){
            // 要找的内容
            char *line_value = strtok(NULL, "=");
            if(line_value == NULL){
                fclose(file_fd);
                sprintf(value, "该参数为空");
                return -1;
            }
            // 去掉两端的空白字符
            while(*line_value == ' ' || *line_value == '\t'){
                line_value++;
            }
            char *end = line_value + strlen(line_value) - 1;
            while(end > line_value && (*end == ' ' || *end == '\t')){
                *end = '\0';
                end--;
            }
            
            strcpy(value, line_value);
            fclose(file_fd);
            return 0;
        }
    }
}
