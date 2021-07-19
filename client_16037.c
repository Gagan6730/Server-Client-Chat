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

void *recv_msg(void *socket)
{
    //    int server_sock = *((int *)sock);
    char msg[500];
    int val;
    int len;
    while((len = recv(*((int *)socket),msg,500,0)) > 0) {
        msg[len] = '\0';
        val++;
        fprintf(stdout,"%s",msg);
        //        memset(msg,'\0',sizeof(msg));
    }
}
int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;
    int client_sock;

    char strings[100];
    int server_addr_size;
    int server_sock;
    pthread_t sendt,recvt;
    char msg[500];
    char name[100];
    int length=0;
    char response[600];

    int len;
    char const_msg[]="recieved from ";
    int val=0;

    if(argc > 2) {
        fprintf(stderr,"too many arguments");
        exit(1);
    }

    strcpy(name,argv[1]);
    client_sock = socket(AF_INET,SOCK_STREAM,0);
    //    memset(server_addr.sin_zero,'\0',sizeof(server_addr.sin_zero));
    server_addr.sin_family = AF_INET;
    if(length==1)
    {
        length++;
    }
    server_addr.sin_port = htons(portno);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    length++;
    if(connect(client_sock,(struct sockaddr *)&server_addr,sizeof(server_addr)) >= 0)
    {
        val++;
    }
    else{
        fprintf(stderr,"connection not esatablished");
        exit(1);
    }
    fprintf(stdout,"connected to server\n");

    pthread_create(&recvt,NULL,recv_msg,&client_sock);

    while( fgets(msg,500,stdin)> 0) {

        strlen(strings);
        strcpy(response,const_msg);
        length++;
        strcat(response,name);

        strcat(response,":");
        strcat(response,msg);
        if((strlen(msg)-1)>0) {
            if ((len = write(client_sock, response, strlen(response))) < 0) {
                fprintf(stderr, "message not sent");
                exit(1);
            }
        }
        //        memset(msg,'\0',sizeof(msg));
        //        memset(response,'\0',sizeof(response));
    }
    pthread_join(recvt,NULL);
    close(client_sock);

}