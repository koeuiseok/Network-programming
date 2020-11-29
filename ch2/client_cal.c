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
	int sock;
	struct sockaddr_in serv_addr;
	int num1, num2, result;
	
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
	
	printf("input 2 num : \n");
	scanf("%d%d", &num1, &num2);
	printf("%d + %d = \n", num1, num2);
	num1 = htonl(num1);
	num2 = htonl(num2);
	
	write(sock, &num1, sizeof(num1));
	write(sock, &num2, sizeof(num2));
	
	read(sock, &result, sizeof(result));
	result = ntohl(result);
	
	printf("%d\n", result);
	close(sock);
	
	return 0;
}
