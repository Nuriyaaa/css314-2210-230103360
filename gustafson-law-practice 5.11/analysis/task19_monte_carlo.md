# Task 19 — Monte Carlo Path Simulation

Monte Carlo paths are largely independent, making path generation and per-path computation highly parallel. In an ideal model, the serial fraction `alpha` is close to zero and Gustafson speedup approaches `P`.

Non-zero serial or coordination costs include:
- initialization and distribution of random-number generator states or seeds;
- merging partial sums and computing final statistics;
- output serialization and checkpointing;
- reproducibility checks and result validation.

A binary-tree reduction over `P` partial values has `log2(P)` communication stages (for power-of-two `P`). If each stage costs latency `L`, a simple reduction-time estimate is

\[
T_{reduce}\approx L\log_2P+\frac{(P-1)m}{B},
\]

where `m` is bytes transferred per partial value and `B` is effective bandwidth. The latency depth grows logarithmically. If reduction time is included in the observed serial/overhead fraction, the effective `alpha` increases with `P` and

\[
S(P)=P-\alpha(P)(P-1)
\]

grows more slowly than the ideal constant-alpha prediction. The actual fraction must be measured for the chosen path count, RNG, and reduction implementation.
