# MusyX stream ID lookup, output mode and mix publication

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete GetPrivateIndex8027b018/160,
CheckOutputMode8027b178/34, SetupVolume8027b1ac/10,
SetupVolumeAndPan8027b1bc/44 and SetHWMix8027a60c/8chex.
Offsets/addresses/masks hexadecimal; row indices/shifts decimal. Static source
deductions, not original stream/audio execution. Read
[stream initialization/kill](MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md),
[hardware stream bridge](MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md) and
[hardware gain consumer](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md).

## Complete first-active-row public-ID lookup

GetPrivateIndex retains raw public ID X in r3. Scan streamInfo803f98e8:64 rows,
stride68hex, total1a00hex. Exactly eight outer iterations, each unrolling eight
rows; ordinals0..63 ascend. For each row read byte+c. Exactly0 skips its ID word;
any nonzero state reads word+4, compares full32 bits against X and returns the
current ordinal on equality. First match wins, even duplicate IDs or unknown
nonzero state bytes. Exhaustion returnsffffffff. Inputffffffff/0 are not special
rejections; either can match an admitted row.

Within each unrolled iteration, later state bytes are read from current pointer
+74 BEFORE pointer advances68; the ID read uses the advanced pointer+4. These
refer to the same next row, not two arrays or a different state gate. No ID
mask/generation decode/active hardware query/voice validation, table write,
allocation/IRQ request or child call. It does not require states exactly1/2;
actual caller state/ID ownership and races remain separate.

## Complete output-mode pointer leaf

CheckOutputMode(PanPtr,SurroundPtr) captures synthFlags8041ee8c once:

| Captured flags | Writes in order |
| --- | --- |
| Bit1 set | Byte PanPtr=64, then byte SurroundPtr=0 |
| Bit1 clear, bit2 set | No writes |
| Both bits clear | Byte SurroundPtr=0 only |

Bits1/2 here mean masks1/2, not bit ordinals. Other flags do not matter. Existing
pan survives both bit1-clear branches. No pointer/null/alias check or input-byte
read. If pointers alias on the first branch, second0 write wins; no snapshot or
alias avoidance. No independent success/status assignment or hardware request.

## Ordered volume/requested-pan/effective-pan writes

SetupVolume(V,A,B,C) stores low8(A) at59, low8(B) at5c, low8(C) at5d, in that
order. It does not set pan, output mode, dirty flags or hardware gain.

SetupVolumeAndPan(V,Volume,Pan,Surround,AuxA,AuxB) stores low8 Volume at59,
AuxA at5c, AuxB at5d, then requested Pan at5e and Surround at5f. AFTER those
five stores capture synthFlags. Preserve raw incoming Pan/Surround registers for
selection: mask1 set selects64/0; otherwise mask2 clear selects incoming Pan/0;
mask2 set selects incoming Pan/incoming Surround. Store selected low bytes to
5a then5b. Requested5e/5f remain the incoming bytes even when effective5a/5b
are overridden. No CheckOutputMode child call or mode reread. Retained incoming
arguments are used rather than rereading requested fields through possible alias.

Both leaves use raw V with no member/active/state/count/index/range check;
byte writes narrow rather than saturate127. No hardware request here.

## Complete hardware-mix request

SetHWMix(V) reads U8 fields in source order59,5c,5d,5a,5b, then word4c as raw
voice index I. With the unsigned double bias80429180 bits4330000000000000,
convert volume/aux bytes through separate single fsubs; multiply each separately
by exact float80429178 bits3c010204 (approximately1/127).

Request hwSetVolume(I,0,scaledVolume,scaledAuxA,scaledAuxB,byte5a<<16,byte5b<<16)
80290f50 at8027a684. Studio argument is literal0. No byte127 cap, gain clamp,
active/system/state/index validation, mode recalculation or flag/record publication.
Source255 gives approximately2.007874, not1. The request consumes effective pan
5a/5b, not requested5e/5f. Child return has no admission/rollback test; the wrapper
restores its stack/LR and makes no independent success assignment.

## Validation and remaining work

[Manifest](../../../tools/musyx_stream_lookup_mode_mix_evidence.json) pins eight
spans (five complete bodies/linked hardware child/two constants),all157 body
instruction anchors and one call. Five lookup/64 individual-slot/six mode/four
finite gain vectors checked, including unknown state, duplicate/sentinel ID,
mode override and out-of-range byte gains. Generic verifier checks source hashes/
anchors/target, not semantic deductions or original behavior. Fresh full ELFs/
listings/generator stay `/tmp`; no original bodies/binary assets committed.

Remaining: streamHandle/AllocEx/Free/Activate/Deactivate/Frq/MixParameter/
output-mode ownership, actual ID/state/voice/volume/mode/resource providers and
valid extent/value/alias/lifetime/IRQ/concurrency domains, transitive DSP/hardware
consumers and original stream/audio traces. Five complete local dependencies
are answered; EVT-01 and native/full-game binding remain OPEN.
