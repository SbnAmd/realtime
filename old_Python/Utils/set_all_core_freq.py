import os

for core in range(16):
    # Command to execute
    command = f'sudo cpufreq-set -c {core} -g userspace'
    command = f'sudo cpufreq-set -c {core} -f {800000}'

    # Run the command
    exit_code = os.system(command)