# RC Light Sensor
+ Uses: ***Rasperry Pi 4***
+ This project is based on [this](https://www.youtube.com/watch?v=dPwW9zmX84E)
    - This is an easy way how to read out sensor data via GPIOs from an analoge sensor that is based on an resistor measurement principle without an ADC (analoge digital converter) like the MCP3008.
    - The light resistor sensor is just an example for the whole group of analoge sensors that work with resistance is their operating principle
+ CAUTION: The higher the value that is measured is, the darker it is!

## Main Takeaways
+ GPIOs have a default state. For the rasperry pi 4, you can look them up [here](https://roboticsbackend.com/raspberry-pi-gpios-default-state/)
+ GPIOs with an _HIGH_ or _LOW_ signal on them are not "real" V_{cc} or GND but they can act like them if you do not have very hard conditions on how stable and exact the voltage needs to be.
+ We can change the purpose of a GPIO-Pin during runtime (the configuration if it is an input or an output pin is _NOT_ fixed during the runtime of our program)
+ The determination of the current sensor value is theoretically based on the loading constant for RC-circuits
    - Reference: https://de.wikipedia.org/wiki/RC-Glied 
    - The time that is measured by our program is determined by the time constant of the loading exponential function  ![\Large U_{C}(t)=U_{max}*(1-e^{-\frac{t}{R*C}}](https://latex.codecogs.com/svg.latex?U_{C}(t)=U_{max}*(1-e^{-\frac{t}{R*C}})) which is ![\Large R*C](https://latex.codecogs.com/svg.latex?R*C))R*C
        * Since C is a fixed value (the condensator does not change its capacity) we measure the change of R depended on the light that goes to the light sensor
+ Since this method of measureing is much more noisey (due to inaccurate GPIO status on voltages under 3.3 volt and the measurement principle itself), I implemented an online calculation for a moving mean of the last `N = 20` measurements in the function `def calculate_moving_avg(measurement)`.
  - You can look up the formula under [the wikipedia page](https://de.wikipedia.org/wiki/Gleitender_Mittelwert) of the moving mean

## Reference
+ I use this light sensor: https://www.amazon.de/-/en/GL12528-Light-Sensitive-Resistor-Dependent/dp/B07MZK5RL8  
