# Retained queued weapon drawing

[The parent](../../source/battle/battle_weapon_draw_continuation.cpp) retains the
whole original callback/Disp/Win/Common/Sub1 graph. Wrapper camera/B are native
invocation identity. Win captures actual global battle, work, cursor, acting unit
and selected weapon; Common makes separate fresh captures. A changed global
between those source reads must remain visible. Context/UI/Camera8/queue ownership
is proved by `current`, checked between synchronous groups and around providers.
Observed loss latches stale until reset. Reset does not undo prefix stores.
See [source ownership](../behavior/battle/RETAINED_QUEUED_WEAPON_DRAW.md).

## Common and scrolling

Signed visible count is min(count,6), including the original negative-count
arithmetic. Ordered cursor-top corrections precede dispatch through the actual
source table at 0x803636e8. Modes 0,2,10/20,30/40 represent opening, steady,
adjacent scroll and wrap scroll. Real branches publish W+0x14=1 before later
geometry/raster children. That input-ready prefix is not complete callback or
mixed GPU submission acknowledgment.

Sub1 retains two six-visible-row wrap tests with selection exceptions 1/5 and
fresh count/top reads. Modes 30/40 interpolate count-6 to/from zero. Adjacent
top changes +1/-1 select modes 10/20. Each transition resets all seven submenu
state/timer pairs. No generic scroll-delta replacement merges these cases.

Class arrays use four-byte lanes at W+0x20 flags, +0x34 state, +0x48 timer and
+0x84 ordinal. Their original private-output domain has five entries. A sixth
required class cell holds at the native domain boundary; it is not clamped or
invented. Negative class counts retain source no-row behavior.

| Class state | Source work |
| --- | --- |
| 0 | Entry interpolation; matrix, parameter and private position written |
| 1 | Steady matrix/parameter; private position stays unwritten |
| 10 | Ring shrink; source ring-offset Y becomes Z, then scale/translation/concat |
| 11 | Hide by clearing flag 1 |

Entry uses fresh wrapped GP-rate*12 followed by signed /60 for each source
interpolation group. X/Y endpoints are -60/55; translation adds source X offset
20. Phase runs toward 360 with only one subtraction when greater, not modulo.
Timer >=3 exposes W+0x1c; the incremented timer strictly greater than duration
selects state 1. State-1 translation is prepared before its later PC; retries
must not execute its builder or reload those operands again.

Ring angle uses the original binary32 PI approximation times table degrees /180.
Shrink is `-fma(.1,timer,-1)`. Nonpositive factor marks state 11, but the current
row still computes geometry; there is no scale clamp or immediate return. Source
rounded add/sub/mul/div and explicit FMA/SDK boundaries remain distinct.

Seven physical submenu slots include buffer row 6. State 0 hides; state 1 uses
negative timers for staggered entry and Y-axis rotation (axis 121); state 2 keeps
six identity-matrix rows with buffer hidden. Scroll states enable the buffer.
Alpha bytes W+0x114+i become 255. Adjacent duration is 12; wraps use
`12 + signed(count-6)/2`. Each reached row interpolation writes shared scroll
W+0x98, then advances its own timer. Do not reduce this to one scroll update.

## Metadata, private knowledge and children

Each of seven metadata iterations captures live Win work scroll and cursor count
before fctiwz. Signed wrapped rank gates 28-byte records; skipped cells stay
unknown. Label/enabled/cost use fresh work+8 row pointers. Icon reads use the
captured battle menu instead. Cost receives retained Win unit/weapon; after that
child, BP rereads the row's weapon and may observe a different pointer.

The exact child order is MainIcon, SubIcon, cursor-window, then optional help.
All borrow the same cursor-owned output object. Missing private cells are not
filled with zero, identity matrices or guessed values. Consumers reproduce their
own original row/flag gates before requiring unwritten inactive cells, and copy
anything needed after the invocation. See [subicons](weapon-subicon-continuation.md).

Help retains Win's earlier selected weapon. Its nonzero item field indexes the
40-byte itemData table at 0x803108a8. Null weapon or special 0x80365488 with no
item instead uses a fresh cursor/row item ID; other weapons read their own +0x0c
help pointer. Zero help key skips the child. Help XY -200/-100 are original
source coordinates, not native screen reflow. Cursor-window carries three actual
raw source words at 0x802ef750/754/758.

SDK/scalar/raster providers own whole partial invocations. Held requests retain
operands, output identity and successful prefixes. An engaged scalar zero
acknowledges a child; only `nullopt` holds. This local source parent is composed
with separate [frame/publication owners](weapon-window-bridges.md); storing an
early readiness byte or a captured request does not submit an entire native batch.
