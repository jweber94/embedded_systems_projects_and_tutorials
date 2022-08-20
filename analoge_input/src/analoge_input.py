#!/usr/bin/python3

# import RPi.GPIO as GPIO
import time
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
from spidev import SpiDev
import RPi.GPIO as GPIO
from threading import Thread
import os

# Setup SPI Bus interface
SPI_SPEED_MAX = 1000000 # 1MHz
USED_SENSOR_VOLTAGE = 3.3 # Volt

# Setup GPIO for PWM dimming
GPIO.setmode(GPIO.BOARD)
PWM_PIN = 37
GPIO.setup(PWM_PIN, GPIO.OUT)



class MCP3008:
    def __init__(self, bus = 0, device = 0):
        self.bus, self.device = bus, device
        self.spi = SpiDev()
        self.open()
        self.spi.max_speed_hz = SPI_SPEED_MAX # 1MHz
 
    def open(self):
        self.spi.open(self.bus, self.device)
        self.spi.max_speed_hz = SPI_SPEED_MAX # 1MHz
    
    def read(self, channel = 0):
        adc = self.spi.xfer2([1, (8 + channel) << 4, 0])
        data = ((adc[1] & 3) << 8) + adc[2]
        return data
            
    def close(self):
        self.spi.close()


class ManagingClass:
    def __init__(self, mcp_obj):
        self.mcp_ref = mcp_obj # to update the data

        ## Plotting
        # Setting up limits
        self.x_length = 200
        self.y_range = [0, 4] # Volt
        
        # Defining the plot window
        self.fig, self.axes = plt.subplots()
        self.x_plt_range = list(range(0, self.x_length))
        self.y_plt_range = [0] * self.x_length
        self.axes.set_ylim(self.y_range)
        self.line, = self.axes.plot(self.x_plt_range, self.y_plt_range)
        
        #self.axes.set_title("Light Sensor")
        self.axes.set_xlabel("Sample No.")
        self.axes.set_ylabel("Voltage")
        
        self.is_plot_activate = False
    
        ## LED dimming
        self.last_photodiod_value = 0
        self.pwm_pin = GPIO.PWM(PWM_PIN, 50) # Update the pwm signal with 50 Hz

        print("Live plotting object initialized")
        

    def animate(self, i):
        self.last_photodiod_value = self.mcp_ref.read(channel = 0) * USED_SENSOR_VOLTAGE /1024.0
        self.y_plt_range.append(self.last_photodiod_value) # read out the senor and add the value to the plot - TODO: Get the data from a business logic class that contains the senor as well as the plotter (and other components)

        self.y_plt_range = self.y_plt_range[-self.x_length:]
        self.line.set_ydata(self.y_plt_range) 
        self.axes.set_xlabel("Sample No.")

        return self.line,
        
    def start_plotting(self):
        if not self.is_plot_activate:
            self.anim = animation.FuncAnimation(self.fig,
                    self.animate,
                    interval=1,
                    blit=True)
            self.is_plot_activate = True
        else:
            print("Plot is already active!")
    
    def show_figure(self):
        plt.show()

    def led_dimm_loop(self):
        try:
            self.pwm_pin.start(0)
            while True:
                self.update_led_light()
                time.sleep(0.5)
        except:
            print("Stopped thread on exception. End program")
            os._exit(1)

    def start_led_light(self):
        self.led_thread = Thread(target=self.led_dimm_loop)
        self.led_thread.start()
        print("PWM Pin started")

    def scale_sensor_input_to_pwm(self):
        if self.last_photodiod_value > 2.3:
            return 0
        else:
            return 100 - int(self.last_photodiod_value * 100.0 / 3.3)

    def update_led_light(self):
        self.pwm_pin.ChangeDutyCycle(self.scale_sensor_input_to_pwm())

    def end_pwm(self):
        self.pwm_pin.stop()

def main():
    print("Started analoge input")    
    try:
        adc = MCP3008()
        manage_instance = ManagingClass(adc)
        manage_instance.start_plotting()
        manage_instance.start_led_light()
        manage_instance.show_figure() # blocking
    except:
        print("Ended program on exception. Shutting down all pins!")
        manage_instance.end_pwm()
        GPIO.cleanup()
        os._exit(1)

if __name__ == "__main__":
    main()
