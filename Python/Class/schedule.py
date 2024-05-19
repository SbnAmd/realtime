from colorama import Fore
from Python.Class.task import Task, TaskStatus


class Scheduler:

    def __init__(self):
        pass

    def get_high_priority_tasks(self, n, owner):
        deadline_dict = dict()
        if n > 0:
            for task in owner.task_list:
                if task.get_status() == TaskStatus.ACTIVE:
                    deadline = (int(owner.get_tick()/task.period) + 1)*task.period - owner.get_tick()
                    deadline_dict[task.get_task_name()] = deadline
            deadline_dict = {k: v for k, v in sorted(deadline_dict.items(), key=lambda item: item[1])}
            print(Fore.LIGHTCYAN_EX + "New deadlines:")
            for k in deadline_dict:
                print(f'{k} deadline = {deadline_dict[k]}')


        cnt = 0
        owner.next_tasks = []
        for task_name in deadline_dict:
            if cnt >= n:
                break
            owner.next_tasks.append(task_name)
            cnt += 1

    def schedule(self, cores, owner):
        _free_cores = 0
        for core in cores:
            if cores[core].is_free():
                _free_cores += 1

        self.get_high_priority_tasks(_free_cores, owner)

