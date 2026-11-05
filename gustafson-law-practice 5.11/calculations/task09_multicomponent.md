# Task 09 — Multi-Component Workload Partitioning

The task gives three execution-time shares: disk I/O and broadcast = 2%, boundary synchronization = 6%, and domain computation = 92%.

Treating the first two phases as the aggregate serial/non-parallel fraction for this simplified model:

\[
\alpha=0.02+0.06=0.08.
\]

The computational parallel fraction is `0.92`.

For `P=512`:

\[
S(512)=512-0.08(511)=471.12.
\]

For `P=2048`:

\[
S(2048)=2048-0.08(2047)=1884.24.
\]

| P | Scaled speedup |
|---:|---:|
| 512 | 471.12 |
| 2,048 | 1,884.24 |

This aggregation is a simplifying assumption: I/O and message passing are not necessarily fully serial in an actual distributed implementation. Their overlap and scaling behavior should be measured separately.
