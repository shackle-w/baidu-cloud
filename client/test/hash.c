#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <openssl/evp.h>

void compute_hash(const EVP_MD *md_type, int fd, unsigned char *result, unsigned int *result_len) {
    EVP_MD_CTX *md_ctx;
    unsigned char buffer[1024];
    ssize_t bytes_read;

    md_ctx = EVP_MD_CTX_new();
    if (!md_ctx) {
        perror("EVP_MD_CTX_new failed");
        exit(EXIT_FAILURE);
    }
    if (EVP_DigestInit_ex(md_ctx, md_type, NULL) != 1) {
        perror("EVP_DigestInit_ex failed");
        EVP_MD_CTX_free(md_ctx);
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        if (EVP_DigestUpdate(md_ctx, buffer, bytes_read) != 1) {
            perror("EVP_DigestUpdate failed");
            EVP_MD_CTX_free(md_ctx);
            exit(EXIT_FAILURE);
        }
    }

    if (EVP_DigestFinal_ex(md_ctx, result, result_len) != 1) {
        perror("EVP_DigestFinal_ex failed");
        EVP_MD_CTX_free(md_ctx);
        exit(EXIT_FAILURE);
    }

    EVP_MD_CTX_free(md_ctx);
}

void print_hash(unsigned char *hash, unsigned int length) {
    for (unsigned int i = 0; i < length; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *file_path = argv[1];
    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    unsigned char md5_result[EVP_MAX_MD_SIZE];
    unsigned char sha1_result[EVP_MAX_MD_SIZE];
    unsigned char sha256_result[EVP_MAX_MD_SIZE];
    unsigned int md5_len, sha1_len, sha256_len;

    // Calculate and print MD5
    lseek(fd, 0, SEEK_SET);  // Reset file descriptor to the beginning
    compute_hash(EVP_md5(), fd, md5_result, &md5_len);
    printf("MD5: ");
    print_hash(md5_result, md5_len);

    // Calculate and print SHA-1
    lseek(fd, 0, SEEK_SET);  // Reset file descriptor to the beginning
    compute_hash(EVP_sha1(), fd, sha1_result, &sha1_len);
    printf("SHA-1: ");
    print_hash(sha1_result, sha1_len);

    // Calculate and print SHA-256
    lseek(fd, 0, SEEK_SET);  // Reset file descriptor to the beginning
    compute_hash(EVP_sha256(), fd, sha256_result, &sha256_len);
    printf("SHA-256: ");
    print_hash(sha256_result, sha256_len);

    close(fd);
    return EXIT_SUCCESS;
}

