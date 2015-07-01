/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    udpbroadcastserver.h
 * @date    5/30/2015
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef UDPBROADCASTSERVER_H
#define UDPBROADCASTSERVER_H

#include <string>
#include <thread>
#include <jsonrpccpp/server/abstractserverconnector.h>

#define UDP_BUFFER_LEN 65507


class UdpBroadcastServer : public jsonrpc::AbstractServerConnector
{
    public:
        UdpBroadcastServer(int port, const std::string &ip);

        bool StartListening();
        bool StopListening();

        bool virtual SendResponse(const std::string& response, void* addInfo = NULL);

    private:
        int m_port;
        std::thread* m_thread;
        bool m_run;
        char m_buffer[UDP_BUFFER_LEN];
        std::string m_ip;

        static void handleConnections(UdpBroadcastServer* _this);

        static void handleRequest(UdpBroadcastServer* _this, std::string ip, std::string message);
};

#endif // UDPBROADCASTSERVER_H
