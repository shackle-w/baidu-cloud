/*
 * 文件名：putsFile.c
 * 创建时间：24.6.6
 * 作者：shackle
 * 描述：该文件实现向服务器发送文件
*/

#include "putsFile.h"

int compute_hash(const EVP_MD *md_type, int fd, unsigned char *result, unsigned int *result_len){
    EVP_MD_CTX *md_ctx;
    unsigned char buffer[BUF_SIZE];
    ssize_t bytes_read;

    md_ctx = EVP_MD_CTX_new();
    if(!md_ctx){
        LOG(ERROR, "EVP_MD_CTX_new failed");
        return -1;
    }
    if(EVP_DigestInit_ex(md_ctx, md_type, NULL) != 1){
        LOG(ERROR, "EVP_DigestInit_ex failed");
        EVP_MD_CTX_free(md_ctx);
        return -1;
    }

    while((bytes_read = read(fd, buffer, sizeof(buffer))) > 0){
        if(EVP_DigestUpdate(md_ctx, buffer, bytes_read) != 1){
            LOG(ERROR, "EVP_DigestUpdate failed");
            EVP_MD_CTX_free(md_ctx);
            return -1;
        }
    }

    if(EVP_DigestFinal_ex(md_ctx, result, result_len) != 1){
        LOG(ERROR, "EVP_DigestFinal_ex failed");
        EVP_MD_CTX_free(md_ctx);
        return -1;
    }

    EVP_MD_CTX_free(md_ctx);

    return 0;
}

int putsFile(int net_fd, user_t *user){

    // 1. 计算文件的哈希值
    char tmp[SMALL_BUF] = {0};
    memcpy(tmp, user->command, sizeof(tmp));
    char *command = strtok(tmp, " ");
    char *file_path = strtok(NULL, " ");
    int fd = open(file_path, O_RDONLY);
    if(fd < 0){
        LOG(ERROR, "打开文件失败");
        return -1;
    }
    
    unsigned char md5_result[EVP_MAX_MD_SIZE];
    unsigned char sha1_result[EVP_MAX_MD_SIZE];
    unsigned char sha256_result[EVP_MAX_MD_SIZE];
    unsigned int md5_len, sha1_len, sha256_len;

    lseek(fd, 0, SEEK_SET);
    if(compute_hash(EVP_md5(), fd, md5_result, &md5_len)==-1){
        return -1;
    }

    lseek(fd, 0, SEEK_SET);
    if(compute_hash(EVP_sha1(), fd, sha1_result, &sha1_len) == -1){
        return -1;
    }

    lseek(fd, 0, SEEK_SET);
    if(compute_hash(EVP_sha256(), fd, sha256_result, &sha256_len) == -1){
        return -1;
    }
    close(fd);

    // 2. 发送命令到服务器
    memcpy(user->receive, sha1_result, BUF_SIZE);
    sendLong(net_fd, user);
}
