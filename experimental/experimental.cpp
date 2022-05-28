#include <TaskManagerIO.h>
#include "IoLogging.h"

class EventTesting : public BaseEvent {
private:
    uint32_t counter = 0;
    const char* name;
public:
    explicit EventTesting(const char *name) : name(name) {}

    void exec() override {
        counter++;
        serdebugF3("exec - counter is ", counter, name);
    }

    uint32_t timeOfNextCheck() override {

        return 60UL * 1000000UL;
    }
};

EventTesting testing1("test1");
EventTesting testing2("test2");

uint32_t thenTime1;
uint32_t thenTime2;

void setup() {
    taskManager.registerEvent(&testing1);
    taskManager.registerEvent(&testing2);
    thenTime1 = millis();
    thenTime2 = millis();
}

void loop() {
    taskManager.runLoop();

    if((millis() -thenTime1) > 100) {
        thenTime1 = millis();
        testing1.markTriggeredAndNotify();
    }

//    if((millis() -thenTime2) > 500) {
//        thenTime2 = millis();
//        testing2.markTriggeredAndNotify();
//    }
}
