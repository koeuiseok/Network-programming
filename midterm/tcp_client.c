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
	int sock;
	struct sockaddr_in serv_addr;
	char buf[1024];
	char fileName[128]={0,};
	int file_len = 0, read_len=0;
	int fd;
	
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
		error_handling("connect() error");
	}	
	
	printf("Input a file name : ");
	fgets(buf, sizeof(buf),stdin);
	buf[strnlen(buf, sizeof(buf))-1]='\0';
	
	write(sock, buf, sizeof(buf));
	sprintf(fileName, "received_%s",buf );
	
	fd = open(fileName, O_CREAT|O_WRONLY, S_IRWXU);
	if(fd ==-1){
		error_handling("open() error");
	}
	read(sock, &file_len, sizeof(file_len));
	file_len = ntohl(file_len);
	
	printf("Received %d bytes of a file\n", file_len);
	
	while((read_len = read(sock, buf, sizeof(buf)))>0)
	{
		if(write(fd, buf, read_len)!= read_len)
		{
			error_handling("write() error");
		}
	}
	printf("Receiving completed\n");
	
	close(sock);
	
	return 0;
	
}

