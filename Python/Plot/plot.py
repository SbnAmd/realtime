import matplotlib.pyplot as plt

# Sample data
tasks = [
    {'task': 'Task 1', 'core': 1, 'start': 0, 'duration': 30000},
    {'task': 'Task 2', 'core': 2, 'start': 15000, 'duration': 45000},
    {'task': 'Task 3', 'core': 1, 'start': 45000, 'duration': 20000},
    {'task': 'Task 4', 'core': 3, 'start': 60000, 'duration': 40000},
    {'task': 'Task 5', 'core': 2, 'start': 70000, 'duration': 25000}
]

# Plotting
fig, ax = plt.subplots(figsize=(10, 6))

# Create a list of unique cores
cores = sorted(set(task['core'] for task in tasks))
y_ticks = [core for core in cores]
y_labels = [f'Core {core}' for core in cores]

# Plot each task as a horizontal bar
for task in tasks:
    ax.broken_barh([(task['start'], task['duration'])], (task['core'] - 0.4, 0.8), facecolors=('tab:blue'))

# Set the y-ticks to be the core numbers
ax.set_yticks(y_ticks)
ax.set_yticklabels(y_labels)

# Format the x-axis to show milliseconds
ax.set_xlabel('Time (ms)')
ax.set_ylabel('CPU Core')
ax.set_title('Real-Time Scheduling on Multi-Core CPU')

# Show grid
ax.grid(True)

# Display the plot
plt.tight_layout()
plt.show()
