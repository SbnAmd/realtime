from enum import Enum
from Python.Class.tick import Tick


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

        # Temperature
        self.temperature_timeline = list()

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

    def add_to_timeline(self, time_slice: dict):
        self.timeline.append(time_slice)

    def add_to_temperature_timeline(self, temp: float):
        self.temperature_timeline.append(temp)

    def update_status(self, status: dict, temp):
        self.update_abs_values(status)
        self.add_to_timeline(self.create_time_slice(status, temp))
        self.add_to_temperature_timeline(temp)

        if status['status'] == 0:
            self.set_status(CoreStatus.IDLE)

    def create_time_slice(self, status: dict, temp):
        time_slice = dict()
        if self.status == CoreStatus.RUNNING:
            time_slice['task'] = status['name']
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
            time_slice['status'] = status['status']
        time_slice['tick'] = self.get_tick()

        return time_slice

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

