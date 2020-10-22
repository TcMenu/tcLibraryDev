//
// Created by David Cherry on 25/06/2020.
//

#ifndef TCLIBRARYDEV_NTPTIMEEVENT_H
#define TCLIBRARYDEV_NTPTIMEEVENT_H

#include <mbed.h>
#include <ctime>
#include <EthernetInterface.h>
#include <TaskManagerIO.h>

/**
 * this is not yet ready for wider usage.
 */
class NTPTimeEvent : public BaseEvent {
public:
    NTPTimeEvent(NetworkInterface* interface, const char* timeServer, int timePort);

    uint32_t timeOfNextCheck() override;

private:
    const time_t EPOCH_CONVERT_OFFSET = (time_t)2208988800UL;
    const char* const  timeServer;
    const int volatile timePort;
    NetworkInterface* const interface;
    Thread ntpThread;

    void acquireNtpOnThread();
    friend void acquireNtpTimeThreadProc(NTPTimeEvent *ntpTime);
protected:
    volatile time_t _presentValue;
};


#endif //TCLIBRARYDEV_NTPTIMEEVENT_H
