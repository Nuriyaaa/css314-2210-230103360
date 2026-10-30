
import time
import math
from multiprocessing import Pool


def heavy_math(n):
    return sum(math.sin(i) * math.cos(i) for i in range(1, n))


def run_experiment(workers, total_chunks=16, chunk_size=2_000_000):
    t0 = time.perf_counter()

    time.sleep(0.40)  # Intentional sequential bottleneck

    with Pool(processes=workers) as pool:
        pool.map(heavy_math, [chunk_size] * total_chunks)

    return time.perf_counter() - t0


def karp_flatt(t1, tp, p):
    return ((p * tp / t1) - 1.0) / (p - 1.0)


if __name__ == "__main__":
    cores = [1, 2, 4]
    runtimes = {}

    for p in cores:
        runtimes[p] = run_experiment(p)
        print(f"P={p} | Runtime={runtimes[p]:.3f}s")

    t1 = runtimes[1]

    for p in [2, 4]:
        tp = runtimes[p]
        speedup = t1 / tp
        e = karp_flatt(t1, tp, p)

        print(
            f"P={p} | Speedup={speedup:.3f}x "
            f"| Karp-Flatt e={e:.3f}"
        )
