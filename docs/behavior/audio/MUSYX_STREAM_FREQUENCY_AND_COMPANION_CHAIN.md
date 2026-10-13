# MusyX stream frequency publication and companion chain

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndStreamFrq8027bd78/668hex,
410 instructions, plus full GPR27..31 helper spans. Offsets/addresses hexadecimal;
counts/shifts decimal. Static source deductions, not original tuning/stream/audio
execution. Read [ID lookup](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md),
[hardware pitch](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md),
[startup frequency consumer](MUSYX_STREAM_THROTTLE_DISPATCH_AND_STARTUP.md),
[free lifecycle](MUSYX_STREAM_FREE_AND_REVERSE_STATE_CLEAR.md) and
[IRQ wrappers](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).

## Four lookup/publication stages

Retain raw public ID and requested frequency F from r3/r4. Each entered stage
requests hwDisableIrq, then finds first nonzero-state/fullword public-ID match
among64 streamInfo803f98e8 rows, stride68hex. Stages0..2 inline eight iterations
of eight row checks; stage3 calls GetPrivateIndex at8027c318. No live-count cap,
sentinel-ID rejection, active hardware query, generation/voice/resource check.
Missing ID immediately unwinds the entered stage and retained outer stages,
without changing that missing row or undoing earlier frequency publications.

| Stage | Disable | Frequency store | Hardware pitch | Own enable |
| --- | --- | --- | --- | --- |
| 0 | 8027bd94 | 8027bef8 | 8027bf58 | 8027c3c4 |
| 1 | 8027bf74 | 8027c0c0 | 8027c128 | 8027c3c0 |
| 2 | 8027c144 | 8027c28c | 8027c2f4 | 8027c3bc |
| 3 | 8027c310 | 8027c32c | 8027c394 | 8027c3b8 |

For every found row store FULL retained F at word54, then reread state+c.
Only EXACT2 enters hardware pitch work. Other fresh states retain the stored
frequency but perform no pitch call. No old-frequency equality suppression,
zero-frequency rejection, state normalization or flag/voice/cursor reset.
The retained input F is used again for every companion; children do not cause
it to be reloaded from a previously written row.

## Exact unsigned conversion, multiply-before-divide and hardware input

For state2, freshly read synthInfo803f8540.word0 as reference R. Convert raw F
and R UNSIGNED32 through double bias80429180 bits4330000000000000 and separate
single fsubs. Read the selected row's raw voice word4c before the final arithmetic.
Use exact single4096 at80429188 bits45800000:

```
scaled = single(4096 * single(F))
pitchFloat = single(scaled / single(R))
```

Apply signed fctiwz, spill its result, read low word, request hwSetPitch(voice,
convertedWord). No signed input reinterpretation, reference0/finite/range gate,
rounding increment, gain clamp or conversion-status branch. This body multiplies
before dividing; the startup contract owns its separate sequence. Preserve the
individual source operations and FP exception/rounding domain rather than assume
algebra alone validates a substitute.

The hardware child owns low16 input narrowing and DSP dirty publication. This
caller does not narrow the conversion word before passing it or inspect child
return. Reference/voice may differ for later companions because their reads occur
after earlier children. Initial mode/state/ID match is not a global snapshot.
Invalid FP/conversion inputs remain unresolved; source has no local repair and
mathematical null outputs are not a claim that a hardware call is skipped.

## Fresh companion, recursion and exact unwind

After row publication/optional pitch call, freshly read row.word0. Sentinel
ffffffff stops local traversal; every other word supplies next raw public ID.
After stage3 non-sentinel request sndStreamFrq(ID,F) at8027c3b4, forwarding the
original retained frequency. Ignore its return. Up to four IDs are processed
locally before recursion; there is no visited set, cycle check, chain-depth cap,
all-row transaction or rollback of preceding stores on a later missing link.

Revisiting state3/unknown nonzero rows still publishes frequency and follows
companion. Acyclicity/termination and valid chain/resource/stack/IRQ domains
remain producer/runtime obligations. At stop, missing ID or recursive return,
issue enables in reverse entered-stage order. Full path:8027c3b8,3bc,3c0,3c4.
No saved prior-state token or status test. Request-count parity does not prove
restoration of original IRQ state; nested child services have separate behavior.

No independent success/failure return assignment or hardware-start/producer/
buffer-exit request. Existing row state, public ID, voice, cursor and selector
are unchanged by local setter stores, apart from transitive child mutations.

## ABI and verification

Frame allocates64 decimal bytes; LR saved at frame+44hex. _savegpr_27 and
_restgpr_27 execute five stores/loads GPR27..31 at oldSP-20..-4 then blr. Complete
24-byte spans pinned in addition to CSV4-byte entry boundaries. Suffix restores
LR/stack. Valid ABI/stack/child preservation remains a domain requirement.

[Manifest](../../../tools/musyx_stream_frequency_chain_evidence.json) pins11
spans,422 anchors (410 body/12 helper),16 calls. Eight pitch vectors cover finite
single-rounding arithmetic plus zero-reference/out-of-range conversion left
unresolved. Fifteen finite stable acyclic-chain models cover lengths1..7, missing
links/mixed or inactive states, stored frequency versus state2 requests and
partial progress/recursion/IRQ counts. Host finite models are not original Gekko
execution or proof of FPSCR/chain/provider domains. Generic verifier checks
hashes/anchors/calls, not semantic deductions. Fresh full ELF/listing/generator
remain `/tmp`; no bodies/binary assets committed. Local whole body answered.

Remaining: stream mix, actual caller/frequency/reference/ID/state/voice/companion/
IRQ/providers and valid value/index/extent/alias/lifetime/concurrency/stack/ABI/
termination/FP/conversion domains, transitive ARAM/DSP consumers and original
frequency/chained partial update/audio traces. EVT-01/native/full-game binding OPEN.

## 2026-10-13 — Mix/local inventory follow-up

[Mix/local inventory](MUSYX_STREAM_MIX_AND_LOCAL_MODULE_COVERAGE.md) closes mix and audits17 tagged stream.c
local functions. Providers/IRQ/FP/chain domains/transitive/original audio/
native binding remain open.
