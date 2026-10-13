# MusyX sound-chain kill and internal voice allocator recycling

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete voiceKillSound80285bd8/c4
and voiceKill802859a4/234hex. Read
[sequence stop caller](MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md),
[external voice lookup](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md) and
[key-off distinction](MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md).
Offsets/addresses hexadecimal; record strides/count literals labeled decimal.
Static mutation/request order, not physical hardware/audio completion.

## Complete external-sound mapping and chain traversal

voiceKillSound initializes result-1, reads sndActive8041ee60 once. Inactive
returns-1 with no map/voice request. Active input external ID-1 skips map lookup.
Otherwise INLINES the ordered vidRoot8041ef2c search: unsigned node.word8 equals
input returns node.wordc; key greater stops with miss; smaller follows node.word0.
No map mutation/IRQ, cycle/null/key/source validation or extra validity request.

For mapped internal identity C until exact-1, index=U8(C), capture record at
current synthVoice8041ee90+wrap32(index*458hex). Read record.wordf4 identity,
then capture record.wordec successor BEFORE comparing identity or child call.
Equal requests voiceKill(index), then sets retained result0. Unequal skips kill.
In BOTH cases follow captured successor; no fresh successor/global voice reread
for that step after child mutation. Stale identity is skip-and-continue, not an
abort or slot-only fallback. No chain/cycle/index/allocation cap or audible check.

Final return-1 means no matching kill request (including mapping/inactive miss);
return0 means at least one identity matched and requested voiceKill, regardless
of what that child did. No conditional controller/key-off replacement.

## Internal record capture and macro/map phase

voiceKill accepts RAW ordinal I, computes wrap32(I*458hex), captures current
synthVoice pointer plus offset as V. No low-byte mask, range/active-system gate,
identity check or IRQ. This caller supplies U8 chain index, but other callers'
raw domain remains required. Separate allocator table base is80405b40.

If V.word34==0, skip the macro/map/priority/free-record phase entirely, but still
execute final stream/hardware requests below. Nonzero:

1. Request vidRemoveVoiceReferences(V)802844d4.
2. Read V.word118 and word114; clear low TWO bits of word118; store it.
3. Store freshly captured word114 unchanged (its source ANDffffffff preserves it).
4. Store V.word110=0; request macMakeInactive(V,2)80283e58.

No returned-map/macro-result check, rollback, whole-voice zero or CPU memory free.
Retained V is used across children; later fields are fresh reads after them.

## Priority-voice unlink and priority-root unlink

Read V.wordf4 AFTER macro request, derive p=U8(identity), A=80406340+4*p.
A.byte0 previous voice, byte1 next voice, half2 membership. Only half2==1
admits this unlink; other values skip it without repair.

Read previous; nonff writes next to previousEntry.byte1; previousff writes next
to voicePrioSortVoicesRoot80406440[V.byte10c]. Then read next fresh; nonff
writes previous to nextEntry.byte0. If nextff AND fresh previousff, this removed
voice was the local only-node case: address priority record B=80406540+4*V.byte10c.
Read B.half2 next priority: nonffff writes B.half0 into nextPriority.half0;
otherwise store B.half0 to voicePrioSortRootListRoot8041ef34. Read B.half0
previous priority: nonffff writes fresh B.half2 into previousPriority.half2.
No B clear or other-priority count update. Finally store A.half2=0.

Accesses preserve branch-dependent fresh reads and unsigned sentinelsff/ffff.
The byte/half indices are not bounds-checked against supplied allocator extents:
voice entries occupy100hex bytes (64 records); root-list entries400hex bytes
(256 priorities). Source/private-ID consistency is not guaranteed by local masks.

## Clear voice state and append free voice

After priority phase (including skipped membership), store V.word34=0 then
V.byte10c=0. Read V.wordf4 freshly; p=U8(identity), F=voiceList80406940+4*p.
Only F.half2==0 inserts; nonzero skips insertion/count change but still reaches
the final identity clear. For insertion:

- Read voiceListRoot8041ef39; store F.half2=1.
- Root!=ff: read voiceListInsert8041ef38 tail; store F.next=ff, F.previous=tail;
  store oldTail.next=p.
- Root==ff: store root=p, F.next=ff, F.previous=ff.
- Store tail=p.
- Read V.byte11d: nonzero decrements voiceFxRunning8041ef37 low8;
  zero decrements voiceMusicRunning8041ef36 low8. No underflow guard.

Then store V.wordf4=ffffffff. No V.wordec chain clear, record release, priority
head validation or free-list/index integrity check. Running counters change only
on new free-list insertion, not every repeated kill invocation.

## Final stream/hardware requests regardless of macro-active word

Read V.byte11c; nonzero requests streamKill(RAW I)8027af80. Then ALWAYS request
hwBreak(RAW I)802909c8. Neither return is inspected; no hardware completion wait
or independent success assignment. Stream flag is not cleared by this body.
An initially inactive word34 still reaches this tail and can request both services.
No assumption that allocator recycle proves DMA/sample/macro resources released.

## Source validation and remaining ownership

[Manifest](../../../tools/musyx_voice_kill_sound_allocator_evidence.json) pins
two complete bodies plus four linked map/macro/stream/hardware bodies:6 spans,
124 anchors/five resolved direct calls. Five mask/four counter/six index/four
chain-route vectors are derived source checks; no original execution. Full fresh
ELFs/listings stay `/tmp/inbox-voice{killsound,kill}.elf`; no body/asset exported.

Remaining: vid reference mutation/mac inactive/stream/hardware internals, voice/
priority/free-list/identity/count providers and valid chain/index/member/cycle/
IRQ/concurrency domains, original sequence-note kill/recycle/audio traces. Two
local bodies are answered; full native/source/runtime/hardware remains OPEN.

[Complete stream kill/init and hardware break/active flags](MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md) now closes those exact tail children. First-match callback and event-bit publication remain distinct from resource/voice/hardware completion; deeper providers/consumers/runtime open.

[Complete macro inactive/active/wait/init](MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md) now closes the exact inactive child, preserving equality early return and conditional wait cleanup. Jobs/interpreter/time/list/providers/original execution remain open.
