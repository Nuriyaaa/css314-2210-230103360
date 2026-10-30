
import time
from multiprocessing import Process, Value, Lock


def contended_worker(shared_counter, lock, iterations):
    for _ in range(iterations):
        with lock:
            shared_counter.value += 1


def run_contended(p, total_increments=2_000_000):
    increments_per_worker = total_increments // p

    counter = Value('i', 0)
    lock = Lock()

    processes = [
        Process(
            target=contended_worker,
            args=(counter, lock, increments_per_worker)
        )
        for _ in range(p)
    ]

    t0 = time.perf_counter()

    for proc in processes:
        proc.start()

    for proc in processes:
        proc.join()

    elapsed = time.perf_counter() - t0

    assert counter.value == total_increments

    return elapsed


def karp_flatt(t1, tp, p):
    return ((p * tp / t1) - 1.0) / (p - 1.0)


if __name__ == "__main__":
    cores = [1, 2, 4]
    runtimes = {}

    for p in cores:
        runtimes[p] = run_contended(p)
        print(f"P={p} | Runtime={runtimes[p]:.3f}s")

    t1 = runtimes[1]

    for p in [2, 4]:
        tp = runtimes[p]
        speedup = t1 / tp
        e = karp_flatt(t1, tp, p)

        print(
            f"P={p} | Runtime={tp:.3f}s "
            f"| Speedup={speedup:.2f}x "
            f"| Karp-Flatt e={e:.3f}"
        )
