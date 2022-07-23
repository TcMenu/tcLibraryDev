
#ifndef TCLIBRARYDEV_TCMENULWIPWEBSERVER_H
#define TCLIBRARYDEV_TCMENULWIPWEBSERVER_H

#include <PlatformDetermination.h>
#include <remote/TcMenuWebServer.h>
#include <remote/BaseRemoteComponents.h>
#include <remote/TcMenuHttpRequestProcessor.h>
#include <remote/TransportNetworkDriver.h>

#define AVAILABLE_FOR_WRITE_WORKS false

/**
 * This global method absolutely must be provided when you are not using the pre-packaged version of embedCONTROL-JS
 * @param server The server reference to provide the static hosting and configuration details to.
 */

namespace tcremote {

    class TcMenuWebSockTransport : public tcremote::AbstractWebSocketTcMenuTransport {
    private:
        socket_t clientFd;
        bool isConsideredOpen;
    public:
        TcMenuWebSockTransport() : clientFd(0), isConsideredOpen(false) {}

        void setClient(socket_t newClient) {
            clientFd = newClient;
            isConsideredOpen = true;
            setState(tcremote::WSS_HTTP_REQUEST);
        }

        bool available() override {
            return (writePosition < bufferSize);
        }

        void close() override;

        bool connected() override {
            return isConsideredOpen;
        }

        int performRawRead(uint8_t *buffer, size_t bufferSize) override;

        int performRawWrite(const uint8_t *data, size_t dataSize) override;
    };

    class TcMenuWebServer : public tcremote::TcMenuLightweightWebServer {
    private:
        int serverPort;
        socket_t acceptSocketFd;
        unsigned long timeStart;
    public:
        explicit TcMenuWebServer(int numConcurrent, int port);

        WebServerResponse * attemptNewConnection() override;

        void initialiseConnection() override;
    };

    int fromWiFiRSSITo4StateIndicator(int strength);
}

void prepareWebServer(tcremote::TcMenuWebServer& webServer);

#endif //TCLIBRARYDEV_TCMENULWIPWEBSERVER_H
