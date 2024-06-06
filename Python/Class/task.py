from Python.Class.tick import Tick
from colorama import Fore


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
        self.timeline = []

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
        print(f'hint1, stat = {self.status}')
        if self.status == self.ACTIVE:
            self.status = self.RUNNING
            self.timeline[-1][1] = self.clock.get_tick()
            self.timeline[-1][-2] = core_id
            print(f'hint2')
            print(Fore.GREEN + f'{self.get_name()} started')

    def check_activation(self):
        if self.status == self.INACTIVE:
            if self.clock.get_tick() % self.period == 0:
                self.activate()

    def activate(self):
        if self.status == self.INACTIVE:
            self.status = self.ACTIVE
            self.instance += 1
            self.deadline = self.period + self.clock.get_tick()
            self.timeline.append([self.clock.get_tick(), 0, 0, -1, 0, self.get_name()])
            print(Fore.YELLOW + f'{self.get_name()} activated')

    def inactivate(self):
        if self.status == self.RUNNING:
            self.status = self.INACTIVE
            self.timeline[-1][2] = self.clock.get_tick()
            print(Fore.CYAN + f'{self.get_name()} inactivated')
        else:
            print(Fore.LIGHTRED_EX + f'{self.get_name()} wants to inactivate but its in {self.status} mode')

    def check_missed_deadline(self):
        if self.status != self.INACTIVE and self.clock.get_tick() > self.deadline:
            self.timeline[-1][3] = self.clock.get_tick()
            print(Fore.RED + f'{self.get_name()} missed deadline')

    def get_timeline(self):
        return self.timeline
