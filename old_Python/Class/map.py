
class Mapper:

    def __init__(self):
        pass

    def map(self, cores, next_tasks: list):
        map_dict = dict()
        for core in cores:
            if cores[core].is_free() and len(next_tasks) > 0:
                map_dict[core] = next_tasks.pop()

        return map_dict