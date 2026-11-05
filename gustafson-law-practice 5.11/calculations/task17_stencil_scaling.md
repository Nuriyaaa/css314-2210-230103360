# Task 17 — 3D Stencil Surface-to-Volume Scaling

For a cubic domain of side length `N`, interior updates are proportional to volume:

\[
W_{compute}=\Theta(N^3),
\]

while a one-cell-thick boundary exchange is proportional to surface area:

\[
W_{comm}=\Theta(N^2).
\]

The communication-to-computation ratio is therefore

\[
\frac{W_{comm}}{W_{compute}}=\Theta\left(\frac{1}{N}\right).
\]

Under weak scaling with total volume proportional to processor count, `N^3∝P`, so `N∝P^(1/3)`. Consequently,

\[
\boxed{\frac{W_{comm}}{W_{compute}}=\Theta(P^{-1/3})}.
\]

The surface-to-volume ratio improves as the global cubic domain grows. This does not automatically guarantee better wall-clock performance: in domain decomposition, each processor's local subdomain may remain fixed, so each node's local halo-to-volume ratio can remain approximately constant. Network latency and topology still matter.
