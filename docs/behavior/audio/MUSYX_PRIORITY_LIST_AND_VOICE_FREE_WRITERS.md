# MusyX priority-list and voice-free writers

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete voiceSetPriority80284a08/24c,
voiceFree80285164/1c4 and hwSetPriority80290800/14hex. Addresses/offsets/masks
hexadecimal; scalar counts decimal. Static ordered mutations, not original
voice/audio observations. Read [aid initialization](MUSYX_ALLOCATION_AID_INITIALIZATION.md),
[selection/accounting](MUSYX_VOICE_ALLOCATION_SELECTION_AND_BOOKKEEPING.md),
[macro inactivity](MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md) and
[voice kill/recycle](MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md).

## Record domains and shared unlink

Inputs are retained voice pointer V and, for priority setter, raw requested P.
Priority voice record A=80406340+4*U8(V.wordf4); previous byte0, next byte1,
membership half2. Head H[p]=80406440+p (byte, sentinel255). Priority sort record
B[p]=80406540+4*p: NEXT half0, PREVIOUS half2 (sentinel65535).
Priority root half8041ef34. Free record F=80406940+4*slot: previous0/next1,
flag half2. Free root/tail bytes8041ef39/8041ef38; FX/Music8041ef37/36.
No active-system/IRQ/null/bounds/generation/member/cycle/sortedness guard.
Slot-derived priority/free addressing masks the identity to its low8; sort
neighbor addressing uses low16*4, not a256-entry validation.

For admitted A.half2==1, unlink with branch-dependent fresh reads:

1. Previous!=255: previousRecord.next=A.next; otherwise replace the old head
   with A.next. Priority setter uses its captured old priority byte for this
   head; free uses a fresh V.byte10c read.
2. Read A.next fresh. If not255, nextRecord.previous=A.previous fresh.
3. Only if next255 AND fresh previous255, remove the old priority sort record:
   derive B from fresh V.byte10c; read B.previous; non65535 sets
   previousPriority.next=B.next, otherwise
   root=B.next. Read B.next fresh; non65535 sets nextPriority.previous=B.previous
   fresh. Neither B half is cleared.
4. A.membership=0.

This shared local sequence is present in priority setter/free and the earlier
voiceKill body. Source offsets establish next0/previous2; the earlier kill
contract's reversed prose labels are corrected in the follow-up below. Own
voice links remain stale; aliases can affect later reads.

## Priority admission, removal and prepend

Capture slot from V.wordf4, A and fixed aid bases. If A.membership==1, capture
old V.byte10c. Equality with P.low8 returns immediately: no list change, voice
priority write, metric refresh or hardware call. On inequality, recheck
A.membership fresh; only still1 executes unlink above. Other initial membership
values skip removal and proceed to insertion without repair.

Insert before the captured new priority's head:

1. A.membership=1.
2. Capture H[P.low8]. Store A.previous=255 then A.next=capturedHead.
3. If capturedHead!=255, store capturedHeadRecord.previous=captured slot;
   no sort insertion. Otherwise insert the priority bucket as described below.
4. H[P.low8]=captured slot.
5. V.byte10c=P.low8.
6. Read fresh V.word110 metric, then fresh V.wordf4 slot and request
   hwSetPriority(U8(fresh identity), packed priority).

packed priority = (P.low8<<24) | ((metric>>15)&00ffffff). Metric is unsigned.
High byte replaces the shifted metric's high byte; a full32-bit P is not used.
Captured allocator slot can differ from the hardware slot under intervening
mutation; no consistency check or child-result check. Same-priority early return
suppresses this metric publication.

## New priority bucket insertion

Only the new head255 path admits sort insertion. Read root fresh:

- Root65535: root=new p; B[p].next=65535 then B[p].previous=65535.
- p<root: B[p].next=old root; B[p].previous=65535; oldRoot.previous=p;
  root=p, in that order.
- Otherwise traverse from root through next half0 until sentinel or the first
  ordinal>p. Each ordinal<=p becomes predecessor. Then predecessor.next=p,
  B[p].previous=predecessor, B[p].next=captured successor; if successor!=65535,
  successor.previous=p.

Comparisons operate on nonnegative byte/half values (signed instructions here
have the same ordering). No duplicate-priority rejection: a malformed state
with empty H[p] but p already in the sort chain can cause aliasing/self-links
under these exact writes. This is a precondition issue, not a runtime trace or
permission to normalize the original structure. No count increment/decrement,
voice eviction, free-list unlink, resource/map or hardware stop occurs here.

## Complete free body and wrapping accounting

voiceFree retains V and requests macMakeInactive(V,2) BEFORE reading identity
or membership, unconditionally. It has no V.word34 admission gate. After the
child, derive priority record from fresh V.wordf4; A.membership==1 executes the
shared unlink. Other values skip priority unlink. Then store V.word34=0 and
V.byte10c=0. Read V.wordf4 fresh again for free record and captured slot.

Only F.flag==0 inserts. Capture free root then F.flag=1:

- Root!=255: read tail fresh; F.next=255, F.previous=tail, oldTail.next=slot.
- Root255: root=slot, F.next=255, F.previous=255.
- Tail=slot.
- Read V.byte11d fresh: nonzero decrements FX counter low8; zero decrements
  Music low8. Underflow0→255 is preserved.

Nonzero F.flag skips insertion/counter change. In all cases finally store
V.wordf4=ffffffff. No metric110 reset, chainec clear, reference release,
hardware break/stream kill, resource release or CPU deallocation. This differs
from voiceKill's enclosing macro-active gate, metric reset and final hardware/
stream requests; those services cannot be inferred from the name voiceFree.

## Hardware priority storage leaf

hwSetPriority takes raw ordinal I and word W. Compute wrap32(I*f8), read
hardware voice base pointer8041ef88, then store W to record.word1c. It does
not mask I, check range/activity/IRQ or send a DSP message. The setter supplies
a low8 ordinal; other caller domains remain required. Publication is a memory
write, not proof of voice ordering or DSP/audio consumption.

## Evidence and remaining bindings

[Manifest](../../../tools/musyx_priority_and_free_writers_evidence.json) pins
three complete bodies and the linked inactive ABI/body identity: four spans,
150 anchors/two calls. Six packed-
priority/five admission/six free-counter vectors are static deductions, not
original execution. Read-only hashes/anchors/calls do not establish runtime
ownership. Full original inspection ELFs/listings remain `/tmp`; no original
binary bodies/assets committed.

Remaining: actual priority/metric/kind/identity/caller providers, other list/
counter writers, coherent allocator extents/counts/membership/sorted/alias/
reinit/IRQ/concurrency/accounting/rollback domains; hardware/DSP consumers and
original allocation/free/replacement/audio traces. EVT-01 remains OPEN.
