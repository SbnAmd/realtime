from enum import Enum
from old_Python.Class.tick import Tick
from old_Python.Class.performance_counter import PerformanceCounter as PerfCount


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

    def set_status(self, status: CoreStatus):
        self.status = status

    def get_current_status(self):
        return self.status

    def get_idx(self):
        return self.core_idx

    def get_running_task_name(self):
        return self.current_task

    def add_to_timeline(self, status, temp):
        new_perfcount = PerfCount(status, None, self.status, self.get_tick(), temp)
        self.timeline.append(new_perfcount)

    def update_status(self, status: dict, temp):
        self.add_to_timeline(status, temp)
        # Check if no task is running
        if status['status'] == 0:
            self.set_status(CoreStatus.IDLE)

    def is_free(self):
        return self.status == CoreStatus.IDLE

