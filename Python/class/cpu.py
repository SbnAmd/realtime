from enum import Enum
from cpu_core import Core
from task import Task, TaskStatus
from tick import Tick
from globals import task_IDes
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
        self.init_pipes()
        self.length_rx_fd = None
        self.status_rx_fd = None
        self.task_tx_fd = None

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

    def get_high_priority_tasks(self, n):
        deadline_dict = dict()
        if n > 0:
            for task in self.task_list:
                if task.get_status() == TaskStatus.ACTIVE:
                    deadline = (int(self.get_tick()/task.period) + 1)*task.period - self.get_tick()
                    deadline_dict[task.get_task_name()] = deadline
            deadline_dict = {k: v for k, v in sorted(deadline_dict.items(), key=lambda item: item[1])}

        cnt = 0
        self.next_tasks = []
        for task_name in deadline_dict:
            if cnt >= n:
                break
            self.next_tasks.append(task_name)
            cnt += 1

    def get_execution_status(self):
        status_data = dict()
        # todo: get data from C
        self.cores['0'].update_status(status_data['performance_counters']['core0'], status_data['temperatures']['core8'])
        self.cores['1'].update_status(status_data['performance_counters']['core1'], status_data['temperatures']['core9'])
        self.cores['2'].update_status(status_data['performance_counters']['core2'], status_data['temperatures']['core10'])
        self.cores['3'].update_status(status_data['performance_counters']['core3'], status_data['temperatures']['core11'])

        # Task status update
        for task in self.task_list:
            if task.get_task_name() == status_data['performance_counters']['core0']['name']:
                task.update_status(status_data['performance_counters']['core0'])
            elif task.get_task_name() == status_data['performance_counters']['core1']['name']:
                task.update_status(status_data['performance_counters']['core1'])
            elif task.get_task_name() == status_data['performance_counters']['core2']['name']:
                task.update_status(status_data['performance_counters']['core2'])
            elif task.get_task_name() == status_data['performance_counters']['core3']['name']:
                task.update_status(status_data['performance_counters']['core3'])
            else:
                task.update_status(None)

        self.power_timeline.append(status_data['power'])
        self.energy_timeline.append(status_data['energy'])

    def schedule(self):

        free_cores = 0
        for core in self.cores:
            if self.cores[core].is_free():
                free_cores += 1
                self.free_cores[str(core.get_idx())] = True
            else:
                self.free_cores[str(core.get_idx())] = False

        self.get_high_priority_tasks(free_cores)

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
        os.write(self.task_tx_fd, byte_array)

    def run_frequency_scaling(self):
        pass

    def run(self):
        while True:
            self.get_execution_status()
            self.schedule()
            self.map_to_core()
            self.execute()
