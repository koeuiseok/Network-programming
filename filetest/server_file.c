#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

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
	socklen_t clnt_addr_size;
	
	char message[] = "Input a file name";
	char filename[64]={0x00} ;
	int str_len;
	
	if(argc != 2){
		fprintf(stderr, "Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);//IPv4
	if(serv_sock == -1)
	{
		error_handling("socket() error");
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;     //IPv4
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error");
	}
	
	if(listen(serv_sock, 5) ==-1)
	{
		error_handling("listen() error");
	}
	
	clnt_addr_size = sizeof(clnt_addr);
	

	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock  ==-1)
	{
		error_handling("accept() error");
	}
	
	write(clnt_sock, message, sizeof(message));
	if(listen(serv_sock, 5) ==-1)
	{
		error_handling("listen() error");
	}
	
	clnt_addr_size = sizeof(clnt_addr);

	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock  ==-1)
	{
		error_handling("accept() error");
	}
	
	str_len = read (serv_sock, filename, sizeof(filename)-1);
	if(str_len ==-1){
		error_handling("read() error");
	}
	
	close(clnt_sock);
	close(serv_sock);
}
