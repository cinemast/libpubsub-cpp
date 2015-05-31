TempSensor
    publishes:
        tempChanged(int temp)
    
HumidtySensor
    publishes:
        humidityChanged(double humidity)

AirspeedSensor
    publishes: 
        windSpeedChanged(double speed)

Shutter
    publishes:
        shutterChanged(bool open)
    subscribes:
        windSpeedChanged(double speed)

Panel
    subscribes:
        tempChanged
        humidityChanged
        shutterChanged

-------------------------------------------------

HumidityPeer:
    start()
        BC topics
    stop()
        do nothing

    subscribe(topic)

    list of subscribers
        call tempChanged

    

ShutterPeer
    
