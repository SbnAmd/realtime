from Python.Class.tick import Tick
from colorama import Fore
from Python.Class.timeline import TimeLine
from copy import deepcopy


class Task:
    INACTIVE = 0
    ACTIVE = 1
    RUNNING = 2

    def __init__(self, name, task_id, execution_time, period, clock: Tick):
        self.status = self.INACTIVE
        self.name = name
        self.task_id = task_id
        self.instance = 0
        self.execution_time = execution_time
        self.period = period
        self.clock = clock
        self.deadline = 0
        self.timeline: list[[TimeLine]] = []

    def get_name(self):
        return self.name

    def get_task_id(self):
        return self.task_id

    def update_status(self):
        pass

    def get_status(self):
        return self.status

    def get_deadline(self):
        if self.status == self.ACTIVE:
            return self.deadline
        else:
            # return inf
            return 1000000

    def run(self, core_id: int):
        if self.status == self.ACTIVE:
            self.status = self.RUNNING
            self.timeline[-1].set_run_tick(self.clock.get_tick())
            self.timeline[-1].set_core(core_id)
            # print(Fore.GREEN + f'{self.get_name()} started')

    def check_activation(self):
        if self.status == self.INACTIVE:
            if self.clock.get_tick() % self.period == 0:
                self.activate()

    def activate(self):
        if self.status == self.INACTIVE:
            self.status = self.ACTIVE
            self.instance += 1
            self.deadline = self.period + self.clock.get_tick()
            tl = TimeLine()
            tl.set_activate_tick(self.clock.get_tick())
            tl.set_task_name(self.get_name())
            self.timeline.append(deepcopy(tl))
            # print(Fore.YELLOW + f'{self.get_name()} activated')

    def inactivate(self, performance_data):
        if self.status == self.RUNNING:
            self.status = self.INACTIVE
            self.timeline[-1].set_inactive_tick(self.clock.get_tick())
            self.timeline[-1].set_performance_data(performance_data)
            # print(Fore.CYAN + f'{self.get_name()} inactivated')
        # else:
            # print(Fore.LIGHTRED_EX + f'{self.get_name()} wants to inactivate but its in {self.status} mode')

    def check_missed_deadline(self):
        if self.status != self.INACTIVE and self.clock.get_tick() > self.deadline:
            self.timeline[-1].set_deadline_tick(self.clock.get_tick())
            # print(Fore.RED + f'{self.get_name()} missed deadline')

    def get_timeline(self):
        return self.timeline
