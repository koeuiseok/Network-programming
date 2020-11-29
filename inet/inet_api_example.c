#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	char *addr1 = "127.212.124.78";
	char *addr2 = "127.212.124.255";
	unsigned long conv_addr;
	struct sockaddr_in inet_addr1, inet_addr2, inet_addr3;
	char *str_ptr, *str_ptr1,*str_ptr2;
	char str_arr[32];
	
	conv_addr = inet_addr(addr1);
	if(conv_addr == INADDR_NONE)
	{
		fprintf(stderr, "Conversion error\n");
	}
	else
	{
		fprintf(stdout, "Network ordered integer addr : %#lx\n", conv_addr);
	}
	 
	
	if(!inet_aton(addr2, &inet_addr1.sin_addr))  // inet_aton : inet_addr 
	{
		fprintf(stderr, "Conversion error\n");
	}
	else
	{
		fprintf(stdout,"Network ordered integer addr : %#lx\n", inet_addr1.sin_addr);
	}
	
	
	inet_addr2.sin_addr.s_addr = htonl(0x01020304);
	inet_addr3.sin_addr.s_addr = htonl(0x01010101);
	
	str_ptr1 = inet_ntoa(inet_addr2.sin_addr);
	strcpy(str_arr, str_ptr1);
	fprintf(stdout, "demical out : %s\n", str_arr);
	
	str_ptr2 = inet_ntoa(inet_addr3.sin_addr);
	fprintf(stdout, "demical out : %s\n", str_ptr2);
	fprintf(stdout, "demical out : %s\n", str_arr);
	
	return 0;
}
