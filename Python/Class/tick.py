
class Tick:

    def __init__(self):
        self._tick = -1

    def tick(self):
        self._tick += 1

    def get_tick(self):
        if self._tick == -1:
            return 0
        return self._tick