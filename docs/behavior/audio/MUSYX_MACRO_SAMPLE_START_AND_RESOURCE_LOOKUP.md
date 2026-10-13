# MusyX macro sample start and resource lookup

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdStartSample8027fc84/200,
dataGetSample8027f0f8/130 and smpcmp8027f0e8/10hex; opcode16 stub80282ea0/10.
Addresses/offsets/masks hexadecimal; scalar values decimal. Static resource/
request/publication semantics, not original sample/pitch/audio observations.
Read [directory/reference residency](MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md),
[binary search](MUSYX_MACRO_LOOP_JUMP_AND_REGISTRY_LOOKUP.md),
[key-state job wrapper](MUSYX_MIDI_CONTROLLER_SETTERS_AND_NOTIFICATION.md) and
[macro fetch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md).

## Complete resource lookup and reference admission

dataGetSample(raw ID,output pointer O) stores ID.low16 to shared key80405ae8.
Initialize ordinal0 and registry pointer804006e8, stride12 decimal. Fresh
registry count half8041ef00 before every iteration; no captured-count/IRQ guard.
For each registry R, request sndBSearch(&key,R.word0,U16(R.half8),32,smpcmp).
Comparator is U16(key.half0)-U16(entry.half0), signed32; registry entry size32.
Search order/duplicate result depends on the existing binary-search contract.

Store returned row pointer to global8041eed8 even on miss. Null or row.half2==0
advances to next directory. First returned row with nonzero reference half2
admits success; no reference increment, acquired-cache request or sample-ID/
address/generation/macro/voice validation. Set shared descriptor pointer
8041eed4=row+c. Then perform output writes in THIS order:

| O offset/width | Source |
| --- | --- |
| 0 word | descriptor.word0 |
| 4 word | fresh global found-row.word8 |
| c word | 0 |
| 14 word | fresh global descriptor.word8 |
| 10 word | fresh global descriptor.word4.low24 |
| 18 word | fresh global descriptor.wordc |
| 1c byte | fresh global descriptor.word4.high8 |
| 8 word | found-row.word1c==0 →0; otherwise that word+fresh registry[ordinal].word0 |

Return0. No whole-output zero; padding bytes1d..1f untouched. Globals are reloaded
between listed source reads; unsupported aliasing can change sources. Failure
returnsffffffff without output writes; key/found-row scratch may still change.
Count0 skips search/output, leaving previous found/descriptor scratch untouched.
No O-null, table sorting/extent or preferred-directory policy added. Match with
reference0 is not active reuse and does not trigger automatic acquisition here.

## Start command lookup and shared descriptor

mcmdStartSample retains voice V/command C. Capture sample ID=(C.word0>>8).low16;
request dataGetSample(ID,&shared descriptor80405b20). Nonzero result skips ALL
subsequent offset/hardware/voice/pitch/flag/job work. No substitute silence,
partial voice clear, free/stop or explicit error status is assigned.

Success rereads C.word0 and uses its high8 MODE, which may differ from lookup's
captured command after a child/shared mutation. C.word4 is read only in modes0/1/2.
Store chosen offset at shared descriptor.wordc:

- Mode0: raw fresh C.word4.
- Mode1: capture V.word158, then C.word4. Factor=(127-U16(word158.high16)).low8;
  product=wrap32(word4*factor); offset=unsigned floor(product/127).
- Mode2: capture V.word158, then C.word4. Factor=U8(word158>>16);
  product=wrap32(word4*factor); offset=unsigned floor(product/127).
- Other modes:0, without word4/word158 read.

The division uses unsigned high-product multiplier02040811hex, subtract/shift/
add/shift sequence, not signed division or float rounding. Multiply wraps BEFORE
division. Factors are not clamped to0..127; mode1 underflow narrows low8.

Read shared offsetc then length10. If offset>=length unsigned, store
wrap32(length-1) to offsetc; otherwise retain it. Length0 therefore publishes
ffffffff, not offset0/refusal. This body does not establish whether length0 is
reachable. Descriptor length's prior low24 masking remains separate from offset.

## Exact hardware request and post-child voice publication

Read high flags114 then low118; subsequently metric110/priority10c, identityf4
and byte197. Request hwInitSamplePlayback with eight register arguments:

| Ordinal | Value |
| --- | --- |
| 1 | identity.low8 |
| 2 | retained sample ID.low16 |
| 3 | &shared descriptor80405b20 |
| 4 | low flags bit100hex CLEAR, boolean0/1 |
| 5 | (priority<<24) OR ((metric>>15)&00ffffff) |
| 6 | FULL captured identityf4 |
| 7 | high flags bit800hex CLEAR, boolean0/1 |
| 8 | byte197 |

These booleans/fields are exact arguments; hardware meanings/consumption remain
open. No return check/rollback. After the child, read FRESH shared descriptor.word0,
store V.word124; store retained sample ID at half12c. Read fresh V.word128;
exactffffffff skips pitch; otherwise request DoSetPitch(V). Then reread lowFlags118,
store OR20hex, request synthKeyStateUpdate(V). Child can mutate later inputs;
no descriptor snapshot restoration, whole flags clear or CPU/sample-reference
free. Actual HW/pitch children remain linked separate contracts; hashes alone
do not prove sample loading, format decoding, DSP start or audible output.

## Opcode registration, evidence and remaining requirements

Opcode16 stub passes V,&shared cstep8041ef08, ignores returned register value,
joins80283894 with initial stop marker0 unchanged. Even lookup failure does not
locally stop this interpreter invocation or undo already advanced PC. No wait,
render-frame timing or separate completion event added.

[Manifest](../../../tools/musyx_macro_sample_start_lookup_evidence.json) pins
three bodies/one region/six linked identities/table:11 spans,138 anchors/eight
calls/one registration. Eight ordered output bindings, seven offset/six extent/
five request-flag vectors and comparator/unsigned division/address deductions
are static checks, not original execution. Full fresh inspection ELFs/listings
stay `/tmp`; no original bodies/binary assets committed.

Remaining: hwInitSamplePlayback/DoSetPitch, actual resident registry/descriptor/
ID/ref/output/command/word158/length/flags/metric/pitch/caller providers, valid
sorted/duplicate/member/range/zero-length/shared-cell/alias/IRQ/concurrency
and original start/refusal/pitch/sample/audio traces. EVT-01 remains OPEN.

### 2026-10-13 — Hardware/pitch provider follow-up

[Complete hardware playback/pitch](MUSYX_HARDWARE_SAMPLE_PLAYBACK_AND_PITCH.md)
now closes these child bodies, including inclusive time-slot reset/full descriptor
copy and unchecked unsigned pitch table arithmetic. Actual descriptor/padding/
ratio/slot/caller domains and DSP/original audio remain open.
