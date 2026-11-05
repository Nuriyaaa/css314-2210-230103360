# Task 10 — Memory Growth Rate vs. Scaled Speedup

The task states a strict per-node memory cap of 32 GB and asks for the maximum cubic grid dimension across 64 and 512 nodes. A bytes-per-grid-cell assumption is required to convert memory into a cell count; the task sheet does not specify one.

**Assumption for a reproducible estimate:** one scalar double-precision value (8 bytes) per grid cell, all aggregate node memory is available for the grid, and 32 GB means 32×10^9 bytes.

For `Q` nodes, total memory is `M=32×10^9 Q` bytes. For an `N×N×N` grid:

\[
8N^3\le 32\times10^9 Q
\quad\Rightarrow\quad
N_{max}=\left\lfloor\left(\frac{32\times10^9Q}{8}\right)^{1/3}\right\rfloor.
\]

| Nodes Q | Aggregate memory | Maximum cells | Approx. cubic dimension N |
|---:|---:|---:|---:|
| 64 | 2.048 TB | 256 billion | 6,349 |
| 512 | 16.384 TB | 2.048 trillion | 12,699 |

The node count increases by 8×, so total memory increases by 8× and the cubic dimension increases by `8^(1/3)=2×`. Real applications need additional arrays, halos, metadata, runtime memory, and safety headroom; therefore these are theoretical upper bounds, not recommended production grid sizes.
