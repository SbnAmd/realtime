import subprocess
from old_Python.Class.cpu import CPU
from old_Python.Class.map import Mapper
from old_Python.Class.schedule import Scheduler
from old_Python.Class.globals import *

binary_path = './cmake-build-debug/realtime'


if __name__ == '__main__':
    cpu = CPU(Scheduler, Mapper, 4, tasks)
    cpu.run()