//
// Created by David Cherry on 25/06/2020.
//

#ifndef TCLIBRARYDEV_NTPTIME_H
#define TCLIBRARYDEV_NTPTIME_H

#include <mbed.h>
#include <ctime>
#include <EthernetInterface.h>

/**
 * this is not yet ready for wider usage.
 */
class NTPTime {
public:
    NTPTime(NetworkInterface* interface, const char* timeServer, int timePort);
    time_t getTimeFromNTP();
private:
    const time_t EPOCH_CONVERT_OFFSET = (time_t)2208988800UL;
    const char* timeServer;
    int timePort;
    NetworkInterface* interface;
};


#endif //TCLIBRARYDEV_NTPTIME_H
