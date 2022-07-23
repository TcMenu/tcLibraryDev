//
// Created by dave on 18/06/2022.
//

#include "tcMenuLwipWebServer.h"

using namespace tcremote;

bool tcLwipHasBeenStarted = false;

bool tcremote::areWeConnected() {
    return tcLwipHasBeenStarted;
}

void TcMenuWebSockTransport::close() {

}

int TcMenuWebSockTransport::performRawRead(uint8_t *buffer, size_t bufferSize) {
    if(clientFd == -1) return -1;
    timeval timeout = {0};
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(clientFd, &readSet);
    int ret = lwip_select(1, &readSet, nullptr, nullptr, &timeout);
    if(ret < 0) {
        serdebugF2("Socket error ", clientFd);
        return -1;
    } else if(ret == 0) {
        return 0;
    } else {
        return lwip_read(clientFd, buffer, bufferSize);
    }
}

int TcMenuWebSockTransport::performRawWrite(const uint8_t *data, size_t dataSize) {
    if(clientFd == -1) return -1;

    size_t bytesToWrite = dataSize;
    while(bytesToWrite > 0 && loopCount < 100) {
        timeval timeout = {0};
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(clientFd, &readSet);
        int ret = lwip_select(1, &readSet, nullptr, nullptr, &timeout);
        if (ret < 0) {
            serdebugF2("Socket error ", clientFd);
            return -1;
        } else if (ret > 0) {
            return lwip_write(clientFd, data, dataSize);
        }
        // wait 1 milli before trying again, no point sooner as network stack is full and needs draining
        taskManager.yieldForMicros(1000);
    }
}

WebServerResponse *TcMenuWebServer::attemptNewConnection() {
    WebServerResponse* resp = getNextAvailableHttpHandler();
    if(resp == nullptr) return nullptr;

    fd_set set;
    timeval timeout = { 0, 0 };
    int rv;
    FD_ZERO(&set); /* clear the set */
    FD_SET(acceptSocketFd, &set);

    int selRes = lwip_select(1, &set, nullptr, nullptr, &timeout);
    if(selRes == -1) {
        serdebugF("Accept select failed");
        return nullptr;
    }

    if(selRes > 0) {
        sockaddr theirAddress;
        socklen_t theirAddrLen = sizeof(theirAddress);
        socket_t client_socket_fd = lwip_accept(acceptSocketFd, (struct sockaddr *) &theirAddress, &theirAddrLen);

        char str[20];
        lwip_inet_ntop(AF_INET, &(theirAddress.sa_data), str, sizeof str);
        str[sizeof(str) - 1] = 0;
        serdebugF2("Client found at ", str);

        reinterpret_cast<TcMenuWebSockTransport*>(resp->getTransport())->setClient(client_socket_fd);
        return resp;
    } else return nullptr;
}

TcMenuWebServer::TcMenuWebServer(int numConcurrent, int port) : TcMenuLightweightWebServer(numConcurrent),
                                                                serverPort(port), acceptSocketFd(-1), timeStart(0) {}

void TcMenuWebServer::initialiseConnection() {
    lwip_socket_init();
    socket_t sockFd = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if(sockFd >= 0) {
        int ioState = 1;
        lwip_ioctl(sockFd, FIONBIO, &ioState);
        sockaddr_in sa = {};
        sa.sin_addr.s_addr = htonl(INADDR_ANY);
        sa.sin_port = serverPort;
        sa.sin_len = sizeof(sa);
        sa.sin_family = AF_INET;

        if(lwip_bind(sockFd, (sockaddr*)&sa, sizeof sa) < 0) {
            serdebugF2("Bind failed ", serverPort);
        }
        if(lwip_listen(sockFd, 5) < 0) {
            serdebugF2("Listen failed ", serverPort);
        }
    }
}


