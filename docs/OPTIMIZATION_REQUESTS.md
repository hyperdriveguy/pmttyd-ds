# Optimization research status

The former REND-01…REND-06 proposal inbox is no longer active. Its static call
counts and fixed performance targets were not based on controlled profiling,
several proposals conflict with the renderer's ordering and GPU ownership, and
safe high-priority work has since been implemented.

Use the current records by purpose:

- [Behavior-preserving repair](optimization/BEHAVIOR_PRESERVING_REPAIR.md) —
  corrected state ownership, upload ranges, and exact-output checks.
- [Optimization pass 2](optimization/IMPLEMENTATION_PASS2.md) — actor metadata,
  bounded workspace, single-bin clipping equivalence, and house-audio ownership.
- [Further optimization audit](optimization/FURTHER_OPTIMIZATION_AUDIT.md) —
  remaining candidates, source locations, safety constraints, and evidence
  needed before implementation or performance claims.

The audit is the current handoff. Treat its candidate list as unmeasured until
whole-loop profiling on New 2DS XL supports it. Preserve original assets,
simulation ticks, event order, draw order, and GPU resource lifetimes.
