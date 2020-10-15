//
// Created by David Cherry on 25/06/2020.
//

#include "mbed.h"
#include "NTPTime.h"
#include <IoLogging.h>

NTPTimeBaseEvent::NTPTimeBaseEvent(NetworkInterface *interface, const char *timeServer, int timePort) {
    this->timeServer = timeServer;
    this->timePort = timePort;
    this->interface = interface;
    this->theTime = 0;
    this->state = AWAITING_LOOKUP;
}

uint32_t NTPTimeBaseEvent::timeOfNextCheck() {
    switch(state) {
        case AWAITING_LOOKUP:
            doLookup();
            return (state == AWAITING_LOOKUP) ? secondsToMicros(1) : millisToMicros(25);
        case AWAITING_SEND:
            attemptSend();
            break;
        case AWAITING_RECEIVE:
            attemptReceive();
            break;
        case TRANSMISSION_COMPLETED:
            // if everything is completed, and the plan is to re-use this event, the just tick over every so often
            // until it's woken up by resetting it's state.
            return secondsToMicros(60);
        default:
            // in this case an unexpected state has occurred, just stop trying.
            setCompleted(true);
            return millisToMicros(100);
    }
    return millisToMicros(50);
}

NTPTimeBaseEvent::~NTPTimeBaseEvent() {
    socket.close();
}

void NTPTimeBaseEvent::doLookup() {
    if(interface->gethostbyname(timeServer, &serverAddress) < 0) {
        return;
    }
    serverAddress.set_port(timePort);
    socket.open(interface);
    socket.set_timeout(10);

    socket.set_blocking(false);

    serdebugF2("Socket address found for ", timeServer);

    state = AWAITING_SEND;
    attemptSend();
}

void NTPTimeBaseEvent::attemptSend() {
    SocketAddress sourceAddr;
    char ntpTx[48] = {0};
    ntpTx[0] = 0x1b;
    auto sentResult = socket.sendto(serverAddress, ntpTx, sizeof(ntpTx));
    if(sentResult == NSAPI_ERROR_WOULD_BLOCK) return;
    if(sentResult <= 0) {
        serdebugF2("NTP: failed on sendto ", sentResult);
        socket.close();
        state = AWAITING_LOOKUP;
    }
    serdebugF2("NTP: send completed, start RX", sentResult);
    state = AWAITING_RECEIVE;
}

void NTPTimeBaseEvent::attemptReceive() {
    SocketAddress sourceAddr;
    int32_t ntpRx[12] = {0};
    int n = socket.recvfrom(&sourceAddr, ntpRx, sizeof(ntpRx));

    if(n == NSAPI_ERROR_WOULD_BLOCK) return;
    if(n > 10) {
        uint32_t tmNowRaw = ntpRx[10];
        uint32_t ret = (tmNowRaw & 0xffU) << 24U;
        ret |= (tmNowRaw & 0xff00U) << 8U;
        ret |= (tmNowRaw & 0xff0000UL) >> 8U;
        ret |= (tmNowRaw & 0xff000000UL) >> 24U;
        theTime = ret - EPOCH_CONVERT_OFFSET;
        state = TRANSMISSION_COMPLETED;
        serdebugF2("Time located as ", (int)theTime);
        setTriggered(true);
    }
    else {
        serdebugF2("Error time response", n);
        socket.close();
        state = AWAITING_LOOKUP;
    }
}
