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

AbstractPubSubPeer::AbstractPubSubPeer(const string ip, int port) :
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

    this->bindAndAddMethod(jsonrpc::Procedure("pubsub.subscribe", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_STRING, "ip",jsonrpc::JSON_STRING,"notification",jsonrpc::JSON_STRING, NULL), &AbstractPubSubPeer::pubsub_subscribeI);
    this->bindAndAddMethod(jsonrpc::Procedure("pubsub.unsubscribe", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_BOOLEAN, "notificationId",jsonrpc::JSON_STRING, NULL), &AbstractPubSubPeer::pubsub_unsubscribeI);
    this->bindAndAddNotification(jsonrpc::Procedure("pubsub.offerTopic", jsonrpc::PARAMS_BY_NAME, "ip",jsonrpc::JSON_STRING,"topics",jsonrpc::JSON_ARRAY, NULL), &AbstractPubSubPeer::pubsub_offerTopicI);

    this->bindAndAddNotification(jsonrpc::Procedure("pubsub.publishinterest", jsonrpc::PARAMS_BY_NAME, "ip",jsonrpc::JSON_STRING,"topic",jsonrpc::JSON_STRING, NULL), &AbstractPubSubPeer::pubsub_publishinterestI);
    this->bindAndAddNotification(jsonrpc::Procedure("pubsub.publishtopics", jsonrpc::PARAMS_BY_NAME, "ip",jsonrpc::JSON_STRING,"topics",jsonrpc::JSON_ARRAY, NULL), &AbstractPubSubPeer::pubsub_publishtopicsI);
}

AbstractPubSubPeer::~AbstractPubSubPeer()
{
    m_bcserver.StopListening();
    m_httpserver.StopListening();
}

bool AbstractPubSubPeer::Start()
{
    return m_bcserver.StartListening() && m_httpserver.StartListening();
}

void AbstractPubSubPeer::Stop()
{
    m_bcserver.StopListening();
    m_httpserver.StopListening();
}

void AbstractPubSubPeer::addPublishTopic(const std::string &name)
{
    m_publishtopics.insert(name);
    Json::Value topics;
    topics.append(name);
    m_bcclient.pubsub_publishtopics(m_ip, topics);
}

void AbstractPubSubPeer::removePublishTopic(const string &name)
{
    m_publishtopics.erase(name);
}

bool AbstractPubSubPeer::hasPublishTopic(const string &name)
{
    if(m_publishtopics.find(name) != m_publishtopics.end())
        return true;
    return false;
}

void AbstractPubSubPeer::addSubscribeTopic(const string &name)
{
    m_subscribetopics.insert(name);
    m_bcclient.pubsub_publishinterest(m_ip, name);
}

void AbstractPubSubPeer::removeSubscribeTopic(const string &name)
{
    m_subscribetopics.erase(name);
    //Unsubscribe from publishers
    vector<Subscriber*> publishers = m_publishers.getSubscriberByTopic(name);
    for (auto publisher : publishers)
    {
        publisher->pubsub_unsubscribe(publisher->getSubscriptionId());
    }
}

bool AbstractPubSubPeer::hasSubscribeTopic(const string &name)
{
    if(m_subscribetopics.find(name) != m_subscribetopics.end())
        return true;
    return false;
}

void AbstractPubSubPeer::publishTopic(const string &name, Json::Value &params)
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
void AbstractPubSubPeer::pubsub_publishinterest(const std::string &ip, const std::string &topic)
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
void AbstractPubSubPeer::pubsub_publishtopics(const std::string &ip, const Json::Value &topics)
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

string AbstractPubSubPeer::pubsub_subscribe(const string &ip, const string &notification)
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

bool AbstractPubSubPeer::pubsub_unsubscribe(const string &notificationId)
{
    return m_subscribers.removeSubscriber(notificationId);
}

void AbstractPubSubPeer::pubsub_offerTopic(const string &ip, const Json::Value &topics)
{
    this->pubsub_publishtopics(ip, topics);
}

bool AbstractPubSubPeer::bindAndAddMethod(const Procedure &proc, AbstractPubSubPeer::methodPointer_t pointer)
{
    if(proc.GetProcedureType() == RPC_METHOD && !this->symbolExists(proc.GetProcedureName()))
    {
        this->handler->AddProcedure(proc);
        this->methods[proc.GetProcedureName()] = pointer;
        return true;
    }
    return false;
}

bool AbstractPubSubPeer::bindAndAddNotification(const Procedure &proc, AbstractPubSubPeer::notificationPointer_t pointer)
{
    if(proc.GetProcedureType() == RPC_NOTIFICATION && !this->symbolExists(proc.GetProcedureName()))
    {
        this->handler->AddProcedure(proc);
        this->notifications[proc.GetProcedureName()] = pointer;
        return true;
    }
    return false;
}

bool AbstractPubSubPeer::symbolExists(const string &name)
{
    if (methods.find(name) != methods.end())
        return true;
    if (notifications.find(name) != notifications.end())
        return true;
    return false;
}


void AbstractPubSubPeer::HandleMethodCall(Procedure &proc, const Json::Value &input, Json::Value &output)
{
    (this->*methods[proc.GetProcedureName()])(input, output);
}

void AbstractPubSubPeer::HandleNotificationCall(Procedure &proc, const Json::Value &input)
{
    (this->*notifications[proc.GetProcedureName()])(input);
}
