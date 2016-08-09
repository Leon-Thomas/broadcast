/*
 * 文件:client_2.c
 * 功能：局域网聊天工具客户端
 * Auther:Leon
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "michat.h"

char send_buf[BUFSIZE];
char recv_buf[BUFSIZE];

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

    //connect
    memset(&serv_addr, 0, sizeof(SI));
    serv_addr.sin_family      = AF_INET;
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr.s_addr);
    serv_addr.sin_port        = htons(atoi(argv[2]));
    if(connect(sfd, (const SA *)&serv_addr, sizeof(SI)) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    
    fd_set read;
    fd_set send;
    FD_ZERO(&read);
    FD_ZERO(&send);
    int maxfd = STDIN_FILENO > sfd ? STDIN_FILENO : sfd;
    while(true)
    {
        struct timeval timeout;
        timeout.tv_sec  = 0;
        timeout.tv_usec = 1000;
        FD_SET(sfd, &read);
        FD_SET(STDIN_FILENO, &send);

        int reval = select(maxfd+1, &read, &send, NULL, &timeout);
        
        if(reval > 0)
        {
            
            if(FD_ISSET(sfd,&read))
            {
                ssize_t rn;
                //读取数据
                if((rn = recv(sfd, recv_buf, BUFSIZE,MSG_DONTWAIT)) > 0)
                {
                    printf("\n%s",recv_buf);
                }
                else if(rn == -1)
                {
                    int err = errno;
                    if(err != EAGAIN || err != EWOULDBLOCK)
                        exit(EXIT_FAILURE);
                } 
            }

            if(FD_ISSET(STDIN_FILENO,&send))
            {
                ssize_t wn;
                //发送数据
                puts("输入信息：");
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
        else if(reval == -1)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Nothing.\n");
        }

    }

   exit(EXIT_SUCCESS);
}

