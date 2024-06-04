import subprocess
from Python.Class.cpu import CPU
from Python.Class.map import Mapper
from Python.Class.schedule import Scheduler
from Python.Class.globals import *
import time
import matplotlib.pyplot  as plt
import matplotlib.animation as animation
import numpy as np

binary_path = './cmake-build-debug/realtime'

tick = 0
def init():
    ax1.set_xlim(0, 1000)
    ax1.set_ylim(0, 90)

    ax2.set_xlim(0, 1000)
    ax2.set_ylim(0, 5)

    return ln, ln2


def update(frame):
    global tick
    fp = open("/sys/class/hwmon/hwmon3/temp6_input", "r")
    temp = fp.read(32)
    fp.close()
    temp = int(temp)/1000

    fp = open("/sys/devices/system/cpu/cpu12/cpufreq/scaling_cur_freq", "r")
    freq = fp.read(32)
    fp.close()
    freq = int(freq)/1000000

    tick += 1
    xdata.append(tick)
    ydata.append(temp)
    ln.set_data(xdata, ydata)

    xdata_freq.append(tick)
    ydata_freq.append(freq)
    ln2.set_data(xdata_freq, ydata_freq)
    return ln, ln2


if __name__ == '__main__':
    fig, (ax1, ax2) = plt.subplots(2, 1)
    xdata, ydata = [], []
    ln, = ax1.plot([], [], 'r-')

    xdata_freq, ydata_freq = [], []
    ln2, = ax2.plot([], [], 'b-')

    ani = animation.FuncAnimation(fig, update, frames=np.linspace(0, 2000, 1), init_func=init, blit=True, interval=100)

    plt.show()


