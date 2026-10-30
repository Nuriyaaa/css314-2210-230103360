
import time
from multiprocessing import Pool


def private_worker(iterations):
    local_sum = 0

    for _ in range(iterations):
        local_sum += 1

    return local_sum


def run_privatized(p, total_increments=20_000_000):
    increments_per_worker = total_increments // p

    tasks = [increments_per_worker] * p

    t0 = time.perf_counter()

    with Pool(processes=p) as pool:
        results = pool.map(private_worker, tasks)

    total = sum(results)

    t1 = time.perf_counter()

    assert total == total_increments

    return t1 - t0


def karp_flatt(t1, tp, p):
    return ((p * tp / t1) - 1.0) / (p - 1.0)


if __name__ == "__main__":
    cores = [1, 2, 4]
    runtimes = {}

    for p in cores:
        runtimes[p] = run_privatized(p)
        print(f"P={p} | Runtime={runtimes[p]:.3f}s")

    t1 = runtimes[1]

    for p in [2, 4]:
        tp = runtimes[p]
        speedup = t1 / tp
        efficiency = speedup / p * 100
        e = karp_flatt(t1, tp, p)

        print(
            f"P={p} | Speedup={speedup:.3f}x "
            f"| Efficiency={efficiency:.1f}% "
            f"| Karp-Flatt e={e:.3f}"
        )
