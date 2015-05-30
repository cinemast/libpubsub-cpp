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

#define UDP_BUFFER_LEN 65507

void onRequest(const std::string &ip, const std::string &message);

class IBroadcastHandler {
    public:
        virtual void onRequest(const std::string &ip, const std::string &message) = 0;
};

class UdpBroadcastServer
{
    public:
        UdpBroadcastServer(int port, IBroadcastHandler &handler);

        bool StartListening();
        void StopListening();

    private:
        int m_port;
        IBroadcastHandler& m_handler;
        std::thread* m_thread;
        bool m_run;
        char m_buffer[UDP_BUFFER_LEN];
        std::string m_ip;

        static void handleConnections(UdpBroadcastServer* _this);

        static void handleRequest(UdpBroadcastServer* _this);
};

#endif // UDPBROADCASTSERVER_H
