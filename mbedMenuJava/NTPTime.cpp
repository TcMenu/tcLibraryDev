//
// Created by David Cherry on 25/06/2020.
//

#include "NTPTime.h"

NTPTime::NTPTime(NetworkInterface *interface, const char *timeServer, int timePort) {
    this->timeServer = timeServer;
    this->timePort = timePort;
    this->interface = interface;
}

time_t NTPTime::getTimeFromNTP() {

    SocketAddress serverAddr;
    if(interface->gethostbyname(timeServer, &serverAddr) < 0) {
        return 0;
    }
    serverAddr.set_port(timePort);

    int32_t ntpRx[12] = {0};
    char ntpTx[48] = {0};
    ntpTx[0] = 0x1b;

    UDPSocket socket;
    socket.open(interface);
    socket.set_timeout(10);
    if(socket.sendto(serverAddr, ntpTx, sizeof(ntpTx)) <= 0) return 0;

    SocketAddress sourceAddr;
    socket.set_blocking(true);
    int n = socket.recvfrom(&sourceAddr, ntpRx, sizeof(ntpRx));
    socket.set_blocking(false);

    if(n > 10) {
        uint32_t tmNowRaw = ntpRx[10];
        uint32_t ret = (tmNowRaw & 0xffU) << 24U;
        ret |= (tmNowRaw & 0xff00U) << 8U;
        ret |= (tmNowRaw & 0xff0000UL) >> 8U;
        ret |= (tmNowRaw & 0xff000000UL) >> 24U;
        return ret - EPOCH_CONVERT_OFFSET;
    }
    else {
        return 0;
    }
}
