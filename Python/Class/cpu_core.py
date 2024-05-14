from enum import Enum
from Python.Class.tick import Tick
from Python.Class.performance_counter import PerformanceCounter as PerfCount


class CoreStatus(Enum):
    IDLE = 0
    RUNNING = 1


class Core(Tick):

    def __init__(self, core_idx: int):
        super().__init__()
        self.status = CoreStatus.IDLE
        self.core_idx = core_idx
        self.current_task = None

        # Execution time
        self.timeline = list()

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

    def set_status(self, status: CoreStatus):
        self.status = status

    def get_current_status(self):
        return self.status

    def get_idx(self):
        return self.core_idx

    def set_running_task(self, task_name):
        self.current_task = task_name

    def get_running_task_name(self):
        return self.current_task

    def add_to_timeline(self, time_slice):
        self.timeline.append(time_slice)

    def update_status(self, status: dict, temp):
        self.update_abs_values(status)
        self.add_to_timeline(self.create_time_slice(status, temp))

        if status['status'] == 0:
            self.set_status(CoreStatus.IDLE)

    def create_time_slice(self, status: dict, temp):
        new_perfcount = PerfCount(status, None, self.status, self.get_tick(), temp)
        return new_perfcount

    def update_abs_values(self, status: dict):
        if self.status == CoreStatus.RUNNING:
            self.cycles += status['cpu_cycles']
            self.instructions += status['cpu_instructions']
            self.cache_misses += status['cpu_cache_misses']
            self.cache_references += status['cpu_cache_references']
            self.branch_misses += status['cpu_branch_misses']
            self.branch_instructions += status['cpu_branch_instructions']
            self.page_faults += status['cpu_page_faults']
            self.context_switches += status['cpu_context_switches']
            self.migrations += status['cpu_migrations']

    def is_free(self):
        return self.status == CoreStatus.IDLE

