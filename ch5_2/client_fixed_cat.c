
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "readnwrite.h"

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

#define BUF_SIZE 128
int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char msg[BUF_SIZE+1];
	char* buf;
	char* chatid;
	int n, len, lenofchatid;
	if(argc != 4)
	{
		fprintf(stderr, "%s <IP> <PORT> <id>\n", argv[0]);
		exit(1);
	}
	
	chatid = argv[3];
	
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
	
	sprintf(msg, "[%s]", chatid);
	lenofchatid = strlen(msg);
	buf = msg + lenofchatid;    //chating id 설정
	
	while(1)
	{
		printf("[%s]",chatid);
		if(fgets(buf, (BUF_SIZE+1)-lenofchatid, stdin)==NULL)
		{
			break;
		}
		len = strlen(buf);
		if(buf[len-1]=='\n'){
			buf[len-1] = '\0';
		}
		n=writen(sock,msg, BUF_SIZE);
		if(n==-1){
			 error_handling("writen() error");
		}
		n=readn(sock, buf, BUF_SIZE);
		if(n==-1){
			error_handling("readn() error");
		}
		buf[n] = '\0';
		printf("%s\n",buf);
	}
	
	close(sock);
	return 0;
}
