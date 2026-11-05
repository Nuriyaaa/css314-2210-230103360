#!/usr/bin/env python3
"""Compare Amdahl strong scaling and Gustafson weak scaling."""
import argparse
import csv
from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--alpha", type=float, default=0.04)
    ap.add_argument("--max-p", type=int, default=16384)
    ap.add_argument("--output-dir", default="benchmarks/simulator_output")
    args = ap.parse_args()
    if not 0.001 <= args.alpha <= 0.20:
        ap.error("alpha must be in [0.001, 0.20]")
    if not 1 <= args.max_p <= 65536:
        ap.error("max-p must be in [1, 65536]")
    out = Path(args.output_dir)
    out.mkdir(parents=True, exist_ok=True)
    ps = np.unique(np.geomspace(1, args.max_p, 80).astype(int))
    sg = ps - args.alpha * (ps - 1)
    sa = 1.0 / (args.alpha + (1.0 - args.alpha) / ps)
    with (out / "scaling.csv").open("w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["processors", "alpha", "gustafson_speedup", "gustafson_efficiency", "amdahl_speedup"])
        for p, g, a in zip(ps, sg, sa):
            w.writerow([int(p), args.alpha, float(g), float(g/p), float(a)])
    plt.figure()
    plt.plot(ps, ps, label="Ideal linear")
    plt.plot(ps, sg, label="Gustafson weak scaling")
    plt.plot(ps, sa, label="Amdahl strong scaling")
    plt.xscale("log", base=2)
    plt.yscale("log", base=2)
    plt.xlabel("Processors (P)")
    plt.ylabel("Speedup")
    plt.title(f"Amdahl vs. Gustafson (alpha={args.alpha:g})")
    plt.grid(True, which="both", alpha=0.25)
    plt.legend()
    plt.tight_layout()
    plt.savefig(out / "scaling_comparison.png", dpi=300)
    print(f"Wrote {out/'scaling.csv'} and {out/'scaling_comparison.png'}")

if __name__ == "__main__":
    main()
