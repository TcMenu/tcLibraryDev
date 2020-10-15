//
// Created by David Cherry on 25/06/2020.
//

#ifndef TCLIBRARYDEV_NTPTIME_H
#define TCLIBRARYDEV_NTPTIME_H

#include <mbed.h>
#include <ctime>
#include <EthernetInterface.h>
#include <TaskManagerIO.h>

/**
 * A very simple mbed NTP time event that is suitable for use with TaskManagerIO. It asynchronously
 * looks up a timeServer, then requests the time and processes the response once it's completed. It
 * is implemented as an event for TaskManagerIO and when the event triggers, the time has been set
 * in the "theTime" variable. To use this event, extend from NTPTimeBaseEvent and override "exec()"
 */
class NTPTimeBaseEvent :public BaseEvent {
public:
    /**
     * Constructs a time event given the network interface, the server address and port.
     * @param interface the network interface to use
     * @param timeServer the time server to connect to.
     * @param timePort the time port to use
     */
    NTPTimeBaseEvent(NetworkInterface* interface, const char* timeServer, int timePort);
    ~NTPTimeBaseEvent() override;

    /**
     * This is the event polling implementation, it takes care of all the processing that is needed
     * in order to connect and request the time. Generally you should not alter this behaviour.
     * @return different times depending on status.
     */
    uint32_t timeOfNextCheck() override;

    /**
     * Tells the event to restart from the beginning to start NTP processing again.
     */
    void restartTimeProcessing() { state = AWAITING_LOOKUP; }
private:
    enum NTPState { AWAITING_LOOKUP, AWAITING_SEND, AWAITING_RECEIVE, TRANSMISSION_COMPLETED };

    const time_t EPOCH_CONVERT_OFFSET = (time_t)2208988800UL;
    const char* timeServer;
    SocketAddress serverAddress;
    UDPSocket socket;
    int timePort;
    NetworkInterface* interface;
    NTPState state;
protected:
    time_t theTime;

    void doLookup();

    void attemptSend();

    void attemptReceive();
};


#endif //TCLIBRARYDEV_NTPTIME_H
