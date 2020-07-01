//
// Created by David Cherry on 27/06/2020.
//

#ifndef TCLIBRARYDEV_SIMHUBCONNECTOR_H
#define TCLIBRARYDEV_SIMHUBCONNECTOR_H

/**
 * this is the maximum number of chars on a line
 */
#define MAX_LINE_WIDTH 32

#include <Arduino.h>
#include <TaskManager.h>
#include <HardwareSerial.h>
#include <IoLogging.h>

class SimhubConnector : public Executable {
private:
    HardwareSerial* serialPort;
    int statusMenuId;
    char lineBuffer[MAX_LINE_WIDTH];
    int linePosition;
public:
    SimhubConnector(HardwareSerial* serialPort, int statusMenuId) {
        this->serialPort = serialPort;
        this->linePosition = 0;
        this->statusMenuId = statusMenuId;
        changeStatus(false);
    }

    void exec() override;
private:
    void processCommandFromSimhub();
    void processTcMenuCommand();
    void changeStatus(bool b);
};


#endif //TCLIBRARYDEV_SIMHUBCONNECTOR_H
