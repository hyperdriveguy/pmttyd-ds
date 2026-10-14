# Native GetMoveFrame callback and operand capture order

Implementation: `include/battle/battle_move_frame_event.hpp`,
`source/battle/battle_move_frame_event.cpp`, `tests/battle_move_frame_event_tests.cpp`.
Contract: [BATTLE_MOVE_FRAME_AND_POSITION.md](BATTLE_MOVE_FRAME_AND_POSITION.md).
Read-only original8010c8c0/1b4 inspection stays /tmp/pm-native-move-frame.*;
no original binary/assets enter history. Verification:
`python3 tools/verify_jump_preparation.py` (13 bodies,68 anchors,5 spans,6 callers).

The callback now composes with actual native event/unit/work bytes and registers
original8010c8c0. It resolves/translates actor,integer X,discarded integer Y,
integer Z and float speed on every call. Integer conversion and distance/divide/
negative fused multiply-subtract/truncation remain required platform providers.
No guessed native distance or count constant replaces these services.

Source-order details beyond the earlier operand summary:

* X is resolved at8010c914,but raw Y is loaded at8010c92c BEFORE single signed
  conversion8010c940 and its evtGetValue call8010c944. Native code retains that
  prefetched raw Y across its explicit conversion provider.
* Similarly raw speed is loaded at8010c968 BEFORE Z conversion8010c97c and
  evtGetFloat8010c980. Preserve that token when conversion mutates source bytes.
* Raw output is captured AFTER float resolution and BEFORE current-work/unit
  lookup. Retained argument pointer and output survive later argument mutations.
* Visible XYZ are queried into private values in order,including unused Y;
  distance gets current X/Z,target X/Z. Only after distance returns is source
  zero80422280 read and,when equal,speed170 read. An equal stored speed reads
  original local fallback1 at8042228c. Neither speed is stored into the unit.
* After dividing,zero80422280 is read AGAIN before truncation/count conversion
  and retained for residual comparison. Do not reload after a numeric callback.

Binary32 fallback equality includes both zero signs;NaN bypasses fallback.
Count is trunc(distance/speed),then incremented with word wrap only if ordered
fused residual distance-convertedCount*speed is positive. Only exact count0 is
replaced by1. Negative nonzero counts remain;this is not a generic ceil/max1.
Raw destinationf1194d80 skips only the store;coordinate/distance/math still run.
Return2 has no first-call/polling gate. Outputs are integer event values.

Native tests cover D101/S5 query21,D100/S5 query20,zero distance1,negative speed
count-19,explicit/local/stored signed-zero speed,NaN bypass,full query on omitted
output,unit-source immutability,raw operand prefetches,current-work/output capture,
post-distance speed reads,second-zero capture,and increment word wrap. Native
registry test uses real event/unit/work RAM. Exceptional truncate/wrap fixtures
are explicit authored provider cases;they do not establish PowerPC FPSCR results
or original caller feasibility. std::hypot/std::fma are injected host providers,
not an asserted original native math backend.

This query remains separate from MovePosition,whose automatic count20 for
D101/S5 is adjusted through speed and whose first call integrates immediately.
The query's result feeds the original first-attack abs(n-22) event expression;
it does not prove motion invocation/impact count. No fixed native schedule is
introduced here. Next:live native MovePosition/movement sound and Mario jump
motion,transitive event graph,source construction/numeric services and complete
outer battle/Game integration plus original execution/hardware validation.
