
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
    int sock;

	int cnt_i;
    int n;
    int plaintext_len,len,ciphertext_len;

	struct sockaddr_in serv_addr;


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


	if(argc != 3)
	{
		fprintf(stderr, "%s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		error_handling("socket() error");
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family =  AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);  //"127.0.0.1" - > 32bit value 
	serv_addr.sin_port = htons(atoi(argv[2]));  // "9190" -> 9190
	
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
	{
		error_handling("connect errror");
	}
	else{
		printf("connected\n");
	}

    while(1)
    {
        printf("Input a message > \n");
        if(fgets(plaintext,BUFSIZE+1, stdin)==NULL)
        {
            break;
        }

        len =strlen(plaintext);
        if (plaintext[len - 1] == '\n')
            plaintext[len - 1] = '\0';
        if (strlen(plaintext) == 0)
            break;

        memset(&msg_out,0,sizeof(msg_out));

        ciphertext_len = encrypt((unsigned char*)plaintext, len, key, iv, msg_out.payload);
        msg_out.msg_len =  htonl(ciphertext_len);

        n = writen(sock, &msg_out, sizeof(APP_MSG));
        if (n == -1)
        {
            error_handling("writen() error");
            break;
        }

        n = readn(sock, &msg_in, sizeof(APP_MSG));
        if (n == -1)
        {
            error_handling("readn() error");
            break;
        }

        msg_in.msg_len = ntohl(msg_in.msg_len);
        printf("\nencryptMsg\n");
        BIO_dump_fp(stdout, (const char *)msg_in.payload, msg_in.msg_len);

        plaintext_len = decrypt(msg_in.payload, msg_in.msg_len, key, iv, (unsigned char *)plaintext);
        BIO_dump_fp(stdout, (const char *)plaintext, plaintext_len);

        plaintext[plaintext_len] = '\0';
        printf("%s\n", plaintext);
    }
    close(sock);
    return 0;
}
