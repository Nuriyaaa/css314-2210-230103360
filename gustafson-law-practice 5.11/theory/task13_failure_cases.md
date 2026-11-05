# Task 13 — Boundary Cases: When Gustafson's Assumption Fails

## Case 1: Breadth-First Search on a power-law graph
BFS has irregular frontier sizes and data-dependent memory access. Increasing graph size can increase remote edge traffic, cache misses, synchronization, and load imbalance. A few high-degree vertices may become contention points. Consequently, communication and coordination time can grow with the graph rather than remain a fixed fraction.

## Case 2: All-pairs shortest paths with a global synchronization pattern
For iterative algorithms such as parallel variants of Floyd–Warshall, each phase may require broad data exchange or barriers. Expanding matrix dimensions increases both arithmetic work and communication volume. If communication or synchronization grows faster than useful computation, the wall-clock time increases and fixed-time weak scaling fails.

## Why the premise breaks
Gustafson's ideal model assumes that the serial fraction remains stable and that the enlarged parallel work is distributed effectively. Real systems may violate both assumptions through contention, memory capacity, network bisection limits, synchronization, and algorithmic complexity changes. Weak scaling must therefore be validated experimentally for the target algorithm and machine.
