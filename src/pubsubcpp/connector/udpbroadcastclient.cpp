/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    udpbroadcastclient.cpp
 * @date    5/30/2015
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "udpbroadcastclient.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define UDP_BUFFER_LEN 65507

UdpBroadcastClient::UdpBroadcastClient(int port, const std::string &address) :
    m_port(port),
    m_address(address)
{
}

void UdpBroadcastClient::SendRPCMessage(const std::string &message, std::string &result) throw (jsonrpc::JsonRpcException)
{
    if (message.size() >= UDP_BUFFER_LEN)
        throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_CONNECTOR);

    int sock, status, buflen, sinlen;
    char buffer[UDP_BUFFER_LEN];
    struct sockaddr_in sock_in;
    int yes = 1;

    sinlen = sizeof(struct sockaddr_in);
    memset(&sock_in, 0, sinlen);
    buflen = UDP_BUFFER_LEN;

    sock = socket (PF_INET,SOCK_DGRAM,IPPROTO_UDP);

    sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_in.sin_port = htons(m_port);
    sock_in.sin_family = PF_INET;

    status = bind(sock, (struct sockaddr *)&sock_in, sinlen);

    status = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );
  //  status = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &yes, sizeof(yes));


    sock_in.sin_addr.s_addr=htonl(-1); //inet_addr(m_address.c_str());
    sock_in.sin_port = htons(m_port);
    sock_in.sin_family = PF_INET;

    sprintf(buffer, message.c_str());
    buflen = strlen(buffer);

    status = sendto(sock, buffer, buflen, 0, (struct sockaddr *)&sock_in, sinlen);

    shutdown(sock, 2);
    close(sock);

}
