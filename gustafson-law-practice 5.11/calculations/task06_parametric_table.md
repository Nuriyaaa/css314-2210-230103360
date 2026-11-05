# Task 06 — Parametric Speedup Across Processor Densities

Given `alpha=0.04`:

\[
S(P)=P-0.04(P-1)=0.96P+0.04,\qquad E(P)=S(P)/P.
\]

| P | S(P) | E(P) | Efficiency |
|---:|---:|---:|---:|
| 16 | 15.40 | 0.962500 | 96.2500% |
| 64 | 61.48 | 0.960625 | 96.0625% |
| 256 | 245.80 | 0.960156 | 96.0156% |
| 1,024 | 983.08 | 0.960039 | 96.0039% |
| 4,096 | 3,932.20 | 0.960010 | 96.0010% |
| 16,384 | 15,728.68 | 0.960002 | 96.0002% |

Efficiency decreases slightly and approaches `1-alpha=0.96`. The absolute speedup grows almost linearly with processor count. This is an idealized model with constant observed serial fraction and no additional communication or resource contention.
