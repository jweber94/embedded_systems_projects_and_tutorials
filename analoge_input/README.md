# Analoge Input with Analoge-Digial-Converter (ADC)
+ Uses: ***Rasperry Pi 4***
+ This project is about getting an analoge input signal via a ADC to a digital signal and read it out with the rasperry pi. 
+ The ADC is a [MCP3008](https://cdn-shop.adafruit.com/datasheets/MCP3008.pdf) which is a 10 Bit analoge digital converter that sends out the converted signal via [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface) (Serial Periferal Interface Bus)

## Main Takeaways
+ Functioning of SPI under the hood. Therefore I worked through the following ressources:
    - https://raspberrypi-aa.github.io/session3/spi.html
    - https://www.youtube.com/watch?v=7uDo_Du5d9g&t=596s
+ What is Bitrate and Baudrate
    - https://www.geeksforgeeks.org/baud-rate-and-its-importance/
+ How do analoge digital converters work?
    - Script of the lecture [Messtechnik 2](https://www.mrt.kit.edu/lehre_SS_Messtechnik_II.php)
+ "Multi Threading" in python (which is a little misleading, since there is no proper multi threading in python)

## Reference
+ This project is based on [this](https://tutorials-raspberrypi.de/raspberry-pi-helligkeitssensor-fotowiderstand/) but is elaborated out a little more to have a live plot of the timeseries of the outputted data from the light sensor and it dimms an LED based on the light that is present.
    - Therefore it builds upon the "led_dimm" project that I did before.
    - TODO: Make a diagram about the breadboard circuit