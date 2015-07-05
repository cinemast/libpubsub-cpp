/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    humiditysensor.cpp
 * @date    6/30/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "HumiditySensor.h"
#include "ports.h"
#include <iostream>

using namespace std;

int main()
{
    HumiditySensor sensor(PORT_BROADCAST, PORT_HUMIDITYSENSOR);
    double humidity = 0.2;
    if(!sensor.Start())
    {
        cerr << "Could not start peer" << endl;
        return 1;
    }
    sensor.autoPublishAll();
    while(true)
    {
        sensor.publishHumidityChanged(humidity+= 0.001);
        sleep(3);
    }

    return 0;
}
