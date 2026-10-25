#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "IoLogging.h"
#include "TaskManagerIO.h"

static const char* TAG = "tcmenu_esp32_idf";

static void taskManagerTask(void* arg) {
    static constexpr int64_t RUN_BUDGET_US = 1000;

    while (true) {
        const int64_t start = esp_timer_get_time();

        do {
            taskManager.runLoop();
        } while ((esp_timer_get_time() - start) < RUN_BUDGET_US);

        vTaskDelay(1);
    }
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "Starting pure ESP-IDF tcMenu example");

    taskManager.schedule(repeatSeconds(1), [] {
        serlogF(SER_DEBUG, "Ticked a second");
    });

    xTaskCreate(taskManagerTask, "TaskManager", 8192, nullptr, 5, nullptr);
}