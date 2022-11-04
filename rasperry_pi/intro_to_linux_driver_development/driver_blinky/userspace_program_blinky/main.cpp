#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> // needed for thread pausing - usleep command

static inline const int PIN = 3;
static inline const int DELAY_TIME = 1000000; // microseconds

int main() {
    // check if the file exists
    std::fstream file("/proc/gpio_interface");
    if (!file.good()) {
        std::cerr << "Could not open file!\n";
        return 1;
    }

    bool led_on = false;
    unsigned int write_length = std::to_string(PIN).size() + 1;
    while (1) {
        if (!led_on) {
            file.write((std::to_string(PIN) + ",1").c_str(), write_length);
            usleep(DELAY_TIME); // https://man7.org/linux/man-pages/man3/usleep.3.html
        } else {
            file.write((std::to_string(PIN) + ",0").c_str(), write_length);
            usleep(DELAY_TIME);
        }
    }
    return 0;
}