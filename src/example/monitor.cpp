/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    humiditysensor.cpp
 * @date    6/30/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <iostream>
#include "AbstractMonitor.h"

using namespace std;

class Monitor : public AbstractMonitor
{

    public:
        Monitor(const std::string &ip, int port) : AbstractMonitor(ip, port)
        {
        }

        virtual void ontempChanged(double temp)
        {
            cout << "Temp changed: " << temp << endl;
        }

        virtual void onhumidityChanged(double humidity)
        {
            cout << "Humidity changed: " << humidity << endl;
        }

        virtual void onairspeedChanged(double speed)
        {
            cout << "Airspeed changed: " << speed << endl;
        }

        virtual void onshutterChanged(bool open)
        {

        }
};

int main()
{
    Monitor m("127.0.0.1", 9999);

    m.addSubscribeTopic(AbstractMonitor::TOPIC_SUBSCRIBE_ONTEMPCHANGED);
    m.addSubscribeTopic(AbstractMonitor::TOPIC_SUBSCRIBE_ONHUMIDITYCHANGED);
    m.addSubscribeTopic(AbstractMonitor::TOPIC_SUBSCRIBE_ONAIRSPEEDCHANGED);
    cout << "Started monitor" << endl;

    m.Start();

    while(true)
        sleep(1);

    return 0;
}
