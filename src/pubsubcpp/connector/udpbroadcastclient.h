/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    udpbroadcastclient.h
 * @date    5/30/2015
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef UDPBROADCASTCLIENT_H
#define UDPBROADCASTCLIENT_H

#include <string>
#include <jsonrpccpp/client/iclientconnector.h>
#include <jsonrpccpp/common/exception.h>

class UdpBroadcastClient : public jsonrpc::IClientConnector
{
    public:
        UdpBroadcastClient(int port, const std::string &address = "255.255.255.255");

        virtual void SendRPCMessage(const std::string& message, std::string& result) throw(jsonrpc::JsonRpcException);

    private:
        int m_port;
        std::string m_address;
};

#endif // UDPBROADCASTCLIENT_H
