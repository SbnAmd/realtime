from enum import Enum
from colorama import Fore
from Python.Class.tick import Tick
from Python.Class.performance_counter import PerformanceCounter as PerfCount


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
        # Absolute performance counters
        self.cycles = 0
        self.instructions = 0
        self.cache_misses = 0
        self.cache_references = 0
        self.branch_misses = 0
        self.branch_instructions = 0
        self.page_faults = 0
        self.context_switches = 0
        self.migrations = 0

        # Temperature
        self.temperature_timeline = list()

        # Execution
        self.timeline = list()

    def get_task_name(self):
        return self.task_name

    def add_to_timeline(self, time_slice):
        self.timeline.append(time_slice)

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
        if self.get_tick() % self.period == 0 and self.get_tick() != 0:
            if self.status is not TaskStatus.INACTIVE:
                self.missed_deadlines.append(self.get_tick())
                print(Fore.WHITE + f'Task {self.task_name} missed deadline')

    def check_activation(self):
        if self.get_tick() % self.period == 0 and self.status == TaskStatus.INACTIVE:
            self.set_status(TaskStatus.ACTIVE)
            print(Fore.WHITE + f'Task {self.task_name} activated at {self.get_tick()}')

    def update_status(self, status, temp: float):

        self.check_deadline()

        if status is not None:
            self.update_abs_values(status)
            self.add_to_timeline(self.create_time_slice(status, temp))

            # Transition just from running to inactive
            if status['status'] == 0 and self.status == TaskStatus.RUNNING:
                self.set_status(TaskStatus.INACTIVE)
                print(Fore.WHITE + f'Task {self.task_name} inactivated')

        self.check_activation()

    def create_time_slice(self, status: dict, temp):
        new_perfcount = PerfCount(status, None, self.status, self.get_tick(), temp)
        return new_perfcount

    def update_abs_values(self, status: dict):
        if self.status == TaskStatus.ACTIVE:
            self.cycles += status['cpu_cycles']
            self.instructions += status['cpu_instructions']
            self.cache_misses += status['cpu_cache_misses']
            self.cache_references += status['cpu_cache_references']
            self.branch_misses += status['cpu_branch_misses']
            self.branch_instructions += status['cpu_branch_instructions']
            self.page_faults += status['cpu_page_faults']
            self.context_switches += status['cpu_context_switches']
            self.migrations += status['cpu_migrations']