# Task 12 — Communication Overhead Penalty Integration

The ideal Gustafson expression is

\[
S_G(P)=P-\alpha(P-1).
\]

Introduce `C(P)`, the communication cost on `P` processors expressed in units of the normalized parallel execution time. A simple first-order penalized model is

\[
\boxed{S_C(P)=P-\alpha(P-1)-C(P)}.
\]

For example, a logarithmic communication model is `C(P)=c log2(P)` and a surface/neighbor model may be approximated by `C(P)=c sqrt(P)`, where `c` is a non-negative coefficient determined by the interconnect and algorithm.

The efficiency becomes

\[
E_C(P)=\frac{S_C(P)}{P}
=1-\alpha+\frac{\alpha}{P}-\frac{C(P)}{P}.
\]

If `C(P)=c log2(P)`, then `C(P)/P` tends to zero, so the asymptotic efficiency remains `1-alpha` in this simplified model. If `C(P)=c sqrt(P)`, its normalized penalty also tends to zero, although finite-scale performance can be materially reduced.

This is a proposal, not a universal law. Communication may also increase the effective serial fraction, overlap with computation, or change the parallel runtime normalization. A more detailed model should distinguish latency, bandwidth, synchronization, and overlap.
