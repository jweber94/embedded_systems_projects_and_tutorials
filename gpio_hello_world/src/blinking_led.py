#!/usr/bin/python3

import RPi.GPIO as GPIO
import time

def main():
    # Setup how to access the GPIO pins
    GPIO.setmode(GPIO.BCM) # use the pin names 
    
    # Define the behaviour of the GPIO 23 (what should it do)
    GPIO.setup(23, GPIO.OUT)
    GPIO.setup(24, GPIO.IN)

    for i in range(0, 5):
        GPIO.output(23, GPIO.HIGH)
        time.sleep(0.5)
        GPIO.output(23, GPIO.LOW)
        time.sleep(0.5)

    print("From now on, we can activate the LED by pressing the knoob")
    while True:
        if GPIO.input(24) == 0:
            GPIO.output(23, GPIO.LOW)
        else:
            GPIO.output(23,GPIO.HIGH)

if __name__ == "__main__":
    main()
