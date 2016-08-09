/*
 * 文件:client_michat.c
 * 功能：局域网聊天工具客户端
 * Auther:Leon*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "michat.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char send_buf[BUFSIZE];
char recv_buf[BUFSIZE];
void *wthread(void *arg)
{
    ssize_t wn;
    int sfd = *(int*)arg;

    while(true)
    {    
        //pthread_mutex_lock(&mutex);
        puts("输入信息：");
        //pthread_mutex_unlock(&mutex);
        if(fgets(send_buf, BUFSIZE,stdin) == NULL)
        {
            exit(EXIT_SUCCESS);
        }
        if((wn = write(sfd, send_buf, strlen(send_buf)+1)) < 0)
        {
            perror("write");
            exit(EXIT_SUCCESS);
        } 
    }
}

int main(int argc, char *argv[])
{
    int sfd;
    SI serv_addr;

    if(argc != 3)
    {
        printf("Usage:<server IP> <port>\n");
        exit(EXIT_FAILURE);
    }

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(SI));
    serv_addr.sin_family      = AF_INET;
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr.s_addr);
    serv_addr.sin_port        = htons(atoi(argv[2]));
    if(connect(sfd, (const SA *)&serv_addr, sizeof(SI)) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    pthread_t pid;
    if(pthread_create(&pid, NULL, wthread,(void*)&sfd) < 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    ssize_t rn;
    while(true)
    {
       if((rn = recv(sfd, recv_buf, BUFSIZE,0)) > 0)
       {
            //pthread_mutex_lock(&mutex);
            printf("%s",recv_buf);
            //pthread_mutex_lock(&mutex);
       }
       else if(rn == -1)
       {
            perror("connect");
            exit(EXIT_FAILURE);
       } 
       else
       {
           exit(EXIT_FAILURE);
       }
    }

   exit(EXIT_SUCCESS);
}

