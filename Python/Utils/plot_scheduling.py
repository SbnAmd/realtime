import matplotlib.pyplot as plt
from Python.Class.timeline import TimeLine

# Example data
cores = ['Core 1', 'Core 2', 'Core 3', 'Core 4']
tasks = [
    ['Task 1', 'Task 2'],
    ['Task 3', 'Task 4'],
    ['Task 5', 'Task 6'],
    ['Task 7', 'Task 8']
]
start_times = [
    [1, 5],  # Start times for Core 1
    [2, 6],  # Start times for Core 2
    [3, 7],  # Start times for Core 3
    [4, 8]   # Start times for Core 4
]
durations = [
    [3, 2],  # Durations for Core 1
    [4, 3],  # Durations for Core 2
    [2, 5],  # Durations for Core 3
    [3, 4]   # Durations for Core 4
]


def plot(timelines: list):
    fig, ax = plt.subplots(figsize=(15, 10))

    # Plotting each core's tasks
    for tl_id in range(len(timelines)):
        timeline = timelines[tl_id]
        for task_index in range(len(timeline)-1):   # ignoring last entry
            # Plot each bar for the core
            tl: TimeLine = timeline[task_index]
            task = tl.get_task_name()
            start = tl.get_run_tick()
            duration = tl.get_duration()
            if duration > 0:
                core_index = tl.get_core()
                bar = ax.barh(cores[core_index], duration, left=start, align='center')
                # Add task name as a label on the bar
                # ax.text(start + duration / 2, cores[core_index], task,
                ax.text(start + duration / 2, bar[0].get_y() + 0.05, task,
                        rotation=90, va='bottom', ha='center', color='black', fontsize=7, fontweight='bold', bbox=dict(facecolor='white', alpha=0.8, edgecolor='none'))

    # Add labels and title
    ax.set_xlabel('Time')
    ax.set_ylabel('Cores')
    ax.set_title('Task Scheduling Result for Multiple Cores')

    # Add grid
    ax.grid(True)

    plt.show()

