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

#include "udpbroadcastclient.h"
#include "udpbroadcastserver.h"
#include "pubsubclient.h"
#include <set>
#include <map>
#include "pubsubbroadcastclient.h"

#include <jsonrpccpp/server/connectors/httpserver.h>
#include <jsonrpccpp/server/iprocedureinvokationhandler.h>
#include <jsonrpccpp/server/requesthandlerfactory.h>
#include <jsonrpccpp/common/procedure.h>
#include <jsonrpccpp/server/iclientconnectionhandler.h>
#include "subscriberlist.h"


class PubSubPeer : public jsonrpc::IProcedureInvokationHandler
{
    public:

        typedef void(PubSubPeer::*methodPointer_t)       (const Json::Value &parameter, Json::Value &result);
        typedef void(PubSubPeer::*notificationPointer_t) (const Json::Value &parameter);

        PubSubPeer(const std::string ip, int port);
        virtual ~PubSubPeer();


        inline virtual void pubsub_subscribeI(const Json::Value &request, Json::Value &response);
        inline virtual void pubsub_unsubscribeI(const Json::Value &request, Json::Value &response);
        inline virtual void pubsub_offerTopicI(const Json::Value &request);
        inline virtual void pubsub_publishinterestI(const Json::Value &request);
        inline virtual void pubsub_publishtopicsI(const Json::Value &request);


        bool Start();
        void Stop();

        virtual void HandleMethodCall(jsonrpc::Procedure& proc, const Json::Value& input, Json::Value& output);
        virtual void HandleNotificationCall(jsonrpc::Procedure& proc, const Json::Value& input);


        void addPublishTopic(const std::string &name);
        void removePublishTopic(const std::string &name);
        bool hasPublishTopic(const std::string &name);

        void addSubscribeTopic(const std::string &name);
        void removeSubscribeTopic(const std::string &name);
        bool hasSubscribeTopic(const std::string &name);

        void publishTopic(const std::string &name, Json::Value &params);

        //UDP Methods
        virtual void pubsub_publishinterest(const std::string& ip, const std::string& topic);
        virtual void pubsub_publishtopics(const std::string& ip, const Json::Value& topics);


        //TCP Methods
        virtual std::string pubsub_subscribe(const std::string& ip, const std::string& notification);
        virtual bool pubsub_unsubscribe(const std::string& notificationId);
        virtual void pubsub_offerTopic(const std::string& ip, const Json::Value& topics);

    protected:
        bool bindAndAddMethod(const jsonrpc::Procedure& proc, methodPointer_t pointer);
        bool bindAndAddNotification(const jsonrpc::Procedure& proc, notificationPointer_t pointer);

    private:
        std::map<std::string, methodPointer_t>          methods;
        std::map<std::string, notificationPointer_t>    notifications;

        jsonrpc::IProtocolHandler* handler;
        int m_port;
        std::string m_ip; //TODO: determine automatically
        UdpBroadcastServer m_bcserver;
        UdpBroadcastClient m_bcclientconnector;
        PubSubBroadcastClient m_bcclient;

        std::set<std::string> m_publishtopics;
        std::set<std::string> m_subscribetopics;

        jsonrpc::HttpServer m_httpserver;

        SubscriberList m_subscribers;
        SubscriberList m_publishers;


         bool symbolExists(const std::string &name);

};

#endif // PUBSUBPEER_H
