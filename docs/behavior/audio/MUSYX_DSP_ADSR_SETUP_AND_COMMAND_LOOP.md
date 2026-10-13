# MusyX DSP ADSR setup and command loop

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salBuildCommandList REGIONS
80289218..80289288,80289bf4..80289c28,80289ac0..80289ad4 and
8028a354..8028a654, ends exclusive. Whole builder80288e00/25f8hex is
fingerprinted; its other setup/sample/mixer/budget/list sections remain open.
Complete sal_update_hostplayinfo8028877c/74, salSynthSendMessage8028b3f8/44
and salDeactivateVoice8028b4dc/60hex. Offsets/addresses/masks hexadecimal;
loop counts/shifts decimal. Read [ADSR setup](MUSYX_RESOURCE_AND_PITCH_ADSR_COMMANDS.md),
[state/release](MUSYX_ADSR_STATE_TRANSITIONS_AND_RELEASE.md),
[advancement](MUSYX_ADSR_ADVANCEMENT_AND_LOW_PRECISION.md) and
[direct caller census](MUSYX_ADSR_LOW_PRECISION_CALLERS_AND_DOL_CENSUS.md).
Static local request/publication order; no measured DSP cadence/audio.

## Retained records and two setup regions

V is the retained hardware voice record in r29; PB is its parameter-block
pointer in r19. These are not the synth voice's458-stride layout. Envelope
Q=V+a4; Q.level8 isV.ac, Q.logc isV.b0 and Q.step10 isV.b4.
Per-index flags are V.word(24+4*s), and per-index pitch words38+4*s.
No new source-valid index/lifetime/type or descriptor normalization is inferred.

At80289218 capture V from the preceding pointer slot. V.bytef0==0 skips to
8028a964. Otherwise capture PB=V.word0 and clear four stack halfwords12/14/16/18.
V.bytef0!=1 branches to still-open existing-voice work at80289ad4. For byte1:
V.wordec=ffffffff; freshly reload V.word0 and store half8000 at its+66;
call adsrSetup(V+a4). Zero return continues at80289288. Nonzero requests
salSynthSendMessage(V,0), then salDeactivateVoice(V), then exits this voice
at8028a964. Callback result is not checked. The cached PB does not recapture a
changed V.word0 after children.

The separate80289bf4 region reads V.word24 bit10. Clear skips setup; set calls
adsrSetup(V+a4). Zero continues at80289c28; nonzero sends message0/deactivates
in the same order and exits at8028a964. This region does not clear bit10 itself.
The intervening format/sample/setup paths are outside this contract.

Start-mode publication80289ac0..ad4 stores stack.word20=1, V.bytef0=2, then
branches directly to8028a370. Initial index s was read from V.byteda at80289910
(single source anchor, not closure of the intervening region). This entry skips
the ordinary prefix below; its retained register/stack domains remain caller work.

## Prefix and first envelope invocation

Ordinary entry8028a354 assembles PB.half7a as high16 and half7c as low16;
stores the resulting word to V.word20; stack.word20=0; s=0. Alternate entry
starts at8028a370 with the retained raw byte index. Let A=V+4*low8(s).

Read A.word24 bit40. If set, call adsrRelease(Q); its return is ignored here.
Reload A.word24, test bit8. If set, read A.word38 three separate times: store
its high half at PB.a6, fresh low half at PB.a8, fresh word at V.e4. Then call
adsrHandle(Q,O=PB+64,D=PB+66). Retain its result R and unsigned PB.half66
as previous delta. Clear PB.half44/46/48/4a/4c, then retain cursor C=V.word4.
These five halfwords count emitted command pairs per index, wrapping at16 bits.

If low8(s)!=0, append command pair(7,1) at C, advance C by4 bytes and increment
PB.half(44+2*s). If s==0, omit that pair. No command-buffer capacity or index
bounds check is present. Request sal_update_hostplayinfo(V), set s=low8(s+1),
retain mask-array base M=stack+10, then test low8(s)<5.
The first adsrHandle is unconditional after the admitted prefix, even if a
release returned completion. An unsupported envelope type can leave PB outputs
unchanged; no local output initialization is added by the advancement child.

## Subsequent iterations: prior completion, masks and release precedence

Each subsequent iteration starts by testing PREVIOUS retained R. Nonzero:
append(7,0), advance C by4, increment this index's count, send message0,
deactivate V and branch to the final completion block. No new envelope call,
mask emission, release or host-position update occurs on that iteration.

Otherwise read unsigned M.half(2*s). If nonzero, inspect bits0..8 in ascending
order. For every set bit b, append(pbOffsets[b],0), advance C by4 and increment
PB.half(44+2*s). Original nine halfword offsets at80303f70 are
0a,0c,18,0e,10,1a,12,14,16hex. Higher mask bits produce no pair. The mask is
reloaded during each bit test; no snapshot/clear is performed by this loop.
The earlier mask producers remain outside this contract.

Set A=V+4*low8(s); capture flags=A.word24. If bit20 set, request
adsrStartRelease(Q,10 decimal), ignore return, set V.bytef1=1 and skip stored
release AND pitch-word command emission. This explicit branch repeats on later
indices with bit20 even if bytef1 is already1; bytef1 is not its admission gate.

