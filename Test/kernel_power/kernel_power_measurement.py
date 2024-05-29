import time
import ctypes
import pandas as pd
import numpy as np

repetition = 1000000
energy = np.zeros(repetition)


def read_energy():
    rapl_path = "/sys/class/powercap/intel-rapl:0/intel-rapl:0:0/energy_uj"
    fd = open(rapl_path, "r")
    energy = int(fd.readline(32))
    fd.close()
    return energy


def calculate_power():
    previous_energy = 0
    cnt = 0
    libc = ctypes.CDLL('libc.so.6')

    i = 0 
    while i < repetition:
        current_energy = read_energy()
        if previous_energy != current_energy:
            energy[i] = current_energy
            previous_energy = current_energy
            i += 1
        libc.usleep(100)
        # print(f'Round {i}')
 

if __name__ == "__main__":
    calculate_power()
    np.savetxt('energy.txt', energy, fmt='%d')
