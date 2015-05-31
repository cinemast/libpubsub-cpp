/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    subscriber.cpp
 * @date    5/30/2015
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "subscriber.h"
#include <sstream>
#include <uuid/uuid.h>

using namespace std;

Subscriber::Subscriber(const std::string &ip, int port) :
    m_httpclient(creatUrl(ip, port, false)),
    PubSubClient(m_httpclient),
    m_subscriptionId(generateUUID())
{
}

Subscriber::Subscriber(const string &ip, int port, string subscriptionId) :
    m_httpclient(creatUrl(ip, port, false)),
    PubSubClient(m_httpclient),
    m_subscriptionId(subscriptionId)
{
}

string Subscriber::getSubscriptionId()
{
    return m_subscriptionId;
}

void Subscriber::setSubscriptionId(const string &id)
{
    m_subscriptionId = id;
}

std::string Subscriber::creatUrl(const std::string &ip, int port, bool ssl)
{
    stringstream result;
    result << "http";
    if (ssl)
        result << "s";
    result << "://" << ip << ":" << port;
    return result.str();
}

string Subscriber::generateUUID()
{
    uuid_t uuid;
    uuid_generate_time(uuid);
    char buff[40];
    sprintf(buff, "%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x\n", uuid[0],uuid[1],uuid[2],uuid[3],uuid[4],uuid[5],uuid[6],uuid[7],uuid[8],uuid[9],uuid[10],uuid[11],uuid[12],uuid[13],uuid[14],uuid[15]);
    return string(buff);
}
