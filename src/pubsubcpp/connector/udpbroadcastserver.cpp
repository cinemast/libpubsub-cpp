/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    udpbroadcastserver.cpp
 * @date    5/30/2015
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "udpbroadcastserver.h"

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

#include <jsoncpp/json/json.h>
#include <iostream>

using namespace std;

UdpBroadcastServer::UdpBroadcastServer(int port, const string &ip) :
    m_port(port),
    m_thread(NULL),
    m_run(false),
    m_ip(ip)
{
}

bool UdpBroadcastServer::StartListening()
{
    m_run = true;
    //m_thread = new thread(UdpBroadcastServer::handleConnections, this);
    handleConnections(this);
}

bool UdpBroadcastServer::StopListening()
{
    if (m_run)
    {
        m_run = false;
        m_thread->join();
        delete m_thread;
        return true;
    }
    return false;
}

bool UdpBroadcastServer::SendResponse(const string &response, void *addInfo)
{
    return true;
}

void UdpBroadcastServer::handleConnections(UdpBroadcastServer *_this)
{
    int sock, status, buflen;
    unsigned sinlen;
    struct sockaddr_in sock_in;

    sinlen = sizeof(struct sockaddr_in);
    memset(&sock_in, 0, sinlen);

    sock = socket (PF_INET,SOCK_DGRAM,IPPROTO_UDP);

    //struct timeval tv;
    //tv.tv_sec = 0;  /* 30 Secs Timeout */
    //tv.tv_usec = 100000;  // Not init'ing this can cause strange errors

    sock_in.sin_addr.s_addr = htonl(INADDR_ANY);//htonl(SO_BROADCAST);
    sock_in.sin_port = htons(_this->m_port);
    sock_in.sin_family = PF_INET;

    status = bind(sock, (struct sockaddr *)&sock_in, sinlen);
    printf("Bind Status = %d\n", status);

    status = getsockname(sock, (struct sockaddr *)&sock_in, &sinlen);
    printf("Sock port %d\n",htons(sock_in.sin_port));

    buflen = UDP_BUFFER_LEN;
    memset(_this->m_buffer, 0, buflen);

    while (_this->m_run)
    {
        cout << "Before recvfrom" << endl;
        status = recvfrom(sock, _this->m_buffer, buflen, 0, (struct sockaddr *)&sock_in, &sinlen);
        cout << "After recvfrom" << endl;
        if (status > 0)
        {
            std::thread t(handleRequest, _this, inet_ntoa(sock_in.sin_addr), _this->m_buffer);
            t.detach();
        }
    }

    shutdown(sock, 2);
    close(sock);
}

void UdpBroadcastServer::handleRequest(UdpBroadcastServer *_this, string ip, string message)
{
    Json::Reader reader;
    Json::Value val;
    reader.parse(message,val);

    val["params"]["ip"] = ip;

    _this->OnRequest(val.toStyledString(), NULL);
}
