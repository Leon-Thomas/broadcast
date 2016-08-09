#ifndef MICHART_H
#define MICHART_H

#ifndef IPv6
typedef struct sockaddr_in SI;
#define IPv4
#else
typedef struct sockaddr_in6 SI;
#endif  //IPv6
typedef struct sockaddr SA;

typedef struct _client
{
    int     sfd;        //套接字描述符
//    char    name[20];   //客户端昵称
    char    ip[16];     //点分十进制ip地址
}Client;

#define PORT            2016    //服务器端口

#define TCP
#ifdef  TCP
#define MAX_LISTENED    15      //tcp 最大监听数
#endif  //TCP

#define BUFSIZE         1024

#endif //MICHART_H
