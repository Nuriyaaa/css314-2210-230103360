# Task 11 — Gustafson vs. Sun–Ni Memory-Bounded Speedup

Gustafson's model assumes a fixed parallel execution time while the workload expands with processor count:

\[
S_G(P)=\alpha+(1-\alpha)P.
\]

The Sun–Ni memory-bounded model generalizes the scaling question by allowing the problem size to grow with available memory rather than assuming that processor count alone determines the workload. A commonly used form is

\[
S_{SN}(P)=\frac{W(P)}{W(1)}
\]

where `W(P)` is the amount of work that fits in the memory available to `P` processors, adjusted for the corresponding execution-time model. Its precise expression depends on the memory-capacity and algorithmic assumptions.

Gustafson can overestimate throughput when memory per node is insufficient, communication grows, load balance worsens, or synchronization becomes dominant. It can underestimate throughput when aggregate memory enables a substantially larger working set or when memory capacity, rather than processor count, is the main limiting resource and grows faster than the assumed workload.

The central distinction is the scaling constraint: Gustafson emphasizes fixed time and expanded work; Sun–Ni emphasizes the relationship between memory capacity, problem size, and performance. A quantitative Sun–Ni prediction requires a stated memory-capacity function and workload model, neither of which is numerically specified in this task.
