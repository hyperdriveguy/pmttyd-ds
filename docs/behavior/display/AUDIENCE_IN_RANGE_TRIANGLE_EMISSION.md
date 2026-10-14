# Audience in-range triangle emission

The native audience backend transforms and validates each source quad as before.
When every original UV lane lies in0..1, it emits the same triangles0/1/2 and
0/2/3 directly into its retained frame vertex vector. Atlas mapping and RGBA are
unchanged. This avoids temporary polygon/vector allocations for ordinary poses.

If any UV lies outside that range, the existing boundary tessellation remains:
cut at0/1, retain both sides and the whole geometry, clamp per piece, then map the
original atlas cell. The original1.075 end-pose extent is not reduced or discarded.
Nonfinite inputs and unavailable frame resources keep their existing rejection.
Neither path removes audience members, source requests, animation visits or
simulation ticks. This is an equivalent native GPU preparation optimization, not
a recovered change to the original draw routine.

Tests exercise all four original UV templates from802fa024 using unchanged
source quad geometry. Each retains area1600, stays inside its own atlas cell and
preserves input RGBA. Existing normals, flips, guest texture reuse, alpha passes,
source-created live members and overshooting-pose tests remain applicable.
Full-crowd graphics performance and native gameplay require separate evidence.
