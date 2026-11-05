# Task 15 — Energy and Power Proportionality

Let each active node consume `Pidle` when idle and an additional dynamic power `Pactive` during computation. For `P` nodes running for wall-clock time `T`, a simple total-energy model is

\[
E_P=P(P_{idle}+P_{active})T.
\]

For one node over the same wall-clock duration:

\[
E_1=(P_{idle}+P_{active})T.
\]

Thus, if all nodes remain powered and active for the full interval,

\[
\frac{E_P}{E_1}=P.
\]

The weak-scaled run can complete `P` times as much work in approximately the same time, but it may consume approximately `P` times the energy under this simplified per-node power assumption.

If `Pactive` is defined as total active power rather than additional dynamic power, the expression should instead be written using the measured total power per node. Real energy depends on utilization, DVFS, idle states, memory and network power, cooling, and execution-time changes. A fair comparison should report joules per useful operation as well as total joules.
