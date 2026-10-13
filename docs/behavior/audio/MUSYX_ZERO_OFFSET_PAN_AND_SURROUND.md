# MusyX zero-offset pan and surround flags, requests and clamps

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete ZeroOffsetHandler region
802781e0..80278328, end exclusive. Addresses/offsets/masks hexadecimal;
shifts decimal. Follows [gain/attenuation](MUSYX_ZERO_OFFSET_GAIN_AND_ATTENUATION.md).
Static source deductions, not measured spatial audio or original execution.
Read [controller wrappers](MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md)
and [pan-field producers](MUSYX_MACRO_PAN_VOLUME_AND_ENVELOPE_COMMANDS.md).

## Captured mode and independent update admission

Retain voice V and carried update indicator r0. Capture synthFlags8041ee8c=G.
If G&1 nonzero, take the fixed-pan branch below; no controller queries.
Otherwise freshly read V.flags114 then118. Only low114&2000 admits the update;
the emitted high118 mask is0. If that bit is clear, freshly read V.word220 and
admit when word220&6 nonzero. Other dirty bits alone do not admit this region.
No voice/index/generation/lifetime/value/address validation here.

On admitted update, freshly read flags118 then114, store118 unchanged, then114
with bit2000 cleared. The flag clear precedes both child requests. This does not
clear the dirty word220 or preserve a pending bit a child later sets.

## Panning: wrapped sum followed by signed clamp

Request inpGetPanning(V)8028f7b8 at80278244. Read V.word174 AFTER the child.
Compute P=wrap32(word174+wrap32((low16(return)-8192)<<9)). Interpret P as
signed32: negative stores0 at word164; nonnegative greater than007f0000 stores
007f0000; otherwise store P. No unbounded/saturating intermediate addition.

For example word174=7fffffff,return8193 gives wrapped800001ff and stores0;
word174=80000000,return8191 gives7ffffe00 and stores007f0000. These are raw
source outcomes, not claims that real voice/controller producers permit them.
The child return has no status gate or controller-domain clamp.

## Fresh surround mode and unsigned cap

After word164 publication, reread synthFlags. Its FRESH bit2 controls surround
query; captured G is not reused. A child changing bit1 does not restart the
fixed-pan branch, whose admission already happened.

Fresh bit2 set: request inpGetSurPanning(V)8028f800 at8027828c. Read V.word178
AFTER child. The rotate9/mask7..22 selects low16(return)<<9, ignoring high bits.
Compute S=wrap32(word178+selectedShift). Store raw S to word168 FIRST. Unsigned
S>007f0000 then overwrites word168 with007f0000; otherwise leave first store.
There is no signed negative-to-zero clamp: rawffffffff is capped high.

Fresh bit2 clear: store0 to word168 without requesting surround. Every admitted
pan-update path then sets r0=1, regardless of values/change/child result. No
hardware update occurs in this region.

## No-update path and fixed-pan branch

If captured G&1==0 and both low114 bit2000 and dirty220&6 were clear:
retain word164 and carried r0. Captured G&2 set preserves word168; clear stores0
to word168 WITHOUT forcing r0=1. There are no children or synthFlags rereads on
this path. Do not infer that every word168 write causes the following volume
block to run.

If captured G&1 nonzero: unconditionally store00400000 at word164, then0 at168.
Freshly read flags114 then118; store118 unchanged, store114 with bit2000 cleared.
OR the Boolean of captured low114&2000 into incoming r0. Other flags118 bits
contribute a literal-zero masked term. The constant stores do not independently
force the indicator, and word220 remains untouched. No pan/surround requests.

All paths meet80278328. Its following update/dirty admission, volume/auxiliary
calculations, hardware publication and requeue remain separate work. This
contract pins raw mode bits, not the game/UI owner or measured speaker mode.

## Evidence and remaining work

[Manifest](../../../tools/musyx_zero_offset_pan_surround_evidence.json) pins
four spans (region/whole handler/two children),82 whole-region instruction anchors
and two calls. Nine pan/eight surround/eight gate vectors checked, including
wrapped signed changes, unsigned cap, fresh/captured mode and fixed-path indicator.
All65536 low16 controller values checked against the decoded rotate-mask. These
are mathematical source checks, not original audio execution. Generic verifier
checks spans/anchors/calls, not semantics or provider domains. Fresh full handler
ELF/listing and generator remain `/tmp`; no bodies/binary assets committed.

Remaining: volume/aux/filter/output/hardware/requeue/epilogue regions, actual
synthFlags/dirty/voice/base/controller writers, valid value/extent/alias/lifetime/
IRQ/concurrency domains, original pan/surround/audio traces and native binding.
EVT-01 remains OPEN.

## 2026-10-13 — Volume/aux output follow-up

[Volume/aux output](MUSYX_ZERO_OFFSET_VOLUME_AND_AUX_OUTPUT.md) closes the next admission/controller/main-aux/
snapshot/hardware request region. Filter/requeue/epilogue and provider/domain/
original audio/native binding remain open.
