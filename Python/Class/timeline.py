
class TimeLine:
    temperature_path_list = [
        "/sys/class/hwmon/hwmon3/temp6_input",
        "/sys/class/hwmon/hwmon3/temp7_input",
        "/sys/class/hwmon/hwmon3/temp8_input",
        "/sys/class/hwmon/hwmon3/temp9_input"
    ]
    frequency_path_list = [
        "/sys/devices/system/cpu/cpu12/cpufreq/scaling_cur_freq",
        "/sys/devices/system/cpu/cpu13/cpufreq/scaling_cur_freq",
        "/sys/devices/system/cpu/cpu14/cpufreq/scaling_cur_freq",
        "/sys/devices/system/cpu/cpu15/cpufreq/scaling_cur_freq",
    ]

    def __init__(self):
        self.active_tick = None
        self.run_tick = None
        self.inactive_tick = None
        self.task_name = None
        self.core = None
        self.deadline_tick = None
        self.performance_data = None
        self.temperature = None
        self.frequency = None

    def set_activate_tick(self, tick: int):
        self.active_tick = tick

    def set_run_tick(self, tick: int):
        self.run_tick = tick

    def get_run_tick(self):
        return self.run_tick

    def get_duration(self):
        try:
            return self.inactive_tick - self.run_tick
        except:
            return -1

    def set_inactive_tick(self, tick: int):
        self.inactive_tick = tick

    def set_task_name(self, name: str):
        self.task_name = name

    def get_task_name(self):
        return self.task_name

    def set_core(self, core: int):
        self.core = core

    def get_core(self):
        return self.core

    def set_deadline_tick(self, tick):
        self.deadline_tick = tick

    @classmethod
    def read_fs_var(self, path):
        fd = open(path, "r")
        var = int(fd.readline(32))
        fd.close()
        return var

    def get_temperature(self):
        mili_degree = self.read_fs_var(self.temperature_path_list[self.core])
        self.temperature = int(mili_degree/1000)

    def get_frequency(self):
        freq_kHz = self.read_fs_var(self.frequency_path_list[self.core])
        self.frequency = round(freq_kHz / 1000000, 3)

    def set_performance_data(self, performance_data: dict):
        self.performance_data = performance_data
        self.get_frequency()
        self.get_temperature()

