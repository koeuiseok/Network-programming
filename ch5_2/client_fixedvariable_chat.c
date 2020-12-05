
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

#define BUF_SIZE 32
int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char msg[BUF_SIZE+1];
	char* buf;
	char* chatid;
	int n, len, lenofchatid;
	int dataLen;
	
	if(argc != 4)
	{
		fprintf(stderr, "%s <IP> <PORT> <ID>\n", argv[0]);
		exit(1);
	}
	
	chatid =argv[3];
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		error_handling("socket() error");
	}
		
		
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family =  AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); 
	serv_addr.sin_port = htons(atoi(argv[2]));
	
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
		if(strlen(buf)==0){
			break;
		}
		//sending fixed part
		dataLen = len=strlen(msg);
		len = htonl(len);
		n = writen(sock, &len, sizeof(int));
		if(n==-1){
			 error_handling("writen() error");
		}
		
		//sending variable part
		n=writen(sock,msg, dataLen);
		if(n==-1){
			 error_handling("writen() error");
		}
		
		//receiving fixed part
		n=readn(sock, &len, sizeof(int));
		len = ntohl(len);
		if(n==-1){
			error_handling("readn() error");
		}
		else if(n==0){
			break;
		}
		
		//receiving variable part
		n=readn(sock, buf, len); 
		if(n==-1){
			error_handling("readn() error");
		}
		else if(n==0){
			break;
		}
		buf[n] = '\0';
		printf("%s\n",buf);
	}
	
	close(sock);
	return 0;
}
