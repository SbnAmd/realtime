
class PerformanceCounter:

    def __init__(self, data, freq, status, tick, temp):
        self.data = data
        self.freq = freq
        self.status = status
        self.tick = tick
        self.temp = temp

    def get_data(self):
        self.data['frequency'] = None
        self.data['status'] = self.status
        self.data['tick'] = self.tick
        self.data['temp'] = self.temp
        return self.data