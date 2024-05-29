import time
import subprocess
import os
from colorama import Fore



prefix_path = '/sys/devices/system/cpu/'
suffix_path = '/cpufreq/'


def change_cpu_frequency(core, frequency: int):
    if frequency > 4300000 or frequency < 800000:
        print(Fore.RED + 'Frequency not available')
        exit(1)

    if isinstance(core, list):
        for _core in core:
            if _core > 15 or _core < 0:
                print(Fore.RED + 'Core not available')
                exit(1)

    elif core > 15 or core < 0:
        print(Fore.RED + 'Core not available')
        exit(1)

    # Check for multiple cores
    if isinstance(core, list):
        for _core in core:
            cmd = [
                f'cd /sys/devices/system/cpu/cpu{_core}/cpufreq;'\
                f'echo {frequency} > scaling_max_freq;'\
                f'echo {frequency} > scaling_min_freq;'\
                f'exit'
            ]
            subprocess.run(cmd)

    else:
        cmd =  f'cd /sys/devices/system/cpu/cpu{core}/cpufreq;'\
               f'sudo echo {frequency} > scaling_max_freq;'\
               f'sudo echo {frequency} > scaling_min_freq;'\
               f'exit'

        # subprocess.run(cmd)
        os.system(cmd)



if __name__ == "__main__":

    print('Changing to 800MHz')
    change_cpu_frequency([i for i in range(4, 16)], 800000)
    # time.sleep(10)
    #
    # print('Changing to 1500MHz')
    # change_cpu_frequency(15, 1500000)
    # time.sleep(10)
    #
    # print('Changing to 2500MHz')
    # change_cpu_frequency(15, 2500000)
    # time.sleep(10)
    #
    # print('Changing to 3500MHz')
    # change_cpu_frequency(15, 3500000)
    # time.sleep(10)
    #
    # print('Changing to 4300MHz')
    # change_cpu_frequency(15, 4000000)
    # time.sleep(10)
    #
    # print('Back to 1500MHz')
    # change_cpu_frequency(15, 1500000)
    exit(0)





