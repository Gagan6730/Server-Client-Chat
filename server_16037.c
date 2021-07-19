#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define portno 8080
struct client_info {
    int sockno;
    char ip_address[INET_ADDRSTRLEN];
};
int clients[100];
int client_count = 0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER ;

struct sockaddr_in server_addr,client_addr;
int server_sock;

socklen_t client_addr_size;
struct client_info cl;


void *recv_msg(void *sock)
{

    struct client_info curr_client = *((struct client_info *)sock);
    char msg[500];
    int len;
    while((len = recv(curr_client.sockno,msg,500,0)) > 0) {
        msg[len] = '\0';
        pthread_mutex_lock(&mutex);
        for(int i = 0; i < client_count; i++) {
            if(send(clients[i],msg,strlen(msg),0) < 0) {
                fprintf(stderr,"sending failure");
                continue;
            }
//		}
//            memset(msg,'\0',sizeof(msg));

        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_lock(&mutex);
    printf("%s disconnected\n",curr_client.ip_address);
    for(int i = 0; i < client_count; i++) {
        if(clients[i] == curr_client.sockno) {

            clients[i]=clients[--client_count];
        }
    }


    pthread_mutex_unlock(&mutex);
}
void add_client(pthread_t rec,int client_sock_num)
{
    pthread_mutex_lock(&mutex);
    //locking the critical section
    inet_ntop(AF_INET, (struct sockaddr *)&client_addr, cl.ip_address, INET_ADDRSTRLEN);
    fprintf(stdout,"%s connected\n",cl.ip_address);
    cl.sockno = client_sock_num;
    clients[client_count++] = client_sock_num;
    pthread_create(&rec,NULL,recv_msg,&cl);
    pthread_mutex_unlock(&mutex);
}
int main(int argc,char *argv[])
{
    int length=0;
    server_sock = socket(AF_INET,SOCK_STREAM,0);
    length++;
//    memset(server_addr.sin_zero,'\0',sizeof(server_addr.sin_zero));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);
    if(length==1)
    {
        length++;
    }
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_addr_size = sizeof(client_addr);
    length++;

    if(bind(server_sock,(struct sockaddr *)&server_addr,sizeof(server_addr)) != 0) {
        fprintf(stderr,"binding unsuccessful");
        exit(1);
    }

    if(listen(server_sock,5) != 0) {
        fprintf(stderr,"listening unsuccessful");
        exit(1);
    }

    while(1) {
        pthread_t recvt;
        int client_sock_num;
        if((client_sock_num = accept(server_sock,(struct sockaddr *)&client_addr,&client_addr_size)) >= 0) {

            add_client(recvt,client_sock_num);
        }
        else
        {
            fprintf(stderr,"accept unsuccessful");
            exit(1);
        }

    }
    return 0;
}