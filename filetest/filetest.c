#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(void)
{
	int fd1, fd2, fd3;
	char buf[] = "Hello, class\n";
	
	fd1 = socket(PF_INET, SOCK_STREAM, 0);
	if(fd1 == -1)
	{
		error_handling("socket() error");
	}
	fd2 = open("test.txt", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
	if(fd2 == -1)
	{
		error_handling("open() error");
	}
	fd3 = socket(PF_INET, SOCK_STREAM, 0);
	if(fd3 == -1)
	{
		error_handling("socket() error");
	}
	
	printf("file descriptor1 : %d\n", fd1);
	printf("file descriptor2 : %d\n", fd2);
	printf("file descriptor3 : %d\n", fd3);
	
	if(write(fd2,buf,sizeof(buf)==-1)){
		error_handling("write() error");
	}
	
	close(fd1);
	close(fd2);
	close(fd3);
	
	fd2 = open("test.txt", O_RDONLY);
	if(fd2 == -1)
	{
		error_handling("open() error");
	}x
	printf("file descriptor2 : %d\n", fd2);
	printf("file data2 : %s\n", buf);
	
	close(fd2);
}
