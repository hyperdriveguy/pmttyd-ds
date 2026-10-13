# MusyX synthesis volume selectors and fade publication

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete synthVolume8027917c/530hex
and sndConvertMs80290550/10hex. Addresses/offsets/masks hexadecimal;
selectors/indices/counts decimal. Static source semantics, not original audible
fade measurements. Read [sequence volume forwarding](MUSYX_SEQUENCE_SPEED_VOLUME_AND_GROUP_FORWARDING.md)
and [synthHandle fade consumer](MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md).

## Raw ABI, duration conversion and target arithmetic

synthVolume(V,T,G,M,ID) receives five raw words in r3..r7. Retain V/G/M/ID.
D=T.low16 is placed in a local stack word; only if D!=0 request
sndConvertMs(&D). Its COMPLETE body loads the pointed word, shifts left8 with
32-bit truncation, stores back, returns. No hardware clock/service/lookup;
no null/alignment guard. In this caller D=(T.low16)<<8, so D lies0..16776960
and is zero exactly when T.low16==0. High T bits are discarded before conversion.
The name alone does not establish physical milliseconds or admitted frame rate.

G.low8 selects the route below. V.low8 is converted by the double-bias literal
80429148=4330000000000000 (2^52), separate single fsubs, then single fmuls
with K at80429120=3c010204 (single-rounded1/127).
Target A=fmuls(K,float(U8(V))). No clamp to0..127 or target1, signed reinterpretation
or host fused arithmetic. V=255 gives a target above1. D nonzero uses the same
bias conversion and fdivs(1280,float(D));1280 literal8042915c=44a00000.
The coefficient origin literal is1 at80429130=3f800000. Preserve these single
operations rather than algebraically replacing the division with5/T.low16.

## Exact selector routes and row membership

synthMasterFader base803f8754, row stride30hex,32 rows. Special selectors
scan all32 rows in ascending order, freshly reading each row.byte2d:

| G.low8 | Admitted row.byte2d values | Scan |
| --- | --- | --- |
| 255 | 0 or1 | All32 |
| 252 | 2 or3 | All32 |
| 250 | 2 | All32 |
| 251 | 3 | All32 |
| 253 | 0 | All32 |
| 254 | 1 | All32 |
| 0..249 | Ignored | One directly indexed row |

For a scan, target A is computed once and retained. Each admitted row gets
mode=M.low8 and sequence wordffffffff, regardless of raw input ID. Other rows
are untouched. No row-active gate or count taken from a table header. A child's
mutation of later row types can affect subsequent admission; no membership
snapshot. The scan ordinal determines the bitmap bit, not the row type.

Direct route selects R=base+G.low8*30hex with no bounds check or row-type gate.
Store mode=M.low8 and sequence word=raw ID. Thus selectors32..249 derive writes
past the32-row table; do not turn them into special broadcasts or silently
clamp them. Valid direct-group bounds must come from callers/providers.
No runtime contents or actual out-of-table execution is claimed here.

## Ordered admitted-row publication

For either route, FIRST store R.byte2c=M.low8, THEN R.word28=selected ID
(raw ID for direct,ffffffff for scan). D determines the rest:

| D | Ordered float writes |
| --- | --- |
| Nonzero | R.float8=old R.float0; R.float4=target A; R.floatc=1; R.float10=fdivs(1280,float(D)) |
| Zero | R.float4=target A; R.float0=target A |

The old current value is read before its float8 store. On direct nonzero route
its load is indexed from retained table base/index; target is computed within
that route after retaining the old current. Scans use the once-computed target.
No row reset, pause-fader writes, active-bit clear or replacement of old
float8/c/10 on ZERO duration. In particular, zero-duration publication leaves
those earlier coefficients and start value intact, yet still sets active below.
Do not infer a stable no-fade state or repair the coefficients in a faithful
local binding. Later synthHandle reads the published row with its own admission
and numerical rules.

After zero-duration float stores ONLY, reload fresh R.word28. Ifffffffff, no
sequence request. Otherwise read fresh R.byte2c and dispatch:

| Stored byte mode | Immediate request |
| --- | --- |
| 1 | seqStop(fresh ID) |
| 2 | seqPause(fresh ID) |
| 3 | seqMute(fresh ID,0,0) |
| 0/4..255 | None |

Core sequence child returns ignored. Mode is a byte, not low4; M=257 stores1
and requests stop for a non-missing ID. This differs from tagged sequence-volume
routing, which selects M.low4. Scan paths contain the same fresh ID/mode checks,
but just stored IDffffffff with no intervening child; absent an external writer,
those immediate requests are skipped. The branch remains part of source; no
extra concurrency guarantee is inferred. Nonzero D performs no immediate
sequence request; fade consumer's eventual dispatch is separately documented.

Finally, after any immediate child request, reload active bitmap8041ee88, OR
row bit and store. For a scan bit=1<<ordinal, ordinal0..31; no captured bitmap
restoration or stop at first matching row. Direct path uses PowerPC slw(1,G.low8):
shift amount=G.low8.low6; if its bit5 is set, result0; otherwise1<<(G.low8.low5).
For valid direct0..31 it is the expected bit. Invalid32..249 can ORzero or
alias another row bit; the unchecked row stores still occur. No host undefined
shift substitution, bounds normalization or independent return status.

## Source/runtime boundary and validation

No synthInfo/sndActive/IRQ/ID validity/group bounds/finite coefficient gate,
voice rebuild, hardware output request or audible-duration measurement here.
This closes the local fader producer paired with the existing consumer, not the
full sequence/group/flag/initialization/concurrency or physical audio contract.

[Manifest](../../../tools/musyx_synth_volume_evidence.json) pins two complete
bodies, five linked fingerprints (ABI save/restore entries, stop/pause/mute),
four exact numeric literals:11 spans,192 instruction anchors,15 direct calls.
All256 selector routes, eight duration/eight target/ten byte-mode/six row-type
vectors are source-derived checks. ABI entry fingerprints are not complete
fallthrough-chain contracts. Fresh full ELFs/listings stay
`/tmp/inbox-synthVolume.elf` and `/tmp/inbox-sndConvertMs.elf`; no original body/
asset exported to source history or original code executed.

Remaining: fader row-type/current/coefficient/bitmap initialization and other
writers, actual caller ID/group/mode/time/volume inputs, direct bounds/alias/
finite-number/IRQ/concurrency invariants, deferred sequence consumers and
original zero/nonzero-duration/tempo/fade/audio traces. EVT-01 and full native/
source/runtime/hardware equivalence remain OPEN.

### Synthesis initialization follow-up

[Complete synthesis initializer](MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md)
now pins local voice/fader/aux/job resets, exact initial row types and callback
installation. Other child services/writers, valid caller/reinit/concurrency
domains and original initialization/scheduling/audio traces remain open.
