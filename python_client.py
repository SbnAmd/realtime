import socket
import json
import os
from multiprocessing import shared_memory
import array

HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 8080         # The port used by the server

def main():

    # Define shared memory name
    shm_name = "my_shared_memory1"

    # Create named pipe (FIFO) for signaling
    fifo_path1 = "/tmp/signal_pipe1"
    fifo_path2 = "/tmp/signal_pipe2"
    fifo_path3 = "/tmp/signal_pipe3"
    if not os.path.exists(fifo_path1):
        os.mkfifo(fifo_path1)
        os.mkfifo(fifo_path2)
        os.mkfifo(fifo_path3)

    # Open named pipe (FIFO) for reading
    signal_fd1 = os.open(fifo_path1, os.O_RDWR)
    signal_fd2 = os.open(fifo_path2, os.O_RDWR)
    signal_fd3 = os.open(fifo_path3, os.O_RDWR)
    # shm_fd = shared_memory.SharedMemory(name=shm_name,create=True, size=2048)
    while True:
        # Receive data from server
        print("waiting for data")

        data = os.read(signal_fd1, 8)

        length = int().from_bytes(data,byteorder='little')
        # os.write(signal_fd,b'1')
        # data = s.recv(length)
        data = os.read(signal_fd2, length)
        print('Received data len:', length)
        # Deserialize JSON data
        print(data)

        data = json.loads(data.decode('utf-8'))


        # Send data to server
        # s.sendall(b'1234')
        os.write(signal_fd3,b'05020304')
        print("schedule sent")
        # shm_fd.buf[:4] = b'1234'

        # Optional: Break the loop or add conditions to stop receiving/sending
        # break









# Entry point
if __name__ == "__main__":
    main()