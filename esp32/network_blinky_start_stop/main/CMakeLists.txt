set(SOURCES network_blinky_start_stop.cpp ./src/blinky.cpp)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . ../include)