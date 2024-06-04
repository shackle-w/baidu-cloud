#include <my_header.h>
// 解密
#include <l8w8jwt/encode.h>
#include <l8w8jwt/decode.h>

int main(void)
{
  // 初始化解码参数结构体
   struct l8w8jwt_decoding_params params;
   l8w8jwt_decoding_params_init(&params);  
   
  // 设置JWT使用的算法，这里使用HS512
   params.alg = L8W8JWT_ALG_HS512;  

   // token值
   //char *str = "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJleHAiOjIxNDc0ODM2NDcsInN1YiI6InByb2plY3QgbmFtZSIsImlzcyI6InNub3ciLCJhdWQiOiJ1c2VyIn0.01lC775YBUPYRvhhBis_KKEh6JMwimEll9YLLFwZkWYGym8zeRkmamNpr5g8q7WlygyR3FCbUn91I0y-RR4IDQ";
   //char *str = "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJleHAiOjIxNDc0ODM2NDcsInN1YiI6InByb2plY3QgbmFtZSIsImlzcyI6InNub3dfbGVlIiwiYXVkIjoidXNlciJ9.VnEv9-lo8jRw59-CyFLj19MIIWTwhwdrdeohOx7UH5Ms5akTkNg7whS0EdbL5s0COh3tlo1jaZp5XBNm12mRMQ";
   char *str = "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJleHAiOjIxNDc0ODM2NDcsInN1YiI6IjI3IiwiaXNzIjoic2hhY2tsZSIsImF1ZCI6Indhbmd3dSJ9.bhGoAgz8vVPQfI8heZhOWkh5uP-1-LfofYLKPoY9nKUKIK4RUmL712JabXjgCa9xHK-g9zgGllt0CXgBybg0eA";
   // 填入token字符串和长度
   params.jwt = (char*)str;
   params.jwt_length = strlen(str);

   // 设置密钥
   char *key="shackle string token key";
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
       printf("Token validation successful!\n");

       // 打印token中解码出来的荷载信息
       for (size_t i = 0; i < claim_count; i++) {
           printf("Claim [%zu]: %s = %s\n", i, claims[i].key, claims[i].value);
       }
       
   } else {
       printf("Token validation failed!\n");
   }

   l8w8jwt_free_claims(claims, claim_count);

   return 0;
}
