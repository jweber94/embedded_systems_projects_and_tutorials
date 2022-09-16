#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "utils.hpp"

// TCP server config
static inline std::string server_ip_addr = "192.168.2.110";
static inline const int server_acceptor_port = 8081;

// WIFI Codes
#define WIFI_SUCCESS 1 << 0
#define WIFI_FAILURE 1 << 1
#define TCP_SUCCESS 1 << 0
#define TCP_FAILURE 1 << 1
#define MAX_FAILURES 10

/* wifi start */
static EventGroupHandle_t wifi_event_group;
static int s_retry_num = 0; // retry counter for AP connection attempts

// Logging tags
static const char* MAIN_TAG = "main";
static const char* WIFI_TAG = "wifi";

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data) {
    ESP_LOGI(MAIN_TAG, "Hello WIFI event");
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(MAIN_TAG, "Connecting to access point...");
        esp_wifi_connect(); // after a successful connection, an IP event will be triggered through the access point due to DHCP
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < MAX_FAILURES) {
            ESP_LOGW(MAIN_TAG, "Reconnecting to access point...");
            esp_wifi_connect();
            s_retry_num++;
        } else {
            xEventGroupSetBits(wifi_event_group, WIFI_FAILURE);
        }
    }
}

static void ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    ESP_LOGI(MAIN_TAG, "Hello IP event");
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data; // cast the void* to the ip event datatype
        ESP_LOGI(MAIN_TAG, "STA IP: " IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(wifi_event_group, WIFI_SUCCESS); // sending the WIFI_SUCCESS signal to the event group
    }
}

// connect to wifi and return the result
esp_err_t connect_wifi_router() {
    int status = WIFI_FAILURE; // on default, we assume that we could not connect to the network
    
    // Initialize esp network interface driver (like "boost::asio::io_context" but as a global enablement of networking)
    ESP_ERROR_CHECK(esp_netif_init()); 

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
    esp_event_handler_instance_t got_ip_event_inst;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            IP_EVENT,
            IP_EVENT_STA_GOT_IP,
            &ip_event_handler,
            NULL,
            &got_ip_event_inst));
    
    // WIFI network settings
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Top",
            .password = "Secret",
	        .threshold = {
                .authmode = WIFI_AUTH_WPA2_PSK
            },
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    // setting the esp to wifi station mode
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) ); // setting wifi mode to be a wifi station
        // explaination: WIFI station (STA) is the opposite of a WIFI access point (AP) (which acts as a modem)

    // apply the wifi configuration to the wifi station
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // startup the station - event loop of the wifi controller starts
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(MAIN_TAG, "Wifi station initialized");

    // wait for successful connection to the configured wifi access point
    EventBits_t bits = xEventGroupWaitBits( // blocking
        wifi_event_group, // here are the status bits are stored for the events
        WIFI_SUCCESS | WIFI_FAILURE,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY
    );

    // checking the result of the ip connection attemped
    if (bits & WIFI_SUCCESS) {
        ESP_LOGI(MAIN_TAG, "Connected to access point");
        status = WIFI_SUCCESS;
    } else if (bits & WIFI_FAILURE) {
        ESP_LOGI(MAIN_TAG, "Failed to connect to access point");
        status = WIFI_FAILURE;
    } else {
        ESP_LOGW(MAIN_TAG, "Unexpected failure");
        status = WIFI_FAILURE;
    }

    /* unregister the connection callbacks from the wifi station event loop since we are now connected */
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, got_ip_event_inst));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_handler_event_inst));
    vEventGroupDelete(wifi_event_group); // delete 'status container' for the wifi connection

    return status;
}

esp_err_t connect_tcp_server() {
    struct sockaddr_in serverInfo = {0};
    char readBuffer[1024] = {0};

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr(server_ip_addr.c_str()); // inet_addr(get_reversed_hex_ip_addr(server_ip_addr).c_str()); // ip address dell
    serverInfo.sin_port = htons(server_acceptor_port); // targeted port

    ESP_LOGE(MAIN_TAG, "Connecting to %s", inet_ntoa(serverInfo.sin_addr.s_addr));

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        ESP_LOGE(MAIN_TAG, "Failed to create a socket");
        return TCP_FAILURE;
    }

    if (connect(sock, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) != 0) {
        ESP_LOGE(MAIN_TAG, "Failed connect to %s!", inet_ntoa(serverInfo.sin_addr.s_addr));
        close(sock);
        return TCP_FAILURE;
    }

    ESP_LOGI(MAIN_TAG, "Connected to TCP server");
    
    // read out the socket to the buffer
    bzero(readBuffer, sizeof(readBuffer));
    int r = read(sock, readBuffer, sizeof(readBuffer) - 1);
    for (int i = 0; i < r; i++) {
        putchar(readBuffer[i]);
    }

    // my 'protocol'
    if (strcmp(readBuffer, "HELLO") == 0) {
        ESP_LOGI(MAIN_TAG, "TCP Server login successful!");
    }

    return TCP_SUCCESS;
} 

/* wifi end */

#include "blinky.hpp"
#include <thread>
#include <chrono>

#define GPIO_1 5
#define GPIO_2 22

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
        ESP_LOGE(MAIN_TAG, "Could not connect to delivered wifi access point. Ending program.");
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
