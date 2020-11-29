#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	struct sockaddr_in addr;
	char* serv_ip ="211.217.168.13";
	char * serv_port = "9190";
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(serv_ip);
	addr.sin_port = htons(atoi(serv_port));
	//addr.sin_addr.s_addr = htonl(INADDR_ANY);
	return 0;
	
}
