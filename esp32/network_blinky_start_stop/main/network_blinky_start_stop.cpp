#include <stdio.h>
#include <thread>
#include <chrono>
#include "blinky.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#define GPIO_1 5
#define GPIO_2 22

static inline const char* MAIN_TAG = "main;";

void blink_blinky(void* pvParameter) {
    Blinky* tmp_blinky = static_cast<Blinky*>(pvParameter);
    tmp_blinky->start(); // blocking
    vTaskDelete(NULL); // ending task
}

extern "C" void app_main(void)
{
    Blinky my_blinky(GPIO_1);
    
    //char* tmp_blinky;
    void * tmp_blinky = &my_blinky;

    xTaskCreate(&blink_blinky, "blinking_task", 2048, tmp_blinky, 5, NULL);
    
    int counter = 0;
    while(counter < 20) {
        counter++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    my_blinky.stop();
    ESP_LOGI(MAIN_TAG, "Finished blinking first");

    Blinky blinky_2(GPIO_2);
    while(1){
        blinky_2.start();
    }
}
