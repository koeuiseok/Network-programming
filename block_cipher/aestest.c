#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>

void aes_simple_test()
{
    AES_KEY aes_enc_key, aes_dec_key;
    unsigned char mk[16] ={0x00,0x01, 0x02,0x03, 0x04,0x05,0x06,0x07,
    0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    unsigned char iv_dec[AES_BLOCK_SIZE] = {0x00,};
    unsigned char iv_enc[AES_BLOCK_SIZE]={0x00,};

    unsigned char plaintext[256] = {0x00,};
    unsigned char ciphertext[256+AES_BLOCK_SIZE] = {0x00,};
    unsigned char recovered[256]= {0x00,};

    AES_set_encrypt_key(mk,128,&aes_enc_key);
    AES_set_decrypt_key(mk,128,&aes_dec_key);

    memset(plaintext, 'A', sizeof(plaintext));
    printf("Plaintext\n");
    BIO_dump_fp(stdout,(const char*)plaintext, sizeof(plaintext));
        
    AES_cbc_encrypt(plaintext, ciphertext, sizeof(plaintext),&aes_enc_key, iv_enc, AES_ENCRYPT); 
    printf("\nciphertext\n");
    BIO_dump_fp(stdout,(const char*)ciphertext, sizeof(ciphertext));
    AES_cbc_encrypt(ciphertext, recovered, sizeof(ciphertext),&aes_dec_key, iv_dec, AES_DECRYPT); 

     printf("\nRecovered\n");
    BIO_dump_fp(stdout,(const char*)recovered, sizeof(recovered));

    if(0!= memcmp(plaintext, recovered, sizeof(plaintext)))
    {
        fprintf(stderr, "\nDECRYPTION FAILED\n");
    }
    else{
        fprintf(stderr, "\nsucceeded\n");
    }

}

int main(int argc, char* argv[])
{

    aes_simple_test();

}

