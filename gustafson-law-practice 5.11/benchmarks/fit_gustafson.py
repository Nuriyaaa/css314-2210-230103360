#!/usr/bin/env python3
"""Summarize weak-scaling runs and fit S(P)=P-alpha(P-1)."""
import argparse
import csv
from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--input", default="benchmarks/results.csv")
    ap.add_argument("--plot-dir", default="plots")
    args = ap.parse_args()
    data = {}
    with open(args.input, newline="") as f:
        for row in csv.DictReader(f):
            data.setdefault(int(row["processors"]), []).append(float(row["runtime_seconds"]))
    if len(data) < 5:
        raise SystemExit("Task 23 requires at least five distinct processor counts.")
    ps = np.array(sorted(data), dtype=float)
    means = np.array([np.mean(data[int(p)]) for p in ps])
    stds = np.array([np.std(data[int(p)], ddof=1) if len(data[int(p)]) > 1 else 0 for p in ps])
    baseline = means[0]
    speedups = baseline / means
    # Least-squares fit through S(P)=P-alpha(P-1), equivalent to fit alpha per point.
    x = ps - 1.0
    y = ps - speedups
    alpha = float(np.dot(x, y) / np.dot(x, x)) if np.dot(x, x) else 0.0
    alpha = max(0.0, min(1.0, alpha))
    out = Path(args.plot_dir)
    out.mkdir(parents=True, exist_ok=True)
    with (out / "empirical_summary.csv").open("w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["processors", "mean_runtime_s", "std_runtime_s", "empirical_speedup", "fitted_gustafson_speedup"])
        for p, m, sd, sp in zip(ps, means, stds, speedups):
            w.writerow([int(p), m, sd, sp, p-alpha*(p-1)])
    ideal = ps
    gust = ps-alpha*(ps-1)
    amdahl = 1/(alpha+(1-alpha)/ps)
    plt.figure()
    plt.plot(ps, ideal, "--", label="Ideal linear")
    plt.plot(ps, gust, label=f"Gustafson fit (alpha={alpha:.4f})")
    plt.plot(ps, amdahl, label="Amdahl (same fraction)")
    plt.errorbar(ps, speedups, yerr=baseline*stds/(means**2), fmt="o", capsize=4, label="Measured (runtime SD propagated)")
    plt.xlabel("Processes")
    plt.ylabel("Speedup")
    plt.title("Empirical vs. theoretical scaling")
    plt.grid(True, alpha=0.25)
    plt.legend()
    plt.tight_layout()
    plt.savefig(out / "scaling_comparison.png", dpi=300)
    print(f"Fitted alpha_observed = {alpha:.6f}")
    print(f"Summary: {out/'empirical_summary.csv'}")
    print(f"Plot: {out/'scaling_comparison.png'}")

if __name__ == "__main__":
    main()
