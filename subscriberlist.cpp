/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    subscriberlist.cpp
 * @date    5/31/2015
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "subscriberlist.h"

SubscriberList::SubscriberList()
{
}

bool SubscriberList::addSubscriber(Subscriber *subscriber)
{
    if (hasSubscriber(subscriber->ip(), subscriber->topic()))
        return false;
    m_subscribers[subscriber->getSubscriptionId()] = subscriber;
    return true;
}

Subscriber *SubscriberList::getSubscriber(const std::string &ip, const std::string &topic)
{
    for (auto &subscriber : m_subscribers)
    {
        if (subscriber.second->ip() == ip && subscriber.second->topic() == topic)
            return subscriber.second;
    }
    return NULL;
}

bool SubscriberList::removeSubscriber(const std::string &subscriptionId)
{
    auto iterator = m_subscribers.find(subscriptionId);

    if (iterator != m_subscribers.end())
    {
        delete m_subscribers[subscriptionId];
    }
    return (m_subscribers.erase(subscriptionId) > 0);
}

bool SubscriberList::hasSubscriber(const std::string &ip, const std::string &topic)
{
    if (getSubscriber(ip, topic) != NULL)
        return true;
    return false;
}
