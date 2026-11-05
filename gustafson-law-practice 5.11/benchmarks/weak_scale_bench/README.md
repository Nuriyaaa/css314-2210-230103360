# Weak-scaling benchmark

Run from the repository root:

```bash
python3 benchmarks/weak_scale_bench/benchmark.py \
  --counts 1,2,4,6,8 \
  --work-per-thread 500000 \
  --repeats 5 \
  --output benchmarks/results.csv
```

Each process receives a fixed iteration count, so total work increases with process count. The script uses Python multiprocessing with the `spawn` start method for cross-platform compatibility. Process creation overhead is included in wall-clock time; this makes the benchmark a practical end-to-end measurement, not a pure kernel-only measurement.

Use at least five distinct process counts and five runs per count. Select counts that are supported by the host and avoid overloading the machine.
