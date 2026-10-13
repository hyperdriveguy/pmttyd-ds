# MusyX AI buffers, DMA callback and IRQ initialization

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete hwInitIrq802927ac/2c,
salInitAi80292500/c8, salStartAi802925c8/20, salCallback802923c0/a8,
salAiGetDest802925e8/30 and hwEnableCompressor802915e8/chex.
Read [bootstrap caller](GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md),
[allocator hooks](MUSYX_DEFAULT_CACHE_RESOURCE_RESET_AND_HOOK_INSTALLATION.md)
and [separate IRQ nesting](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).
Offsets/addresses below are hexadecimal; explicit decimal counts labeled.

## Complete initial interrupt ownership

hwInitIrq requests OSDisableInterrupts80299ad4. Store its raw returned state to
word8041efe0, THEN store MusyX U16 counter8041efe4=1. No old-counter check,
thread lock, restore or preservation of a prior nested state. Local return is
that child's r3, not a separate status assignment. Bootstrap calls hwEnableIrq
after successful DSP initialization; its decrement1→0 requests restoration of
this saved state. Failure paths in hwInit do not locally unwind it. Child IRQ
writes and actual OS/thread behavior remain separate; safe reinitialization
requires source ownership, not automatic merging with game record IRQ counters.

## Complete AI allocation and null failure

Inputs: raw callback C(r3), flags F(r4), mutable rate pointer R(r5). Retain C/R;
F is not read. Request salMallocPhysical(a00), then unconditionally publish
returned pointer to salAIBufferBase8041f004 BEFORE the null branch.
If null return0. No clear/free/previous-buffer recovery or further state writes.
A failed repeated init clears the base even while other old state may remain.

Nonzero: request memset(pointer,0,a00)800050b4. Reload fresh global base and
request DCFlushRange(base,a00)80296a54. Then write, in exact order:

| Global | Value |
| --- | --- |
| salLogicIsWaiting8041effc | word0 |
| salDspIsDone8041f000 | word1 |
| salAIBufferIndex8041f008 | byte1 |
| salLogicActive8041eff8 | word0 |
| userCallback8041f00c | raw retained C |

There are four280hex-byte slots in the a00 allocation. No allocator alignment
proof or old-buffer free; actual provider residency/coherence remains separate.

## Callback registration and initial DMA request

Request AIRegisterDMACallback(salCallback802923c0)802a8254; old callback return
is ignored. AFTER registration, read fresh byte index and fresh global base.
Request AIInitDMA(wrap32(base-80000000+index*280),280)802a8298. This is literal
word arithmetic for the physical argument, not an arbitrary host pointer cast.
No bounds/null/alignment recheck of these freshly read globals.

Store synthInfo803f8540.word4=20hex; store32000 decimal through retained R;
return1. No R null/alias guard. Since the rate-pointer write is LAST, aliasing
R to shared state can change prior publications. No independent hardware-rate
query or actual playback evidence is supplied by the32000 assignment.

salStartAi requests AIStartDMA802a8320 once and returns without a separate
success assignment, readiness check, source/buffer/rate publication or drain.

## Complete DMA callback and audio-logic gate

salCallback ignores incoming r3. Read byte index, then global CPU base; compute
next=(index+1)%4 using emitted signed-remainder arithmetic. Store byte next,
then request AIInitDMA(wrap32(capturedBase-80000000+next*280),280).
Request OSGetTick8029d864 and store returned low word to salLastTick8041eff4.
Buffer rotation/DMA request/timestamp occur BEFORE the logic-admission gates.

Read fresh salDspIsDone. If0, store salLogicIsWaiting=1 and return; no callback.
If nonzero, read salLogicActive. Nonzero returns without writing waiting/active.
If active0: store salLogicActive=1; request OSEnableInterrupts80299ae8;
load fresh userCallback and invoke it indirectly with no null check or local
argument setup. Then request OSDisableInterrupts80299ad4, store active0, return.
Both OS service returns are ignored; this is enable-then-disable, not a saved
state restore pair or MusyX nesting-counter update. No waiting clear, DSP-done
clear or callback-duration/exception recovery occurs here. Reentrant callbacks
still rotate buffers and publish tick before the active gate; user/OS callbacks
and DSP completion writers remain prerequisites for composed behavior.

## Complete CPU destination and compressor leaves

salAiGetDest reads byte index then base, computes (index+2)%4 and returns
wrap32(base+computedSlot*280) as a CPU address. No global index store, physical
subtraction, allocation/state guard, actual play-position query or DMA request.
Its two-slots-ahead selection differs from callback's next slot.

hwEnableCompressor stores word1 to dspCompressorOn8041ef74, then returns.
No hardware register write, compression transform, threshold/rate configuration,
callback or previous-value return. Actual compressor readers/output remain open.

## Validation and remaining source/runtime

[Manifest](../../../tools/musyx_ai_buffers_callback_irq_init_evidence.json) pins
six complete bodies plus nine linked allocator/OS/SDK/helper identities:
15 spans/83 anchors/11 resolved direct calls. Derived extent4*280=a00, all256
possible byte index arithmetic cases and nine DSP-done/active branch vectors are static checks,
not original execution. Fresh complete ELFs/listings stay `/tmp/inbox-salai*.elf`,
`/tmp/inbox-hwirqinit.elf` and `/tmp/inbox-hwcompressor.elf`.

Remaining: actual DMA/AI SDK, physical allocation/coherence, user callback/frame
engine, DSP-done/waiting/active/base/index writers and alias/concurrency/reinit
invariants; original interrupts/buffer/frame/audio timing. Six complete local
bodies are answered; full source/native/hardware/runtime remains OPEN.

[Complete DSP task/callback/control source](MUSYX_DSP_TASK_CALLBACKS_AND_COMMAND_MAIL.md) now supplies initialization/resume-ready writes, waiting consumption and command ready-clear. Done callback only wakes a thread queue. Actual SDK/DSP/builder/frame/global source/runtime remains open.
