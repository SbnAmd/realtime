from time import sleep
from colorama import Fore


class Tick:

    def __init__(self, period):
        self._tick = 0
        self.period = period
        self.functions = []

    def tick(self):
        # print(Fore.CYAN + f'********************* clock {self.get_tick() * self.period * 1000} *********************')
        for function in self.functions:
            function()
        self._tick += 1

    def get_tick(self):
        if self._tick == -1:
            return 0
        return self._tick

    def register_function(self, func):
        self.functions.append(func)

    def run(self, cycles):
        sleep(10 * self.period)
        for i in range(cycles):
            sleep(self.period)
            self.tick()

