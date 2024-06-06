from Python.Class.task import Task
from heapq import nsmallest
from random import shuffle
from colorama import Fore
from operator import attrgetter
import random


class Scheduler:
    def __init__(self, tasks: dict, cpu):
        self.tasks = tasks
        self.cpu = cpu
        self.check_schedulability()

    def check_schedulability(self):
        ut = 0.0
        for _, task in self.tasks.items():
            ut += (task.execution_time/task.period)
        if ut > self.cpu.core_count:
            print(f'Ths tasks are not scheduable, utilizatoin = {ut}')
            exit(1)

    def check_tasks_activation_and_deadline(self):
        for _, task in self.tasks.items():
            task.check_activation()
            task.check_missed_deadline()

    def get_new_tasks(self, count):
        _tasks = [task for _, task in self.tasks.items() if task.status == task.ACTIVE]
        high_priority_task_ides = nsmallest(count, _tasks, key=attrgetter('deadline'))
        return high_priority_task_ides

    def map_task_to_core(self, free_cores_dict: dict, new_tasks: list):
        shuffle(new_tasks)
        for core_name, is_free in free_cores_dict.items():
            if is_free == 1:
                if len(new_tasks) > 0:
                    self.cpu.cores[core_name].run_task(new_tasks.pop())

    def schedule(self):
        self.cpu.check_for_ack()
        self.check_tasks_activation_and_deadline()
        num_free_cores, free_cores_dict = self.cpu.free_cores()
        if num_free_cores > 0:
            # This is where the selective mechanism occur
            new_count = random.randint(0, num_free_cores)
            # new_tasks = self.get_new_tasks(new_count)
            new_tasks = self.get_new_tasks(num_free_cores)
            self.map_task_to_core(free_cores_dict, new_tasks)
            # print(Fore.CYAN + f'At tick {self.cpu.clock.get_tick()}, scheduled {new_count} tasks')


if __name__ == "__main__":
    tasks = [2, 0, 3, 3, 7, 3, -1, 20]
    print(tasks.index(3))
    print(tasks.index(3))


