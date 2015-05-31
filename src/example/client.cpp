#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAXBUF 65536

#include <pubsubcpp/pubsubpeer.h>

#define HUMIDITY_PUBLISH_TEMPCHANGED "tempChanged"

class HumiditySensorPeer : public PubSubPeer
{
    public:
        HumiditySensorPeer(const std::string &ip, int port) : PubSubPeer(ip, port)
        {

        }


        void publishTempratureChanged(double temp)
        {
            Json::Value params;
            params["temp"] = temp;
            this->publishTopic(HUMIDITY_PUBLISH_TEMPCHANGED, params);
        }
};


int main(int argc, char*argv[])
{
    HumiditySensorPeer peer("192.168.13.78", 55555);

    peer.Start();

    peer.addPublishTopic(HUMIDITY_PUBLISH_TEMPCHANGED);

    peer.Stop();

}
