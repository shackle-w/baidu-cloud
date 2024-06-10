#include "head.h"
#include <l8w8jwt/encode.h>
#include <l8w8jwt/encode.h>
#include <l8w8jwt/decode.h>

int getToken(int userId, char *buf ){
    char id[20];
    sprintf(id, "%d", userId);

    char* jwt; 
    size_t jwt_length;  
    struct l8w8jwt_encoding_params params;
    l8w8jwt_encoding_params_init(&params); 
    params.alg = L8W8JWT_ALG_HS512;  

    params.sub = id;  
    params.iss = "snow";  
    params.aud = "user";  
    params.exp = 0x7fffffff;  
    params.iat = 0;  


    // 设置加密密钥
    params.secret_key = (unsigned char*)"snow string token key";
    params.secret_key_length = strlen((char *)params.secret_key);

    // 输出变量
    params.out = &jwt;
    params.out_length = &jwt_length;

    // 加密
    l8w8jwt_encode(&params);

    // 输出结果
    strcpy(buf, jwt);

    // 释放token 字符串的内存
    l8w8jwt_free(jwt);
    return 0;
}

// 根据token, 对token解析获得userId
int getUserId(char * token){
    int userId;

    // 初始化解码参数结构体
    struct l8w8jwt_decoding_params params;
    l8w8jwt_decoding_params_init(&params);  

    // 设置JWT使用的算法，这里使用HS512
    params.alg = L8W8JWT_ALG_HS512;  

    // 填入token字符串和长度
    params.jwt = (char*)token;
    params.jwt_length = strlen(token);

    // 设置密钥
    char *key="snow string token key";
    params.verification_key = (unsigned char*)key;
    params.verification_key_length = strlen(key);

    // 定义: 接收结果的指针和变量
    struct l8w8jwt_claim *claims = NULL;
    size_t claim_count = 0;
    enum l8w8jwt_validation_result validation_result;

    // 解码: 获得token中存储的信息
    int decode_result = l8w8jwt_decode(&params, &validation_result, &claims, &claim_count);

    // 判断是否是正确的token
    if (decode_result == L8W8JWT_SUCCESS && validation_result == L8W8JWT_VALID) {
        userId = atoi(claims[3].value);
    } else {
        userId = -1;
    }

    l8w8jwt_free_claims(claims, claim_count);
    return userId;
}
