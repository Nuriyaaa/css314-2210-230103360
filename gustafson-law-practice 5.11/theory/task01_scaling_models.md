# Task 01 — Strong Scaling vs. Weak Scaling Foundations

## Amdahl's fixed-size model
Amdahl's Law assumes that the total problem size is fixed. Let `s` be the serial fraction and `1-s` the parallel fraction. With `P` processors, the normalized runtime is

\[
T_A(P)=s+\frac{1-s}{P}
\]

and the speedup is

\[
S_A(P)=\frac{1}{s+(1-s)/P}.
\]

As `P` tends to infinity, the parallel part approaches zero but the serial part remains. Therefore,

\[
\lim_{P\to\infty}S_A(P)=\frac{1}{s}.
\]

The problem dimension does not grow with processor count: the same total work is divided among more processors. This is called **strong scaling** or fixed-size scaling.

## Gustafson's fixed-time model
Gustafson's model starts from a parallel execution with approximately fixed wall-clock time. As more processors become available, the workload is enlarged so that the parallel portion uses the additional capacity. If `alpha` is the serial fraction of the scaled parallel execution, then

\[
S_G(P)=\alpha+(1-\alpha)P=P-\alpha(P-1).
\]

The workload grows with `P`; for a workload whose operation count is proportional to a dimension `N`, that dimension may grow proportionally to `P`. For higher-order algorithms, the dimension grows according to the algorithm's work complexity, not necessarily linearly.

## Comparison
| Property | Amdahl | Gustafson |
|---|---|---|
| Scaling | Strong | Weak / scaled-size |
| Total problem size | Fixed | Increases with P |
| Runtime reference | Fixed workload | Approximately fixed parallel time |
| Serial fraction | Measured relative to serial baseline | Observed in parallel execution |
| Limit | `1/s` | Approximately linear in P if alpha stays bounded |

The models answer different questions. Amdahl asks how much faster the same job can finish. Gustafson asks how much larger a job can be completed in roughly the same time. Neither law guarantees ideal scaling: communication, synchronization, memory capacity, and load imbalance can change with `P`.
