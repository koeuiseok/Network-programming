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

#define BUF_SIZE 	1024
#define OPSZ 		4

int calculate(int opnum, int opnds[], char op)
{
	int result = ntohl(opnds[0]);
	int cnt_i;
	
	switch(op)
	{
		case '+':
			for(cnt_i = 1; cnt_i < opnum; cnt_i++)
			{
				result += ntohl(opnds[cnt_i]);
			}
			break;
		case '-':
			for(cnt_i = 1; cnt_i < opnum; cnt_i++)
			{
				result -= ntohl(opnds[cnt_i]);
			}
			break;
		case '*':
			for(cnt_i = 1; cnt_i < opnum; cnt_i++)
			{
				result *= ntohl(opnds[cnt_i]);
			}
			break;
	}
	return result;
}

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	
	char opinfo[BUF_SIZE];
	int result, opnd_cnt, cnt_i;
	int msgLen;
	
	if(argc != 2){
		fprintf(stderr, "Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET, SOCK_STREAM,0);
	if(serv_sock ==-1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		error_handling("bind() error");
	}
	if(listen(serv_sock,5)==-1){
		error_handling("listen() error");
	}
	
	for(cnt_i=0; cnt_i <5; cnt_i++)
	{
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if(clnt_sock == -1)
		{
			error_handling("accept() error");
		}	
		else{
			printf("connected %d\n", cnt_i);
		}
		opnd_cnt = 0;
		if(readn(clnt_sock, &opnd_cnt,1)<0)
		{
			error_handling("readn() error");
		}
		msgLen = opnd_cnt*OPSZ + 1;
	    if(readn(clnt_sock, opinfo,msgLen)<0)
		{
			error_handling("readn() error");
		}		
		
		//calculate
		result = calculate(opnd_cnt, (int*)opinfo, opinfo[msgLen-1]);
		printf("result for clnt %d : %d", cnt_i, result);
		result = htonl(result);
		
		if(sizeof(result) != writen(clnt_sock, (char*)&result, sizeof(result))){
			error_handling("writen() error");		
		}
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}
