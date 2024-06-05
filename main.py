import subprocess
from Python.Class.cpu import CPU
from old_Python.Class.globals import *
import os

binary_path = './cmake-build-debug/realtime'


if __name__ == '__main__':
    # res = subprocess.run(binary_path, capture_output=True, text=True)
    cpu = CPU(4, tasks)
    cpu.run()