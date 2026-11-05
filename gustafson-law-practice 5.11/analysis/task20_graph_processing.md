# Task 20 — Parallel Graph Traversal and Irregular Workloads

BFS and PageRank on power-law graphs are difficult weak-scaling workloads because graph structure is irregular.

**BFS:** frontier sizes vary by level. High-degree vertices can create work imbalance, while visited-state updates require synchronization or atomic operations. Distributed execution may exchange frontier information across partitions.

**PageRank:** each iteration performs repeated edge-based updates and typically requires convergence checks or reductions. Degree skew, irregular memory access, and partition boundaries affect communication and load balance.

Regular dense linear algebra has predictable work per tile, regular memory access, and well-understood communication patterns. Graph algorithms instead exhibit data-dependent work and access patterns. Enlarging a graph can change its degree distribution, diameter, partition quality, and communication volume, so the serial fraction may not remain constant. Gustafson's model is still a useful ideal reference, but graph weak scaling must be validated using measured frontier balance, edge cut, memory behavior, and communication time.
