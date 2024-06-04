import asyncio
import threading
import time
import os


fifo_path1 = "/tmp/signal_pipe1"
fifo_path2 = "/tmp/signal_pipe2"


async def async_function():
    print("Starting async function...")
    os.mkfifo(fifo_path1)
    os.mkfifo(fifo_path2)
    length_rx_fd = os.open(fifo_path1, os.O_RDWR)
    length_tx_fd = os.open(fifo_path2, os.O_RDWR)
    while True:
        data = os.read(length_rx_fd, 4)
        data = int.from_bytes(data, byteorder='little', signed=True)
        print(data)
        if data < 0 :
            break
        else:
            os.write(length_tx_fd, bytes(str(data*2), 'utf-8'))
            print(len(str(data*2)))
    print("Async function completed")

async def main():
    print("Starting main...")

    # Schedule the async function to run in the event loop
    async_task = asyncio.create_task(async_function())

    await async_task

    print("Main completed")

# Run the event loop
asyncio.run(main())
