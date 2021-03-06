/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    humiditysensor.cpp
 * @date    6/30/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "AirspeedSensor.h"
#include "ports.h"
#include <iostream>

using namespace std;

int main()
{
    AirspeedSensor sensor(PORT_BROADCAST, PORT_AIRSENSOR);
    int speed = 34;

    if(!sensor.Start())
    {
        cerr << "Could not start peer" << endl;
        return 1;
    }
    sensor.autoPublishAll();
    while(true)
    {
        sensor.publishAirspeedChanged(speed++);
        sleep(1);
    }

    return 0;
}
