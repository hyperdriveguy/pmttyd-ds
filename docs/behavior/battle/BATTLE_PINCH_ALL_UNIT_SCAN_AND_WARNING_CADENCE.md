# All-unit pinch scan and shared warning countdown

Scope: complete `BattleCheckAllPinchStatus`, US G8ME01 revision 0,
`8011efd8..8011f1eb` (0x214 bytes). Static evidence, not measured playback.
Its [manager caller](BATTLE_SEQUENCE_MANAGER_BANKS_AND_OUTER_HOOK.md) invokes
it in bank 0 after init with F=1, and banks 1/2 before first-act/turn with F=0.
Those bank choices do not prove an invocation rate in seconds or render frames.

## Phase 1: update every registered unit

Retain input battle owner B and flag F. For each signed slot I=0..63, call
`BattleGetUnitPtr(B,I)` freshly; nonnull U receives
`BtlUnit_CheckPinchStatus(U,F)`. No type, status, side or HP filter applies to
this loop and service returns are ignored. See [per-unit transitions](BATTLE_PINCH_THRESHOLD_FLAGS_AND_TRANSITIONS.md).
Callbacks may alter registry entries seen by later iterations. Do not snapshot
the entire unit registry at entry.

## Phase 2: search first flag, then second

Start a fresh increasing slot scan for bit 0x20000000. Advance to the next
slot only when U is null, the current flag is clear, `BtlUnit_CheckStatus(U,27)`
returns nonzero, or S8(`BtlUnit_GetBelong(U)`) is not zero. Calls are ordered
flag test, status service, side service; later tests read the retained U afresh.

The first actor passing those four preconditions ends this scan unconditionally.
Its fresh WORD(U+8) must be 222 or signed 224..230 and its S16(U+0x10c) must
be positive to mark FOUND. An invalid kind or nonpositive HP ends the scan
with FOUND=false; later slots are not visited. This is a first-prequalified
scan, not an existential search over all fully eligible units. For example a
prequalified dead slot 0 can hide a living flag-bearing slot 1 in this group.

If FOUND=true, process the shared countdown and return. This skips the entire
second group even if no sound plays. Otherwise start again at slot 0 for bit
0x10000000, with the same service ordering, first-prequalified stop, kind and
HP checks. A failed first group may therefore allow a second-group warning.
Each group obtains registry entries independently.

## Countdown and sound

Both groups share BYTE(B+0x18ff8). For a found group:

1. Read the byte unsigned, subtract one and store its low eight bits.
2. Read that byte again as signed eight-bit. If positive, return immediately.
3. Otherwise store 60, then read WORD(B+0x1720). If bit 0x1 is set, call
   `psndSFXOn` with the group's source key; otherwise return without sound.

| Found bit | Original key address | Original key |
| --- | --- | --- |
| 0x20000000 | 802efc04 | SFX_BTL_DANGER1 |
| 0x10000000 | 802efc14 | SFX_BTL_PINCH1 |

Reset occurs even when the sound gate is clear. Group changes carry the same
countdown forward rather than restarting a separate timer. Old values 0, 128
and 129 produce respectively reset-to-60, stored-positive-127, and reset-to-60:
this is byte wrap followed by signed interpretation, not saturation. With
stable qualifying actors and initial zero, the first call resets/requests a
sound and the next request occurs 60 qualifying invocations later. No measured
60 Hz or one-second cadence is claimed. F affects only per-unit callbacks;
F!=0 does not suppress the warning phase or countdown.

## No found group

If neither scan marks FOUND, call `BattleGetMarioPtr(B)`. When nonnull, clear
WORD(Mario+0x238), then always clear BYTE(B+0x18ff8) even if Mario is absent.
No party +0x238 field is cleared here. This routine assigns no defined scalar
return status; its caller ignores the incidental r3 value.

## Native requirements and remaining dependencies

Preserve the all-64 update pass, separate ordered scans, shadowing by final
ineligible actors, shared wrapping byte timer, gate read after reset, and Mario
reset ordering. Resolve status 27, side retrieval, sound delivery and cadence
through their respective service contracts; this complete caller does not close
those implementations. [Evidence/source binding](BATTLE_PINCH_AUDIENCE_REQUESTS_AND_SOURCE_THRESHOLDS.md)
pins the complete bodies and original keys.
