# Task 02 — Formal Derivation of Scaled Speedup

Normalize the observed parallel runtime on `P` processors to one time unit:

\[
s+p=1,
\]

where `s` is the serial portion and `p` is the parallel portion of the parallel execution. Thus `p=1-s`.

During the parallel run, the serial work takes `s` time units. The parallel work takes `p` time units on `P` processors. If the same enlarged workload were executed serially, the parallel portion would require `Pp` time units, assuming ideal division of that work across processors and no extra overhead.

The equivalent serial execution time is therefore

\[
T_{serial}=s+Pp.
\]

The observed parallel time is normalized to

\[
T_{parallel}=s+p=1.
\]

Speedup is serial time divided by parallel time:

\[
S(P)=\frac{s+Pp}{s+p}=s+Pp.
\]

Since `s+p=1`, substitute `p=1-s`:

\[
S(P)=s+P(1-s)=P-s(P-1).
\]

If the serial fraction is denoted by `alpha` rather than `s`, then

\[
\boxed{S(P)=P-\alpha(P-1)}.
\]

Equivalently, expanding the expression gives `S(P)=alpha+(1-alpha)P`. The result is Gustafson's scaled speedup. It assumes that the parallel workload can be expanded and that the measured serial fraction remains applicable to the scaled execution.
