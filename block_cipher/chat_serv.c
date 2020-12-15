#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUF_SIZE    128
#define MAX_CLNT    256

void* handle_clnt(void* arg);
void send_msg(char*msg, int len);
void error_handling(char* msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];

pthread_mutex_t mutex;



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

    pthread_t t_id;

	if(argc != 2){
		fprintf(stderr, "Usage : %s <port>\n", argv[0]);
		exit(1);
	}

    pthread_mutex_init(&mutex, NULL);

	
	serv_sock = socket(PF_INET, SOCK_STREAM,0);
	if(serv_sock ==-1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
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

        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL,handle_clnt,(void*)&clnt_sock);
        pthread_detach(t_id);

    }
    close(serv_sock);
    return 0;
}

void* handle_clnt(void* arg)
{
    int clnt_sock =*((int*)arg);
    int str_len =0, cnt_i;
    char msg[BUF_SIZE];

    while((str_len =  read(clnt_sock, msg, sizeof(msg)))!=0)
    {
        send_msg(msg, str_len);
    }
    pthread_mutex_lock(&mutex);
    for(cnt_i =0; cnt_i <clnt_cnt; cnt_i++)
    {
        if(clnt_sock == clnt_socks[cnt_i])
        {
            while(cnt_i ++ <clnt_cnt-1)
            {
                clnt_socks[cnt_i] = clnt_socks[cnt_i+1];
            }
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutex);
    close(clnt_sock);
    
    return NULL;
}

void send_msg(char*msg, int len)
{
    int cnt_i;

    pthread_mutex_lock(&mutex);
    for(cnt_i =0; cnt_i <clnt_cnt; cnt_i++)
    {
        write(clnt_socks[cnt_i], msg, len);
    }
    pthread_mutex_unlock(&mutex);
}


void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}