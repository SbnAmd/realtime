from task import Task
import os


fifo_file_names = [
    "/core1/tx",
    "/core1/rx",
    "/core2/tx",
    "/core2/rx",
    "/core3/tx",
    "/core3/rx",
    "/core4/tx",
    "/core5/rx",

]


class Core:
    IDLE = 0
    RUNNING = 1

    def __init__(self, core_id):
        self.status = self.IDLE
        self.core_id = core_id
        self.task = None
        os.mkfifo(fifo_file_names[core_id*2])
        os.mkfifo(fifo_file_names[core_id*2+1])
        self.rx_fd = os.open(fifo_file_names[core_id*2], os.O_RDWR)
        self.tx_fd = os.open(fifo_file_names[core_id*2+1], os.O_RDWR)

    def __del__(self):
        os.close(self.rx_fd)
        os.close(self.tx_fd)

    def run_task(self, task: Task):
        task.run()
        self.status = self.RUNNING
        self.task = task
        self.send_task_id(task.task_id)

    def send_task_id(self, task_id):
        os.write(self.tx_fd, bytes(str(task_id), 'utf-8'))

    def wait_for_ack(self):
        _ = os.read(self.rx_fd, 4)
        self.status = self.IDLE
        self.task.instance()
        self.task = None

    def update_task_status(self):
        pass

    def is_free(self):
        return self.status == self.IDLE

