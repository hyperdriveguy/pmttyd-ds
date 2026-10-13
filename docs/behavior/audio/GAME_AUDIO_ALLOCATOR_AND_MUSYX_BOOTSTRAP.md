# Game audio allocator providers and MusyX bootstrap requests

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndMalloc800e28f8/28,
sndFree800e28d0/28, sndInit8028dacc/138 and hwInit80290704/c4hex.
SoundInit is scoped ONLY to800e239c..800e2424 exclusive; its remaining4achex
bytes/effects/loader/settings tail is not recovered here. Read
[hook/default/cache/reset producers](MUSYX_DEFAULT_CACHE_RESOURCE_RESET_AND_HOOK_INSTALLATION.md),
[ARAM initialization](MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md) and
[group loader](GAME_SOUND_GROUP_WARMUP_AND_LOADER_REQUESTS.md).
Addresses/offsets below are hexadecimal; literal decimal values are labeled.

## Original game allocator providers and startup prefix

sndMalloc(rawSize) requests __memAlloc(0,rawSize)800300f0; sndFree(rawPointer)
requests __memFree(0,rawPointer)800300c4. Both move INPUT r3 to r4, force r3=0
and preserve child r3 return. No local alignment/size/null/error or physical
address conversion. The installed physical allocation hook also routes through
sndMalloc; its name alone does not supply extra physical allocation semantics.
SDK heap/memory providers remain child contracts.

SoundInit's original table802e3cb8 contains exactly two words:
800e28f8(sndMalloc),800e28d0(sndFree). It captures both into stack words10/14
BEFORE hardware startup. Requests, in exact order:

1. ARInit(r13+1c04,8 decimal)802a8d3c.
2. ARQInit802aa87c.
3. AIInit(0)802a868c.
4. ARAlloc(00bfc000)802a8ccc; return ignored.
5. sndSetHooks(&capturedStackPair)802915c0.
6. sndInit(64,12,44,1,1,00bfc000), all decimal except final raw hex size.

No intervening return check or source pointer from ARAlloc is forwarded. The
scoped prefix also ignores sndInit's result: the next source instruction begins
effect/settings initialization regardless. That continuation is a source branch
fact, not proof that initialization succeeded or audio played. The caller uses
a separate stack copy, avoiding the installer self-alias example. Dynamic setup/
provider/hook lifetime and whole SoundInit remain open.

## Complete sndInit configuration and hardware failure

Inputs r3..r8: raw voice count V, bytes X/Y, raw studio count T, flags F,
ARAM-size word U. Retain F/U. FIRST store sndActive8041ee60=0.
Write synthInfo803f8540 byte210=min_unsigned(V&ff,64 decimal),
byte213=min_unsigned(T&ff,8 decimal). Then create stack rate word=32000 decimal;
store byte211=U8(X), read fresh byte210 for hardware voice count;
store byte212=U8(Y), read fresh byte213 for hardware studio count.
No minimum/nonzero check or wider input rejection: V100hex becomes0, not64.
The meanings/domains of X/Y remain source/caller contracts beyond these stores.

Request hwInit(&stackRate,freshVoiceByte,freshStudioByte,F). Nonzero return
immediately exits sndInit with that return, leaving sndActive0 and the prior
configuration writes. No rollback/reset cleanup or later resource init occurs.
No preexisting-active gate: reinitialization clears active first.

## Complete sndInit successful request order

Hardware return0 admits this sequence:

1. Request aramGetFirstUserAddress; retain base B. Then read synthInfo.byte210,
   retain that voice byte across later requests.
2. Request aramGetUserBytes(U), returning wrap32(U-500).
3. Request dataInitStack(B,returnedBytes), then dataInit(0,U).
4. Request seqInit80276770.
5. Store synthIdleWaitActive8041ee98=0.
6. Request synthInit(32000 decimal,retainedVoiceByte)802797f8.
7. Request streamInit8027a544, vsInit80286b5c, then s3dInit(F)8028da94.
8. Store sndActive=1; return0.

Child returns after hardware success are ignored. The stack rate passed to hwInit
is NOT reread for synthInit: that request uses literal32000. Default-cache end
is wrap32(B+wrap32(U-500)); using the first-user leaf B=wrap32(ARBase+500), it
reduces arithmetically to wrap32(ARBase+U). Hardware capacity clamping/residency
must not be inferred from this computation or the final active byte.
Original seq/synth/stream/virtual-sample/3D child initialization remains separate.

## Complete hwInit local request graph

Retain rate-pointer R, voice word V, studio word T and raw flags F. Request
hwInitIrq802927ac, then store salFrame8041efbc=0, salAuxFrame8041efbb=0,
salMessageCallback8041ef8c=0, in that order.
Request salInitAi(snd_handle_irq802905b0,F,R)80292500. Return0 refuses with-1.
On nonzero, request salInitDspCtrl(V&ff,T&ff,F&1)80288104. Return0 refuses-1.
On nonzero, if F&8 request hwEnableCompressor802915e8; then request
salInitDsp(F)80292618. Return0 refuses-1. Otherwise request hwEnableIrq802927d8,
salStartAi802925c8; return0. Only these three admission child returns are checked.
No success/failure cleanup or unwinding of earlier IRQ/AI/DSP/control mutations.
Flags and child behavior are preserved; actual hardware init/rate/callback/DMA
and compressor semantics are not inferred from their symbols or linked hashes.

## Validation and remaining source/runtime domains

[Manifest](../../../tools/musyx_game_allocator_bootstrap_evidence.json) pins four
complete bodies, the88hex-byte SoundInit prefix, original8-byte hook table and23
linked-only child identities:29 spans,134 anchors and25 resolved direct calls.
Nine derived byte-width/clamp vectors separate low-byte conversion from numeric
clamping. Original table values independently match the two provider symbols.
Fresh complete ELFs/listings remain `/tmp/inbox-{game,snd,hw,sound}*.elf`.
No original function executes; no body/bank asset is exported to source history.

Remaining: whole SoundInit/effects/loader tail, actual OS/AR/AI/DSP/IRQ/frame/
sequence/synth/stream/virtual-sample/3D children and source/provider/size/reset/
concurrency/lifetime invariants; original startup/failure/audio traces. These four
complete local bodies and scoped game prefix are answered; full native/hardware/
audio/runtime equivalence remains OPEN.
