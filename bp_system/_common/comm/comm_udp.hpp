#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <cstdint>

#ifdef __linux__ 
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    //#include <netdb.h>
#elif 	_WIN64
    #include <WinSock2.h>
    #include <ws2tcpip.h>
    //#include <Windows.h>
#endif

#define UDP_PORT_RECV 65002
#define UDP_PORT_SEND 65005 //50001

#define UDP_SEND_IP "127.0.0.1"
#define UDP_RECV_IP "127.0.0.1"

class CommUdp
{
private:
    /* data */
#ifdef __linux__ 
    int recv_socket;
    int send_socket;
    struct sockaddr_in recv_address;
    struct sockaddr_in send_address;
#elif 	_WIN64
    SOCKET send_socket;
    SOCKET recv_socket;
//    SOCKET send_sock;
    WSADATA recv_wsaData;
    WSADATA send_wsaData;
    SOCKADDR_IN send_address;
    SOCKADDR_IN recv_address;
#endif
public:
    CommUdp(/* args */);
    ~CommUdp();
    void send_data(const uint8_t *data, int data_size);
    int recv_data(uint8_t *data, int data_size);
    void set_send_address(const char *ip, int port);
    void set_recv_address(const char *ip, int port);
    void reset_send_address(const char *ip, int port);
    void reset_recv_address(const char *ip, int port);
    void close_send_socket();
    void close_recv_socket();
};

