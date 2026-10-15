# Battle stage storage, objects and spotlights

These are packed guest-memory helpers. Addresses are 32-bit source tokens;
`B` denotes BattleWork, with current ownership read through `0x8041eb90`.
See [battle camera](battle-camera.md) for framing, [feedback drawing](battle-feedback-draw.md)
for TimingA/reels and [battle controllers](battle-control.md) for parent ordering.
Recovered evidence belongs in the [behavior contracts](../behavior/README.md).

## Stage work and floor

Inline stage work is `B+0x163fc`, size `0xb3c`. Twelve entries begin at `+4`,
stride `0xe8`; active bit 1 selects the actual entry-main child, while inactive
entries free their scratch pointer at `+0xe4`. Main captures stage ownership
before spotlight main and keeps it through all children. It clears a pointer
only after the real free provider acknowledges. Init clears storage, then retains
source explicit writes, constants and order; it does not run stage-object or
spotlight initialization itself.

Four byte interpolators use the following relative offsets (all hexadecimal):

| Count | Start/end bytes | Signed mode byte | Duration | Output byte |
| --- | --- | --- | --- | --- |
| af0 | aea / aec | b00 | af8 | ae8 |
| af4 | aeb / aed | b01 | afc | ae9 |
| b10 | b0a / b0c | b20 | b18 | b08 |
| b14 | b0b / b0d | b21 | b1c | b09 |

Signed-positive counts decrement *before* interpolation. The implementation then
reads unsigned endpoints, signed low-byte mode, duration and fresh remaining
count. Elapsed is wrapped `duration-remaining`; a real truncation provider converts
the result before low-byte output storage. The second byte-output group starts
at 140, the first at zero. These source counters count main visits, not measured
render frames. No rollback or retained prefix cursor exists in this parent;
retrying after a missing interpolation/free/child can replay earlier effects.

Floor query ignores passed B/X/Y and recaptures the global stage. If Z is at or
below `stage+0xb2c`, it returns the float word at `0x80422170`; otherwise it uses
`stage+0xb34`. Equality takes the first branch, unordered Z the second. The small
floor initializer writes only these two fields, aliased by absolute BattleWork
offsets `0x16f28/30`, from source words `0x80422b44/48`. The EVT wrapper still
resolves/converts X, reads its raw output token and resolves/converts Z before
capturing B; the leaf then captures B again. This ordering remains observable
through services even though X is unused by the final floor expression.

## Map objects

Thirty-two stage-object records begin at `B+0x1715c`, stride `0x7c`. Only a positive
signed ordinal participates. Four XYZ vectors at row offsets `4/0x10/0x28/0x1c`
compare against caches 0x30 bytes later. Float inequality keeps NaN dirty and
treats signed zeros as equal. A changed vector copies X/Y before either store,
then reads/stores Z. Dirty bit 2 is cleared before map calls. This cache state
records the original source traversal, not native rendering completion.

Layer 2 additionally consumes shared rotation `B+0x180e0` and cache `+0x180ec`.
Do not replace that one shared cache with per-object snapshots. The descriptor
and layer are reread afterward; names are resolved freshly for each map service.
Dirty layers below 8 translate by `(vector4+vector10)+vector28`. Layers 2/3 then
translate about Y pivot `+0x14/+8`, rotate, and freshly read the positive pivot
for the final translation. Layer 2 uses the shared rotation, layer 3 the record
rotation. Other eligible layers rotate directly by row `+0x1c`.

Visibility bit 1 is synchronized with actual map-group flags independently,
including clean records and layers at least 8. A failed transform can follow
cache publication and earlier successful transforms; this immediate parent has
no cursor to replay only its unfinished suffix. The final source config leaf is
a verified no-op; it does not supply missing map services.

## Spotlight ownership

Global `0x804175a0` points at work containing count, bank and draw gate at
`+0/+4/+8`. Records have stride `0x2c`. Separate session SBSS/color state is not
reset merely because a BattleWork or map-heap owner is replaced.

Entry searches for an inactive halfword bit 1. It requires a real free record;
a full positive-count scan reaches the bank end, rather than growing or wrapping.
Defaults come from table `0x802f37a8`, then color `0x804295d4`. Init publishes
count 5 before allocation, clears the admitted bank and creates five entries.
X is `FMA(source step, i, source offset)` from `0x80422b1c/18`; original XYZ loads
remain before overrides. Allocation unavailability can follow count publication.

Main preserves a discarded count read; only a positive signed draw gate submits
camera 6/class 0 callback `0x80137d20`, argument zero and priority from
`0x80422b14`. The source count loop adds no observable per-record work. Free
calls the actual provider and retains all work fields, including the old pointer;
repeating this leaf is not automatically idempotent.
