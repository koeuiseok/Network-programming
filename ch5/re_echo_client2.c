
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
	char message[BUF_SIZE+1];
	int str_len, recv_len,recv_cnt;
	
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
	
	while(1){
		printf("input message (Q to quit) : ");
		fgets(message, BUF_SIZE, stdin);
		if(!strcmp(message, "q\n")||!strcmp(message, "Q\n"))
		{
			break;
		}
		str_len = strlen(message);
		if(str_len != writen(sock, message, strlen(message)))
		{
			error_handling("written() error");
		}
		
		recv_cnt = readn(sock, message, str_len);
		if(recv_cnt <0)
		{
			error_handling("readn() error");
		}
		message[recv_cnt] = '\0';
		
		printf("Message from server : %s", message);
	}
	close(sock);
	
	return 0;
}

