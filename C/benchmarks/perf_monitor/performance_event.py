import ctypes
import os
import time
from colorama import Fore


class PerformanceEvent:

    class PerfEventAttr(ctypes.Structure):
        _fields_ = [
            ("type", ctypes.c_uint),
            ("size", ctypes.c_uint),
            ("config", ctypes.c_ulong),
            ("sample_period", ctypes.c_ulong),
            ("sample_type", ctypes.c_ulong),
            ("read_format", ctypes.c_ulong),
            ("flags", ctypes.c_ulong),
            ("wakeup_events", ctypes.c_uint),
            ("bp_type", ctypes.c_uint),
            ("bp_addr", ctypes.c_ulonglong),
            ("bp_len", ctypes.c_ulonglong),
            ("branch_sample_type", ctypes.c_ulong),
            ("sample_regs_user", ctypes.c_ulong),
            ("sample_stack_user", ctypes.c_uint),
            ("clockid", ctypes.c_int),
            ("sample_regs_intr", ctypes.c_ulong),
            ("aux_watermark", ctypes.c_uint),
            ("reserved_1", ctypes.c_uint),
            ("reserved_2", ctypes.c_uint),
        ]

    __NR_perf_event_open = 298  # System call number for perf_event_open (may vary based on kernel version)
    PERF_TYPE_HARDWARE = 0
    PERF_COUNT_HW_CPU_CYCLES = 0
    PERF_COUNT_HW_INSTRUCTIONS = 1
    PERF_COUNT_HW_CACHE_REFERENCES = 2
    PERF_COUNT_HW_CACHE_MISSES = 3
    PERF_COUNT_HW_BRANCH_INSTRUCTIONS = 4
    PERF_COUNT_HW_BRANCH_MISSES = 5
    PERF_FLAG_FD_CLOEXEC = 8

    def __init__(self):
        self.cycles_pe = self.create_performance_event(self.PERF_TYPE_HARDWARE, self.PERF_COUNT_HW_CPU_CYCLES)
        self.instructions_pe = self.create_performance_event(self.PERF_TYPE_HARDWARE, self.PERF_COUNT_HW_INSTRUCTIONS)
        self.cache_miss_pe = self.create_performance_event(self.PERF_TYPE_HARDWARE, self.PERF_COUNT_HW_CACHE_MISSES)
        self.cache_ref_pe = self.create_performance_event(self.PERF_TYPE_HARDWARE, self.PERF_COUNT_HW_CACHE_REFERENCES)
        self.branch_miss_pe = self.create_performance_event(self.PERF_TYPE_HARDWARE, self.PERF_COUNT_HW_BRANCH_MISSES)
        self.branch_instructions_pe = self.create_performance_event(self.PERF_TYPE_HARDWARE, self.PERF_COUNT_HW_BRANCH_INSTRUCTIONS)

    def create_performance_event(self, event_type, config):
        pe = self.PerfEventAttr()
        pe.type = event_type
        pe.size = ctypes.sizeof(self.PerfEventAttr)
        pe.config = config
        pe.flags = 0
        pe.sample_period = 0
        pe.sample_type = 0
        pe.read_format = 0
        pe.wakeup_events = 0

        return pe

    def perf_event_open(self, attr, pid, cpu, group_fd, flags):
        libc = ctypes.CDLL('libc.so.6', use_errno=True)
        return libc.syscall(self.__NR_perf_event_open, ctypes.byref(attr), pid, cpu, group_fd, flags)

    def perf_fd_open(self, pe, core):
        fd = self.perf_event_open(pe, -1, core, -1, 0)
        if fd == -1:
            errno = ctypes.get_errno()
            raise OSError(errno, os.strerror(errno))

        # Reset and start the counter
        libc = ctypes.CDLL('libc.so.6')
        libc.ioctl(fd, 0x2400, 0)  # PERF_EVENT_IOC_RESET
        libc.ioctl(fd, 0x2401, 0)  # PERF_EVENT_IOC_ENABLE

        return fd

    def capture(self, core, wait_time=1):
        fds = {
            "CPU cycles": self.perf_fd_open(self.cycles_pe, core),
            "Instructions": self.perf_fd_open(self.instructions_pe, core),
            "Cache references": self.perf_fd_open(self.cache_ref_pe, core),
            "Cache misses": self.perf_fd_open(self.cache_miss_pe, core),
            "Branch instructions": self.perf_fd_open(self.branch_instructions_pe, core),
            "Branch misses": self.perf_fd_open(self.branch_miss_pe, core),
        }

        time.sleep(wait_time)

        libc = ctypes.CDLL('libc.so.6')
        cache_misses = ctypes.c_longlong()
        cache_refs = ctypes.c_longlong()
        branch_misses = ctypes.c_longlong()
        branch_instructions = ctypes.c_longlong()
        libc.ioctl(fds["CPU cycles"], 0x2402, 0)  # PERF_EVENT_IOC_DISABLE
        libc.ioctl(fds["Instructions"], 0x2402, 0)  # PERF_EVENT_IOC_DISABLE
        libc.ioctl(fds["Cache references"], 0x2402, 0)  # PERF_EVENT_IOC_DISABLE
        libc.ioctl(fds["Cache misses"], 0x2402, 0)  # PERF_EVENT_IOC_DISABLE
        libc.ioctl(fds["Branch instructions"], 0x2402, 0)  # PERF_EVENT_IOC_DISABLE
        libc.ioctl(fds["Branch misses"], 0x2402, 0)  # PERF_EVENT_IOC_DISABLE

        libc.read(fds["Cache references"], ctypes.byref(cache_refs), ctypes.sizeof(cache_refs))
        libc.read(fds["Cache misses"], ctypes.byref(cache_misses), ctypes.sizeof(cache_misses))
        libc.read(fds["Branch misses"], ctypes.byref(branch_misses), ctypes.sizeof(branch_misses))
        libc.read(fds["Branch instructions"], ctypes.byref(branch_instructions), ctypes.sizeof(branch_instructions))
        cache_miss_rate = float(cache_misses.value/cache_refs.value)
        branch_miss_rate = float(branch_misses.value/branch_instructions.value)
        print(Fore.YELLOW + f'Cache miss rate {round(cache_miss_rate*100)} %, and branch miss rate {round(branch_miss_rate*100)} % on  core {core}')

        for event_name, fd in fds.items():
            os.close(fd)  # Close the file descriptor


if __name__ == "__main__":
    perf = PerformanceEvent()

    while(True):
        perf.capture(12)