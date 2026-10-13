# MusyX sequence validity, private-ID lookup and stop lifecycle

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndSeqGetValid80279d6c/30,
sndSeqStop80279d9c/38, seqGetPrivateId802728f0/68 and seqStop80273704/2b0hex.
Read [game cleanup caller](GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md)
and [MusyX IRQ nesting](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).
Offsets/addresses hexadecimal; explicit decimal counts/strides labeled.
Static local service behavior, not original sequencer/audio observations.

## Complete active-then-paused private-ID lookup

Input external sequence word X. Retain X; capture seqActiveRoot8041ee5c.
Search key K=X&7fffffff. Traverse node.word0 next pointers; compare each node's
RAW wordc unsigned to K. First equal returns zero-extended node.byte9, with
ONLY the original X bit80000000 inserted into the result. Do not mask the stored
key or copy all other input bits into private ID. Miss/null active list then
loads FRESH seqPausedRoot8041ee58 and performs the same search. Total miss-1.

No initial X==-1/sndActive guard, IRQ, membership/order/cycle/count validation,
node allocation/free or generation check. It is a linear first-match search;
active duplicates win over paused duplicates. X and X|80000000 share search key
but return differently tagged private IDs. Valid private-byte/index and root
lifetimes remain source invariants beyond this complete reader.

## Complete public validity and stop wrappers

GetValid requests seqGetPrivateId(INPUT X), then returns0 exactly for returned
wordffffffff, otherwise1. Emitted word arithmetic is
`((wrap32(ffffffff-result) | wrap32(result+1)) >> 31)`.
No IRQ/active flag or node-state/voice/hardware query. It validates local lookup
existence, not audibility, and returns only0/1 under the literal word operation.

sndSeqStop retains raw X; Disable MusyX IRQ, request seqStop(X), Enable, return.
It does NOT retain the core result across Enable or assign a separate status.
No validity precheck, active-system gate, resource free or input normalization.
The nesting wrapper and direct OS frame-critical calls remain distinct.

## Core lookup and high-bit stop mode

SeqStop INLINES the same active-then-paused lookup: retain input bit31, compare
input low31 to raw wordc, compose byte9|originalHighBit, miss-1. It does not call
seqGetPrivateId. Miss returns without instance mutation or voice request.

On match, original input bit31 selects mode. Tagged result high bit SET:
clear tag, select instance base803ebdc0+wrap32(privateID*1888hex); read byte8.
State0 returns without writes; any nonzero state stores wordefc=0 and returns.
No unlink, note kill/recycle, state reset, immediate stop or core success code.
The meaning/producer of wordefc remains open; do not rename this branch as a
complete pause, restart or playback-stop operation without consumer evidence.

Untagged result selects the same instance array without a range guard. Original
BSS seqInstance occupiesc440hex bytes, eight1888-byte instances. Byte9 values
8..255 can address beyond it; lookup success alone is not a bounds guarantee.
No source-null/ID-range/generation or fresh external-key recheck.

## State1 active removal and note kill requests

For untagged instance Q, read state byte8. State1:
read Q.previous(word4). Nonnull writes previous.next=Q.next; null writes
seqActiveRoot=Q.next. Then traverse note lists rooted at Q.worde64 and Q.worde68,
in that order, then Q.worde6c. For each node N:
request voiceKillSound(N.word8)80285bd8; AFTER return load N.next(word0).
No captured-next protection, voice-validity precheck or child-return admission.
Child list mutation can affect traversal; no cycle/count/null repair.

AFTER all three kill traversals, process those same list heads again, in order.
For each nonnull head H, freshly walk next pointers to tail T. Read noteFree8041ee4c;
if nonnull set tail.next=oldFree and oldFree.previous=tail. Then reload that list
head, store noteFree=head, clear its instance head word to0. No whole-node clear,
head.previous reset, SDK free or hardware completion wait. Old free root is
reloaded for its previous-link store; source alias/concurrency domains matter.
All three lists join the same free chain; later heads prepend earlier ones.
State1 note handling is skipped for the other state branches below.

## State2 paused removal and shared instance recycling

State2 unlinks only predecessor.next or seqPausedRoot, using Q.next. It performs
no note-kill/list recycling. States0 and3..255 skip both special unlink/note paths
but still enter shared recycling; there is no unknown-state refusal.

Shared tail for EVERY untagged matched instance, in exact order:

1. Fresh Q.next; if nonnull, store Q.previous into next.previous.
2. Capture seqFreeRoot8041ee54; store Q.stateByte8=0.
3. If captured free root nonnull, write its.previous=Q.
4. RELOAD free root, store Q.next=freshRoot; store Q.previous=0.
5. Store seqFreeRoot=Q.

Only state1/2 perform prior predecessor/root detachment; other states therefore
require source consistency. Do not add a generic detach or refuse state0 to hide
potentially inconsistent lists. Key/private-byte/project/note/other instance
fields remain. No reference decrement, resource unload, allocated-object free,
return-value assignment or request for physical playback completion.

## Verification and remaining ownership

[Manifest](../../../tools/musyx_sequence_valid_stop_evidence.json) pins four
complete bodies, five linked IRQ/voice/register helpers,157 original anchors
and8 resolved direct calls:9 spans. Nine validity-word, five lookup-width and
five state-routing vectors plus original8-instance layout are derived/source
checks, not original execution. Fresh full ELFs/listings stay `/tmp/inbox-seq*.elf`.
No original body/asset exported to history.

Remaining: instance/key/private-byte/state/list/note/free/efc producers and valid
index/cycle/member/alias/concurrency domains, voiceKillSound and full sequencer/
resource lifetime, original validity/tagged-stop/note/audio traces. Four local
bodies are answered; full native/source/runtime/hardware remains OPEN.

[Complete sequence initialization/free-chain producer](MUSYX_SEQUENCE_INITIALIZATION_AND_FREE_CHAINS.md) now establishes initial IDs0..7/state0/empty active-paused roots and free chains. Constructors/later writers and reset/payload/voice/resource/runtime domains remain open.

[Complete sound-chain/internal voice kill](MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md) now closes this exact note-stop child and allocator request core. Map/macro/stream/hardware and source/list/voice/runtime lifetime remain open.
