import os
import pandas as pd

freq_list = [
            800000,
            1000000,
            1500000,
            2000000,
            2500000,
            3000000,
            3500000,
            4000000,
            4300000,
            ]

for freq in freq_list:
    head = {
        'power': [],
        'core0_temp': [],
        'core1_temp': [],
        'core2_temp': [],
        'core3_temp': [],
        'core4_temp': [],
        'core5_temp': [],
        'core6_temp': [],
        'core7_temp': [],
        'core8_temp': [],
        'core9_temp': [],
        'core10_temp': [],
        'core11_temp': [],
        'core12_temp': [],
        'core13_temp': [],
        'core14_temp': [],
        'core15_temp': [],
            }
    for core in range(16):
        # Command to execute
        command = f'sudo cpufreq-set -c {core} -f {4000000}'

        # Run the command
        exit_code = os.system(command)