# Gustafson's Law & Weak Scaling — Practice Repository

**Student ID:** `230103360`  
**Course:** Parallel & Distributed Computing Architecture  
**Practice:** Gustafson's Law & Weak Scaling  
**Instructor:** Sufyan bin Uzayr

## Project overview
This repository contains the written derivations, numerical calculations, algorithmic analyses, benchmark programs, empirical data, plots, and hardware notes for Tasks 01–25.

## Repository map
- `theory/` — Tasks 01–05 and 11–15.
- `calculations/` — Tasks 06–10 and 16–17.
- `analysis/` — Tasks 18–20 and 24.
- `benchmarks/simulator.py` — Task 21.
- `benchmarks/weak_scale_bench/` — Task 22.
- `benchmarks/results.csv` and `benchmarks/fit_gustafson.py` — Task 23.
- `plots/` — Task 24 figures.

## Core equations
Gustafson scaled speedup:
\[
S_G(P)=P-\alpha(P-1)=1+(1-\alpha)(P-1)
\]
Parallel efficiency:
\[
E_G(P)=\frac{S_G(P)}{P}=1-\alpha\left(1-\frac{1}{P}\right)
\]
Amdahl fixed-size speedup:
\[
S_A(P)=\frac{1}{s+(1-s)/P}
\]

Here, `P` is the processor count, `alpha` is the serial fraction measured in the parallel execution, and `s` is the serial fraction of the fixed-size workload measured against the serial baseline. They are not automatically interchangeable unless the problem explicitly assumes equal values.

## Environment and execution
Recommended: Python 3.10+ with NumPy and Matplotlib.

```bash
python3 -m pip install numpy matplotlib
python3 benchmarks/simulator.py --alpha 0.04 --max-p 16384
python3 benchmarks/weak_scale_bench/benchmark.py --counts 1,2,4,8 --work-per-thread 1000000 --repeats 5
python3 benchmarks/fit_gustafson.py --input benchmarks/results.csv
```

Use thread counts supported by the host. On Apple Silicon, the benchmark uses Python multiprocessing and reports the actual process count. Do not assume that logical CPU count equals the number of performance cores.

## Empirical workflow (Tasks 22–24)
1. Run the weak-scaling benchmark on the target machine with at least five distinct process counts.
2. Use five independent runs for each process count.
3. Preserve the generated raw `benchmarks/results.csv`.
4. Run the fitting script. It estimates the observed serial fraction by fitting the measured normalized speedup to `S(P)=P-alpha(P-1)`.
5. Generate plots and complete `analysis/task24_hardware_audit.md` with the actual machine details and observed deviations.

**Empirical run recorded:** Apple M4 MacBook Air, 16 GB LPDDR5, 10 CPU cores (4 performance + 6 efficiency). The 2,000,000-iteration-per-process run includes five repetitions at P=1,2,4,6,8. See `benchmarks/results.csv`, `plots/empirical_summary.csv`, and `analysis/task24_hardware_audit.md`. The fitted benchmark-level serial fraction is recorded in the Task 24 report. It includes Python multiprocessing/runtime overhead and is not a hardware-only serial fraction.

## Task index
| Task | Main artifact |
|---|---|
| 01–05 | `theory/task01` … `theory/task05` |
| 06–10 | `calculations/task06` … `calculations/task10` |
| 11–15 | `theory/task11` … `theory/task15` |
| 16–17 | `calculations/task16` … `calculations/task17` |
| 18–20 | `analysis/task18` … `analysis/task20` |
| 21 | `benchmarks/simulator.py` |
| 22 | `benchmarks/weak_scale_bench/benchmark.py` |
| 23 | `benchmarks/results.csv`, `benchmarks/fit_gustafson.py` |
| 24 | `plots/` and `analysis/task24_hardware_audit.md` |
| 25 | This README |

## Submission checklist
- [ ] Run Tasks 22–24 on the actual target machine.
- [ ] Replace hardware placeholders with measured information.
- [ ] Check that generated CSV and plots are committed.
- [ ] Commit the project to the existing personal Git repository.
