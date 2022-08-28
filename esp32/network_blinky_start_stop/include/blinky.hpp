#pragma once

#include <tuple>
#include <string>

static inline const char* BLINKY_TAG = "Blinky";

class Blinky
{
    public:
        Blinky(int gpio_num);
        ~Blinky();
        void start();
        void stop();
    private:
        std::tuple<int, bool> m_gpio_num_status; // <gpio number, status> where status could be on = true or off = false
        std::string m_blinky_name;
        bool m_terminate_blinky;
};