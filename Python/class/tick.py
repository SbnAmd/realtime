
class Tick:

    def __init__(self):
        self._tick = 0

    def tick(self):
        self._tick += 1

    def get_tick(self):
        return self._tick