#include "blinky.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/gpio.h"

Blinky::Blinky(int gpio_num) : m_terminate_blinky(false)
{
    m_gpio_num_status = std::make_tuple(gpio_num, false);
    m_blinky_name = std::string(BLINKY_TAG) + "_" + std::to_string(gpio_num);

    ESP_LOGI(m_blinky_name.c_str(), "Starting blink 1 process");
    gpio_reset_pin(gpio_num_t(std::get<0>(m_gpio_num_status)));
    gpio_set_direction(gpio_num_t(std::get<0>(m_gpio_num_status)), GPIO_MODE_OUTPUT);

    ESP_LOGI(m_blinky_name.c_str(), "Created blinky with GPIO %d", std::get<0>(m_gpio_num_status));
}

Blinky::~Blinky() {
    m_terminate_blinky = true;
    ESP_LOGI(m_blinky_name.c_str(), "Delete blinky");
}

void Blinky::start()
{
    ESP_LOGI(m_blinky_name.c_str(), "Start blinking on GPIO %d", std::get<0>(m_gpio_num_status));
    while(!m_terminate_blinky) {
        ESP_LOGI(m_blinky_name.c_str(), "New blinking cycle on gpio %d", std::get<0>(m_gpio_num_status));
        gpio_set_level(gpio_num_t(std::get<0>(m_gpio_num_status)), 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        gpio_set_level(gpio_num_t(std::get<0>(m_gpio_num_status)), 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        gpio_set_level(gpio_num_t(std::get<0>(m_gpio_num_status)), 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        gpio_set_level(gpio_num_t(std::get<0>(m_gpio_num_status)), 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return;
}

void Blinky::stop()
{
    m_terminate_blinky = true;
    ESP_LOGI(m_blinky_name.c_str(), "Stop blinking on GPIO %d", std::get<0>(m_gpio_num_status));
}