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

#include "connector/udpbroadcastclient.h"
#include "connector/udpbroadcastserver.h"
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


template<class S>
class PubSubPeer : public jsonrpc::IProcedureInvokationHandler
{
    public:

        typedef void(S::*methodPointer_t)       (const Json::Value &parameter, Json::Value &result);
        typedef void(S::*notificationPointer_t) (const Json::Value &parameter);


        PubSubPeer(const std::string ip, int port) :
            m_port(port),
            m_bcserver(port, ip),
            m_bcclientconnector(port),
            m_bcclient(m_bcclientconnector),
            m_httpserver(port),
            m_ip(ip),
            handler(jsonrpc::RequestHandlerFactory::createProtocolHandler(jsonrpc::JSONRPC_SERVER_V2, *this))
        {
            m_bcserver.SetHandler(this->handler);
            m_httpserver.SetHandler(this->handler);

            this->bindAndAddMethod(jsonrpc::Procedure("pubsub.subscribe", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_STRING, "ip",jsonrpc::JSON_STRING,"notification",jsonrpc::JSON_STRING, NULL), &PubSubPeer::pubsub_subscribeI);
            this->bindAndAddMethod(jsonrpc::Procedure("pubsub.unsubscribe", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_BOOLEAN, "notificationId",jsonrpc::JSON_STRING, NULL), &PubSubPeer::pubsub_unsubscribeI);
            this->bindAndAddNotification(jsonrpc::Procedure("pubsub.offerTopic", jsonrpc::PARAMS_BY_NAME, "ip",jsonrpc::JSON_STRING,"topics",jsonrpc::JSON_ARRAY, NULL), &PubSubPeer::pubsub_offerTopicI);

            this->bindAndAddNotification(jsonrpc::Procedure("pubsub.publishinterest", jsonrpc::PARAMS_BY_NAME, "ip",jsonrpc::JSON_STRING,"topic",jsonrpc::JSON_STRING, NULL), &PubSubPeer::pubsub_publishinterestI);
            this->bindAndAddNotification(jsonrpc::Procedure("pubsub.publishtopics", jsonrpc::PARAMS_BY_NAME, "ip",jsonrpc::JSON_STRING,"topics",jsonrpc::JSON_ARRAY, NULL), &PubSubPeer::pubsub_publishtopicsI);
        }

        virtual ~PubSubPeer()
        {
            m_bcserver.StopListening();
            m_httpserver.StopListening();
        }

    private:

        void pubsub_subscribeI(const Json::Value &request, Json::Value &response)
        {
            response = this->pubsub_subscribe(request["ip"].asString(), request["notification"].asString());
        }

        void pubsub_unsubscribeI(const Json::Value &request, Json::Value &response)
        {
            response = this->pubsub_unsubscribe(request["notificationId"].asString());
        }

        void pubsub_offerTopicI(const Json::Value &request)
        {
            this->pubsub_offerTopic(request["ip"].asString(), request["topics"]);
        }

        void pubsub_publishinterestI(const Json::Value &request)
        {
            this->pubsub_publishinterest(request["ip"].asString(), request["topic"].asString());
        }

        void pubsub_publishtopicsI(const Json::Value &request)
        {
            this->pubsub_publishtopics(request["ip"].asString(), request["topics"]);
        }

    public:
        bool Start()
        {
            return m_bcserver.StartListening() && m_httpserver.StartListening();
        }

        void Stop()
        {
            //Unsubscribe from all publishers
            for (auto subscription : m_subscribetopics)
            {
                removeSubscribeTopic(subscription);
            }

            m_bcserver.StopListening();
            m_httpserver.StopListening();
        }

        void addPublishTopic(const std::string &name)
        {
            m_publishtopics.insert(name);
            Json::Value topics;
            topics.append(name);
            m_bcclient.pubsub_publishtopics(m_ip, topics);
        }

        void removePublishTopic(const std::string &name)
        {
            m_publishtopics.erase(name);
        }

        bool hasPublishTopic(const std::string &name)
        {
            if(m_publishtopics.find(name) != m_publishtopics.end())
                return true;
            return false;
        }

        void addSubscribeTopic(const std::string &name)
        {
            m_subscribetopics.insert(name);
            m_bcclient.pubsub_publishinterest(m_ip, name);
        }

