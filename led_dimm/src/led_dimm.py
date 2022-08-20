#!/usr/bin/python3

import RPi.GPIO as GPIO
import time

PIN_NO = 37
PWM_FREQUENCY = 50 # Hz

# setup the GPIOs
GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN_NO, GPIO.OUT) # GPIO26 on the board

# define the pwm pin
pin = GPIO.PWM(PIN_NO, PWM_FREQUENCY)

def main():
    print("Hello World!")
    try:
        print("Start dimming")
        pin.start(0)

        while True:
            for dc in range(0, 101, 5):
                pin.ChangeDutyCycle(dc)
                time.sleep(0.1)
            for dc in range(100, -1, -5):
                pin.ChangeDutyCycle(dc)
                time.sleep(0.1)
    except:
        pin.stop()
        GPIO.cleanup()
        print("Ending program on exception")

if __name__ == "__main__":
    main()
