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
	int serv_sock;
	int clnt_sock;
	char buf[BUF_SIZE+1];
	int n;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	
	if(argc != 2){
		fprintf(stderr, "Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET, SOCK_STREAM,0);
	if(serv_sock ==-1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
	{
		error_handling("bind() error");
	}
	
	if(listen(serv_sock,5)==-1)
	{
		error_handling("listen() error");
	}
	
	while(1){
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr ,&clnt_addr_size);
		if(clnt_sock == -1){
			error_handling("accept() error");
		}
		printf("\n [TCP Server] Client connected IP = %s, port = %d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
		while(1)
		{
			n = readn(clnt_sock, buf, BUF_SIZE);
			if(n==-1){
				error_handling("read() error");
				break;
			}
			else if(n==0){   //receiving EOF
				break;
			}
			buf[n] = '\0';
			printf("[TCP/%s:%d] = %s \n ", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port),buf);
		}
		close(clnt_sock); 
		printf("[TCP server] Client closed IP = %s, port = %d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
	}

	close(serv_sock);

	return 0;
}
