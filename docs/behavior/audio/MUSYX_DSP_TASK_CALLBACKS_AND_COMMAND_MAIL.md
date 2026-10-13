# MusyX DSP task initialization, callbacks and command mail

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salInitDsp80292618/d4,
dspInitCallback80292468/10, dspResumeCallback80292478/64,
dspDoneCallback802924dc/24, salCtrlDsp802926ec/74 and
salGetStartDelay80292760/4chex. Read
[bootstrap](GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md),
[AI state/callback](MUSYX_AI_BUFFERS_CALLBACK_AND_IRQ_INITIALIZATION.md) and
[IRQ nesting](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).
Hexadecimal addresses/offsets below; explicit decimal literals labeled.

## Complete task record publication and initialization wait

salInitDsp does NOT read its input flags; r3 is overwritten while constructing
addresses. dsp_task80410c60 has50hex bytes. Load U16 dspSlaveLength8041e408,
then publish task fields in exact order:

| Offset | Value |
| --- | --- |
| +c | dspSlave803bef20 |
| +10 | Loaded program length, zero-extended word |
| +14 | word0 |
| +18 | dram_image80410cc0 |
| +1c | word2000 |
| +20 | word0 |
| +24 | half10 |
| +26 | half30 |
| +28 | dspInitCallback80292468 |
| +2c | dspResumeCallback80292478 |
| +30 | dspDoneCallback802924dc |
| +34 | word0 |
| +4 | word0 |

Task words0/+8/+38 onward and the2000-byte DRAM image are not cleared by this
body. The original program length is1ec0; source identity is verified, but DSP
instruction semantics/decoder/output are not recovered here.

Request OSInitThreadQueue(&salWaitForDSPThreadQueue8041efe8)8029c79c,
DSPInit802aaa9c, DSPAddTask(&dsp_task)802aab60, in order. Ignore child returns.
ONLY AFTER adding the task, store salDspInitIsDone8041eff0=0, then request
hwEnableIrq. Busy-read init flag until nonzero, request hwDisableIrq, return1.
No task-admission failure return, timeout, thread sleep or memory free/rollback.
An immediate init callback before the clear could be overwritten; SDK interrupt/
callback deferral and initial nesting ownership are real source prerequisites.
The enable/disable pair uses MusyX nesting, distinct from direct OS calls in
resume/AI callbacks. Completion wait is not measured startup latency.

## Three complete callbacks and distinct ready meanings

InitCallback stores salDspIsDone8041f000=1, THEN salDspInitIsDone8041eff0=1.
No thread wake, user dispatch, waiting clear or independent status return.

ResumeCallback FIRST stores DspIsDone=1. Read salLogicIsWaiting8041effc;
if0 return. If nonzero, store waiting0 BEFORE reading salLogicActive8041eff8.
Active nonzero returns, so pending waiting has already been consumed even when
no user callback runs. Active0 stores active1, requests OSEnableInterrupts,
loads FRESH userCallback8041f00c and invokes it indirectly without null checking
or local argument setup. Request OSDisableInterrupts, store active0, return.
No saved-state restoration, MusyX nesting update, tick publication or DMA rotation.
Both direct OS returns are ignored; callback exceptions/duration are not handled.

DoneCallback requests OSWakeupThread(&salWaitForDSPThreadQueue)8029d69c once.
It does NOT store DspIsDone, init-done, waiting or active. Task done and resume
are not interchangeable state producers. Actual SDK meaning/invocation order,
thread waiters and other writers remain separate.

## Complete command builder and mailbox handshake

salCtrlDsp retains raw input X. Request salGetStartDelay; request
salBuildCommandList(X,returnedDelay)80288e00. Ignore builder return; then load
FRESH dspCmdList8041ef70 and retain pointer P. Store salDspIsDone=0.
Request PPCSync80295004; ONLY AFTER sync read U16 dspCmdFirstSize8041ef6c.
Request DSPSendMailToDSP(babe0000|loadedSize)802aaa88.
Poll DSPCheckMailToDSP802aaa50 until returned word0. Then send retained P
and poll the same child until0 again. Pointer is retained from BEFORE ready-clear/
sync; size is fresh AFTER sync. No mailbox-empty precheck before first send,
null/size check, rollback, timeout, sleep or wait for actual DSP processing.
The second drain proves only the local mailbox query condition; it does not
publish DspIsDone=1 or prove command execution/audio completion.
Builder internals, command pointer/content lifetime and SDK/mail protocol remain open.

## Complete raw start-delay arithmetic

GetStartDelay requests OSGetTick8029d864 and retains returned low word T.
Read bus word B at800000f8, then salLastTick8041eff4. Compute unsigned-word:

`D = high32(0x431bde83 * (B >> 2)) >> 15`

`N = wrap32(wrap32(T - salLastTick) << 3)`

Return unsigned divwu(N,D). No zero-divisor check or signed elapsed/64-bit wrap
correction. Divisor0 is outside a valid source-domain guarantee; no host fallback
or defined quotient is invented here. Delay units/bus provenance/execution timing
need actual platform/caller evidence. These literal operations do not count
simulation/action-command ticks or rendering frames.

## Validation and remaining ownership

[Manifest](../../../tools/musyx_dsp_task_callbacks_control_evidence.json) pins six
complete bodies,13 linked-only child/helper identities and two original program/
length spans:21 spans,98 instruction anchors and16 resolved direct calls.
Original length1ec0 equals the supplied program span; no program body is exported.
Derived9 resume-gate/5 first-mail/6 clock-wrap-divisor vectors keep invalid cases
explicit and do not execute original functions. Full fresh ELFs/listings remain
`/tmp/inbox-dsp*.elf`; generic verifier checks source identities/anchors only.

Remaining: DSP instruction/task/SDK mail/IRQ semantics, command builder/control
state/voice engine, actual callback/thread/global writers and valid initialization/
clock/size/pointer/concurrency domains; original startup/resume/mail/audio traces.
Six local bodies are answered; full native/hardware/source/runtime remains OPEN.
