
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "readnwrite.h"

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

#define BUF_SIZE 128
int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char buf[BUF_SIZE+1];
	char *testdata[] = {"HELLO", "I am KOOKMIN", "nice to meet you","hi"};
	int dataLen;
	int n, cnt_i;
	if(argc != 3)
	{
		fprintf(stderr, "%s <IP> <PORT>\n", argv[0]);
		exit(1);
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family =  AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);  //"127.0.0.1" - > 32bit value 
	serv_addr.sin_port = htons(atoi(argv[2]));  // "9190" -> 9190
	
	
	
	for(cnt_i =0; cnt_i < sizeof(testdata)/sizeof(char*); cnt_i++)
	{
		sock = socket(PF_INET, SOCK_STREAM, 0);
		if(sock == -1)
		{
			error_handling("socket() error");
		}
		if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		{
			error_handling("connect errror");
		}
		else{
			printf("connected\n");
		}
		dataLen = strnlen(testdata[cnt_i], sizeof(buf));
		strncpy(buf, testdata[cnt_i], dataLen);
		n = writen(sock, buf, dataLen);
		if(n==-1){
			error_handling("write() error");
		}
		printf("[TCP_Client] %d byte sent\n", n);
		close(sock);
	}

	return 0;
}
