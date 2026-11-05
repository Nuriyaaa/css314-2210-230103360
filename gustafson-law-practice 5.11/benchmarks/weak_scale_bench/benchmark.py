#!/usr/bin/env python3
"""Multiprocessing weak-scaling benchmark using a deterministic numerical workload."""
import argparse
import csv
import math
import multiprocessing as mp
import os
import platform
import statistics
import sys
import time
from pathlib import Path

def worker(args):
    start, count = args
    acc = 0.0
    # Independent deterministic work; each process receives the same number of iterations.
    for i in range(start, start + count):
        x = (i % 1000003) / 1000003.0
        acc += math.sqrt(x + 1.0) * math.sin(x + 0.25)
    return acc

def run_once(processes, work_per_process):
    chunk = max(1, work_per_process // processes)
    # Give each process a fixed amount of work (weak scaling).
    jobs = [(1 + j * work_per_process, work_per_process) for j in range(processes)]
    t0 = time.perf_counter()
    with mp.get_context("spawn").Pool(processes=processes) as pool:
        checksum = sum(pool.map(worker, jobs))
    return time.perf_counter() - t0, checksum

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--counts", default="1,2,4,8")
    ap.add_argument("--work-per-thread", type=int, default=500000)
    ap.add_argument("--repeats", type=int, default=5)
    ap.add_argument("--output", default="benchmarks/results.csv")
    args = ap.parse_args()
    counts = [int(x) for x in args.counts.split(",") if x.strip()]
    if len(set(counts)) != len(counts) or any(p < 1 for p in counts):
        ap.error("counts must be distinct positive integers")
    if args.repeats < 5:
        ap.error("Use at least 5 independent runs per configuration (Task 23).")
    rows = []
    for p in counts:
        for run in range(1, args.repeats + 1):
            elapsed, checksum = run_once(p, args.work_per_thread)
            rows.append({"processors": p, "run": run, "work_per_process": args.work_per_thread,
                         "runtime_seconds": f"{elapsed:.9f}", "checksum": f"{checksum:.12f}",
                         "platform": platform.platform(), "python": sys.version.split()[0]})
            print(f"P={p:>3} run={run} time={elapsed:.4f}s checksum={checksum:.5f}")
    out = Path(args.output)
    out.parent.mkdir(parents=True, exist_ok=True)
    with out.open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    print(f"Saved {len(rows)} observations to {out}")
    print(f"Logical CPUs reported by os.cpu_count(): {os.cpu_count()}")

if __name__ == "__main__":
    main()
