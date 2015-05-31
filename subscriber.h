/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    subscriber.h
 * @date    5/30/2015
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <string>
#include "pubsubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

class Subscriber : public PubSubClient
{
    public:
        //Used by the publisher
        Subscriber(const std::string &ip, int port);

        //Used by the subscriber
        Subscriber(const std::string &ip, int port, std::string subscriptionId);

        std::string getSubscriptionId();
        void setSubscriptionId(const std::string &id);

        std::string topic() const;
        void setTopic(const std::string &topic);

        std::string ip() const;
        void setIp(const std::string &ip);

    private:
        jsonrpc::HttpClient m_httpclient;
        std::string m_subscriptionId;
        std::string m_topic;
        std::string m_ip;

        static std::string creatUrl(const std::string &ip, int port, bool ssl = false);
        static std::string generateUUID();


};

#endif // SUBSCRIBER_H
