# Field party clear, kill, restoration, stop and run callbacks

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
local DOL bodies and actual wall/lecture gor imports. This closes callback
dispatch and small pointer/run-mode services, not all party creation/destruction,
mode initialization, save state or original battle transition execution.

| Original service | DOL address | Size |
| --- | --- | --- |
| evt_mario_clear_party | 0x800e2da0 | 0x6c |
| evt_mario_kill_party | 0x800e2e0c | 0x6c |
| evt_mario_set_prev_party_pos | 0x800e2f00 | 0x158 |
| evt_party_run | 0x800eae70 | 0x48 |
| evt_party_stop | 0x800eaeb8 | 0x48 |
| marioGetPtr | 0x8005e0c0 | 0x8 |
| partyGetPtr | 0x800bf1b4 | 0x20 |
| partyStop | 0x800becb8 | 0x24 |
| partyRun | 0x800becdc | 0x30 |
| partyChgRunMode | 0x800bcb2c | 0x6c |

## Raw slot dispatch for clear and kill

Both callbacks call marioGetPtr, which reads the pointer at r13+0x19e0
(US anchor 0x8041e900). They read operand 0 **raw**, without evtGetValue,
and add it to the Mario work pointer. The byte at **M+0x245+slot** is
sign-extended. If negative, return 2 without a destruction service call.
There is no slot range check or null Mario check in these wrappers.

If that byte is nonnegative:

| Wrapper | Raw slot 0 | Any raw nonzero slot |
| --- | --- | --- |
| clear | partyKill(signed byte), DOL 0x800be69c | partyKill2(1), DOL 0x800be594 |
| kill | partyKill(signed byte) | partyKill(1) |

Both then return 2. The nonzero slot's byte participates in the existence
test but **does not become the destruction argument**; the argument is
literal 1. In particular, treating clear and kill as identical for every
slot would change the original dispatch.

The preceding wall event's clear(1) at REL 0x49168 therefore invokes
partyKill2(1) only if M+0x246 is nonnegative. The lecture's kill(0) at
0x49858 invokes partyKill with signed M+0x245 if nonnegative. The
[partyKill/partyKill2 bodies and saved-kind writes](FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md)
are now specified locally;pose release/allocator and actual transition execution
remain dependencies; these callbacks alone do not establish resource reset
or the eventual saved partner identity.

## Previous-position callback may request creation

evt_mario_set_prev_party_pos captures the callback first-call argument,
operand array and Mario pointer. Operand 0 is again a raw slot. If the
first-call argument is zero, it immediately returns 2: there is no polling
continuation or repeated creation attempt in this callback.

On a first call, if signed M+0x245+slot is nonnegative, return 2 without
reading the three coordinate operands. Otherwise resolve operands 1..3
with **evtGetValue**, in X,Y,Z order, and convert each signed integer to
a single-precision coordinate using the original bias conversion
(0x802e3eb8 = 0x4330000080000000). These are integer resolutions, unlike
the float-writing Mario position query and float-resolving intersection.
For the encoded LV coordinates used here, evtGetValue reads the local word,
decodes the scaled float and then fctiwz truncates toward zero. The constructor
therefore receives integer-truncated coordinates after the query's 1/1024
quantization; preserving fractional coordinates through this callback would
change the original conversion route. Ordinary integer words remain integers.

Next read signed byte **M+0x247+slot**. Zero returns 2 without creation,
after the coordinate reads have already happened. Negative values are not
excluded by this check. With a nonzero saved byte:

| Raw slot | Requested service |
| --- | --- |
| 0 | partyEntryPos(saved signed byte, float X,Y,Z), DOL 0x800be84c |
| Nonzero | partyEntry2Pos(saved signed byte, float X,Y,Z), DOL 0x800bd6d8 |

If the service result is signed-negative, return 2 without writing a slot
index. Otherwise store its low byte at **M+0x245+slot** and return 2.
There is no local allocation-failure retry, coordinate clamp, saved-byte clear,
resource restore or explicit battle-state test.

Thus the lecture's request at REL 0x498e8 **can create a field party through
the called service** when the current slot is negative and saved byte is
nonzero. The field event lacks a separate create opcode; that does not imply
its previous-position callback can only change coordinates. Which saved
byte kill/transition leaves, and what constructors restore, require their
[constructor/destructor bodies](FIELD_PARTY_POSITION_CONSTRUCTORS.md) and
original lifetime evidence.

## Stop/run select a party index through a separate route

evt_party_stop and evt_party_run read operand 0 raw. If it equals zero,
load the current party index from r13+0x1bb0 (**0x8041ead0**). Every
nonzero raw operand selects literal index 1; it is not an arbitrary pointer
index or evtGetValue operand. Then call partyGetPtr(index).

partyGetPtr returns null for a signed-negative index. Otherwise it reads
the pointer table at r13+0x1bb8 (**0x8041ead8**) with index shifted left
two, without an upper-bound check. Each wrapper skips its mode service if
the returned pointer is null and returns callback code 2 in all normal cases.
This route does not directly consult M+0x245 in the wrapper.

partyStop always calls partyChgRunMode(P,0). partyRun reads byte P+0x34
and calls partyChgRunMode(P,1) **only if it is zero**. A nonzero current
mode is left unchanged; run is not an unconditional reset to default mode.

partyChgRunMode captures the requested mode and pointer. If the old byte
P+0x34 is neither 9 nor 3, copy it into P+0x35. Call the service at
0x800bcb98 with (P,old mode,new mode), then store the requested mode's low
byte at P+0x34 and OR bit 8 into the word at P+8. The service call occurs
**before** those last writes. Its [local initialization](FIELD_PARTY_MODE_INITIALIZATION.md) and
[partyMain eligibility](FIELD_PARTY_DRIVER_AND_INDEX_OWNERSHIP.md) are now
specified. Reentrancy,all mode update bodies and scheduler composition remain
open. Do not claim run/stop directly starts or stops all related EVT entries.

## Verification and open composition

tools/verify_field_lecture_callbacks.py with field_lecture_callback_evidence.json
checks these ten complete bodies and the two
[coordinate/intersection bodies](FIELD_SEGMENT_INTERSECTION.md), plus containing
hashes for three general variable services whose used local branches alone are
specified. It checks 54 selected instructions, six constants and ten actual gor
imports. Repeatable reports
live under ignored artifacts/reference/behavior. Binary fingerprints and
dispatch checks do not prove original party transitions or on-disk saves.

Next:destructor hook/pose/heap internals and constructor all-kind tables/init/
resource persistence;run-mode initialization
and global current-index writers;field-to-battle teardown and original lesson
accept/decline/interruption/reentry traces. None of these local callback closures
completes all party mechanics, the lecture or the game.
