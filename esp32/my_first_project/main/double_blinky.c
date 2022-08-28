#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
 
 #include "esp_log.h"

#define BLINK_GPIO 5
#define BLINK_GPIO_22 22
 
 
void hello_task(void *pvParameter)
{
    static const char * TASK_NAME = "Hello Task";
	while(1)
	{
	    ESP_LOGI(TASK_NAME, "Hello world!\n");
	    vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}
 
void blinky_1(void *pvParameter)
{
    static const char * TASK_NAME = "Blinky 1 Task";
    ESP_LOGI(TASK_NAME, "Starting blink 1 process");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
        ESP_LOGW(TASK_NAME, "Blinking on core %d with blinky_1", xPortGetCoreID());
        ESP_LOGI(TASK_NAME, "Blinking ON\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        ESP_LOGI(TASK_NAME, "Blinking OFF\n");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(900 / portTICK_PERIOD_MS);
    }
}
 
void blinky_2(void *pvParameter)
{
    char * blinky_2_task_name = pcTaskGetName(NULL);
    ESP_LOGI(blinky_2_task_name, "Starting blink 1 process");
    gpio_reset_pin(BLINK_GPIO_22);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO_22, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
        ESP_LOGW(blinky_2_task_name, "Blinking on core %d with blinky_2", xPortGetCoreID());
        ESP_LOGI(blinky_2_task_name, "Double blink cycle started\n");
        gpio_set_level(BLINK_GPIO_22, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);

        gpio_set_level(BLINK_GPIO_22, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);

        gpio_set_level(BLINK_GPIO_22, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(BLINK_GPIO_22, 0);
        vTaskDelay(700 / portTICK_PERIOD_MS);
        ESP_LOGI(blinky_2_task_name, "Double blink cycle finished\n");
    }
}

void app_main()
{
    // Creating tasks for freeRTOS WITHOUT a dedicated core assigned to them
    // xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    // xTaskCreate(&blinky_1, "blinky", 8129,NULL,5,NULL );
    // xTaskCreate(&blinky_2, "blinky_2", 8129,NULL,5,NULL );

    char * main_task_name = pcTaskGetName(NULL);
    ESP_LOGI(main_task_name, "Name of the main task is %s", main_task_name);

    // Creating tasks for freeRTOS WITH a dedicated core assigned to them
    xTaskCreatePinnedToCore(
                    &blinky_1,   /* Function to implement the task */
                    "blinky_1", /* Name of the task */
                    8129,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    0);  /* Core where the task should run */

    xTaskCreatePinnedToCore(
                    &blinky_2,   /* Function to implement the task */
                    "blinky_2", /* Name of the task */
                    8129,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    1);  /* Core where the task should run */
}