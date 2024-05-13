import subprocess
from Python.Class.cpu import CPU
from Python.Class.map import Mapper
from Python.Class.schedule import Scheduler

binary_path = './cmake-build-debug/realtime'


tasks = [
    {'name': "Qsort-Large",
     'period': 5,
     'execution_time': 2
     }   ,

    {'name': "Qsort-Small",
     'period': 6,
     'execution_time': 1
     }   ,

    {'name': "Bitcounts-Large",
     'period': 7,
     'execution_time': 5
     }   ,

    {'name': "Bitcounts-Small",
     'period': 8,
     'execution_time': 2
     }   ,

    {'name': "Basicmath-Large",
     'period': 9,
     'execution_time': 6
     }   ,

    {'name': "Basicmath-Small",
     'period': 10,
     'execution_time': 3
     }
]

if __name__ == '__main__':
    cpu = CPU(Scheduler ,Mapper, 4, tasks)
    cpu.run()