# Basics on the esp32

# Ressources
+ https://embeddedtutorials.com/ --> VERY good ressource for my purposes

## Operating system and development tools
### Documentation
+ You can find the esp32 documentation with a lot of code examples [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/index.html)
+ Most code (e.g. networking code) is similar or mostly similar to POSIX programming/syscalls
### FreeRTOS
+ The esp32 uses freeRTOS which is a library, written in C that is statically linked against my application
    - It is adjusted to the esp32 ARM Cortex processor within the idf.py (IoT Development Framework) that is delivered by espressive (The creator of the esp32 processor)
+ You can define tasks that will be schedules by the freeRTOS as a real time application
    - It is possible to assign tasks to specific cores of the esp32 (which has two cores)
+ FreeRTOS is a C library --> You need to program in C if you want to have full control over the process management of the esp32 
+ The entrypoint of your application (from which you can start off other processes) is the `void app_main(){}` function
+ Since the esp uses a predefined freeRTOS, we can adjust it's configuration by using the `idf.py menuconfig`
+ In contrast to vanilla freeRTOS, we do not need to call `void vTaskStartScheduler()` since this will be called implicitly by the idf freeRTOS at the end of the `void app_main()` function
#### Tasks in FreeRTOS
+ If we create a task with `xTaskCreate`, it must not end with a `return` statement in the code. You must use the `vTaskDelete(NULL);` to delete a task
+ To hand over parameters to a task, we need to cast a struct into a void pointer and then `static_cast` it back in its native form as a pointer. See the `network_blinky_start_stop/main/network_blinky_start_stop.cpp` for an example 

### idf
+ Is maintained by espressive and can be used for all ESP boards (not only esp32 which is just one product of espressive)
+ idf (IoT Development Framework) is the build system for the esp32 (and other espressive chips). It builds upon cmake!
+ There is a VSCode extension to have VSCode as a very convenient IDE that will be used to program and flash the esp32

#### Createing a project with idf
+ Is the go-to way of starting a new application for the esp32!
    - It will create `CMakeLists.txt` and a `/src` folder where your code will be written in.
    - You can edit the `CMakeLists.txt` in order to add multiple files to your C project 

# ESP32 Dev Board Wireing
+ The wireing can be looked up [here](https://unsinnsbasis.de/esp32-pin-belegung/) 
+ If you setup a 5V current on the `V_in` pin and add Ground to one of the `GND` pins, the esp will start with the lastly flashed program

# ESP32 and WiFi
+ The ESP32 can be used as a IP client as well as a IP server. It has a library implementation to act as a WiFi router to which you can connect to. 
    - On connection you can setup a callback that is then invoked. 
    - For example: You can use this functionallity to serve a website that delivers WiFi credentials to the ESP. After receiving them, the esp stops publishing its SSID and registers it as a part of the network that is delivered by the configured router (aka SSID and password). After this has happened, the esp32 goes into its actual business logic

## NVS (Non Volantile Storage)
+ Can be used to store persistend data on a partition on the esp32 at development/production time and is then persisted
    - The data is persisted there, even if the device is currently not powered
+ Usually it saves a binary BLOB (Binary large object), which is just a chunk of data that needs to be interpreted correctly to extract meaningful information out of it
+ Commonly it is used to store serial numbers, manufacturer, ... of a device
+ The esp32 has a nvs API
    - Which is also used to store wifi credentials persistendly

## ESP32 Event Loop
+ The esp32 has a default event loop upon which you can build your own event definitions with their callback interfaces
    - The idea behind this is to have an infinite loop running in the background and whenever an event is occuring, the callback that is associated with the event will be invoked.
    - You can start off the default event loop by calling `esp_event_loop_create()` which contains the default wifi events with its callbacks
    - Reference: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_event.html?highlight=esp_event_loop#

## ESP32 API basics
+ `ESP_ERROR_CHECK()`:
    - Checks if a function that is called within delivers that esp_err_t code "ESP_OK" and if not it will print the call stack to the serial output and ends the program
    - Implemented as a C makro
    - If you want to *not* terminate your program after this, you need to use `ESP_ERROR_CHECK_WITHOUT_ABORT()`
    - Reference: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_err.html?highlight=esp_error_check#c.ESP_ERROR_CHECK
