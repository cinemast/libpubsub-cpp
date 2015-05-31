/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    pubsubpeer.h
 * @date    5/30/2015
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef PUBSUBPEER_H
#define PUBSUBPEER_H

#include "udpbroadcastserver.h"
#include "udpbroadcastclient.h"
#include "pubsubclient.h"
#include <set>
#include <map>
#include "pubsubbroadcastclient.h"
#include "pubsubbroadcastserver.h"
#include "pubsubserver.h"
#include "pubsubclient.h"

#include <jsonrpccpp/server/connectors/httpserver.h>
#include "subscriberlist.h"

class PubSubPeer : private PubSubBroadcastServer, private PubSubServer
{
    public:
        PubSubPeer(int port);

        void addPublishTopic(const std::string &name);
        bool hasPublishTopic(const std::string &name);

        void addSubscribeTopic(const std::string &name);
        bool hasSubscribeTopic(const std::string &name);


        //UDP Methods
        virtual void pubsub_publishinterest(const std::string& ip, const std::string& topic);
        virtual void pubsub_publishtopics(const std::string& ip, const Json::Value& topics);


        //TCP Methods
        virtual std::string pubsub_subscribe(const std::string& ip, const std::string& notification);
        virtual bool pubsub_unsubscribe(const std::string& notificationId);
        virtual void pubsub_offerTopic(const std::string& ip, const Json::Value& topics);

    private:
        int m_port;
        std::string m_ip; //TODO: determine automatically
        UdpBroadcastServer m_bcserver;
        UdpBroadcastClient m_bcclient;
        std::set<std::string> m_publishtopics;
        std::set<std::string> m_subscribetopics;

        jsonrpc::HttpServer m_httpserver;

        SubscriberList m_subscribers;
        SubscriberList m_publishers;

};

#endif // PUBSUBPEER_H
