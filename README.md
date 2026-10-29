# CS482 Parallel Computing Lab Practicum

**Student ID:** 230103360  
**Platform:** MacBook Air, Apple M4  
**OS:** macOS 26.6.2  
**Python:** 3.14.3

## Milestone 0 — Environment
- `src/profile_env.py`: host environment profiling.
- `src/salt.py`: student-specific benchmark parameters.
- Results: `results/host_profile.json`, `results/salted_params.json`.

## Milestone 1 — Locality
- `src/stride_bench.py`: memory stride benchmark.
- `src/dense_kernel.py`: loop-interchange experiment.
- Results: `results/stride_results.tsv`, `results/loop_interchange.json`.

## Milestone 2 — Sharing
- `src/false_sharing.py`: comparison of unpadded, padded, and privatized counters.
- Result: `results/false_sharing_karp_flatt.tsv`.

## Milestone 3 — Scaling
- `src/scaling_study.py`: NumPy stencil baseline.
- Results: `results/scaling_metrics.tsv`, `results/roofline_summary.json`.

**Important limitation:** The current M3 script is a NumPy baseline and does not implement actual parallel strong/weak scaling. The results should not be interpreted as a completed parallel scaling experiment.

## Verification
The provided `verify_outputs.py` script and generated `verification_receipt.sig` were not present in the local project when checked. Run the instructor-provided verification procedure before claiming final verification is complete.
