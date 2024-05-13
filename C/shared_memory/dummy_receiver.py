import os

def read_from_shared_memory(shm_name):
    # Open shared memory object
    shm_fd = os.open(shm_name, os.O_RDONLY)
    if shm_fd == -1:
        print("Failed to open shared memory")
        exit(1)

    # Read data from shared memory
    data = os.read(shm_fd, 4096)
    print("Data read from shared memory:", data.decode())

    # Close shared memory
    os.close(shm_fd)

# Main function
def main():
    # Define shared memory name
    shm_name = "/my_shared_memory"

    # Create named pipe (FIFO) for signaling
    fifo_path = "/tmp/signal_pipe"
    if not os.path.exists(fifo_path):
        os.mkfifo(fifo_path)

    # Open named pipe (FIFO) for reading
    signal_fd = os.open(fifo_path, os.O_RDONLY)

    # Main loop to continuously read signal and data
    while True:
        # Wait for signal from C program
        data = os.read(signal_fd, 7)
        print("Data read from shared memory:", data.decode())

        if(data.decode() == "READ9"):
            break

        # Read from shared memory
        # read_from_shared_memory(shm_name)

# Entry point
if __name__ == "__main__":
    main()
