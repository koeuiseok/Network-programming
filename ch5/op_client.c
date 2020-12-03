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

#define BUF_SIZE 	 1024
#define RLT_SIZE 	 4
#define OPSZ 		 4
 
int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char opmsg[BUF_SIZE];
	int opnd_cnt, result;
	int msgLen, operand, temp;
	char IPAddr[] = "127.0.0.1";
	int cnt_i;
	
	if(argc != 2)
	{
		fprintf(stderr, "%s <PORT>\n", argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		error_handling("socket() error");
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family =  AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IPAddr);  //"127.0.0.1" - > 32bit value 
	serv_addr.sin_port = htons(atoi(argv[1]));  // "9190" -> 9190
	
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
	{
		error_handling("connect errror");
	}
	else{
		printf("connected.....\n");
	}
	
	fputs("Operand count : ",stdout);
	scanf("%d", &opnd_cnt);
	opmsg[0] = (char)opnd_cnt;
	
	for(cnt_i=0; cnt_i<opnd_cnt; cnt_i++)
	{
		printf("Operand %d : ",cnt_i + 1);
		scanf("%d", &operand);
		temp = htonl(operand);
		memcpy(&opmsg[cnt_i*OPSZ+1],&temp, sizeof(int) );		
	}
	
	fgetc(stdin);
	fprintf(stdout,"Operator : ");
	scanf("%c", &opmsg[opnd_cnt*OPSZ+1]);
	msgLen = opnd_cnt*OPSZ+2;
	
	if(msgLen != writen(sock, opmsg, msgLen))  
	//출력 버퍼에 남아있는 공간이 메시지 렝스보다 작은 경우에 반환 되기 때문에 writen 씀
	{
		error_handling("writen() error");
	}
	
	if(readn(sock, &result, RLT_SIZE)<0){
		error_handling("readn() error!");  
	}
	
	result = ntohl(result);
	printf("Operation result : %d\n", result);
	close(sock);
	return 0;
}

