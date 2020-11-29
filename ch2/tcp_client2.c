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
	char message[128];
	int str_len, read_len=0;
	int idx=0;
	
	if(argc !=3){
		error_handling("Write <name> <IP>  <PORT>");
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1){
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
	
	while((str_len = read(sock, &message[idx++], 1))){
		if(str_len == -1)
		{
			error_handling("read() error");
		}
		printf("%d\n",str_len);
		read_len += str_len;
	}
	
	printf("message from server : %s\n", message);
	printf("data length : %d",read_len);
	
	close(sock);
	return 0;
}


