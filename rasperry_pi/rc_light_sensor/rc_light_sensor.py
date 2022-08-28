#!/usr/bin/python3

import RPi.GPIO as GPIO
import time
import datetime
import matplotlib.animation as animation
import matplotlib.pyplot as plt

# rasperry pi
GPIO.setmode(GPIO.BOARD) # use pin numeration not GPIO names
SIGNAL_PIN = 7

# Live plotting setup
x_length = 200
y_range = [0, 600]
Y_MAX = 600
Y_MAX_LAST_UPDATE = datetime.datetime.now()
Y_UPDATE_TIMEOUT = 10

fig, axes = plt.subplots()
plt_x_range = list(range(0, x_length))
plt_y_range = [0] * x_length

axes.set_ylim(y_range)
line, = axes.plot(plt_x_range, plt_y_range)

axes.set_xlabel("Sample no.")
axes.set_ylabel("Light")

# online moving average calculation
N = 20
M_t_1 = 0  


def calculate_moving_avg(measurement):
    global plt_y_range
    global M_t_1
    global N

    m_t = M_t_1 + measurement/N - plt_y_range[len(plt_y_range)-N]/N
    M_t_1 = m_t
    return m_t


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

def animate(i):
    global plt_y_range
    global line
    global axes
    global Y_MAX
    global Y_UPDATE_TIMEOUT
    global Y_MAX_LAST_UPDATE
    measurement = calculate_moving_avg(rc_load_time())
    print("Current value = " + str(measurement))
    plt_y_range.append(measurement)
    plt_y_range = plt_y_range[-x_length:]
    line.set_ydata(plt_y_range)
   
    # Automatic plot recale
    # update Y_MAX if there is a new maximum
    if Y_MAX < measurement:
        Y_MAX = measurement

    # cyclic update
    if (datetime.datetime.now() - Y_MAX_LAST_UPDATE).total_seconds() > Y_UPDATE_TIMEOUT: 
        if Y_MAX not in plt_y_range: # Y_MAX is not in the last 200 measurements - then update based on the last maximum value
            Y_MAX = max(plt_y_range)
        print("Setting ylim to " + str(Y_MAX))
        axes.set_ylim([0, (Y_MAX+10)])

    return line,  
    
def start_plotting():
    global fig
    anim = animation.FuncAnimation(
            fig,
            animate,
            interval=1
        )
    plt.show()

def main():
    print("Start receiving light measurement")

    try:
        start_plotting()
    except:
        GPIO.cleanup()
        print("Finish program on exception.")

if __name__ == "__main__":
    main()
