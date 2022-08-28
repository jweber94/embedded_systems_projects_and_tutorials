# Network blinky in C++
This is my first project with the esp32 that uses C++ and networking with the esp32

## Using C++ with idf
+ Using C++ and all its STL components to have a more convenient possibiliy to program in a object oriented manner is very easy. You just need to define the `CMAKE_CXX_STANDARD` within the top level `CMakeLists.txt` and use the `cmake_minimum_required(VERSION 3.8)`.
+ Besides this, you need to define your source and include files as `.cpp` and `.hpp`.
+ I wanted to use C++ in this project since i wanted to define an arbitary amound of Pins that the esp32 should blink LEDs on. Besides this, I really like C++ more then plain C and therefore it is more convenient for me to program in my common structures.
+ Reference: https://embeddedtutorials.com/eps32/esp-idf-cpp-with-cmake-for-esp32/

## Networking with the WiFi Module
