from Python.Class.core import Core
from Python.Class.task import Task
from Python.Class.schedule import Scheduler, RandomScheduler, SelectiveScheduler
from Python.Class.tick import Tick
from Python.Utils.plot_scheduling import plot
import matplotlib.pyplot as plt
import numpy as np
import subprocess
from colorama import Fore

CYCLES_TO_RUN = 9000
PREVIOUS_ENERGY = 0


class CPU:
    energy_path = "/sys/class/powercap/intel-rapl:0/intel-rapl:0:0/energy_uj"
    freq_path_prefix = "/sys/devices/system/cpu/cpu"

    def __init__(self, core_count, _tasks, scheduler_idx=0):
        self.core_count = core_count
        self.clock = Tick(0.01)
        self.cores = {f'core{i}': Core(i, self.clock) for i in range(core_count)}
        self.tasks = {
            f'{_tasks[i]["name"]}': Task(_tasks[i]['name'], i, _tasks[i]['execution_time'], _tasks[i]['period'],
                                         self.clock) for i in range(len(_tasks))}
        if scheduler_idx == 0:
            self.scheduler = Scheduler(self.tasks, self)
        elif scheduler_idx == 1:
            self.scheduler = RandomScheduler(self.tasks, self)
        elif scheduler_idx == 2:
            self.scheduler = SelectiveScheduler(self.tasks, self)

        self.power_timeline = []
        self.temperature_timeline = [[], [], [], []]

    def free_cores(self):
        num_free_cores = 0
        free_core_dict = {}
        for core_name, core in self.cores.items():
            if core.is_free():
                num_free_cores += 1
                free_core_dict[core_name] = 1
            else:
                free_core_dict[core_name] = 0

        return num_free_cores, free_core_dict

    def check_for_ack(self):
        for _, core in self.cores.items():
            if not core.is_free():
                core.check_for_ack()

    @classmethod
    def read_fs_var(self, path):
        fd = open(path, "r")
        var = int(fd.readline(32))
        fd.close()
        return var

    @classmethod
    def write_fs_var(self, path, value):
        fd = open(path, "w")
        var = fd.writelines(value)
        fd.close()

    def get_power(self):
        global PREVIOUS_ENERGY
        energy_uj = self.read_fs_var(self.energy_path)
        if PREVIOUS_ENERGY != 0:
            power = (energy_uj - PREVIOUS_ENERGY) / 10000
            self.power_timeline.append(power)
        PREVIOUS_ENERGY = energy_uj

    def get_temperature(self):
        for _, core in self.cores.items():
            self.temperature_timeline[core.core_id].append(core.get_temperature())

    def get_missed_deadlines(self):
        cnt = 0
        for _, task in self.tasks.items():
            cnt += task.get_missed_deadlines()
            task.reset_missed_deadlines()
        return cnt

    def schedule(self, drop=None):
        self.get_power()
        self.get_temperature()
        self.scheduler.schedule(n=drop)

    def print_core_status(self):
        for _, core in self.cores.items():
            core.print_status()

    def shutdown(self):
        for _, core in self.cores.items():
            core.shutdown()

    def plot_scheduling(self):
        # [activation_tick, run_tick, inactivation_tick, missed_deadline_tick, core_id, task_name]
        all_timeline = []
        for _, task in self.tasks.items():
            all_timeline.append(task.get_timeline())

        plot(all_timeline)

    def plot_power_and_temperatures(self):
        fig, axs = plt.subplots(5, 1, figsize=(15, 15))

        axs = axs.flatten()

        # Plot each list in a separate subplot
        axs[0].plot(self.power_timeline[20:])
        axs[0].set_title('Power Consumption')
        axs[0].set_xlabel('Time (10ms)')
        axs[0].set_ylabel('Power (w)')

        axs[1].plot(self.temperature_timeline[0])
        axs[1].set_title('Temperature Core 1')
        axs[1].set_xlabel('Time (10ms)')
        axs[1].set_ylabel('Temperature (C)')

        axs[2].plot(self.temperature_timeline[1])
        axs[2].set_title('Temperature Core 2')
        axs[2].set_xlabel('Time (10ms)')
        axs[2].set_ylabel('Temperature (C)')

        axs[3].plot(self.temperature_timeline[2])
        axs[3].set_title('Temperature Core 3')
        axs[3].set_xlabel('Time (10ms)')
        axs[3].set_ylabel('Temperature (C)')

        axs[4].plot(self.temperature_timeline[3])
        axs[4].set_title('Temperature Core 4')
        axs[4].set_xlabel('Time (10ms)')
        axs[4].set_ylabel('Temperature (C)')

        # Adjust layout
        plt.tight_layout()
        # Display the plot
        plt.show()

    def start_cores(self):
        binary_command = "./cmake-build-debug/realtime"

        # Run the binary in another process without blocking
        process = subprocess.Popen(
            binary_command,
            stdout=subprocess.PIPE,  # Redirect stdout to a pipe
            stderr=subprocess.PIPE,  # Redirect stderr to a pipe
            stdin=subprocess.PIPE,  # Redirect stdin to a pipe
            text=True  # Ensure text mode for Python 3.x
        )

    def change_frequency(self, freq, core=-1):
        frequency = freq * 1000000
        if core > -1:
            path1 = self.freq_path_prefix + f'{core + 12}/cpufreq/scaling_max_freq'
            path2 = self.freq_path_prefix + f'{core + 12}/cpufreq/scaling_min_freq'
            self.write_fs_var(path1, frequency)
            self.write_fs_var(path2, frequency)
        else:
            for i in range(12, 16):
                path1 = self.freq_path_prefix + f'{i}/cpufreq/scaling_max_freq'
                path2 = self.freq_path_prefix + f'{i}/cpufreq/scaling_min_freq'
                self.write_fs_var(path1, frequency)
                self.write_fs_var(path2, frequency)

    def get_frequencies(self):
        freqs = []
        for i in range(12, 16):
            path1 = self.freq_path_prefix + f'{i}/cpufreq/scaling_max_freq'
            freqs.append(self.read_fs_var(path1))
        return freqs

    def get_observation(self):
        # todo: return frequencies, free cores, temperatures, power
        # freqs = self.get_frequencies()
        # power = self.power_timeline[-1]
        temps = [core.get_temperature() for _, core in self.cores.items()]
        free_cores, _ = self.free_cores()
        avg_temp = sum(temps)/len(temps)
        missed_deadlines = self.get_missed_deadlines()
        return [free_cores, avg_temp, missed_deadlines]

    def run(self):
        print('Starting scheduler')
        self.start_cores()
        self.clock.register_function(self.schedule)
        self.clock.run(CYCLES_TO_RUN)
        self.shutdown()
        # self.plot_scheduling()
        # self.plot_power_and_temperatures()

    def save(self):
        combined_list = list(zip(self.power_timeline, self.temperature_timeline[0], self.temperature_timeline[1],
                                 self.temperature_timeline[2], self.temperature_timeline[3], ))
        numpy_array = np.array(combined_list)
        np.savetxt("scheduling.txt", numpy_array)
