/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    subscriberlist.h
 * @date    5/31/2015
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef SUBSCRIBERLIST_H
#define SUBSCRIBERLIST_H

#include <map>
#include "subscriber.h"

class SubscriberList
{
    public:
        SubscriberList();

        /**
         * @brief addSubscriber
         * @param subscriber
         * @return true if not existed yet, false otherwise
         */
        bool addSubscriber(Subscriber* subscriber);

        Subscriber* getSubscriber(const std::string &ip, const std::string &topic);

        bool removeSubscriber(const std::string &subscriptionId);

        bool hasSubscriber(const std::string &ip, const std::string &topic);

    private:
        std::map<std::string, Subscriber*> m_subscribers;
};

#endif // SUBSCRIBERLIST_H
