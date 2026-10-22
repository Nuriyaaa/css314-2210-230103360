
import hashlib
import os
import time
from concurrent.futures import ThreadPoolExecutor

STUDENT_ID = 230103360
TASK_COUNT = 10_000
HASH_ROUNDS = 500
PAYLOAD = (str(STUDENT_ID).encode() * 100)[:1024].ljust(1024, b"0")


def cpu_task(task_id):
    digest = hashlib.sha256(PAYLOAD + task_id.to_bytes(4, "little")).digest()

    for _ in range(HASH_ROUNDS - 1):
        digest = hashlib.sha256(PAYLOAD + digest).digest()

    return digest


def run_batch(pool_size):
    start = time.perf_counter()

    with ThreadPoolExecutor(max_workers=pool_size) as executor:
        results = list(executor.map(cpu_task, range(TASK_COUNT)))

    elapsed_ms = (time.perf_counter() - start) * 1000.0

    if len(results) != TASK_COUNT:
        raise RuntimeError("Task count verification failed")

    return elapsed_ms


def main():
    cores = os.cpu_count() or 1

    pool_sizes = [
        1,
        max(1, cores // 2),
        cores,
        cores * 2,
        cores * 4,
        cores * 16,
        256
    ]

    pool_sizes = list(dict.fromkeys(pool_sizes))

    print("=== TASK 5: THREAD OVERSUBSCRIPTION THRESHOLD ===")
    print(f"Student ID: {STUDENT_ID}")
    print(f"Logical cores: {cores}")
    print(f"CPU-bound tasks: {TASK_COUNT}")
    print(f"Hash rounds per task: {HASH_ROUNDS}")
    print()
    print(f"{'Pool Size':>10} | {'Execution Time (ms)':>20} | {'Speedup':>10} | {'Status'}")
    print("-" * 68)

    timings = []

    for pool_size in pool_sizes:
        elapsed_ms = run_batch(pool_size)
        timings.append((pool_size, elapsed_ms))

    baseline = timings[0][1]
    worsening_pool = None

    for index, (pool_size, elapsed_ms) in enumerate(timings):
        speedup = baseline / elapsed_ms if elapsed_ms > 0 else 0.0

        if index == 0:
            status = "BASELINE"
        elif worsening_pool is None and elapsed_ms > timings[index - 1][1]:
            worsening_pool = pool_size
            status = "WORSENING START"
        elif worsening_pool is not None and pool_size >= worsening_pool:
            status = "OVERSUBSCRIBED"
        else:
            status = "OK"

        print(
            f"{pool_size:>10} | {elapsed_ms:>20.3f} | "
            f"{speedup:>9.3f}x | {status}"
        )

    print()

    if worsening_pool is not None:
        print(f"First observed worsening pool size: {worsening_pool}")
        print("This is the first measured pool slower than the previous tested size.")
    else:
        print("First observed worsening pool size: Not detected")
        print("Execution time did not increase between adjacent tested pool sizes.")

    print("Note: Timing results are system-dependent and may vary between runs.")


if __name__ == "__main__":
    main()
