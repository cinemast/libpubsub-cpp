/**
 * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!
 */

#ifndef JSONRPC_CPP_STUB_PUBSUBBROADCASTCLIENT_H_
#define JSONRPC_CPP_STUB_PUBSUBBROADCASTCLIENT_H_

#include <jsonrpccpp/client.h>

class PubSubBroadcastClient : public jsonrpc::Client
{
    public:
        PubSubBroadcastClient(jsonrpc::IClientConnector &conn, jsonrpc::clientVersion_t type = jsonrpc::JSONRPC_CLIENT_V2) : jsonrpc::Client(conn, type) {}

        void pubsub_publishinterest(const std::string& ip, const std::string& topic) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["ip"] = ip;
            p["topic"] = topic;
            this->CallNotification("pubsub.publishinterest",p);
        }
        void pubsub_publishtopics(const std::string& ip, const Json::Value& topics) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["ip"] = ip;
            p["topics"] = topics;
            this->CallNotification("pubsub.publishtopics",p);
        }
};

#endif //JSONRPC_CPP_STUB_PUBSUBBROADCASTCLIENT_H_