#include "enc.h"
#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <assert.h>
#include <fcntl.h>

void handleErrors();
int encrypt(unsigned char* plaintext,int plaintext_len,unsigned char* key, unsigned char*iv, unsigned char* ciphertext);
int decrypt(unsigned char* ciphertext,int ciphertext_len,unsigned char* key, unsigned char*iv, unsigned char* recovered);


int encrypt(unsigned char* plaintext,int plaintext_len,unsigned char* key, unsigned char*iv, unsigned char* ciphertext)
{
    EVP_CIPHER_CTX* ctx = NULL;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        handleErrors();
    }
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    {
        handleErrors();        
    }

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    {
        handleErrors();        
    }
    ciphertext_len = len;
    if(1 != EVP_EncryptFinal(ctx, ciphertext+len, &len))
    {
        handleErrors();        
    }
    ciphertext_len += len;
    if(ctx !=NULL)
    {
        EVP_CIPHER_CTX_free(ctx);
    }

    return ciphertext_len;
}

int decrypt(unsigned char* ciphertext,int ciphertext_len,unsigned char* key, unsigned char*iv, unsigned char* recovered)
{
    EVP_CIPHER_CTX* ctx = NULL;
    int len;
    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        handleErrors();
    }

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    {
        handleErrors();        
    }
    if(1 != EVP_DecryptUpdate(ctx, recovered, &len, ciphertext, ciphertext_len))
    {
        handleErrors();        
    }
    plaintext_len = len;
    if(1 != EVP_DecryptFinal(ctx, recovered+len, &len))
    {
        handleErrors();        
    }
    plaintext_len+=len;

    if(ctx !=NULL){
        EVP_CIPHER_CTX_free(ctx);
    }

    return plaintext_len;
}

void handleErrors()
{
    fprintf(stderr,"error\n");
    exit(1);
}