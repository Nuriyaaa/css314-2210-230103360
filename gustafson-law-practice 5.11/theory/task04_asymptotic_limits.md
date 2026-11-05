# Task 04 — Asymptotic Analysis of Parallel Efficiency

Gustafson speedup is

\[
S_G(P)=P-\alpha(P-1).
\]

Divide by `P`:

\[
E_G(P)=\frac{S_G(P)}{P}=1-\alpha+\frac{\alpha}{P}.
\]

For constant `alpha`,

\[
\boxed{\lim_{P\to\infty}E_G(P)=1-\alpha}.
\]

Thus the efficiency approaches a non-zero constant when `alpha<1`. For example, if `alpha=0.04`, the limiting efficiency is `0.96` or 96%.

For Amdahl's fixed-size model,

\[
S_A(P)=\frac{1}{s+(1-s)/P}
\]

and

\[
E_A(P)=\frac{S_A(P)}{P}
=\frac{1}{Ps+(1-s)}.
\]

If `s>0`, the denominator grows without bound, hence

\[
\boxed{\lim_{P\to\infty}E_A(P)=0}.
\]

If `s=0`, the idealized Amdahl efficiency remains 1. The contrast follows from the assumptions: fixed total work causes diminishing work per processor, while weak scaling increases total work to keep processors usefully occupied.
