import os
import time
import platform
from concurrent.futures import ThreadPoolExecutor

import numpy as np

STUDENT_ID = 230103360

ARRAY_SIZE = 20_000_000
P = 0.95


def amdahl_speedup(p, parallel_fraction=P):
    """Calculate the theoretical speedup using Amdahl's Law."""
    return 1.0 / ((1.0 - parallel_fraction) + parallel_fraction / p)


def parallel_sum(data, workers):
    """Sum array chunks using the specified number of worker threads."""
    chunks = np.array_split(data, workers)

    with ThreadPoolExecutor(max_workers=workers) as executor:
        partial_sums = executor.map(np.sum, chunks)
        return sum(partial_sums)


def main():
    max_cores = os.cpu_count() or 1

    rng = np.random.default_rng(seed=STUDENT_ID)
    data = rng.random(ARRAY_SIZE, dtype=np.float64)

    worker_counts = sorted(set([
        1, 2, 4, max_cores, max_cores * 2
    ]))

    results = []
    baseline_time = None
    expected_sum = None

    print("=== TASK 2: Multi-Core Scaling vs. Amdahl's Law ===")
    print(f"Student ID: {STUDENT_ID}")
    print(f"OS: {platform.system()}")
    print(f"Logical CPU cores: {max_cores}")
    print(f"Array size: {ARRAY_SIZE:,}")
    print(f"Parallelizable fraction (P): {P:.2f}")
    print("\nRunning benchmarks...")

    for workers in worker_counts:
        parallel_sum(data[:min(100_000, ARRAY_SIZE)], workers)

        start = time.perf_counter()
        total = parallel_sum(data, workers)
        elapsed = time.perf_counter() - start

        if baseline_time is None:
            baseline_time = elapsed
            expected_sum = total
            
        if not np.isclose(total, expected_sum, rtol=1e-12, atol=1e-8):
            raise ValueError(
                f"Sum mismatch with {workers} workers: {total}"
            )

        speedup = baseline_time / elapsed
        theoretical = amdahl_speedup(workers)

        results.append((
            workers,
            elapsed * 1000,
            speedup,
            theoretical
        ))

    print("\n" + "=" * 66)
    print(
        f"{'Cores':>8} | {'Wall Time (ms)':>15} | "
        f"{'Speedup':>10} | {'Amdahl Upper Bound':>18}"
    )
    print("-" * 66)

    for workers, wall_ms, speedup, theoretical in results:
        print(
            f"{workers:>8} | {wall_ms:>15.3f} | "
            f"{speedup:>10.3f} | {theoretical:>18.3f}"
        )

    print("=" * 66)
    print(f"\nSum verification: {expected_sum:.10f}")
    print("Benchmark completed successfully.")


if __name__ == "__main__":
    main()
