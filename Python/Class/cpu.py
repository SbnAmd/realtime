from Python.Class.core import Core
from Python.Class.task import Task
from Python.Class.schedule import Scheduler
from Python.Class.tick import Tick
from Python.Utils.plot_scheduling import plot
from colorama import Fore

CYCLES_TO_RUN = 300000

class CPU:

    def __init__(self, core_count, _tasks):
        self.core_count = core_count
        self.clock = Tick(0.01)
        self.cores = {f'core{i}': Core(i, self.clock) for i in range(core_count)}
        self.tasks = {
            f'{_tasks[i]["name"]}': Task(_tasks[i]['name'], i, _tasks[i]['execution_time'], _tasks[i]['period'],
                                         self.clock) for i in range(len(_tasks))}
        self.scheduler = Scheduler(self.tasks, self)

    def free_cores(self):
        num_free_cores = 0
        free_core_dict = {}
        for core_name, core in self.cores.items():
            if core.is_free():
                num_free_cores += 1
                free_core_dict[core_name] = 1
            else:
                free_core_dict[core_name] = 0

        return num_free_cores, free_core_dict

    def check_for_ack(self):
        for _, core in self.cores.items():
            core.check_for_ack()

    def schedule(self):
        self.scheduler.schedule()

    def print_core_status(self):
        for _, core in self.cores.items():
            core.print_status()

    def shutdown(self):
        for _, core in self.cores.items():
            core.shutdown()

    def plot(self):
        # [activation_tick, run_tick, inactivation_tick, missed_deadline_tick, core_id, task_name]
        all_timeline = []
        for _, task in self.tasks.items():
            all_timeline.append(task.get_timeline())

        plot(all_timeline)

    def run(self):
        print('Starting scheduler')
        self.clock.register_function(self.schedule)
        self.clock.run(CYCLES_TO_RUN)
        self.shutdown()
        self.plot()
