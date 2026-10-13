# MusyX stream deactivation and companion chain

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndStreamDeactivate8027d10c/714hex,
453 instructions. Addresses/offsets hexadecimal; stages/counts decimal. Static
source deductions, not original deactivation/DSP/audio execution. Read
[activation chain](MUSYX_STREAM_ACTIVATION_COMPANIONS_AND_COMPOSITION.md),
[voice block/unblock](MUSYX_VOICE_BLOCK_UNBLOCK_AND_ALLOCATION_PEEK.md),
[public ID lookup](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md) and
[IRQ wrappers](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).

## Five local lookup stages

Retain raw input public ID. Each entered stage requests hwDisableIrq, then
searches64 streamInfo803f98e8 rows, stride68hex, for first nonzero state+c and
exact fullword public ID4 match. Stages0..3 inline eight iterations of eight
checks; stage4 calls GetPrivateIndex. No live-count bound, sentinel input
rejection, generation decode, duplicate-ID resolution or hardware-active query.

| Stage | Disable | voiceUnblock | Own ordinary enable |
| --- | --- | --- | --- |
| 0, primary | 8027d128 | 8027d2ac | 8027d800 |
| 1 | 8027d2d0 | 8027d440 | 8027d7fc |
| 2 | 8027d464 | 8027d5d4 | 8027d7f8 |
| 3 | 8027d5f8 | 8027d764 | 8027d7f4 |
| 4 | 8027d788 | 8027d7c8 | 8027d7f0 |

At any missing ID, stop that chain and unwind entered stages. This does not
undo earlier rows. Unknown nonzero states can match; inactive0 is skipped during
lookup. The helper at stage4 is requested at8027d790, not used for earlier scans.
No pointer/index/value/resource-lifetime validation or alternate ID search.

## Fresh state, unblock-before-publication and retained fields

After match retain row offset and state-base, reread state+c. EXACT1 or EXACT2:
read raw voice word4c, request voiceUnblock(voice), ignore return, then store
state+c=3 unconditionally. No ffffffff voice rejection or state/generation/ID
recheck after child. Child mutations do not add a success gate or prevent the
retained ordinal's state publication. Read the linked child contract for its
local release/voice-list semantics; these calls do not prove completed DSP stop.

Any other fresh state, including3/unknown/0, skips unblock and state store.
A fresh0 after a previously admitted lookup still reaches companion read; no
second public-ID admission. No buffer-exit/free, cursor reset, voice4c clear,
public-ID/flags/source/format/preference reset or whole-record clear here.
Deactivated state3 remains eligible for nonzero-state ID lookup and reactivation.

After either state branch freshly read row.word0 as companion. Sentinelffffffff
stops local traversal; otherwise next stage consumes that raw ID. There is no
chain snapshot or prerequisite that companion state is currently active.
The fresh read follows the unblock/state write and can observe child mutations.

## Recursive tail and exact IRQ unwind

Stage4 non-sentinel companion requests sndStreamDeactivate(ID) at8027d7ec.
Ignore its return. One invocation therefore processes up to five IDs before
recursing. No visited set, chain/recursion bound, cycle guard or duplicate-row
suppression. Revisiting state3 skips unblock but still follows companion, so
state publication alone does not prove termination. Valid original chain writers/
acyclicity/stack/resource domains remain separate requirements.

Missing ID or companion sentinel at stage j branches to that stage's enable in
the table, then calls each enclosing stage's enable in reverse order. Full local
path:8027d7f0,7f4,7f8,7fc,800. No special voice-failure return path: unblock return
is never tested. Earlier stages issue their local enable only after deeper stages/recursive
children return. The requests have no saved IRQ restore token.
Equal request counts do not prove restoration of original interrupt state or safe
nested critical sections; children have their own IRQ/OS behavior.

After final enable restore LR/GPR29..31 and release32 decimal stack bytes.
There is no independent success/failure return assignment, even for missing ID;
do not interpret residual r3 as a documented Boolean. The complete body requests
no producer callback, initial upload, hardware start or stream-buffer release.

## Source validation and remaining work

[Manifest](../../../tools/musyx_stream_deactivation_chain_evidence.json) pins
five spans (complete body/four linked children),all453 instruction anchors and17
call sites. Five state/fifteen finite acyclic-chain models checked: lengths1..7,
missing links at every local depth, mixed states, already-deactivated rows and
inactive gap. Models retain cursor/voice fields, publish3 only after unblock,
and preserve earlier progress at a missing link; they exclude child internals/
concurrent mutations. Generic verifier checks hashes/anchors/calls, not source
semantic deductions or original execution. Fresh full ELF/listing/generator
remain `/tmp`; no bodies/binary assets committed. Local whole body answered.

Remaining: stream free/frequency/mix, actual ID/state/voice/companion/IRQ/providers
and valid value/index/extent/alias/lifetime/concurrency/stack/ABI/termination
domains, transitive ARAM/DSP consumers and original deactivation/reactivation/
chained failure/audio traces. EVT-01 and full native/full-game binding OPEN.

## 2026-10-13 — Free/reverse-clear follow-up

[Free/reverse clear](MUSYX_STREAM_FREE_AND_REVERSE_STATE_CLEAR.md) closes the caller's full deactivate/buffer-exit/
companion/recursive/reverse-state-clear body. Resource/chain/IRQ/provider domains/
original free/audio/native binding remain open.
