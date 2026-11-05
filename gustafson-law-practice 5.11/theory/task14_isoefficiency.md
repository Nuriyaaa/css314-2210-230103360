# Task 14 — Isoefficiency Relation with Gustafson Scaling

Isoefficiency describes how total problem size `W(P)` must grow with processor count to maintain a chosen parallel efficiency. In an ideal weak-scaling program, each processor receives a constant amount of work `w`; hence

\[
W(P)=Pw=\Theta(P).
\]

If the serial fraction `alpha` remains constant, Gustafson efficiency is

\[
E_G(P)=1-\alpha+\frac{\alpha}{P},
\]

which approaches `1-alpha`. Therefore, a linear growth in total work is sufficient to maintain asymptotically constant efficiency in this idealized model:

\[
\boxed{W(P)=\Theta(P)}.
\]

This is the ideal weak-scaling isoefficiency relation. It should not be confused with the isoefficiency function of a particular implementation, which must account for communication and synchronization overhead. If overhead grows with `P`, the problem size may need to grow faster than linearly to preserve the same efficiency, or the target may become unattainable under memory or time constraints.
