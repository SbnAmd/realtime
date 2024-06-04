from tick import Tick


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

    def run(self):
        if self.status == self.ACTIVE:
            self.status = self.RUNNING
            self.timeline[-1][1] = self.clock.get_tick()

    def check_activation(self):
        if self.status == self.INACTIVE:
            if self.clock.get_tick() % self.period == 0:
                self.activate()

    def activate(self):
        if self.status == self.INACTIVE:
            self.status = self.ACTIVE
            self.instance += 1
            self.deadline = self.execution_time + self.clock.get_tick()
            self.timeline.append([self.clock.get_tick(), 0, 0, -1])

    def inactivate(self):
        if self.status == self.RUNNING:
            self.status = self.INACTIVE
            self.timeline[-1][2] = self.clock.get_tick()

    def check_missed_deadline(self):
        if self.status != self.INACTIVE and self.clock.get_tick() > self.deadline:
            self.timeline[-1][3] = self.clock.get_tick()
