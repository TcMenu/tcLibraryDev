#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "IoLogging.h"
#include "TaskManagerIO.h"
#include "oled/TcAdaSH1107I2C.h"
#include "PlatformDeterminationWire.h"
#include "Fonts/OpenSansRegular8pt.h"

#define SDA_PIN GPIO_NUM_6
#define SCL_PIN GPIO_NUM_7

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

static Esp32IdfI2CWrapper * wireRef = nullptr;

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "Starting pure ESP-IDF tcMenu example");

    wireRef = new Esp32IdfI2CWrapper();
    wireRef->begin(SDA_PIN, SCL_PIN);
    auto* display = new TcAdaSH1107I2C(wireRef, 0x3C, 128, ENCMODE_UTF8);

    display->begin();
    display->clearDisplay();
    display->fillRect(0, 0, 20, 20, WHITE);
    display->setFont(OpenSansRegular8pt);
    display->setCursor(0, 30);
    display->setTextColor(WHITE);
    display->print("Tc ");
    display->print(display->width());
    display->print(", ");
    display->print(display->height());
    display->fillCircle(12, 80, 15, WHITE);
    display->display();

    taskManager.schedule(repeatSeconds(1), [] {
        serlogF(SER_DEBUG, "Ticked a second");
        serlogF2(SER_DEBUG, "Wire valid ", wireRef->isValid());
    });

    xTaskCreate(taskManagerTask, "TaskManager", 8192, nullptr, 5, nullptr);
}
