/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    AbstractPubSubPeer.cpp
 * @date    5/30/2015
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "pubsubpeer.h"
#include "subscriber.h"
#include <jsonrpccpp/server/requesthandlerfactory.h>

using namespace std;
using namespace jsonrpc;

PubSubPeer::PubSubPeer(const string ip, int port) :
    m_port(port),
    m_bcserver(port),
    m_bcclientconnector(port),
    m_bcclient(m_bcclientconnector),
    m_httpserver(port),
    m_ip(ip),
    handler(RequestHandlerFactory::createProtocolHandler(JSONRPC_SERVER_V2, *this))
{
    m_bcserver.SetHandler(this->handler);
    m_httpserver.SetHandler(this->handler);

    this->bindAndAddMethod(jsonrpc::Procedure("pubsub.subscribe", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_STRING, "ip",jsonrpc::JSON_STRING,"notification",jsonrpc::JSON_STRING, NULL), &PubSubPeer::pubsub_subscribeI);
    this->bindAndAddMethod(jsonrpc::Procedure("pubsub.unsubscribe", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_BOOLEAN, "notificationId",jsonrpc::JSON_STRING, NULL), &PubSubPeer::pubsub_unsubscribeI);
    this->bindAndAddNotification(jsonrpc::Procedure("pubsub.offerTopic", jsonrpc::PARAMS_BY_NAME, "ip",jsonrpc::JSON_STRING,"topics",jsonrpc::JSON_ARRAY, NULL), &PubSubPeer::pubsub_offerTopicI);

    this->bindAndAddNotification(jsonrpc::Procedure("pubsub.publishinterest", jsonrpc::PARAMS_BY_NAME, "ip",jsonrpc::JSON_STRING,"topic",jsonrpc::JSON_STRING, NULL), &PubSubPeer::pubsub_publishinterestI);
    this->bindAndAddNotification(jsonrpc::Procedure("pubsub.publishtopics", jsonrpc::PARAMS_BY_NAME, "ip",jsonrpc::JSON_STRING,"topics",jsonrpc::JSON_ARRAY, NULL), &PubSubPeer::pubsub_publishtopicsI);
}

PubSubPeer::~PubSubPeer()
{
    m_bcserver.StopListening();
    m_httpserver.StopListening();
}

void PubSubPeer::pubsub_subscribeI(const Json::Value &request, Json::Value &response)
{
    response = this->pubsub_subscribe(request["ip"].asString(), request["notification"].asString());
}

void PubSubPeer::pubsub_unsubscribeI(const Json::Value &request, Json::Value &response)
{
    response = this->pubsub_unsubscribe(request["notificationId"].asString());
}

void PubSubPeer::pubsub_offerTopicI(const Json::Value &request)
{
    this->pubsub_offerTopic(request["ip"].asString(), request["topics"]);
}

void PubSubPeer::pubsub_publishinterestI(const Json::Value &request)
{
    this->pubsub_publishinterest(request["ip"].asString(), request["topic"].asString());
}

void PubSubPeer::pubsub_publishtopicsI(const Json::Value &request)
{
    this->pubsub_publishtopics(request["ip"].asString(), request["topics"]);
}

bool PubSubPeer::Start()
{
    return m_bcserver.StartListening() && m_httpserver.StartListening();
}

void PubSubPeer::Stop()
{
    m_bcserver.StopListening();
    m_httpserver.StopListening();
}

void PubSubPeer::addPublishTopic(const std::string &name)
{
    m_publishtopics.insert(name);
    Json::Value topics;
    topics.append(name);
    m_bcclient.pubsub_publishtopics(m_ip, topics);
}

void PubSubPeer::removePublishTopic(const string &name)
{
    m_publishtopics.erase(name);
}

bool PubSubPeer::hasPublishTopic(const string &name)
{
    if(m_publishtopics.find(name) != m_publishtopics.end())
        return true;
    return false;
}

void PubSubPeer::addSubscribeTopic(const string &name)
{
    m_subscribetopics.insert(name);
    m_bcclient.pubsub_publishinterest(m_ip, name);
}

void PubSubPeer::removeSubscribeTopic(const string &name)
{
    m_subscribetopics.erase(name);
    //Unsubscribe from publishers
    vector<Subscriber*> publishers = m_publishers.getSubscriberByTopic(name);
    for (auto publisher : publishers)
    {
        publisher->pubsub_unsubscribe(publisher->getSubscriptionId());
        m_publishers.removeSubscriber(publisher->getSubscriptionId());
    }

}

bool PubSubPeer::hasSubscribeTopic(const string &name)
{
    if(m_subscribetopics.find(name) != m_subscribetopics.end())
        return true;
    return false;
}

void PubSubPeer::publishTopic(const string &name, Json::Value &params)
{
    vector<Subscriber*> subscribers = m_subscribers.getSubscriberByTopic(name);
    for (auto subscriber : subscribers)
    {
        try
        {
            subscriber->CallNotification(name, params);
        }
        catch(JsonRpcException& e)
        {
            //if subscribers throws error, remove it from subscriptions
            m_subscribers.removeSubscriber(subscriber->getSubscriptionId());
        }
    }
}

//On subscriber broadcasts interest
void PubSubPeer::pubsub_publishinterest(const std::string &ip, const std::string &topic)
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
void PubSubPeer::pubsub_publishtopics(const std::string &ip, const Json::Value &topics)
{
    for (unsigned int i=0; i < topics.size(); i++)
    {
        if(hasSubscribeTopic(topics[i].asString()))
        {
            Subscriber* sub = new Subscriber(ip,m_port);
            string id = sub->pubsub_subscribe(m_ip, topics[i].asString());
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

string PubSubPeer::pubsub_subscribe(const string &ip, const string &notification)
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

bool PubSubPeer::pubsub_unsubscribe(const string &notificationId)
{
    return m_subscribers.removeSubscriber(notificationId);
}

void PubSubPeer::pubsub_offerTopic(const string &ip, const Json::Value &topics)
{
    this->pubsub_publishtopics(ip, topics);
}

bool PubSubPeer::bindAndAddMethod(const Procedure &proc, PubSubPeer::methodPointer_t pointer)
{
    if(proc.GetProcedureType() == RPC_METHOD && !this->symbolExists(proc.GetProcedureName()))
    {
        this->handler->AddProcedure(proc);
        this->methods[proc.GetProcedureName()] = pointer;
        return true;
    }
    return false;
}

bool PubSubPeer::bindAndAddNotification(const Procedure &proc, PubSubPeer::notificationPointer_t pointer)
{
    if(proc.GetProcedureType() == RPC_NOTIFICATION && !this->symbolExists(proc.GetProcedureName()))
    {
        this->handler->AddProcedure(proc);
        this->notifications[proc.GetProcedureName()] = pointer;
        return true;
    }
    return false;
}

bool PubSubPeer::symbolExists(const string &name)
{
    if (methods.find(name) != methods.end())
        return true;
    if (notifications.find(name) != notifications.end())
        return true;
    return false;
}


void PubSubPeer::HandleMethodCall(Procedure &proc, const Json::Value &input, Json::Value &output)
{
    (this->*methods[proc.GetProcedureName()])(input, output);
}

void PubSubPeer::HandleNotificationCall(Procedure &proc, const Json::Value &input)
{
    (this->*notifications[proc.GetProcedureName()])(input);
}
