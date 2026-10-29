
import csv
import statistics
import time
from pathlib import Path

import numpy as np


PROJECT_DIR = Path(__file__).resolve().parent.parent
RESULTS_DIR = PROJECT_DIR / "results"

BUFFER_ELEMENTS = 16_777_216
STRIDES = [1, 2, 4, 8, 16, 32, 64, 128]
WARMUP_RUNS = 5
RECORDED_RUNS = 5


def measure_stride(data, stride):
    view = data[::stride]
    start = time.perf_counter_ns()
    total = np.sum(view, dtype=np.float64)
    elapsed_ns = time.perf_counter_ns() - start

    return elapsed_ns, float(total), view.size


def main():
    RESULTS_DIR.mkdir(parents=True, exist_ok=True)

    data = np.ones(BUFFER_ELEMENTS, dtype=np.float64)
    output_path = RESULTS_DIR / "stride_results.tsv"

    rows = []

    for stride in STRIDES:
        for _ in range(WARMUP_RUNS):
            measure_stride(data, stride)

        timings = []
        checksums = []

        for _ in range(RECORDED_RUNS):
            elapsed_ns, total, elements_touched = measure_stride(
                data, stride
            )
            timings.append(elapsed_ns)
            checksums.append(total)

        median_ns = statistics.median(timings)
        latency_ns = median_ns / elements_touched

        rows.append({
            "stride": stride,
            "elements_touched": elements_touched,
            "median_time_ns": median_ns,
            "latency_ns_per_element": latency_ns,
            "checksum": checksums[-1],
        })

        print(
            f"stride={stride:3d} | "
            f"elements={elements_touched:>9,d} | "
            f"median={median_ns:>12,.0f} ns | "
            f"latency={latency_ns:.4f} ns/element"
        )

    with output_path.open("w", newline="", encoding="utf-8") as file:
        writer = csv.DictWriter(
            file,
            fieldnames=[
                "stride",
                "elements_touched",
                "median_time_ns",
                "latency_ns_per_element",
                "checksum",
            ],
            delimiter="\t",
        )
        writer.writeheader()
        writer.writerows(rows)

    print(f"\nResults saved to: {output_path}")


if __name__ == "__main__":
    main()
