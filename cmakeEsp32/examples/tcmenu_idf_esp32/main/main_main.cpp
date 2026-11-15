#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/spi_master.h"
#include "IoLogging.h"
#include "TaskManagerIO.h"
#include "PlatformDeterminationWire.h"

#define SDA_PIN GPIO_NUM_6
#define SCL_PIN GPIO_NUM_7

#define SPI_CLK_PIN GPIO_NUM_8
#define SPI_MISO_PIN GPIO_NUM_9
#define SPI_MOSI_PIN GPIO_NUM_10

static const char* TAG = "tcmenu_esp32_idf";

void setup();

static void taskManagerTask(void* arg) {
    static constexpr int64_t RUN_BUDGET_US = 1000;
    setup();
    while (true) {
        const int64_t start = esp_timer_get_time();

        do {
            taskManager.runLoop();
        } while ((esp_timer_get_time() - start) < RUN_BUDGET_US);

        vTaskDelay(1);
    }
}

spi_device_handle_t spiBus = nullptr;


extern "C" void app_main(void) {
    ESP_LOGI(TAG, "Starting pure ESP-IDF tcMenu example");

    // in this example we are using the I2C pins so commented out.
/*    wireRef = new Esp32IdfI2CWrapper();
    wireRef->begin(SDA_PIN, SCL_PIN);*/

    // We create an spi bus
    spi_bus_config_t buscfg = {};
    buscfg.miso_io_num = SPI_MISO_PIN;
    buscfg.mosi_io_num = SPI_MOSI_PIN;
    buscfg.sclk_io_num = SPI_CLK_PIN;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = 0;

    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    spi_device_interface_config_t devcfg = {};
    devcfg.clock_speed_hz = 4 * 1000 * 1000;
    devcfg.mode = 0;
    devcfg.spics_io_num = -1;
    devcfg.queue_size = 7;

    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spiBus);
    ESP_ERROR_CHECK(ret);

    xTaskCreate(taskManagerTask, "TaskManager", 8192, nullptr, 5, nullptr);
}
