# Positional SFX slot provisioning: corrected complete local body

US G8ME01 disc0 revision0, __psndSFXOn800d9934/0x540; independently
reinspected2026-10-13. Replaces incorrect name-only, mask, mute, reuse,
reclaim, default-parameter, handle-wrap and global-base claims in the earlier
version. Static disassembly, not original playback or backend equivalence.
See [Off](BATTLE_POSITIONAL_SFX_OFF.md), [accessors](BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md)
and [update ownership](POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md).

## Key ABI and source selection

Capture r3..r10 as key,volume,pan,delay,position,user-position-mode,distance,
mute-override. These names describe local uses, not verified caller domains.
Retain all eight before processing. The signed test of key at800d9974 admits:

- Signed key>=0: use raw key word directly as K, with no strcmp.
- Signed key<0: treat it as a string pointer, scan4242 stride20 records using
  strcmp(row.word0,key). Match retains its ordinal; miss supplies K0.

Thus numeric sound IDs and original0x8xxxxxxx name pointers coexist.
Neither a name miss nor raw key0 directly returns failure. Compute I=K&0x1fff
(LOW13 bits) and R=80316b28+20*I. If R.word4==0, return-1. There is no
local index bound for arbitrary nonnegative K. Table extent4242 is evidenced
by symbol size0x14b68; the mask alone admits8192 rows.

## Global and content gates

G=803df6e8, original `psnd` BSS symbol, extent0x60. It is not803df188.
If G.unsignedHalf56 mask4 is set AND r10==0, return-1. Nonzero r10 bypasses
this mute gate; earlier prose reversed that condition.

Exactly I in {91,2250,446,447,517} takes the cooldown gate. Read signed byte
G5c: nonzero returns-1; zero stores6 before continuing. All other indices
skip this gate. Then evtGetValue(NULL,0xf5de0180); if its signed result<=2
and I==869, return-1. Keep this story query/request order rather than
inventing a generic cooldown for every sound.

## Slot search, preference and reclaim

P array=803df988,40 records of40 bytes. First scan uses20 loop iterations
with TWO records each: all40 slots, not only20. For each record:

- Occupied K' with (K'&1fff)==I and flags6 mask100: if local preference D0,
  store low16(G54+8) back to G54 and retain that low16 value in D.
- Free word0==-1: retain this slot/index; keep scanning. The LAST free slot
  from this first scan is selected. Other slot fields are not validated.

If a free slot was retained, skip reclaim. Otherwise scan40 slots ascending.
A free slot is accepted immediately. An occupied slot is eligible for reclaim
only when its unsignedHalfC is STRICTLY GREATER than R.unsignedByte10;
comparison is not equality. Invoke the same ordered flag1/engine-handle/backend
stop logic as [Off](BATTLE_POSITIONAL_SFX_OFF.md), clearing old key/engine
handle before selecting it. The raw stop selector here is the loop slot
ordinal (its low8), not its encoded allocation handle. First admitted record
wins. No selected pointer after this pass returns-1.

Same-key preference is an offset input for the new slot; it does not replace
its table key. Search/reclaim do not execute the eventual backend start.

## Defaults, flags and field publication

Low8 volume==ff replaces volume with127. Low8 pan==ff replaces pan with
R.byteD. These are LIVE equality defaults, not unreachable clamps. Other
values narrow on the eventual stores without saturation.

Set flags6=0101, then OR these gates in instruction order:

| Condition | OR mask |
| --- | --- |
| G.half56 mask10 | 0010 |
| Raw K mask04000000 | 0020 |
| I==1304 or1858 | 0280 |
| R.byteE==1 | 0200 |
| Raw K mask00010000 | 0400 |
| Raw K mask00020000 | 0800 |
| Raw K mask00040000 | 0080 |

Offsets/masks in these tables are hexadecimal. Store RAW K to P.word0
at800d9d24. A resolved name supplies its ordinal; a numeric key can retain
additional high bits. All row readers still mask low13 bits. There is no
index-versus-packed-key conflict.

| P offset / width | Ordered source |
| --- | --- |
| 08 /byte | Retained/defaulted volume |
| 09 /byte | R.byteC |
| 0A /byte | Retained/defaulted pan |
| 0E /half | Truncated delay/preference expression below |
| 10 /half | low16(5*D), distinct from the delay expression |
| 0B /byte | R.byteF |
| 0C /half | ZERO-extended R.byte10, not a row halfword copy |
| 14 /half | Signed R.half12, bit-preserving store |
| 1C,20,24 /word | Position words if position pointer nonzero; first pair captured before first stores |
| 12 /half | Low16 r9 distance |
| 16 /half | 32000 decimal |
| 04 /half | Low16 incremented G.half50 generation |

Delay arithmetic uses original single operations, integer conversion doubles
802e3c60=4330000080000000 and802e3c68=4330000000000000, and float
804218cc=1000. Let F=S32(current GP.word4), A=U16(r6), D=retained preference,
C=U8(R.byte11). Compute in source order:
`q=fdivs(1000,F); q=fdivs(A,q); q=fadds(D,q); q=fadds(C,q); fctiwz(q)`.
Store low16 conversion word at P0e. No zero-refresh or invalid-float guard.
P10 receives low16(5*D), not a second copy of fctiwz's result.

For nonzero position pointer, r8==0 ORs flags2; nonzero r8 ORs flags4.
For a null pointer, retain prior position words (no whole-record clear) and
neither bit is added. The selected slot can therefore retain stale coordinates
until a later producer writes them; do not synthesize a zero position here.

Finally increment unsigned16 G50, store its low16 to P4, RELOAD unsigned16 G50,
shift left8, OR chosen slot ordinal and return that handle. This guarantees
handle bits8..23 agree with P4 even on generation wrap. It supplies no engine
handle P18 at this stage: [the update](POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md)
processes pending flag1 later. The public return identifies a slot/allocation;
it is distinct from the backend voice/channel word stored at P18.

## Validation and remaining boundaries

Whole provisioning body, selected instruction anchors, source table and numeric
constants are in [metadata](../../../tools/positional_sfx_corrections_evidence.json).
Run `python3 tools/verify_overworld_roads.py --manifest tools/positional_sfx_corrections_evidence.json`.
Inspection ELF/disassembly remain `/tmp/inbox-sfxon.*`, outside history.

Complete local provisioning does not establish engine startup/macro execution,
spatial attenuation, all scene/slot writers, resource loading or original audio
observations. Full psndSFXMain middle/start/position/volume/effect paths remain
partially recovered. G5c decrements in psndMain, not psndSFXMain; G54 resets at
the latter's tail. Original asset/resource and event ordering must be retained.
