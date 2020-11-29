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

int main(int argc, char* argv[])
{
	int fd1 = -1, fd2 = -2;
	int n; 
	char buf[1024];
	
	if(argc != 3)
	{
		error_handling("argument error");
	}
	
	fd1 = open(argv[1], O_RDONLY, S_IRWXU);
	if(fd1 == -1){
		error_handling("open() error");
	}
	
	fd2 = open(argv[2], O_CREAT | O_WRONLY|O_TRUNC, S_IRWXU);
	if(fd2 == -1){
		error_handling("open() error");
	}
	while((n=read(fd1, buf, sizeof(buf)))>0)
	{
		if(write(fd2, buf, n)!=n){
			error_handling("write() error");
		}
	}
	
	if(fd1 != -1){
		close(fd1);
	}
	if(fd2 != -1){
		close(fd2);
	}
	
	return 0;
}
