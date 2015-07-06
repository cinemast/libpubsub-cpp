/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    humiditysensor.cpp
 * @date    6/30/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "AbstractMonitor.h"
#include "ports.h"
#include <iostream>

using namespace std;

class Monitor : public AbstractMonitor
{
    public:
        Monitor(uint16_t port, uint16_t port2) : AbstractMonitor(port, port2) {}

        virtual void airspeedChanged(double speeed)
        {
            cout << "AirSpeed: " << speeed << " km/h" <<  endl;
        }

        virtual void humidityChanged(double humidity)
        {
            cout << "Humidity: " << humidity << " %" << endl;
        }

        virtual void tempChanged(double temp)
        {
            cout << "Temp: " << temp << " °C" << endl;
        }

        virtual void shutterChanged(bool open)
        {
            cout << "Shutter changed: " << open << endl;
        }
};

int main (int argc, const char * argv[])
{
    Monitor m(PORT_BROADCAST, PORT_MONITOR);

    if (!m.Start())
    {
        cerr << "Could not start peer" << endl;
        return 1;
    }

    //m.addSubscribeTopic();
    m.autoSubscribeAll();

    while(1)
    {
        sleep(1);
    }

    return 0;
}
