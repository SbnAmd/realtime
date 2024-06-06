from Python.Class.task import Task
import os
from colorama import Fore
import select
import json


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


class Core:
    IDLE = 0
    RUNNING = 1

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
        self.rx_fd = os.open(fifo_file_names[core_id*2], os.O_RDWR | os.O_NONBLOCK)
        self.tx_fd = os.open(fifo_file_names[core_id*2+1], os.O_RDWR | os.O_NONBLOCK)
        self.timeline = []

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
        self.timeline.append([task.get_name(), self.clock.get_tick(), 0])

    def send_task_id(self, task_id):
        # print(f'Core {self.core_id} sent : {bytes(str(task_id), "utf-8")}')
        os.write(self.tx_fd, bytes(str(task_id), 'utf-8'))

    def get_performance_data(self):
        data_len = os.read(self.rx_fd, 8)
        length = int().from_bytes(data_len,byteorder='little')
        data = os.read(self.rx_fd, length)
        return json.loads(data.decode('utf-8'))

    def check_for_ack(self):
        readable, _, _ = select.select([self.rx_fd], [], [], 0)
        if self.rx_fd in readable:
            performance_data = self.get_performance_data()
            self.status = self.IDLE
            self.task.inactivate(performance_data)
            self.task = None
            self.timeline[-1][2] = self.clock.get_tick()
            print(Fore.YELLOW + f'Core {self.core_id} acked')
        else:
            print(Fore.LIGHTRED_EX + f'Core {self.core_id} not acked')

    def update_task_status(self):
        pass

    def is_free(self):
        return self.status == self.IDLE

    def print_status(self):
        if self.status == self.RUNNING:
            print(Fore.LIGHTGREEN_EX + f'Core {self.core_id} : {self.task.get_name()}')
        else:
            print(Fore.LIGHTGREEN_EX + f'Core {self.core_id} : IDLE')

    def shutdown(self):
        os.write(self.tx_fd, bytes(str(-1), 'utf-8'))

