# LED dimming with PWM
+ Uses: ***Rasperry Pi 4***
+ This tutorial is based on [this](https://tutorials-raspberrypi.de/programmieren-lernen-raspberry-pi-gpio-pwm/)

## Main Takeaways
+ What is PWM (pulse wide modulation) and what can you do with it
    - Mostly used to control servo motors - I leared how servos work under the hood
        * Reference: https://www.youtube.com/watch?v=g4BM1pjm64s
+ The dimming works based on the [RMS](https://en.wikipedia.org/wiki/Root_mean_square) of the PWM signal
    - The amplitude of the signal stays the same, just the duration (=duty circle) changes to modulate the RMS
