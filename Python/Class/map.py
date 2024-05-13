
class Mapper:

    def __init__(self):
        pass

    def map(self, cores, next_tasks: list):
        map_dict = dict()
        for core in cores:
            if cores[core].is_free():
                map_dict[core] = next_tasks.pop()

        return map_dict