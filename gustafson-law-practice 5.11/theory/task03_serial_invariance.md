# Task 03 — The Serial Fraction Invariance Paradox

The apparent paradox comes from comparing serial fractions measured against different reference workloads.

In Amdahl's model, `s` is the serial fraction of a fixed-size program measured on a uniprocessor baseline. The total work is fixed, so adding processors only shortens the parallel part. The serial part becomes a limiting fraction of the original workload.

In Gustafson's model, `alpha` is observed during the parallel execution of the scaled problem. The parallel workload is enlarged as `P` increases, while the elapsed parallel time is kept approximately constant. The serial component is therefore measured as a fraction of this larger parallel-run workload, not as the same fixed serial-baseline fraction used by Amdahl.

For a parallel run normalized to one time unit, the serial part is `alpha` and the parallel part is `1-alpha`. The equivalent serial time for the scaled workload is

\[
T_{serial}=\alpha+P(1-\alpha),
\]

so

\[
S_G(P)=P-\alpha(P-1).
\]

There is no contradiction: Amdahl and Gustafson use different workload assumptions and frames of reference. Measuring the serial fraction on the parallel machine makes the scaling interpretation explicit. It does **not** mean serial overhead disappears. If synchronization, communication, or setup overhead grows with `P`, the observed `alpha` may increase and Gustafson's idealized prediction will become optimistic.
