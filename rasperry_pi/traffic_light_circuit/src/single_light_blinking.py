#!/usr/bin/python3

import RPi.GPIO as GPIO
import time

# setting up how to access the GPIO pins - using the pin number, NOT the GPIO name
GPIO.setmode(GPIO.BOARD)

def main():
    print("Starting the traffic light")
    
    GPIO.setup(37, GPIO.OUT)

    # program loop
    while True:
        GPIO.output(37, True)
        time.sleep(0.5)
        GPIO.output(37, False)
        time.sleep(0.5)


if __name__ == "__main__":
    main()
