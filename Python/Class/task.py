from enum import Enum

import numpy as np
from colorama import Fore
from Python.Class.tick import Tick
from Python.Class.performance_counter import PerformanceCounter as PerfCount
import pandas as pd


class TaskStatus(Enum):
    INACTIVE = 0
    ACTIVE = 1
    RUNNING = 2


class Task(Tick):

    def __init__(self, name, period, execution_time):
        super().__init__()
        self.task_name = name
        self.period = period
        self.execution_time = execution_time
        self.missed_deadlines = []

        self.status = TaskStatus.INACTIVE

        # Execution
        self.timeline = list()

    def get_task_name(self):
        return self.task_name

    def add_to_timeline(self, status: dict, temp):
        new_perfcount = PerfCount(status, None, self.get_status_str(), self.get_tick(), temp)
        self.timeline.append(new_perfcount)

    def set_status(self, status):
        self.status = status

    def get_status(self):
        return self.status

    def get_status_str(self):
        if self.status == TaskStatus.INACTIVE:
            return "INACTIVE"
        elif self.status == TaskStatus.ACTIVE:
            return "ACTIVE"
        elif self.status == TaskStatus.RUNNING:
            return "RUNNING"

    def check_deadline(self):
        # Check if task started next period
        if self.get_tick() % self.period == 0 and self.get_tick() != 0:
            # Check if task is running or active
            if self.status is not TaskStatus.INACTIVE:
                self.missed_deadlines.append(self.get_tick())
                # print(Fore.RED + f'Task {self.task_name} missed deadline, deadline = {self.get_tick()}')

    def check_activation(self):
        # Check if task started new period
        if self.get_tick() % self.period == 0 and self.status == TaskStatus.INACTIVE:
            self.set_status(TaskStatus.ACTIVE)
            # print(Fore.WHITE + f'Task {self.task_name} activated at {self.get_tick()}')

    def check_inactivation(self, status):
        # Check if task is done in last execution
        if status['status'] == 0 and self.status == TaskStatus.RUNNING:
            self.set_status(TaskStatus.INACTIVE)
            # print(Fore.LIGHTGREEN_EX + f'Task {self.task_name} inactivated')

    def update_status(self, status, temp: float):
        # Check if missed deadline
        self.check_deadline()

        # Check if task is running
        if status is not None:
            # Save Execution data
            self.add_to_timeline(status, temp)
            # Check if task must be inactive
            self.check_inactivation(status)

        # Activate task
        self.check_activation()

    def report(self):
        print(f'{self.get_task_name()} is reporting')
        perfcount_0 = self.timeline[0]
        data = {
                 'name': [],
                 'core_id': [],
                 'cpu_cycles': [],
                 'cpu_instructions': [],
                 'cpu_cache_misses': [],
                 'cpu_cache_references': [],
                 'cpu_branch_misses': [],
                 'cpu_branch_instructions': [],
                 'cpu_page_faults': [],
                 'cpu_context_switches': [],
                 'cpu_migrations': [],
                 'duration': [],
                 'status': [],
                 'frequency': [],
                 'tick': [],
                 'start_tick': [],
                 'end_tick': [],
                 'temp': []
            }

        for perfcount in self.timeline:
            perfcount_data = perfcount.get_data()
            print(perfcount_data)
            for metric in data:
                if metric in perfcount_data:
                    data[metric].append(perfcount_data[metric])
                else:
                    data[metric].append(np.NaN)
        df = pd.DataFrame(data=data).to_excel(f'Result/{self.get_task_name()}.xlsx')

