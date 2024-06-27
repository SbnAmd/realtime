from Python.Class.cpu import CPU
from Python.Class.globals import *

binary_path = './cmake-build-debug/realtime'


if __name__ == '__main__':

    cpu_normal = CPU(4, tasks, 2)
    cpu_normal.run()
    cpu_normal.save()
    #
    # cpu_random = CPU(4, tasks, True)
    # cpu_random.run()
    # cpu_random.save()

    # compare_result()

