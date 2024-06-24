from Python.Class.task import Task
import os
from colorama import Fore
import select
import json
import time


fifo_file_names = [
    "/tmp/core1_tx",
    "/tmp/core1_rx",
    "/tmp/core2_tx",
    "/tmp/core2_rx",
    "/tmp/core3_tx",
    "/tmp/core3_rx",
    "/tmp/core4_tx",
    "/tmp/core5_rx",

]


def decode_digit(number):
    if number < 10:
        return f'0{number}'
    elif number > 9:
        return f'1{number % 10}'


class Core:
    IDLE = 0
    RUNNING = 1
    temperature_path_list = [
        "/sys/class/hwmon/hwmon3/temp6_input",
        "/sys/class/hwmon/hwmon3/temp7_input",
        "/sys/class/hwmon/hwmon3/temp8_input",
        "/sys/class/hwmon/hwmon3/temp9_input"
    ]

    def __init__(self, core_id, clock):
        global fifo_file_names
        self.status = self.IDLE
        self.core_id = core_id
        self.task = None
        self.clock = clock
        try:
            os.mkfifo(fifo_file_names[core_id*2])
            os.mkfifo(fifo_file_names[core_id*2+1])
        except:
            pass
        self.rx_fd = os.open(fifo_file_names[core_id*2], os.O_RDWR)
        # self.rx_fd = os.open(fifo_file_names[core_id*2], os.O_RDWR | os.O_NONBLOCK)
        self.tx_fd = os.open(fifo_file_names[core_id*2+1], os.O_RDWR)
        self.timeline = []
        self.total_sent = 0

    def unlink(self):
        try:
            os.unlink(fifo_file_names[self.core_id*2])
            os.unlink(fifo_file_names[self.core_id*2 + 1])
        except:
            pass

    def __del__(self):
        os.close(self.rx_fd)
        os.close(self.tx_fd)
        self.unlink()

    def run_task(self, task: Task):
        task.run(self.core_id)
        self.status = self.RUNNING
        self.task = task
        self.send_task_id(task.task_id)

    def send_task_id(self, task_id):
        # print(f'Core {self.core_id} sent : {bytes(str(task_id), "utf-8")}')
        data = bytearray(decode_digit(task_id) + str(self.clock.get_tick()), 'utf-8')
        len_pad = 16 - len(data)
        data.extend(b'\x00' * len_pad)
        self.total_sent += os.write(self.tx_fd, data)
        # print(Fore.YELLOW + f'Core[{self.core_id}] sent task[{task_id}], data = {data}')

    def get_performance_data(self):
        try:
            data_len = os.read(self.rx_fd, 8)
            length = int().from_bytes(data_len,byteorder='little')
            data = os.read(self.rx_fd, length)
            if len(data) < length:
                while len(data) < length:
                    data += os.read(self.rx_fd, length)
            return json.loads(data.decode('utf-8'))
        except:
            return None

    def check_for_ack(self):
        readable, _, _ = select.select([self.rx_fd], [], [], 0)
        if self.rx_fd in readable:
            performance_data = self.get_performance_data()
            self.status = self.IDLE
            self.task.inactivate(performance_data)
            self.task = None

    @classmethod
    def read_fs_var(self, path):
        fd = open(path, "r")
        var = int(fd.readline(32))
        fd.close()
        return var

    def update_task_status(self):
        pass

    def is_free(self):
        return self.status == self.IDLE

    def print_status(self):
        pass
        # if self.status == self.RUNNING:
        #     print(Fore.LIGHTGREEN_EX + f'Core {self.core_id} : {self.task.get_name()}')
        # else:
        #     print(Fore.LIGHTGREEN_EX + f'Core {self.core_id} : IDLE')

    def shutdown(self):
        time.sleep(0.5)
        self.check_for_ack()
        os.write(self.tx_fd, bytes(str(-1), 'utf-8'))
        # time.sleep(0.5)

    def get_temperature(self):
        return int(self.read_fs_var(self.temperature_path_list[self.core_id]) / 1000)



