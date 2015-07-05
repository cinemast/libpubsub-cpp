/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file
 * @date    7/6/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/


#include "TempSensor.h"
#include "ports.h"
#include <iostream>

using namespace std;

int main()
{
    TempSensor sensor(PORT_BROADCAST, PORT_TEMPSENSOR);
    double temp = 12.4;

    if(!sensor.Start())
    {
        cerr << "Could not start peer" << endl;
        return 1;
    }
    sensor.autoPublishAll();
    while(true)
    {
        sensor.publishTempChanged(temp++);
        sleep(1);
    }

    return 0;
}
