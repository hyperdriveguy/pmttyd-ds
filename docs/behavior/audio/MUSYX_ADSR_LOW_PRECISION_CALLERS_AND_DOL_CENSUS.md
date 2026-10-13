# MusyX ADSR low-precision callers and DOL call census

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete LowPrecisionHandler REGIONS
80277738..80277788,802779d0..80277a30,80277d7c..80277dbc and
80277e34..80277e50, ends exclusive. Whole body80277724/744hex is
fingerprinted; modulation/pan/pitch/controller/requeue sections outside these
regions remain open. Offsets/addresses/masks hexadecimal; shifts decimal.
Read [job caller](MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md),
[advancement](MUSYX_ADSR_ADVANCEMENT_AND_LOW_PRECISION.md),
[state/release](MUSYX_ADSR_STATE_TRANSITIONS_AND_RELEASE.md) and
[hardware layouts](MUSYX_CONTROLLER_ADSR_AND_HARDWARE_LAYOUTS.md).
Static source order, not original execution or measured cadence.

## Admission and elapsed timestamp region

Input I is retained without truncation. Capture synthVoice8041ee90 and compute
V=base+wrap32(I*458). Request hwIsActive(I)802907d8. If child returns0 AND
V.word34==0, branch directly to dirty tail80277e34, skipping the remaining
handler work. Otherwise retain V; read synthRealTime8041eea0.high then low.
Read V.previous.low28 then high24. Subtract low with subc and high with subfe;
retain only the wrapped low difference E in r31. High subtraction result is
not used by the subsequent handler math. Store captured new low to V.word28,
then new high to V.word24. No elapsed clamping, atomic pair publication,
OS timestamp query or conversion into fifteen envelope invocations.
The intervening modulation sections are outside this delivery's semantics.

V is captured BEFORE hwIsActive and retained afterward; a changed global base
does not make the remaining handler recapture V. No pointer/member/index/type/
generation/lifetime validation appears here. The call convention and original
caller invariants must justify raw I and descriptor availability.

## Envelope request and bit clear region

After earlier modulation work, read V.flags114/118. Only flags114&200 controls
this block; the emitted zero mask on flags118 adds no condition. If clear,
skip the request. If set, call adsrHandleLowPrecision(V+1e8,O=stack+0a,
D=stack+08) at80277a04. These distinct stack halfwords are disjoint from the
retained descriptor under ordinary valid-stack execution. No local read of
these temporary outputs appears in the full handler; this caller uses the
child's descriptor mutations and returned status.

A zero result reaches the next region without clearing bit200. Any nonzero
result freshly loads flags118 AND flags114, stores118 unchanged, then stores
114 with200 cleared. The clear uses the post-child flags, preserving other
fresh bits; it does not restore captured pre-child flags. No immediate voice
free, hardware stop, envelope reset or loop restart. The advancement wrapper
can mutate the descriptor up to fifteen times during one admitted call, but
source here does not relate that count to E or a measured elapsed duration.

## Independent pitch-envelope level contribution region

Later, after the still-open pitch/portamento computation has supplied r30,
read flags114/118 AGAIN. If fresh flags114&200 is clear, skip contribution.
If set, read V.word1f0 (level8 of descriptor atV+1e8), then SIGNED halfword
V.half210 (descriptor+28). Compute signed high level=arithmeticSigned32(level)
>>16; product=wrap32(signedDepth*highLevel); contribution=arithmeticSigned32(
product)>>7. Add contribution with wrap32 to r30.

The gate is independent of the earlier request gate: a child can clear bit200
and suppress this later contribution. The half210 depth is not the published
O stack halfword. This reads the freshly advanced descriptor level, whereas
adsrHandle publishes its pre-update level to O. Descriptor/layout/type/depth
writers and remaining pitch lookup/scaling/hardware requests require their own
contracts; no physical frequency unit follows from this multiplication alone.

## Dirty tail region

Both the inactive admission shortcut and the ordinary end reach80277e34.
Read V.bytea8. Zero skips writes. Nonzero: clear bytea8, then replace word220
with00007fff. This is replacement, not OR with previous dirty bits. No envelope
or descriptor field reset. Epilogue/register restoration is outside the region.

## Complete six-target DOL direct-call census

Scan all seven DOL text slots from the read-only ISO header. Two have nonzero
length and are fingerprinted in the manifest. At every aligned word, admit
PowerPC opcode18 with LK=1, sign-extend the26-bit branch displacement and honor
AA when resolving target. Retain only exact entry targets for the six services
below; resolve each site to its containing US .text symbol. Result:14 calls.
This is a direct link-bearing DOL instruction census, not a claim about executed
paths, unlinked tail branches, indirect calls, embedded address references, REL
imports, runtime patching or dynamic code. Section hashes pin the scan input;
the generic verifier validates hashes/sites/targets, not census completeness.

| Site | Caller | Target |
| --- | --- | --- |
| 80277a04 | LowPrecisionHandler | adsrHandleLowPrecision |
| 80278a04 | synthHandle | adsrRelease |
| 80280778 | mcmdSetPitchADSR | adsrSetup |
| 80286674 | adsrSetup | salChangeADSRState |
| 802869c8 | adsrHandle | salChangeADSRState |
| 80286aa4 | adsrHandle | salChangeADSRState |
| 80286b14 | adsrHandleLowPrecision | adsrHandle |
| 80289264 | salBuildCommandList | adsrSetup |
| 80289c04 | salBuildCommandList | adsrSetup |
| 8028a38c | salBuildCommandList | adsrRelease |
| 8028a3c4 | salBuildCommandList | adsrHandle |
| 8028a50c | salBuildCommandList | adsrStartRelease |
| 8028a534 | salBuildCommandList | adsrRelease |
| 8028a594 | salBuildCommandList | adsrHandle |

The synthHandle release site uses V+1e8 in its admitted key-off branch; see
[job controls](MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md).
The DSP builder uses a different retained record's descriptor+ a4 at its
seven sites. Its loop/state/command-publication semantics remain open here.
No assumption that its descriptor is identical to the synth voice descriptor,
that both calls execute at the same rate, or that direct callers exhaust runtime
ownership. Existing setup/state/advancement contracts close the corresponding
local children; whole caller hashes alone do not close remaining semantics.

## Validation and remaining source/runtime

[Manifest](../../../tools/musyx_adsr_low_precision_callers_evidence.json) pins
four regions, nine linked body fingerprints and two DOL text sections:
15 spans,80 anchors,15 direct calls (14 census calls plus hwIsActive).
Five elapsed/five flag/five signed pitch-contribution vectors are deductions,
not original execution. Full fresh handler/DSP inspection ELFs/listings and
census generator remain `/tmp`; no original bodies or binary assets committed.

Remaining: LowPrecisionHandler modulation/controller/portamento/pitch/requeue
sections, DSP builder's complete envelope setup/release/update/command regions,
other descriptor/flag/depth/time/index providers, source-valid value/alias/stack/
IRQ/concurrency/lifetime domains, indirect/REL/runtime callers and original
voice/envelope/DSP/audio traces. Four local regions and scoped direct census are
answered; EVT-01 and full native binding remain OPEN.
