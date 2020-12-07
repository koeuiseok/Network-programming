#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 128

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

//in parent process
int read_routine(int sock, char *buf)
{
    while(1)
    {
        sleep(5);
        int str_len = read(sock, buf, BUF_SIZE);
        if(str_len == 0 )
        {
            return 0;
        }
        buf[str_len] = '\0';
        printf("Message from server : %s\n", buf);
    }
}

//in child process
void write_routine(int sock, char* buf)
{
    while(1)
    {
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n")|| !strcmp(buf, "Q\n"))
        {
            shutdown(sock, SHUT_WR); 
        }
        write(sock, buf, strlen(buf));
    }
}

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char buf[BUF_SIZE+1];
	int str_len;
	
    pid_t pid;

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
	
	pid = fork();

    if(pid == 0)
    {
        write_routine(sock, buf);
    }
    else{
        read_routine(sock, buf);
    }

	close(sock);
	
	return 0;
}
