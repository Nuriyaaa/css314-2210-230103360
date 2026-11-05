# Task 18 — N-Body Gravitational Problem Scaling

## Direct all-pairs method
The direct method evaluates interactions between particle pairs and performs `Theta(N^2)` work per iteration. If the total work is scaled proportionally to `P` and ideal parallelism is assumed, then

\[
N^2\propto P\Rightarrow N\propto P^{1/2}.
\]

## Barnes–Hut tree code
Barnes–Hut typically requires `Theta(N log N)` work for a well-behaved spatial distribution. To scale total work proportionally to `P`, solve

\[
N\log N\propto P.
\]

This is not a pure power law; approximately, `N` grows like `P/log P` (up to logarithmic corrections). Tree construction and traversal introduce irregular communication and load-balance costs.

## Serial/parallel ratio
The direct method has a regular pairwise computation but may require reductions and data distribution. Barnes–Hut reduces asymptotic work but introduces tree dependencies, irregular traversal, and possible synchronization. Thus, the serial/communication fraction is implementation- and distribution-dependent; it cannot be assigned a single numeric value from the complexity classes alone.
