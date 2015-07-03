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
    AirspeedSensor sensor("192.168.13.78", 9999);
    sensor.addPublishTopic(AirspeedSensor::TOPIC_PUBLISH_AIRSPEEDCHANGED);
    int speed = 34;

    sensor.Start();
    while(true)
    {
        cout << "Published airspeed" << endl;
        sensor.publishAirspeedChanged(speed++);

        sleep(1);
    }

    return 0;
}
