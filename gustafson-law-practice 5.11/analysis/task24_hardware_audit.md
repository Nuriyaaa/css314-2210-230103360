# Task 24 — Hardware Audit and Empirical/Ideal Comparison

## Hardware configuration (measured on the test machine)

- Device: MacBook Air
- Model identifier: Mac16,12
- Chip: Apple M4
- CPU cores: 10 total (4 performance + 6 efficiency)
- Logical CPUs reported by `os.cpu_count()`: 10
- Unified memory: 16 GB LPDDR5
- Memory manufacturer: Hynix
- L1 instruction cache reported by macOS: 131,072 bytes (128 KiB)
- L1 data cache reported by macOS: 65,536 bytes (64 KiB)
- L2 cache reported by macOS: 4,194,304 bytes (4 MiB)
- L3 cache: not reported by the supplied `sysctl` query
- Operating system: macOS (version not captured in the supplied hardware output)
- Python: 3.14

## Benchmark methodology

The benchmark uses Python multiprocessing with the `spawn` start method. Each process executes a deterministic numerical loop with a fixed workload of 2,000,000 iterations. Five independent runs were collected at each process count: 1, 2, 4, 6, and 8. The total workload therefore increases with process count, which follows the weak-scaling setup.

The raw measurements are stored in `benchmarks/results.csv`. Summary statistics and measured scaled speedups are stored in `plots/empirical_summary.csv`.

## Results

The measured weak-scaling speedup is calculated as

\[
S_{measured}(P)=P\frac{T_1}{T_P},
\]

where `T1` is the mean runtime at one process and `TP` is the mean runtime at `P` processes.

The least-squares fit to

\[
S_G(P)=P-\alpha(P-1)
\]

gives

\[
\boxed{\alpha_{observed}=0.384491\;(38.45\%)}.
\]

| Processes | Mean runtime (s) | Sample SD (s) | Measured weak speedup |
|---:|---:|---:|---:|
| 1 | 0.198100 | 0.019674 | 1.000× |
| 2 | 0.198720 | 0.005254 | 1.994× |
| 4 | 0.226220 | 0.017265 | 3.503× |
| 6 | 0.280160 | 0.008397 | 4.243× |
| 8 | 0.326400 | 0.014134 | 4.855× |


## Interpretation and limitations

The measured speedup increases with process count but remains below ideal linear scaling for the larger configurations. The fitted serial fraction is an **effective benchmark-level parameter**. It includes all effects represented in elapsed time, such as process startup, Python multiprocessing overhead, scheduling, and resource contention; it must not be interpreted as the fraction of inherently serial CPU instructions in the Apple M4.

The one-process configuration has noticeable run-to-run variation. In addition, the benchmark uses Python processes rather than a native OpenMP kernel, so the results characterize this Python multiprocessing workload and its runtime overhead, not the maximum hardware parallel efficiency of the M4.

The experiment uses five runs per configuration and reports sample standard deviation. The error bars in the plot propagate the baseline and per-configuration runtime standard deviations approximately; with only five observations, they should be interpreted as descriptive uncertainty rather than a formal confidence interval.

The system report did not provide an L3 cache value in the queried output, so no L3 size is claimed. Memory channel count and sustained memory bandwidth were not measured and are therefore not asserted.

## Conclusion

On this MacBook Air M4, the selected Python multiprocessing workload shows measurable weak-scaling improvement as process count increases, but it does not achieve ideal linear speedup. The result demonstrates the practical impact of runtime overhead and shared-resource contention in a real system. The fitted `alpha` is specific to this benchmark configuration.
