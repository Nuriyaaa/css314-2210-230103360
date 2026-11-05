# Task 05 — Historical nCUBE 10 Scenario

The task sheet describes a 1988 result by Gustafson, Montry, and Benner on a 1,024-processor nCUBE 10, reporting speedups greater than 1,000× and an apparent conflict with Amdahl's Law for a 0.4% serial fraction.

Under a fixed-size interpretation with `s=0.004`, Amdahl predicts

\[
S_A(1024)=\frac{1}{0.004+0.996/1024}\approx 205.6.
\]

This is far below 1,000×. The comparison is not valid if the experiment enlarged the problem to use the full machine. Under Gustafson's scaled-size interpretation with `alpha=0.004`,

\[
S_G(1024)=1024-0.004(1023)=1019.908.
\]

This exceeds 1,000× and is consistent with the scenario stated in the task sheet.

The key mechanism is problem expansion: the parallel machine performs a much larger total computation in roughly the same elapsed time, rather than merely dividing a fixed computation. The serial fraction is a small portion of the scaled parallel execution.

**Scope note:** the supplied practice sheet gives the headline processor count, serial fraction, and reported speedup. It does not provide the original benchmark's detailed workload dimensions, timing table, or hardware configuration. This reconstruction therefore verifies the mathematical consistency of the stated scenario; it does not claim to reproduce undocumented historical measurements.
