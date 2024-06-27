import numpy as np
import matplotlib.pyplot as plt


two_drop_data = np.loadtxt("scheduling_4g.txt")
one_drop_data = np.loadtxt("scheduling_3g.txt")
no_drop_data = np.loadtxt("scheduling_2g.txt")

two_temp_avg = np.mean(two_drop_data[:, 1:], axis=1)
one_temp_avg = np.mean(one_drop_data[:, 1:], axis=1)
no_temp_avg = np.mean(no_drop_data[:, 1:], axis=1)

x = np.arange(two_temp_avg.shape[0])

plt.figure(figsize=(10, 6))
x = np.arange(two_temp_avg.shape[0])
plt.plot(x, two_temp_avg, label='2 Drop', linestyle='-', color='r')  # Line style and color can be customized
plt.plot(x, one_temp_avg, label='1 Drop', linestyle='--', color='g')
plt.plot(x, no_temp_avg, label='no drop', linestyle=':', color='b')
#
# x = np.arange(two_temp_avg.shape[0])
# plt.plot(x[2750:], two_drop_data[2750:, 0], label='2 Drop', linestyle='-', color='r')  # Line style and color can be customized
# plt.plot(x[2750:], one_drop_data[2750:, 0], label='1 Drop', linestyle='--', color='g')
# plt.plot(x[2750:], no_drop_data[2750:, 0], label='no drop', linestyle=':', color='b')

# Step 4: Add labels and title
plt.xlabel('X-axis (Index/Time Steps)')
plt.ylabel('Values')
plt.title('Plot of Three Columns from a NumPy Array')

# Add a legend to differentiate the lines
plt.legend()

# Optionally, add grid for better readability
plt.grid(True)

# Show the plot
plt.show()


