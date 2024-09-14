
#include <TaskManagerIO.h>
#include <cstdio>

taskid_t millisTask;
int counter = 0;

class MyCountingExecutable : public Executable {
private:
    int myCounter = 0;
    const char* myName;
public:
    MyCountingExecutable(const char* name) : myName(name) {}
    ~MyCountingExecutable() override = default;

    void exec() override {
        myCounter++;
        printf("%s count is %d\n", myName, myCounter);
    }
};

int main() {
    stdio_init_all();

    // here we schedule work to be done every half second, it will schedule the above executable class, and the final
    // parameter tells task manager to own the memory.
    taskManager.schedule(repeatMillis(500), new MyCountingExecutable("Executable Counter"), true);

    // schedule some work to be done immediately, IE treat taskManager as a queue.
    taskManager.execute([] {
        printf("Queued execution\n");
    });

    // schedule something to be done every 250 millis
    millisTask = taskManager.schedule(repeatMillis(250), [] {
        printf("250 millis schedule\n");
    });

    // scheduling something once, and when called, cancelling another task.
    taskManager.schedule(onceSeconds(20), [] {
        printf("Cancel 250 millis schedule\n");
        taskManager.cancelTask(millisTask);
    });

    // schedule in seconds.
    taskManager.schedule(repeatSeconds(1), [] {
        printf("Once a second schedule, counter=%d\n", counter);
        taskManager.yieldForMicros(100000);
        printf("A bit later\n");
    });

    // schedule repeatedly in micros.
    taskManager.schedule(repeatMicros(250), [] {
        counter++;
    });

    while (true) {
        taskManager.runLoop();
        auto toNextTime = taskManager.microsToNextTask();
        sleep_us(toNextTime);
    }
}
