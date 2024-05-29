import subprocess
from Python.Class.cpu import CPU
from Python.Class.map import Mapper
from Python.Class.schedule import Scheduler
from Python.Class.globals import *

binary_path = './cmake-build-debug/realtime'


if __name__ == '__main__':
    cpu = CPU(Scheduler, Mapper, 4, tasks)
    cpu.run()