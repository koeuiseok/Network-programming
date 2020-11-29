#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>


void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])   //argv : file_name, ip, port, copy
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[1024];
	int str_len;
	
	int out_fd;
	
	if(argc != 4)
	{
		fprintf(stderr, "%s <IP> <PORT>\n", argv[0]);
		exit(1);
	}
	
	out_fd = open(argv[3], O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
	if(out_fd == -1)
	{
		error_handling("file open() error");
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
		error_handling("connect() error");
	}	
	
	printf("executing connect() function\n");
	
	memset(message, 0, 1024);
	while(1)
	{
		str_len = read(sock, message, sizeof(message));
		
		if(str_len == -1 )
		{
			error_handling("read() error");
		}
		
		if(str_len >0)
		{
			printf("Message from server : %s\n", message);
			write(out_fd, message, str_len);
			
		}
		else if(str_len == 0)
		{
			break;
		}
	}
	
	
	if(out_fd != -1)
	{
		close(out_fd);
	}
	
	close(sock);
	
	return 0;
	
}

