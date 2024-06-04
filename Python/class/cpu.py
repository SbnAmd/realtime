from core import Core
from task import Task
from old_Python.Class.globals import task_IDes, tasks
import json
from colorama import Fore
# from Test.status_generator import StatusGenerator
import os


class CPU:

    def __init__(self, scheduler, mapper, core_count, _tasks):
        self.scheduler = None
        self.mapper = None
        self.core_count = core_count
        self.cores = {f'core{i}':Core(i) for i in range(core_count)}
        self.tasks = {f'{_tasks[i]["name"]}': Task(_tasks[i]['name'], i, _tasks[i]['execution_time'], _tasks[i]['period'], ) for i in range(len(_tasks))}

        # Init
        self.check_schedubility()
        self.init(scheduler, mapper)

    def check_schedubility(self):
        ut = 0.0
        for t in tasks:
            ut += (t['execution_time']/t['period'])
        if ut > self.core_count:
            print(f'Ths tasks are not scheduable, utilizatoin = {ut}')
            exit(1)

    def check_free_cores(self):
        pass

    def send_new_schedule(self, data):
        pass

    def get_execution_status(self):
        pass

    def schedule(self):
        self.scheduler.schedule(self.cores, self)

    def map_to_core(self):
        pass

    def execute(self):
        pass

    def run_frequency_scaling(self):
        pass

    def run(self):
        pass