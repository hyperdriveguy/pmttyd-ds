# MusyX emitter removal, all-emitter traversal and FX unload

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndRemoveEmitter8028c4f8/a8,
s3dKillAllEmitter8028c5fc/b8 and s3dKillEmitterByFXID8028c6b4/fchex.
Read [emitter query leaves](SOUND_EFFECT_SPATIAL_CONTROLS_AND_EMITTER_LOOKUP.md),
[group pop caller](MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md) and
[key-off core](MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md).
Offsets/addresses hexadecimal; explicit decimal counts/strides labeled.

## Shared linked-list and flag fields

s3dEmitterRoot is word8041efa4. Used emitter E fields: word0 next, word4 previous,
wordc flags, word38 external voice, unsignedHalf40 FX ID. Removal requires flags
bit10000; there is no membership search, generation/null/cycle check or local
pointer allocation/free. List/flag consistency remains a caller/source invariant.

The unlink operation, in exact order, for an admitted active emitter:

1. Read E.next; if nonnull, read E.previous and store it to next.word4.
2. Read E.previous AGAIN; if nonnull, read E.next and store it to previous.word0.
   Otherwise read E.next and store it to global root.
3. Read E.flags freshly, clear ALL high16 bits via low16 mask, store wordc.
4. Read E.voice38. If exact-1 skip; otherwise request synthSendKeyOff(voice).

Own next/previous/voice fields are not cleared. Low16 flags remain. No head
identity check, voice-validity query, key-off return check, hardware kill/free,
resource unregister or restoration if a child fails. Linked neighbor aliasing
can affect later fresh reads; do not turn this into an immutable pointer snapshot.
The returned/stored voice identity remains available in memory after unlink.

## Complete individual removal and return semantics

sndRemoveEmitter retains input E, reads sndActive8041ee60 once. Zero returns0
without E access or IRQ requests. Nonzero requests hwDisableIrq; reads E.flags.
Bit10000 clear skips every unlink/flag/key-off operation; set performs the shared
operation above. In BOTH branches request hwEnableIrq and return1.
Thus return1 means active-system admission, not proof of removal or key-off
success. No second active read after disable, no inactive-emitter failure return.

The IRQ pair is the existing MusyX nesting counter/saved-state service, not the
game record counter or direct frame-critical OS wrappers. Actual list/IRQ/thread
ownership and child voice lifetime remain separate.

## Complete all-emitter traversal

KillAll captures global root ONCE; null returns without iteration. For each
retained current node, read sndActive then capture node.next BEFORE the active
gate and any IRQ/unlink/key-off. If active0 skip mutation, but still follow the
captured next; it does not abort the whole list or avoid reading inactive nodes.
If active nonzero, Disable, read current flags and run the same conditional
unlink/low16-clear/key-off sequence, Enable. Advance to retained next regardless
of flag/gate/child result; repeat until null.

No delegated call to sndRemoveEmitter: these are inlined writes and requests.
No root reload, whole-list detach, node free, progress/count bound or independent
success assignment. Removed nodes' retained links may keep pointing to other
nodes; child mutation/new insertions are not rescanned automatically. Even an
inactive system requires a valid captured node chain for traversal.

## Complete FX-array removal traversal

Inputs raw pointer A to FX records, raw unsigned-word count N. Each source FX
entry has stride10 DECIMAL bytes, ID unsignedHalf0. Capture global root once.
For each current emitter, capture next FIRST, reset local FX pointer=A and CTR=N.
N0 skips all ID reads for that node but still follows retained next.

For each of N entries: read current emitter.unsignedHalf40 FRESH, then array
unsignedHalf0; compare unsigned. Mismatch advances array pointer by10 and loop
counter. First equal stops the array search: read sndActive; zero skips removal
and advances node. Nonzero Disable, fresh flags gate, same inlined unlink/clear/
key-off, Enable, then advance node. Never try later duplicate matching entries,
even if active0 or emitter flag prevented mutation. Count is not masked toU16
or capped; NULL/record bounds/alignment/FX validity are not checked locally.

Each emitter uses the same original A/N, but identity is reread per array visit.
No sorted search, group-ID test, project/resource free, root reload or explicit
removed-count return. Group pop supplies the original FX entry array/count;
actual flag/FX/source/list domains remain open beyond this complete local body.

## Caller census, validation and remaining ownership

[Manifest](../../../tools/musyx_emitter_removal_evidence.json) pins three complete
bodies plus linked Disable/Enable/key-off identities:6 spans/117 anchors/9
resolved direct calls. Whole-DOL aligned direct caller census finds9 sites:
Remove3 in SoundEfxMain,2 in SoundDropData,1 in SoundEfxStop,1 in SoundEfxPlayEx;
KillAll1 in sndSilence; KillByFX1 in sndPopGroup. This is not indirect/REL/dynamic
coverage or complete caller semantics. Six high-half clear and12 active/flag
vectors are derived checks, not original execution.

Fresh complete inspection ELFs/listings stay `/tmp/inbox-emitter{remove,killall,killfx}.elf`.
No original function executes or body/asset is exported to history. Remaining:
emitter construction/active/list/FX writers, valid pointer/member/cycle/array/IRQ/
concurrency domains, full caller unload/silence and macro/voice/MIDI key-off effect,
original remove/unload/audio traces. Three local lifecycle bodies are answered;
full native/source/runtime/hardware equivalence remains OPEN.
