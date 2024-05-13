from enum import Enum
from Python.Class.cpu_core import Core
from Python.Class.task import Task, TaskStatus
from Python.Class.tick import Tick
from Python.Class.globals import task_IDes
# from Test.status_generator import StatusGenerator

import os

fifo_path1 = "/tmp/signal_pipe1"
fifo_path2 = "/tmp/signal_pipe2"
fifo_path3 = "/tmp/signal_pipe3"


class CPUStatus(Enum):
    IDLE = 0
    RUNNING = 1


class CPU(Tick):

    def __init__(self, scheduler, mapper, core_count, tasks):
        super().__init__()
        self.scheduler = None
        self.mapper = None
        self.core_count = core_count
        self.task_list = list()
        self.cores = dict()
        self.power_timeline = list()
        self.energy_timeline = list()

        # Scheduling
        self.next_tasks = None
        self.task_map = None
        self.free_cores = dict()

        # Init
        self.init_tasks(tasks)
        self.init_cores()
        self.init_scheduler(scheduler)
        self.init_mapper(mapper)

        # Communication with C code
        # fixme : commented for test
        # self.init_pipes()
        self.length_rx_fd = None
        self.status_rx_fd = None
        self.task_tx_fd = None

        # todo: for test
        # self.fake_generator = StatusGenerator()

    def __del__(self):
        os.close(self.length_rx_fd)
        os.close(self.status_rx_fd)
        os.close(self.task_tx_fd)

    def init_cores(self):
        for idx in range(self.core_count):
            self.cores[str(idx)] = Core(idx)

    def init_scheduler(self, scheduler):
        self.scheduler = scheduler()

    def init_mapper(self, mapper):
        self.mapper = mapper()

    def init_tasks(self, tasks):
        for task in tasks:
            self.task_list.append(Task(task['name'], task['period'], task['execution_time']))

    def init_pipes(self):
        if not os.path.exists(fifo_path1):
            os.mkfifo(fifo_path1)
            os.mkfifo(fifo_path2)
            os.mkfifo(fifo_path3)

        # Open named pipe (FIFO) for reading
        self.length_rx_fd = os.open(fifo_path1, os.O_RDWR)
        self.status_rx_fd = os.open(fifo_path2, os.O_RDWR)
        self.task_tx_fd = os.open(fifo_path3, os.O_RDWR)

    def get_execution_status(self):
        status_data = self.fake_generator.gen_status()
        # todo: get data from C
        self.cores['0'].update_status(status_data['performance_counters']['core0'], status_data['temperatures']['core8'])
        self.cores['1'].update_status(status_data['performance_counters']['core1'], status_data['temperatures']['core9'])
        self.cores['2'].update_status(status_data['performance_counters']['core2'], status_data['temperatures']['core10'])
        self.cores['3'].update_status(status_data['performance_counters']['core3'], status_data['temperatures']['core11'])

        # Task status update
        for task in self.task_list:
            if task.get_task_name() == status_data['performance_counters']['core0']['name']:
                task.update_status(status_data['performance_counters']['core0'], status_data['temperatures']['core8'])
            elif task.get_task_name() == status_data['performance_counters']['core1']['name']:
                task.update_status(status_data['performance_counters']['core1'], status_data['temperatures']['core9'])
            elif task.get_task_name() == status_data['performance_counters']['core2']['name']:
                task.update_status(status_data['performance_counters']['core2'], status_data['temperatures']['core10'])
            elif task.get_task_name() == status_data['performance_counters']['core3']['name']:
                task.update_status(status_data['performance_counters']['core3'], status_data['temperatures']['core11'])
            else:
                task.update_status(None, None)

        self.power_timeline.append(status_data['power'])
        self.energy_timeline.append(status_data['energy'])

    def schedule(self):
        self.scheduler.schedule(self.cores, self.free_cores, self)

    def map_to_core(self):
        self.task_map = self.mapper.map(self.cores, self.next_tasks)

    def execute(self):
        byte_array = bytearray()
        for core_id in ['0', '1', '2', '3']:
            if core_id in self.task_map:
                byte_array.extend(task_IDes[self.task_map[core_id]][0].encode())
                byte_array.extend(task_IDes[self.task_map[core_id]][1].encode())
            else:
                byte_array.extend('0'.encode())
                byte_array.extend('0'.encode())
        # fixme : commented for test
        # os.write(self.task_tx_fd, byte_array)
        for core in self.cores:
            self.cores[core].tick()
        for task in self.task_list:
            task.tick()

    def run_frequency_scaling(self):
        pass

    def run(self):
        while True:
            self.get_execution_status()
            self.schedule()
            self.map_to_core()
            self.execute()
