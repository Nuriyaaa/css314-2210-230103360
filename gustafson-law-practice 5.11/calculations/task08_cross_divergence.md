# Task 08 — Amdahl vs. Gustafson at P = 128

Given `s=alpha=0.08` and `P=128`.

Amdahl fixed-size speedup:

\[
S_A=\frac{1}{0.08+0.92/128}
=\frac{1}{0.0871875}
\approx 11.4695.
\]

Gustafson scaled speedup:

\[
S_G=128-0.08(127)=117.84.
\]

Using Amdahl as the denominator, percentage discrepancy is

\[
\frac{S_G-S_A}{S_A}\times100
=\frac{117.84-11.4695}{11.4695}\times100
\approx 927.4\%.
\]

Gustafson's prediction is approximately 10.27 times Amdahl's prediction. They diverge because Amdahl holds total problem size fixed, while Gustafson scales the workload to use the additional processors. The same numerical fraction is assumed for this exercise, although in real measurements the two fractions may have different reference frames.
