
import json
import statistics
import time
from pathlib import Path

import numpy as np
from numba import njit


PROJECT_DIR = Path(__file__).resolve().parent.parent
RESULTS_DIR = PROJECT_DIR / "results"

STUDENT_SEED = 17981
M = 768
RUNS = 3
TOLERANCE = 1e-12


@njit
def multiply_ijk(A, B):
    n = A.shape[0]
    C = np.zeros((n, n), dtype=np.float64)

    for i in range(n):
        for j in range(n):
            total = 0.0
            for k in range(n):
                total += A[i, k] * B[k, j]
            C[i, j] = total

    return C


@njit
def multiply_ikj(A, B):
    n = A.shape[0]
    C = np.zeros((n, n), dtype=np.float64)

    for i in range(n):
        for k in range(n):
            a_value = A[i, k]
            for j in range(n):
                C[i, j] += a_value * B[k, j]

    return C


def median_runtime(function, A, B):
    timings = []
    result = None

    # First call compiles the Numba function.
    function(A, B)

    for _ in range(RUNS):
        start = time.perf_counter_ns()
        result = function(A, B)
        timings.append(time.perf_counter_ns() - start)

    return statistics.median(timings), result


def main():
    RESULTS_DIR.mkdir(parents=True, exist_ok=True)

    rng = np.random.default_rng(STUDENT_SEED)

    # Check correctness on a small matrix first.
    A_small = rng.random((32, 32))
    B_small = rng.random((32, 32))

    C_ijk_small = multiply_ijk(A_small, B_small)
    C_ikj_small = multiply_ikj(A_small, B_small)
    C_reference = A_small @ B_small

    if not np.allclose(C_ijk_small, C_reference, rtol=0, atol=TOLERANCE):
        raise RuntimeError("i-j-k correctness check failed")

    if not np.allclose(C_ikj_small, C_reference, rtol=0, atol=TOLERANCE):
        raise RuntimeError("i-k-j correctness check failed")

    print("Small-matrix correctness checks passed.")

    # Generate the benchmark matrices.
    A = rng.random((M, M))
    B = rng.random((M, M))

    # Warm up both compiled implementations before measuring.
    multiply_ijk(A, B)
    multiply_ikj(A, B)

    time_ijk, C_ijk = median_runtime(multiply_ijk, A, B)
    time_ikj, C_ikj = median_runtime(multiply_ikj, A, B)

    if not np.allclose(C_ijk, C_ikj, rtol=0, atol=TOLERANCE):
        raise RuntimeError("Benchmark results differ beyond tolerance")

    speedup = time_ijk / time_ikj if time_ikj else None

    results = {
        "student_id": "230103360",
        "matrix_dimension": M,
        "seed": STUDENT_SEED,
        "runs": RUNS,
        "tolerance": TOLERANCE,
        "implementation": "Numba njit",
        "ijk_median_time_ns": time_ijk,
        "ikj_median_time_ns": time_ikj,
        "speedup_ijk_over_ikj": speedup,
        "max_absolute_difference": float(np.max(np.abs(C_ijk - C_ikj))),
    }

    output_path = RESULTS_DIR / "loop_interchange.json"
    output_path.write_text(json.dumps(results, indent=2), encoding="utf-8")

    print(json.dumps(results, indent=2))
    print(f"Saved: {output_path}")


if __name__ == "__main__":
    main()
