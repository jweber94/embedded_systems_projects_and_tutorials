# Traffic Light Circuit
+ Uses: ***Rasperry Pi 4***
+ This project is inspired by [this](https://tutorials-raspberrypi.de/ampelschaltung-mit-gpio-teil-1/) and [this](https://tutorials-raspberrypi.de/ampelschaltung-mit-gpio-teil-2/) which I did before. 
+ The goal of this project is to enlight one of two LEDs and have a smooth change by dimming the LEDs on change if you press a button that is connected to the rasperry pi. 
+ `/src/single_light_blinking.py` is just a test script if a properly did the wireing for the LEDs!

## Main takeaways
+ Having fun with object oriented programming in python while lighting LEDs with the rasperry pi.
+ Doing the traffic light tutorials from the reference: 
    - How to use a transistor in a electronic circuit in practice (the pin in the middle has a basis current what cases the transistor to conduct). The basis current is injected by the GPIO high.