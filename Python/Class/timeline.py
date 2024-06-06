
class TimeLine:

    def __init__(self):
        self.active_tick = None
        self.run_tick = None
        self.inactive_tick = None
        self.task_name = None
        self.core = None
        self.deadline_tick = None
        self.performance_data = None

    def set_activate_tick(self, tick: int):
        self.active_tick = tick

    def set_run_tick(self, tick: int):
        self.run_tick = tick

    def get_run_tick(self):
        return self.run_tick

    def get_duration(self):
        try:
            return self.inactive_tick - self.run_tick
        except:
            return -1

    def set_inactive_tick(self, tick: int):
        self.inactive_tick = tick

    def set_task_name(self, name: str):
        self.task_name = name

    def get_task_name(self):
        return self.task_name

    def set_core(self, core: int):
        self.core = core

    def get_core(self):
        return self.core

    def set_deadline_tick(self, tick):
        self.deadline_tick = tick

    def set_performance_data(self, performance_data: dict):
        self.performance_data = performance_data