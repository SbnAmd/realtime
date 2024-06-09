import numpy as np
import matplotlib.pyplot as plt


def compare_result():
    normal = np.loadtxt("normal_scheduling.txt")
    random = np.loadtxt("random_scheduling.txt")

    diff = normal - random[:-1, :]

    fig, axs = plt.subplots(5, 1, figsize=(15, 15))

    axs = axs.flatten()
    window_size = 10
    power_moving_avg = np.convolve(diff[20:, 0], np.ones(window_size)/window_size, mode='valid')
    # Plot each list in a separate subplot
    axs[0].plot(power_moving_avg)
    axs[0].set_title('Power Consumption')
    axs[0].set_xlabel('Time (10ms)')
    axs[0].set_ylabel('Power (w)')
    axs[0].axhline(y=0, color='r', linestyle='--', label='y = 0')

    axs[1].plot(diff[20:, 1])
    axs[1].set_title('Temperature Core 1')
    axs[1].set_xlabel('Time (10ms)')
    axs[1].set_ylabel('Temperature (C)')
    axs[1].axhline(y=0, color='r', linestyle='--', label='y = 0')

    axs[2].plot(diff[20:, 2])
    axs[2].set_title('Temperature Core 2')
    axs[2].set_xlabel('Time (10ms)')
    axs[2].set_ylabel('Temperature (C)')
    axs[2].axhline(y=0, color='r', linestyle='--', label='y = 0')

    axs[3].plot(diff[20:, 3])
    axs[3].set_title('Temperature Core 3')
    axs[3].set_xlabel('Time (10ms)')
    axs[3].set_ylabel('Temperature (C)')
    axs[3].axhline(y=0, color='r', linestyle='--', label='y = 0')

    axs[4].plot(diff[20:, 3])
    axs[4].set_title('Temperature Core 4')
    axs[4].set_xlabel('Time (10ms)')
    axs[4].set_ylabel('Temperature (C)')
    axs[4].axhline(y=0, color='r', linestyle='--', label='y = 0')

    # Adjust layout
    plt.tight_layout()
    # Display the plot
    plt.show()