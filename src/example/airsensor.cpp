/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    humiditysensor.cpp
 * @date    6/30/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "AirspeedSensor.h"
#include <iostream>

using namespace std;

int main()
{
    AirspeedSensor sensor(9100, 9105);
    int speed = 34;

    if(!sensor.Start())
    {
        cerr << "Could not start peer" << endl;
        return 1;
    }

    sensor.addPublishTopic(AirspeedSensor::TOPIC_PUBLISH_AIRSPEEDCHANGED);


    while(true)
    {
        //cout << "Published airspeed" << endl;
        sensor.publishAirspeedChanged(speed++);

        sleep(1);
    }

    return 0;
}
