from enum import Enum
from old_Python.Class.cpu_core import Core, CoreStatus
from old_Python.Class.task import Task, TaskStatus
from old_Python.Class.tick import Tick
from Python.Class.globals import task_IDes, tasks
import json
from colorama import Fore
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
        self.free_cores = 0

        self.length_rx_fd = None
        self.status_rx_fd = None
        self.task_tx_fd = None

        # Init
        self.check_schedubility()
        self.init(scheduler, mapper)

    def __del__(self):
        if self.length_rx_fd is not None:
            os.close(self.length_rx_fd)
            os.close(self.status_rx_fd)
            os.close(self.task_tx_fd)

    def check_schedubility(self):
        ut = 0.0
        for t in tasks:
            ut += (t['execution_time']/t['period'])
        if ut > self.core_count:
            print(f'Ths tasks are not scheduable, utilizatoin = {ut}')
            exit(1)

    def init(self, scheduler, mapper):
        # Init cores
        for idx in range(self.core_count):
            self.cores[str(idx)] = Core(idx)
        # Init scheduler and mapper
        self.scheduler = scheduler()
        self.mapper = mapper()

        # Init tasks
        for task in tasks:
            self.task_list.append(Task(task['name'], task['period'], task['execution_time']))

        # Init pipes
        if not os.path.exists(fifo_path1):
            os.mkfifo(fifo_path1)
            os.mkfifo(fifo_path2)
            os.mkfifo(fifo_path3)
        # Open named pipe (FIFO) for reading
        self.length_rx_fd = os.open(fifo_path1, os.O_RDWR)
        self.status_rx_fd = os.open(fifo_path2, os.O_RDWR)
        self.task_tx_fd = os.open(fifo_path3, os.O_RDWR)

    def get_performance_data(self):
        # print("waiting for data")
        data = os.read(self.length_rx_fd, 8)
        length = int().from_bytes(data,byteorder='little')
        if length > 0:
            data = os.read(self.status_rx_fd, length)
            return json.loads(data.decode('utf-8'))
        else:
            print("received last data")
            return None

    def check_free_cores(self):
        for core in self.cores:
            if self.cores[core].is_free():
                return True

        return False

    def update_cores(self, status_data):
        for core_idx in range(4):
            core_temperature = status_data['temperatures'][f'core{core_idx + 8}']
            if f'core{core_idx}' in status_data['performance_counters']:
                core_status_data = status_data['performance_counters'][f'core{core_idx}']
                self.cores[str(core_idx)].update_status(core_status_data, core_temperature)

    def update_tasks(self, status_data):
        print(status_data)
        for task in self.task_list:
            task_name = task.get_task_name()
            for core_idx in range(4):
                if f'core{core_idx}' in status_data['performance_counters']:
                    core_data = status_data['performance_counters'][f'core{core_idx}']
                    if task_name == core_data['name']:
                        task.update_status(core_data, status_data['temperatures'][f'core{core_idx + 8}'])
                        break
            else:  # If no matching task name is found
                task.update_status(None, None)

    def send_new_schedule(self, data):
        json_string = json.dumps(data)
        os.write(self.task_tx_fd, bytes(json_string, 'utf-8'))

    def get_execution_status(self):
        # Get data
        status_data = self.get_performance_data()
        if status_data is not None:
            # Update cores and tasks
            self.update_cores(status_data)
            self.update_tasks(status_data)
            # Add power and energy
            self.power_timeline.append(status_data['power'])
            self.energy_timeline.append(status_data['energy'])

        return status_data

    def schedule(self):
        self.scheduler.schedule(self.cores, self)

    def map_to_core(self):
        # New tasks are mapped to cores
        if len(self.next_tasks) > 0:
            self.task_map = self.mapper.map(self.cores, self.next_tasks)

    def execute(self):
        schedule_data = {}
        self.free_cores = 0

        for core_id in ['0', '1', '2', '3']:
            if self.cores[core_id].is_free():
                self.free_cores += 1
     
        for core_id in ['0', '1', '2', '3']:
            if core_id in self.task_map:
                task_code = self.task_map.get(core_id, '0')
                task_id = task_IDes.get(task_code, '00')
                schedule_data[f'core{core_id}'] = int(task_id)

                # Change mapped cores and tasks status
                self.cores[core_id].set_status(CoreStatus.RUNNING)
                for task in self.task_list:
                    if task.get_task_name() == self.task_map[core_id]:
                        task.set_status(TaskStatus.RUNNING)
                        self.free_cores -= 1
            else:
                schedule_data[f'core{core_id}'] = -1

        # print(Fore.RED + f'Free cores after schedule= {self.free_cores}')
        self.send_new_schedule(schedule_data)
        self.task_map = {}

        # Tick tasks and cores
        for core in self.cores:
            self.cores[core].tick()
        for task in self.task_list:
            task.tick()
        return schedule_data

    def run_frequency_scaling(self):
        pass

    def inform(self, status_data, schedule_data, T):
        stat_list = []
        task_name_list = []
        for core_idx in range(4):
            core_status_data = status_data['performance_counters'][f'core{core_idx}']
            stat_list.append(core_status_data['status'])

        print(Fore.YELLOW + f'recv status:\n '
                            f'\tcore0 = {stat_list[0]}, \n\tcore1 = {stat_list[1]}, \n\tcore2 = {stat_list[2]}, \n\tcore3 = {stat_list[3]}')
        for core in schedule_data:
            if schedule_data[core] == -1:
                task_name_list.append('None')
            else:
                for task_name in task_IDes:
                    if int(task_IDes[task_name]) == schedule_data[core]:
                        task_name_list.append(task_name)
                        break
        print(Fore.GREEN + f'scheduled tasks:\n \tcore0 --> {task_name_list[0]}, \n\tcore1 --> {task_name_list[1]}, \n\tcore2 --> {task_name_list[2]}, \n\tcore3 --> {task_name_list[3]}')
        print(Fore.CYAN + f'Tasks status:')
        for task in self.task_list:
            print(Fore.CYAN + f'\t{task.get_task_name()}: {task.get_status_str()}')
        print(Fore.MAGENTA + f'*************** T = {T} ***************')

    def run(self):
        T = 0;
        while True:
            status_data = self.get_execution_status()
            if status_data is not None:
                if self.check_free_cores():
                    self.schedule()
                    self.map_to_core()
                schedule_data = self.execute()

                # self.inform(status_data, schedule_data, T)
                T += 1
            else:
                break

        for task in self.task_list:
            task.report()