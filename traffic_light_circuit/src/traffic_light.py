#!/usr/bin/python3

import RPi.GPIO as GPIO
import time
import logging

# Setting up
GPIO.setmode(GPIO.BOARD)
log = logging.getLogger("TrafficLight")
logging.basicConfig(level = logging.INFO)
SWITCHTIME = 0.5

GPIO.setup(37, GPIO.OUT)
GPIO.setup(38, GPIO.OUT)
GPIO.setup(40, GPIO.IN)


class TafficLight():
    def __init__(self, switch_time, logger):
        # GPIO.setup(37, GPIO.OUT)
        # GPIO.setup(38, GPIO.OUT)
        # GPIO.setup(40, GPIO.IN)

        self.m_blue = False
        self.m_red = False
        self.m_blue_pin_no = 37
        self.m_red_pin_no = 38
        self.m_switch_time = switch_time
        self.m_logger_ref = logger
        
        # create initial state
        self.light_pin(self.m_red_pin_no)
        self.m_red = True
        self.darken_pin(self.m_blue_pin_no)
        self.m_blue = False

        log.info("Traffic Light started")

    def light_pin(self, pin_no):
        GPIO.output(pin_no, True)

    def darken_pin(self, pin_no):
        GPIO.output(pin_no, False)

    def switch_to_blue(self):
        # go to blue
        self.light_pin(self.m_blue_pin_no)
        self.m_blue = True
        time.sleep(self.m_switch_time)

        self.darken_pin(self.m_red_pin_no)
        self.m_red = False

        self.m_logger_ref.info("Activated blue light")

    def switch_to_red(self):
        # go to red
        self.light_pin(self.m_red_pin_no)
        self.m_red = True
        time.sleep(self.m_switch_time)

        self.darken_pin(self.m_blue_pin_no)
        self.m_blue = False

        self.m_logger_ref.info("Activated red light")

    def switch(self):
        if self.m_red and not self.m_blue:
            self.switch_to_blue()
           
        elif not self.m_red and self.m_blue:
            self.switch_to_red()

    def check_switch_trigger(self):
        if GPIO.input(40) == 1:
            return True
        return False

    def darken_all(self):
        self.darken_pin(self.m_red_pin_no)
        self.m_red = False

        self.darken_pin(self.m_blue_pin_no)
        self.m_blue = False

        self.m_logger_ref.warning("Darkened all lights")
       
def main():
    log.info("Starting the traffic light simulation with two leds")
    
    traffic_light_obj = TafficLight(SWITCHTIME, log)
    
    log.info("Created traffic light object. Can start the functioning now")

    try:
        while True:
            if traffic_light_obj.check_switch_trigger():
                traffic_light_obj.switch()
    except:
        # on error - darken all lights
        traffic_light_obj.darken_all()
        log.error("Shutting down on exception")

if __name__ == "__main__":
    main()
