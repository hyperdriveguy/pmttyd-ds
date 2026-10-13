# MusyX audio frame and auxiliary callback order

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete snd_handle_irq802905b0/154,
hwIRQEnterCritical80292848/20, hwIRQLeaveCritical80292868/20,
hwSetTimeOffset802907c8/8, hwGetTimeOffset802907d0/8,
streamCorrectLoops8027af7c/4 and salHandleAuxProcessing8028b53c/158hex.
Read [AI callback](MUSYX_AI_BUFFERS_CALLBACK_AND_IRQ_INITIALIZATION.md),
[DSP command mail](MUSYX_DSP_TASK_CALLBACKS_AND_COMMAND_MAIL.md) and
[bootstrap](GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md).
Hexadecimal addresses/offsets below; explicit decimal counts labeled.
Static original-source order only; physical cadence/original audio remain open.

## Complete frame entry and critical wrappers

snd_handle_irq reads sndActive8041ee60 ONCE. Zero returns without any request
or global mutation. Nonzero executes the whole sequence below without another
active check, even if a child changes that byte.

EnterCritical requests OSDisableInterrupts80299ad4 and preserves its r3 return;
LeaveCritical requests OSEnableInterrupts80299ae8 and preserves its return.
Neither touches the MusyX U16 nesting/saved state or a game-record IRQ counter.
The frame handler ignores all returned OS states: pairs are disable/enable,
not saved-state restores. Nested/thread/OS ownership is a source prerequisite.

streamCorrectLoops is ONLY blr: no stream scan, loop correction or status write.
The frame still issues that local call first; do not infer work from its name.

## Complete frame request and publication order

For admitted entry, execute:

1. streamCorrectLoops.
2. Enter; salAiGetDest; salCtrlDsp(returned CPU destination); Leave.
3. Enter; salHandleAuxProcessing; Leave.
4. Enter; advance frame bytes and clear voice fields; Leave (details below).
5. For EXACTLY offsets0..4 decimal, in ascending order: Enter;
   hwSetTimeOffset(offset); seqHandle(100hex)80275d54;
   synthHandle(100hex)80278838; Leave. Offset is retained in the local loop;
   child returns are ignored. No fresh input/AI status/admission check.
6. Enter; hwSetTimeOffset(0); s3dHandle8028d36c; Leave.
7. Enter; streamHandle8027a698; Leave.
8. Enter; vsSampleUpdates802872e4; Leave; return.

SetTimeOffset stores INPUT r3 low8 to salTimeOffset8041efb8; GetTimeOffset
reads that byte and returns zero-extended. No timer/global voice update.
The last explicit offset is0, but later children may write it; final state is
not assumed immutable. Five calls with argument100 do not prove five physical
pad samples, rendering frames, action-command ticks or measured audio intervals.
Composed seq/synth/3D/stream/virtual-sample engines remain child contracts.
DSP commands are submitted BEFORE the subsequent sequence/synth update requests.

## Frame-byte advance and voice clear

Inside step4, read salAuxFrame8041efbb, then salFrame8041efbc. Store low8
salFrame XOR1, then store (oldAux+1)%3 using emitted signed multiply/remainder.
For all possible byte inputs, the aux calculation equals unsigned modulo3;
frame XOR only flips bit0 and does NOT normalize arbitrary bytes to0/1.

Set local ordinal0 and zero word. Before every iteration, freshly read byte
salNumVoices8041efba; compare low8 ordinal unsigned. For admitted ordinal i,
use stridef8 in dspVoice (global pointer8041ef88); store zero at descriptor
+24/+28/+2c/+30/+34 in order. RELOAD dspVoice separately for every store.
Increment ordinal, repeat gate. Count0 skips all stores. No pointer/count/capacity
validation or whole-voice clear; other fields remain. The local count is live,
not captured once, and source initialization must establish actual allocation.
This reset occurs after aux callbacks and before the five update slices.

## Complete studio traversal and A callback

AuxProcessing starts at dspStudio80407c68, ordinal0; before each iteration read
fresh salMaxStudioNum8041efb9 and compare low8 ordinal. Increment ordinal and
advance studio pointer bybc after each visit. Supplied registry is5e0hex bytes,
eight studios. No count<=8 validation, pointer/cycle check or whole-array copy.
Only studio.byte50==1 enters processing; all other values skip both callbacks.
That byte is checked ONCE for the studio, not rechecked after callback A.

For admitted studio, load callback A at wordac. Null skips A. Nonnull:
read fresh salAuxFrame, then studio.wordb4 as retained info argument. Compute
slot=(aux+2)%3; load retained buffer P from studio.word(30+4*slot).
Publish stack array [P,wrap32(P+280),wrap32(P+500)], then invoke retained
callbackA(0,&stackArray,retainedInfo). No P/null/extent validation or result check.
After callback, request DCFlushRangeNoSync(retainedP,780)80296ab4.
The flush uses retained P even if callback changed stack/global/studio pointers.
No independent flush of mutated stack-array values or synchronization follows.

## Fresh B gate and callback

AFTER A and its flush (or skipped A), read fresh studio.word54. Nonzero skips B;
zero admits reading callback B at wordb0. Null skips B. Otherwise repeat the
same construction with FRESH salAuxFrame, retained studio.wordb8 info and buffer
from word(3c+4*slot), slot=(aux+2)%3. Invoke B(0,&stackArray,info), then flush
retained buffer780 bytes with DCFlushRangeNoSync. No second byte50 admission
check: A can alter word54/B/frame/buffer/info before B's reads while changing
byte50 alone does not suppress same-visit B. Callback pointers are retained
before the other arguments are gathered, not reread immediately before call.
Then advance to the next studio and recheck the live global count.

Three channels have280hex bytes each, totaling780; these are source extents,
not a recovered format/sample-count/physical duration. Aux callback DSP/effect
semantics, installed providers, ownership/reentrancy and actual cache coherence
remain separate. The caller's critical section does not prove callbacks cannot
change these fields or interrupt ownership.

## Validation and remaining source/runtime domains

[Manifest](../../../tools/musyx_audio_frame_aux_processing_evidence.json) pins
seven complete bodies,10 linked-only child/helper identities,127 instruction
anchors and29 resolved direct calls. All256 aux/frame-byte arithmetic cases,
five-slice order, three-channel extent and six aux-gate vectors are derived
source checks, not original execution. Fresh full ELFs/listings stay
`/tmp/inbox-sndframeirq.elf`, critical/timeoffset/streamcorrect/auxprocess files.
Generic verifier establishes source identities/anchors, not callback cadence.

Remaining: actual sequence/synth/stream/3D/virtual-sample/command-builder engines,
voice/studio/count/aux provider writers and pointer/format/reentrancy/IRQ domains;
original DMA/frame/aux/audio observations. Seven complete local bodies are
answered; full native/source/hardware/runtime equivalence remains OPEN.
