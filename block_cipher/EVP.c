#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <assert.h>


void handleErrors()
{
    fprintf(stderr,"error\n");
    exit(1);
}

void aes_evp_test()
{
    EVP_CIPHER_CTX* ctx = NULL;
    int len;
    int ciphertext_len;
    int plaintext_len;

    unsigned char plaintext[256] = {0x00,};
    unsigned char ciphertext[256+AES_BLOCK_SIZE] = {0x00,};
    unsigned char recovered[256+AES_BLOCK_SIZE]= {0x00,};
    unsigned char mk[16] ={0x00,0x01, 0x02,0x03, 0x04,0x05,0x06,0x07,
    0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    unsigned char iv_dec[AES_BLOCK_SIZE] = {0x00,};
    unsigned char iv_enc[AES_BLOCK_SIZE]={0x00,};


    memset(plaintext, 'A', sizeof(plaintext));

    
    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        handleErrors();
    }
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aria_128_cbc(), NULL, mk, iv_enc))
    {
        handleErrors();        
    }
    plaintext_len = sizeof(plaintext);
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    {
        handleErrors();        
    }
    if(1 != EVP_EncryptFinal(ctx, ciphertext+len, &len))
    {
        handleErrors();        
    }
    printf("Plaintext\n");
    BIO_dump_fp(stdout, (const char *)plaintext, plaintext_len);

    printf("\nciphertext\n");
    ciphertext_len = plaintext_len+len;
    BIO_dump_fp(stdout, (const char *)ciphertext, ciphertext_len);

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aria_128_cbc(), NULL, mk, iv_dec))
    {
        handleErrors();        
    }

    if(1 != EVP_DecryptUpdate(ctx, recovered, &len, ciphertext, ciphertext_len))
    {
        handleErrors();        
    }
    if(1 != EVP_DecryptFinal(ctx, recovered+len, &len))
    {
        handleErrors();        
    }
    plaintext_len+=len;
    printf("\nRecovered\n");
    BIO_dump_fp(stdout, (const char *)recovered, plaintext_len);

    EVP_CIPHER_CTX_free(ctx);
}


int main(int argc, char* argv[])
{

    aes_evp_test();

}