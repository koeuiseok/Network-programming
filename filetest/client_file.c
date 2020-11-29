#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int serv_sock; //listening socket
	int clnt_sock; // data socket

	struct sockaddr_in serv_addr;   // server's IP and Port
	struct sockaddr_in clnt_addr;  // client's IP and Port
	char message[32];
	int str_len;
	char filename[64]={0x00,};
	
	if(argc != 3)
	{
		fprintf(stderr, "%s <IP> <PORT>\n", argv[0]);
		exit(1);
	}
	
	clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(clnt_sock == -1)
	{
		error_handling("socket() error");
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family =  AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);  //"127.0.0.1" - > 32bit value 
	serv_addr.sin_port = htons(atoi(argv[2]));  // "9190" -> 9190
	
	if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
	{
		error_handling("connect() error");
	}	
	
	printf("executing connect() function\n");
	
	str_len = read (clnt_sock, message, sizeof(message)-1);
	
	if(str_len == -1)
	{
		error_handling("read() error");
	}
	
	printf("%s : \n", message);
	scanf("%s",filename);
	
	if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
	{
		error_handling("connect() error");
	}	
	
	if(write(serv_sock, filename, sizeof(filename))==-1)
	{
		error_handling("22write error");
	}

	close(serv_sock);
	close(clnt_sock);
	
	return 0;
}

