# Audience attack EVT callbacks

Implementation: [source](../../source/battle/battle_audience_attack_events.cpp),
[API](../../include/battle/battle_audience_attack_events.hpp).
Evidence: [original event execution](../behavior/battle/AUDIENCE_ATTACK_EVENT_EXECUTION.md).
This guide describes reusable callback ownership; current native effect/display
integration has its own frontier and is not inferred from callback completion.

The registry binds original `_attack_aud` `0x801465e0` and getter `0x801466c4`.
It refuses if either address is already registered. Each native Call captures
backend providers once and uses VM actor/value services. Identity includes actual
EventEntry address, lifetime, ID, command and function. Direct cursor additionally
pins arguments and every actor/value service identity, including lifecycle hooks.
A recycled entry address does not inherit an old cursor. Reentry returns EVT 0;
provider hold also maps to 0, while engaged source 2 finishes and erases the Call.
Caller still owns VM/event retirement and external resource cancellation.

Getter captures four raw output destinations before querying. Attack evaluates
its slot argument and then mode, even though the source query overwrites the
private slot afterward. Both call cached GetItemOn2, not a first-eligible scan:
capture audience A, read `A+0x138bc`, resolve that member with fresh global battle,
reread A's cached slot and copy XYZ at `A+0x138c0/c4/c8`. Item kind output is null
here, so no extra held-kind query occurs. Original cache storage is not rewritten.

Member address is current global battle `+0x2858+0x1bc+slot*0x134`. Each member
query captures global again. This leaf has no slot-bounds/eligibility guard; valid
selected-cache ownership belongs to its caller. Native typed locals replace
source stack outputs without invented guest addresses.

Getter writes slot, X, Y, Z in order. Exact destination sentinel `-250000000`
skips both conversion and store. Slot is integer; coordinates use actual source
truncation. A held later conversion preserves captured XYZ and prior stores;
reset cannot undo them. Missing store/conversion never becomes a fake output.

Attack always adds original binary32 Y literal `0x80422fac` to its local position.
Mode exactly 1 skips effect/audio; other signed modes call `effHitEntry(0,3)`
with scale at `0x80422fb0`. The volatile Y intermediate preserves source float
rounding. After the actual effect completes, fetch the selected member's fresh
XYZ `+0x48/+0x4c/+0x50`, without the local effect offset. That captured position
feeds positional key `0x802f429c`, followed by pitch 1023. Any engaged sound
handle, including -1, is acknowledged. True effect completion may include actual
allocation failure because its scalar return is ignored. Missing providers hold.

Only afterward does another fresh member query OR flag `0x40`. This callback
does not clear items or issue the later holding/damage owner's lifecycle signal.
Publishing flag early would change the source interruption boundary.

Queued pitch helper follows source no-op domains: handle -1 or empty key -1 is
complete. Low eight handle bits select a slot, with no source generation check.
Queued flag bit 0 admits a uint16 pitch store. Out-of-bank native indexes and
occupied live-engine branches stay unavailable. A queued store requires no
fabricated hardware-mixing completion.

See [audience ownership](battle-audience-items.md) and
[Trio draw bindings](trio-draw-bindings.md) for adjacent owners. This callback does
not prove physical input admission, touch visibility, whole effect rendering or
playable original battle readiness.
