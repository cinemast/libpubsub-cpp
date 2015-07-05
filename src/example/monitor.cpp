/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    humiditysensor.cpp
 * @date    6/30/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "AbstractMonitor.h"
#include <iostream>

using namespace std;

class Monitor : public AbstractMonitor
{
    public:
        Monitor(uint16_t port, uint16_t port2) : AbstractMonitor(port, port2) {}

        virtual void onairspeedChanged(double speeed)
        {
            cout << "AirSpeed: " << speeed << endl;
        }

};

int main (int argc, const char * argv[])
{
    Monitor m(9100, 9102);

    if (!m.Start())
    {
        cerr << "Could not start peer" << endl;
        return 1;
    }

    m.addSubscribeTopic(Monitor::TOPIC_SUBSCRIBE_ONAIRSPEEDCHANGED);

    while(1)
    {
        cout << "In idle mode" << endl;
        sleep(1);
    }

    return 0;
}
