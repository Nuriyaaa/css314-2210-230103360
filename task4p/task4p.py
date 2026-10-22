
import multiprocessing as mp
from multiprocessing import shared_memory
import time
import numpy as np

STUDENT_ID = 230103360
ARRAY_SIZES = [100_000, 1_000_000, 10_000_000]


def pickle_worker(conn):
    conn.send("ready")
    while True:
        data = conn.recv()
        if data is None:
            break
        result = float(np.sum(data))
        conn.send(result)
    conn.close()


def shared_memory_worker(conn):
    conn.send("ready")
    while True:
        message = conn.recv()
        if message is None:
            break

        name, size = message
        shm = shared_memory.SharedMemory(name=name)
        try:
            array = np.ndarray((size,), dtype=np.float64, buffer=shm.buf)
            result = float(np.sum(array))
            conn.send(result)
        finally:
            del array
            shm.close()

    conn.close()


def benchmark_pickle(array):
    parent_conn, child_conn = mp.Pipe()
    process = mp.Process(target=pickle_worker, args=(child_conn,))
    process.start()
    child_conn.close()

    parent_conn.recv()

    start = time.perf_counter()
    parent_conn.send(array)
    result = parent_conn.recv()
    elapsed_ms = (time.perf_counter() - start) * 1000.0

    parent_conn.send(None)
    process.join()
    parent_conn.close()

    if not process.exitcode == 0:
        raise RuntimeError("Pickle worker failed")

    return elapsed_ms, result


def benchmark_shared_memory(array):
    shm = shared_memory.SharedMemory(create=True, size=array.nbytes)

    try:
        shared_array = np.ndarray(array.shape, dtype=array.dtype, buffer=shm.buf)
        np.copyto(shared_array, array)
        del shared_array

        parent_conn, child_conn = mp.Pipe()
        process = mp.Process(
            target=shared_memory_worker,
            args=(child_conn,)
        )
        process.start()
        child_conn.close()

        parent_conn.recv()

        start = time.perf_counter()
        parent_conn.send((shm.name, array.size))
        result = parent_conn.recv()
        elapsed_ms = (time.perf_counter() - start) * 1000.0

        parent_conn.send(None)
        process.join()
        parent_conn.close()

        if not process.exitcode == 0:
            raise RuntimeError("Shared memory worker failed")

        return elapsed_ms, result

    finally:
        shm.close()
        shm.unlink()


def main():
    print("=== TASK 4: MEMORY TRANSFER PROFILE ===")
    print(f"Student ID: {STUDENT_ID}")
    print("Method: CPU multiprocessing")
    print("Data type: float64")
    print()

    for size in ARRAY_SIZES:
        rng = np.random.default_rng(STUDENT_ID + size)
        array = rng.random(size, dtype=np.float64)

        pickle_ms, pickle_sum = benchmark_pickle(array)
        shared_ms, shared_sum = benchmark_shared_memory(array)

        if not np.isclose(pickle_sum, shared_sum, rtol=1e-12, atol=1e-8):
            raise AssertionError(f"Results do not match for N={size}")

        print(f"N = {size:,}")
        print(f"Path A (Pipe + Pickle): {pickle_ms:.3f} ms")
        print(f"Path B (Shared Memory): {shared_ms:.3f} ms")
        print(f"Verification: PASS")
        print()


if __name__ == "__main__":
    mp.freeze_support()
    main()
