
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <assert.h>

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "readnwrite.h" 
#include "enc.h"
#include "msg.h"


void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}


int main(int argc, char* argv[])
{
	int serv_sock =-1;
	int clnt_sock=-1;

	int cnt_i;
    int n;
    int plaintext_len,len,ciphertext_len;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

    APP_MSG msg_in;
    APP_MSG msg_out;

    char plaintext[BUFSIZE + AES_BLOCK_LEN] = {0x00,};

    unsigned char key[AES_KEY_128] ={0x00,};
    unsigned char iv[AES_KEY_128] ={0x00,};

    for(cnt_i =0; cnt_i < AES_KEY_128; cnt_i++)
    {
         key[cnt_i]= (unsigned char)cnt_i;
         iv[cnt_i] = (unsigned char)cnt_i;
    }

	if(argc != 2){
		fprintf(stderr, "Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM,0);
	if(serv_sock ==-1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
	{
		error_handling("bind() error");
	}
	
	if(listen(serv_sock,5)==-1)
	{
		error_handling("listen() error");
	}
	printf("waiting....\n");

    while(1)
    {
        clnt_addr_size = sizeof(clnt_addr);

        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
        {
            error_handling("accept() error");
        }
        printf("[%s:%d] connected\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

        while(1)
        {
            n=readn(clnt_sock, &msg_in, sizeof(APP_MSG));
            if(n==-1)
            {
                error_handling("readn() error");
            }

            msg_in.msg_len = ntohl(msg_in.msg_len);
            printf("\nencryptMsg\n");
            BIO_dump_fp(stdout, (const char*)msg_in.payload, msg_in.msg_len);

            plaintext_len = decrypt(msg_in.payload, msg_in.msg_len, key,iv, (unsigned char*)plaintext);
            BIO_dump_fp(stdout, (const char*)plaintext, plaintext_len);
        
            plaintext[plaintext_len] = '\0';
            printf("%s\n",plaintext);

            printf("Input a message > \n");
            if(fgets(plaintext, BUFSIZE+1,stdin)==NULL)
            {
                break;
            }
            len = strlen(plaintext);
            if(plaintext[len-1]=='\n'){
                plaintext[len-1] = '\0';
            }
            if(strlen(plaintext)==0){
                break;
            }

            ciphertext_len = encrypt((unsigned char*)plaintext,len,key,iv, msg_out.payload);
            msg_out.msg_len = htonl(ciphertext_len);
            
            n=writen(clnt_sock, &msg_out, sizeof(APP_MSG));
            if(n==-1)
            {
                error_handling("writen() error");
                break;
            }
        }
        close(clnt_sock);
        printf("[%s:%d] closed\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    }
    close(serv_sock);
    return 0;
}