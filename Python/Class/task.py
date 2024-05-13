from enum import Enum
from Python.Class.tick import Tick


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

    def add_to_timeline(self, time_slice: dict):
        self.timeline.append(time_slice)

    def set_status(self, status):
        self.status = status

    def get_status(self):
        return self.status

    def update_status(self, status, temp: float):

        if status is not None:
            self.update_abs_values(status)
            self.add_to_timeline(self.create_time_slice(status, temp))

            if status['status'] == 0:
                self.set_status(TaskStatus.INACTIVE)

        if self.get_tick() % self.period == 0:
            self.set_status(TaskStatus.ACTIVE)

    def create_time_slice(self, status: dict, temp):
        time_slice = dict()
        if self.status == TaskStatus.RUNNING:
            time_slice['cycles'] = status['cpu_cycles']
            time_slice['instructions'] = status['cpu_instructions']
            time_slice['cache_misses'] = status['cpu_cache_misses']
            time_slice['cache_references'] = status['cpu_cache_references']
            time_slice['branch_misses'] = status['cpu_branch_misses']
            time_slice['branch_instructions'] = status['cpu_branch_instructions']
            time_slice['page_faults'] = status['cpu_page_faults']
            time_slice['migrations'] = status['cpu_migrations']
            time_slice['context_switches'] = status['cpu_context_switches']
            time_slice['frequency'] = None
            time_slice['temperature'] = temp
            time_slice['duration'] = status['duration']
        time_slice['status'] = self.status
        time_slice['tick'] = self.get_tick()

        return time_slice

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