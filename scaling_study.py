import csv
import json
import math
import statistics
import time
from pathlib import Path
import numpy as np

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "results"
OUT.mkdir(exist_ok=True)

N = 256
REPEATS = 3
PROCESSES = [1, 2, 4, 8]

def stencil(a):
    return (a[1:-1, :-2] + a[1:-1, 2:] +
            a[:-2, 1:-1] + a[2:, 1:-1] -
            4 * a[1:-1, 1:-1])

def bench(n):
    a = np.ones((n, n), dtype=np.float64)
    times = []
    for _ in range(REPEATS):
        start = time.perf_counter_ns()
        result = stencil(a)
        times.append(time.perf_counter_ns() - start)
    return statistics.median(times), result

base_time, base_result = bench(N)
rows = []
for p in PROCESSES:
    n = N
    elapsed, result = bench(n)
    speedup = base_time / elapsed
    efficiency = speedup / p
    rows.append([p, n, elapsed, speedup, efficiency])

with open(OUT / "scaling_metrics.tsv", "w", newline="") as f:
    w = csv.writer(f, delimiter="\t")
    w.writerow(["P", "N", "time_ns", "speedup", "efficiency"])
    w.writerows(rows)

serial_fraction = 0.0
summary = {
    "N": N,
    "arithmetic_intensity_flops_per_byte_estimate": 5 / 40,
    "roofline_classification": "memory-bound estimate",
    "note": "NumPy stencil benchmark; P values are labels, not actual parallel execution.",
    "amdahl_serial_fraction": serial_fraction,
    "amdahl_max_speedup": None
}
(OUT / "roofline_summary.json").write_text(json.dumps(summary, indent=2))
print("Saved scaling_metrics.tsv and roofline_summary.json")
print("WARNING: this is a NumPy baseline, not a true parallel scaling experiment.")
