import csv
import ctypes
import multiprocessing as mp
import statistics
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "results" / "false_sharing_karp_flatt.tsv"
K = 5_000_000
PROCESSES = (1, 2, 4, 8)
REPEATS = 3


def shared_worker(shared, index, stride, count):
    for _ in range(count):
        shared[index * stride] += 1


def private_worker(count, queue):
    value = 0
    for _ in range(count):
        value += 1
    queue.put(value)


def run_shared(p, padded):
    stride = 8 if padded else 1
    shared = mp.RawArray(ctypes.c_longlong, p * stride)
    workers = [
        mp.Process(target=shared_worker, args=(shared, i, stride, K))
        for i in range(p)
    ]
    start = time.perf_counter_ns()
    for w in workers:
        w.start()
    for w in workers:
        w.join()
    elapsed = time.perf_counter_ns() - start
    assert all(shared[i * stride] == K for i in range(p))
    return elapsed


def run_private(p):
    queue = mp.Queue()
    workers = [
        mp.Process(target=private_worker, args=(K, queue))
        for _ in range(p)
    ]
    start = time.perf_counter_ns()
    for w in workers:
        w.start()
    for w in workers:
        w.join()
    elapsed = time.perf_counter_ns() - start
    total = sum(queue.get() for _ in range(p))
    assert total == p * K
    queue.close()
    return elapsed


def main():
    OUT.parent.mkdir(parents=True, exist_ok=True)
    rows = []
    modes = {
        "unpadded": lambda p: run_shared(p, False),
        "padded": lambda p: run_shared(p, True),
        "privatized": run_private,
    }

    for mode, run in modes.items():
        baseline = None
        for p in PROCESSES:
            timings = [run(p) for _ in range(REPEATS)]
            median = int(statistics.median(timings))
            if p == 1:
                baseline = median
            e = (
                (p * median / baseline - 1) / (p - 1)
                if p > 1 else ""
            )
            rows.append([mode, p, K, median, e])
            print(f"{mode:10s} P={p} median={median:,} ns")

    with OUT.open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f, delimiter="\t")
        writer.writerow([
            "mode", "P", "K_per_process", "median_time_ns",
            "karp_flatt_e"
        ])
        writer.writerows(rows)

    print(f"\nSaved: {OUT}")


if __name__ == "__main__":
    mp.freeze_support()
    main()
