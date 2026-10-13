# Positional SFX accessors: corrected complete local contracts

US G8ME01 disc0 revision0, independently reinspected2026-10-13.
Earlier low-five-bit, shifted-index, pitch-branch and Chk arithmetic claims are
superseded here. Static bodies only; no original playback trace.

| Function | Address / hexadecimal byte size | Result / mutation |
| --- | --- | --- |
| psndSFX_get_vol | 800d9558/48 | Unsigned volume byte, or0 for raw handle/key-1 |
| psndSFXChk | 800d95a0/58 | 0 for matching generation and occupied key; -1 otherwise |
| psndSFX_dist | 800d96c0/38 | Store low16 caller value at slot12 |
| psndSFX_pit | 800d9744/8c | Queued local pitch or engine pitch path below |
| psndSFX_vol | 800d97d0/38 | Store low8 caller value at slot8 |

All resolve P=803df988+40*(rawHandle&ff), with decimal stride40. All reject
rawHandle==-1 before slot access. Get-vol/dist/vol/pit then reject P.word0==-1;
Chk instead checks allocation halfword first and derives key liveness below.
There is no low8 slot bound or stale-generation test in the other accessors.
Stores truncate without clamping. Void leaves supply no scalar result contract.

## Generation and key-liveness check

Chk compares (handle>>8)&ffff with P.unsignedHalf4; mismatch returns-1.
After equality, read key=P.word0 and compute wrapping words:
`a=key+1; b=-1-key; return S32(~(a|b))>>31`.
Because b=~key, this equals arithmetic shift of `key & ~(key+1)`.
Its sign bit is set exactly when key==ffffffff; the returned value is then-1,
otherwise0. It is not a positive1 overflow flag or an early key-null branch.

The start routine reloads the incremented unsigned16 allocation counter before
shifting it into handle bits8..23. Counter wrap therefore preserves the encoded
comparison. It can reuse an old generation after65536 allocations; there is no
proof of permanent handle uniqueness. The previously asserted divergence at
counter overflow was incorrect.

## Pitch and deferred versus started slots

At800d9784/788, test flags=P.unsignedHalf6 mask1. NONZERO stores low16 pitch
at P14 and returns without engine request. ZERO takes the backend path:
row=80316b28+20*(key&1fff), then read row.word4. If numeric mask80000000 is
set, return unchanged. Otherwise call SoundEfxSetPitch800e0bbc with P.word18
and original pitch r4. This body has no separate song-mask or engineHandle-1
check. Do not infer engine eligibility beyond these actual tests.

The two consecutive branch-not-equal instructions800d97b0/7b4 test the SAME
condition and target the same return. Preserve that observation; no invented
second descriptor-bit check. row+0 is a name pointer, while row+4 is the
backend descriptor. Both pitch and [Off](BATTLE_POSITIONAL_SFX_OFF.md) read+4.

## Field widths and acceptance

P0 key word, P4 generation unsigned16, P6 flags unsigned16, P8 volume unsigned8,
P12 distance unsigned16, P14 pitch low16, P18 engine handle word. Off clears
P0/P18, while [position update](BATTLE_POSITIONAL_SFX_POSITION_UPDATE.md) writes
P1c/20/24. These fields are relative offsets in hexadecimal, not an exhaustive
record schema or lifetime proof.

Acceptance: rawHandle-1 before access, occupied/free key, matching/mismatched
allocation,255/256 volume and65535/65536 distance truncation, flags1 local pitch
versus flags0 backend path, descriptor80000000 suppression, raw engineHandle-1
passed through when other guards permit. Handle bounds/callback aliases remain
source-domain obligations, not original graceful failures.

Verify [metadata](../../../tools/positional_sfx_corrections_evidence.json) using
`python3 tools/verify_overworld_roads.py --manifest tools/positional_sfx_corrections_evidence.json`.
All five whole bodies and selected branch/mask/load/store anchors are pinned.
[Provisioning](POSITIONAL_SFX_SLOT_PROVISIONING.md) and
[update ownership](POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md) record
newly recovered source transitions and still-open backend/runtime boundaries.
