
# Karp-Flatt Practice Lab Results

## Student Information
- Student ID: 230103360
- Course: Parallel Computing
- Topic: Empirical Scalability Diagnostics

## Hardware Profile
- CPU Model: Apple M4
- Logical Cores: 10
- Python Version: 3.14.3
- Operating System: macOS

## Experimental Results

Average runtimes from three independent runs.

| Experiment | T1 (s) | T2 (s) | T4 (s) | e(2) | e(4) | Architectural Diagnosis |
|---|---:|---:|---:|---:|---:|---|
| A: Serial Tail | 2.663 | 1.603 | 1.077 | 0.204 | 0.206 | Fixed Amdahl serialization |
| B: Lock Contention | 2.898 | 9.082 | 7.955 | 5.267 | 3.326 | Severe mutex contention |
| C: Privatized State | 0.309 | 0.162 | 0.112 | 0.050 | 0.149 | Improved scalability |

## Experiment A — Serial Tail

The first experiment introduced an intentional sequential delay of 0.40 seconds.

The average Karp-Flatt values were 0.204 for two processes and 0.206 for four processes.

The values remained almost constant as the number of processes increased. This confirms the presence of a fixed sequential portion of the program, consistent with Amdahl's Law.

## Experiment B — Lock Contention

The second experiment used a shared counter protected by a mutex lock.

The execution time increased from 2.898 seconds with one process to 9.082 seconds with two processes and 7.955 seconds with four processes.

The Karp-Flatt values were significantly higher than in Experiment A. This demonstrates substantial synchronization overhead and lock contention.

The e(4) value was lower than e(2), indicating that contention did not increase monotonically on the Apple M4 processor.

## Experiment C — Privatized State

The third experiment replaced the shared counter with private local variables.

Each process accumulated its own result without using a shared lock. The partial results were combined once after all processes finished.

The average execution time decreased from 0.309 seconds with one process to 0.112 seconds with four processes.

The four-process speedup was approximately 2.76x, with parallel efficiency of 69.1%.

This demonstrates that privatization reduced synchronization overhead and improved parallel performance.

## Conclusion

The three experiments demonstrated different parallel performance behaviors.

Experiment A showed the effect of a fixed sequential code section.

Experiment B demonstrated the high performance cost of shared-state synchronization and mutex contention.

Experiment C showed how privatizing shared state can improve scalability by reducing synchronization overhead.

Overall, the experiments demonstrated the importance of identifying architectural bottlenecks when optimizing parallel applications.
