#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed proc id : %d\n", pid);

}

#define BUF_SIZE 128

int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;
	char message[BUF_SIZE+1];
	int str_len, cnt_i;
	int state;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

    pid_t pid;
	struct sigaction act;

	if(argc != 2){
		fprintf(stderr, "Usage : %s <port>\n", argv[0]);
		exit(1);
	}

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
	
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
	printf("waiting....\n");

	while(1)
	{
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        if(clnt_sock == -1)
		{
			continue;
		}
		else{
			printf("New client connected");
		}

        pid = fork();

        if(pid ==0)
        {
            close(serv_sock);
    		while((str_len = read(clnt_sock, message, BUF_SIZE)) !=0)
    		{
		    	write(clnt_sock, message, str_len);
		    }

            close(clnt_sock);
            puts("client disconnected..");
            return 0;
        }
        else
        {
            close(clnt_sock);
        }


	}
    close(serv_sock);
	return 0;
}