If bit20 clear, read bytef1. Nonzero skips stored release and pitch commands.
Zero permits captured flags bit40 to call adsrRelease(Q), ignoring return.
Then reload A.word24 and test bit8. Set emits two pairs(53,high16(fresh38))
and(54,low16(fresh38)), advances C by8 and adds2 to the index count; reread
word38 again into V.e4. A release child can change this fresh bit8/read data.
Do not merge these independent reads into one captured pitch value.

## Advancement output commands, loop bound and final publication

Capture V.wordb4 (Q.step10) BEFORE adsrHandle(Q,O=stack+0e,D=stack+0c).
Retain returned R. Compare previous unsigned delta against stack.half0c.

| Delta comparison | Captured step | Appended pairs | Previous delta update |
| --- | --- | --- | --- |
| Equal | Zero | None | None |
| Equal | Nonzero | (32,stack.half0e) | None |
| Different | Any | (32,stack.half0e),(33,stack.half0c) | Reload stack.half0c |

Pair IDs are hexadecimal. Advance C by4 per pair and increment the index count
accordingly. A transition child may change step later; emission still uses the
captured pre-call step. Stack outputs are pre-update level and quantized delta
under the admitted advancement domains, not the post-transition descriptor.
Unsupported types leave these stack halfwords unwritten; initialized/residue
invariants must come from callers. No fabricated default output is supplied.

Request sal_update_hostplayinfo(V); increment s; compare low8(s)<5. Ordinary
start0 with zero child results makes five advancement calls, without a call after the
fifth even when its result is0. Raw starts1..4 make5-start calls; starts5..254
make one call; start255 wraps to0 and can make six calls. This describes control
flow assuming child results0, not source-valid index domains. Raw out-of-range
indices can exceed five count/mask slots; no safe indexing is inferred.

At8028a62c, if retained R nonzero, send message0 and deactivate AGAIN. The
prior-completion branch already requested both, so it reaches a second message/
deactivation pair; the callback can observe/mutate state between requests.
If R==0 skip these requests. In both cases reread V.word4 as cache-flush base;
request DCFlushRange(base,wrap32(C-base)) at8028a650. Cursor C is retained
rather than stored back to V.word4 by this region. Callback mutations can change
the freshly read base; unsigned wrap/alias/cache-buffer domains remain open.
Other builder work resumes at8028a654.

## Complete host playback-position leaf

sal_update_hostplayinfo(V) returns immediately if V.word8c==0. Otherwise read
PB=V.word0 and unsigned PB.half8. If mode==2, step=00200000; otherwise
step=wrap32(V.worde4<<5). Read old low V.e0, add wrap32(step<<16), store low.
Reload low and compare unsigned oldLow>freshLow to choose carry1 or0. Read high
V.dc; store wrap32(high+(step logical>>16)+carry). No modulus, sample-end
notification, pitch calculation or mode-range guard. Mode2 adds32 to high and0
to low per admitted invocation. Shift wrap can discard high rate bits before
accumulation; do not replace it with widened multiplication. Shared aliases/
writers can affect the fresh low/high loads. No scalar status result is assigned.

## Complete message callback and deactivation leaves

salSynthSendMessage(V,msg) captures V and callback salMessageCallback8041ef8c.
Null callback returns0 without reading V.word18. Otherwise request the captured
function indirectly with r3=msg,r4=V.word18; propagate its return unchanged.
No callback registration/type/lifetime/IRQ guard or meaning of numeric msg is
established by this wrapper. Builder callers here ignore its returned word.

salDeactivateVoice(V): bytef0==0 returns immediately. Otherwise capture previous
V.word10. Nonnull: previous.wordc=V.wordc. Null: capture studio bytef3 and next
V.wordc; write next to studio root at80407c68+bc*studio+48. Then RELOAD next
V.wordc; if nonnull, store freshly read V.word10 to next.word10. Finally
V.bytef0=0. Own links, other fields, envelope, callback, memory and hardware
state are not cleared by this leaf. No membership/studio bounds/lifetime checks;
invalid overlapping records can affect later fresh reads. Repeated deactivation
is locally a no-op once bytef0 stays0, but preceding callbacks can reactivate it.

## Validation and remaining source/runtime

[Manifest](../../../tools/musyx_dsp_adsr_command_loop_evidence.json) pins three
complete leaves, four regions, six linked body fingerprints, nine numeric
command-offset halves and the alternate index source:14 spans,309 anchors and
18 direct calls. Six loop/eight release-precedence/five delta/five host-position
vectors are source deductions, not original execution. Full original inspection
ELFs/listings remain `/tmp`; no bodies/binary assets committed.

Remaining: full DSP builder sample/mixer/budget/mask/index/record/setup/source
producers and branch admission, callback registration/transitive services,
cache/DSP command consumption, active/studio/list/stack/type/value/alias/lifetime/
IRQ/concurrency domains, physical invocation cadence and original voice/envelope/
command/audio traces. These four regions/three leaves close the seven local
ADSR call sites; full builder/native binding and EVT-01 remain OPEN.
