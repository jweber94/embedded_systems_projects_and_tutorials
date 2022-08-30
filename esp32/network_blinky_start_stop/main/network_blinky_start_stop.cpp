#include <stdio.h>
#include <thread>
#include <chrono>
#include "blinky.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_wifi.h"
// #include "esp_system.h"
#include "nvs_flash.h"


#define GPIO_1 5
#define GPIO_2 22

// Logging tags
static inline const char* MAIN_TAG = "main";
static inline const char* WIFI_TAG = "wifi";

// WIFI Codes
#define WIFI_SUCCESS 1 << 0
#define WIFI_FAILURE 1 << 1
#define TCP_SUCCESS 1 << 0
#define TCP_FAILURE 1 << 1
#define MAX_FAILURES 10

/* wifi start */
static EventGroupHandle_t wifi_event_group;

esp_err_t connect_wifi_router() {
    int status = WIFI_FAILURE; // on default, we assume that we could not connect to the network
    
    // Initialize esp network interface driver (like "boost::asio::io_context" but as a global enablement of networking)
    ESP_ERROR_CHECK(esp_netif_inti()); 

    // Start default event loop to enable the default wifi events with their callbacks (see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html for details)    
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    // create default wifi station object for the wifi driver
    esp_netif_create_default_wifi_sta();

    // Create wifi config with default settings - wpa, task core, beacons, buffer sizes, ...
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg)); // init wifi object on the driver with the given configuration to let acutal callbacks happen on wifi events (with the given config)

    // handler assignment to store status information about wifi events
    wifi_event_group = xEventGroupCreate();

    // register callbacks to the wifi events
    esp_event_handler_instance_t wifi_handler_event_inst;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            WIFI_EVENT,                 // base event
            ESP_EVENT_ANY_ID,           // specific - ESP_EVENT_ANY_ID = catch all event
            &wifi_event_handler,        // callback to invoke
            NULL,                       // parameters
            &wifi_handler_event_inst    // handler to the events
        )
    );

    // IP event --> Used to 
    esp_event_handler_instance_t got_ip_event_instance;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            IP_EVENT,
            IP_EVENT_STA_GOT_IP,
            &ip_event_handler,
            NULL,
            &got_ip_event_handler
        )
    );

    // next todo: wifi_config_t

    ESP_LOGI(WIFI_TAG, "Successfully connected to configured WIFI network.");
    return WIFI_SUCCESS;
}

esp_err_t connect_tcp_server() {
    ESP_LOGI(WIFI_TAG, "Successfully connected to configured TCP server.");
    return TCP_SUCCESS;
} 

/* wifi end */



void blink_blinky(void* pvParameter) {
    Blinky* tmp_blinky = static_cast<Blinky*>(pvParameter);
    tmp_blinky->start(); // blocking
    vTaskDelete(NULL); // ending task
}

extern "C" void app_main(void)
{
    // connection status
    esp_err_t status = WIFI_FAILURE;
    
    // initialize non volantile storage to store the wifi credentials persistendly on the device
    esp_err_t ret = nvs_flash_init(); // initialize non volantile flash storage
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // reinitialize on purpose
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret); // in case of an error, the program will terminate and the call stack is printed to the serial output of the esp32
    
    status = connect_wifi_router();
    if (WIFI_SUCCESS != status) {
        ESP_LOGE(MAIN_TAG, "Could not connect to delivered wifi connection. Ending program.");
        return;
    }

    status = connect_tcp_server();
    if (status != TCP_SUCCESS) {
        ESP_LOGE(MAIN_TAG, "Could not connect to delivered IP server connection. Ending program.");
        return;
    }

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    // After we connected to the wifi we start the regular blinky

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
