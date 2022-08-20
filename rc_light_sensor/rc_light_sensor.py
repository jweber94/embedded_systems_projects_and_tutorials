#!/usr/bin/python3

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
SIGNAL_PIN = 7
RECEIVER_PIN = 8

def rc_load_time():
    count = 0

    # input signal
    GPIO.setup(SIGNAL_PIN, GPIO.OUT)
    GPIO.output(SIGNAL_PIN, GPIO.LOW)
    time.sleep(0.1)
    
    # receive info back
    GPIO.setup(SIGNAL_PIN, GPIO.IN)

    while (GPIO.input(SIGNAL_PIN) == GPIO.LOW):
        count += 1
    return count


def main():
    print("Start receiving light measurement")

    try:
        while True:
            print(rc_load_time())
    except:
        GPIO.cleanup()
        print("Finish program on exception.")

if __name__ == "__main__":
    main()
