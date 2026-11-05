# Task 16 — Dense Matrix Multiplication Scaling (GEMM)

For dense multiplication of two `N×N` matrices, work is proportional to `N^3`. Under ideal weak scaling, total work grows proportionally to processor count `P` while runtime remains constant.

Set

\[
N^3\propto P
\quad\Rightarrow\quad
\boxed{N\propto P^{1/3}}.
\]

Relative to a baseline dimension `N1` on one processor:

\[
N(P)=N_1P^{1/3}.
\]

| P | N(P)/N1 |
|---:|---:|
| 1 | 1.000 |
| 8 | 2.000 |
| 27 | 3.000 |
| 64 | 4.000 |
| 512 | 8.000 |
| 1,000 | 10.000 |

The matrix storage requirement is proportional to `N^2`, so aggregate memory grows as `P^(2/3)` under this dimension rule. This derivation assumes ideal parallelization and ignores data movement, communication, and memory bandwidth limits.