        void removeSubscribeTopic(const std::string &name)
        {
            m_subscribetopics.erase(name);
            //Unsubscribe from publishers
            std::vector<Subscriber*> publishers = m_publishers.getSubscriberByTopic(name);
            for (auto publisher : publishers)
            {
                publisher->pubsub_unsubscribe(publisher->getSubscriptionId());
                m_publishers.removeSubscriber(publisher->getSubscriptionId());
            }

        }

        bool hasSubscribeTopic(const std::string &name)
        {
            if(m_subscribetopics.find(name) != m_subscribetopics.end())
                return true;
            return false;
        }

    protected:

        void publishTopic(const std::string &name, Json::Value &params)
        {
            std::vector<Subscriber*> subscribers = m_subscribers.getSubscriberByTopic(name);
            for (auto subscriber : subscribers)
            {
                try
                {
                    subscriber->CallNotification(name, params);
                }
                catch(jsonrpc::JsonRpcException& e)
                {
                    //if subscribers throws error, remove it from subscriptions
                    m_subscribers.removeSubscriber(subscriber->getSubscriptionId());
                }
            }
        }

    private:
        //On subscriber broadcasts interest
        void pubsub_publishinterest(const std::string &ip, const std::string &topic)
        {
            if (hasPublishTopic(topic))
            {
                //offerTopic
                Subscriber sub(ip, m_port);
                Json::Value topics;
                for (auto &topic : m_publishtopics)
                    topics.append(topic);
                sub.pubsub_offerTopic(m_ip, topics);
            }
        }

        //on publisher broadcass topics
        void pubsub_publishtopics(const std::string &ip, const Json::Value &topics)
        {
            for (unsigned int i=0; i < topics.size(); i++)
            {
                if(hasSubscribeTopic(topics[i].asString()))
                {
                    Subscriber* sub = new Subscriber(ip,m_port);
                    std::string id = sub->pubsub_subscribe(m_ip, topics[i].asString());
                    if (id != "")
                    {
                        sub->setSubscriptionId(id);
                        m_publishers.addSubscriber(sub);
                    }
                    else
                    {
                        delete sub;
                    }
                }
            }
        }

        std::string pubsub_subscribe(const std::string &ip, const std::string &notification)
        {
            Subscriber* sub = m_subscribers.getSubscriber(ip, notification);
            if (sub == NULL)
            {
                sub = new Subscriber(ip, m_port);
                sub->setTopic(notification);
                m_subscribers.addSubscriber(sub);
            }
            return sub->getSubscriptionId();
        }

        bool pubsub_unsubscribe(const std::string &notificationId)
        {
            return m_subscribers.removeSubscriber(notificationId);
        }

        void pubsub_offerTopic(const std::string &ip, const Json::Value &topics)
        {
            this->pubsub_publishtopics(ip, topics);
        }

    protected:

        bool bindAndAddMethod(const jsonrpc::Procedure &proc, PubSubPeer::methodPointer_t pointer)
        {
            if(proc.GetProcedureType() == jsonrpc::RPC_METHOD && !this->symbolExists(proc.GetProcedureName()))
            {
                this->handler->AddProcedure(proc);
                this->methods[proc.GetProcedureName()] = pointer;
                return true;
            }
            return false;
        }

        bool bindAndAddNotification(const jsonrpc::Procedure &proc, PubSubPeer::notificationPointer_t pointer)
        {
            if(proc.GetProcedureType() == jsonrpc::RPC_NOTIFICATION && !this->symbolExists(proc.GetProcedureName()))
            {
                this->handler->AddProcedure(proc);
                this->notifications[proc.GetProcedureName()] = pointer;
                return true;
            }
            return false;
        }

    private:

        bool symbolExists(const std::string &name)
        {
            if (methods.find(name) != methods.end())
                return true;
            if (notifications.find(name) != notifications.end())
                return true;
            return false;
        }


        void HandleMethodCall(jsonrpc::Procedure &proc, const Json::Value &input, Json::Value &output)
        {
             S* instance = dynamic_cast<S*>(this);
            (instance->*methods[proc.GetProcedureName()])(input, output);
        }

        void HandleNotificationCall(jsonrpc::Procedure &proc, const Json::Value &input)
        {
            S* instance = dynamic_cast<S*>(this);
            (instance->*notifications[proc.GetProcedureName()])(input);
        }


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

};

#endif // PUBSUBPEER_H
