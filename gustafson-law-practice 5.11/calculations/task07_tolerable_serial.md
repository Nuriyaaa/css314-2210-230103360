# Task 07 — Maximum Tolerable Serial Overhead

Given `P=8192` and required `S(P)>=7500`:

\[
S(P)=P-\alpha(P-1).
\]

Rearrange the requirement:

\[
P-\alpha(P-1)\ge 7500
\]
\[
\alpha(P-1)\le P-7500
\]
\[
\alpha\le\frac{8192-7500}{8192-1}
=\frac{692}{8191}.
\]

Therefore,

\[
\boxed{\alpha_{max}=0.08448236\approx 8.4482\%}.
\]

The parallel machine can tolerate a serial fraction no greater than approximately 8.4482% and still meet the specified scaled speedup threshold, under the Gustafson model.
