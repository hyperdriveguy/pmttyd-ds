# MusyX VID lifecycle and last-started reset

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete vidInit802843f4/e0,
vidRemoveVoiceReferences802844d4/368, vidMakeRoot8028483c/14,
vidGetPublicId802849b4/54 and voiceResetLastStarted802860c4/74hex.
Addresses/offsets/masks hexadecimal; counts decimal. Static ordered memory
operations, not original chain/lifetime/audio observations. Read
[VID constructor](MUSYX_MACRO_START_PRIORITY_AND_VID_CONSTRUCTION.md),
[block/unblock](MUSYX_VOICE_BLOCK_UNBLOCK_AND_ALLOCATION_PEEK.md),
[kill/recycle](MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md) and
[last-started initialization/setter](MUSYX_MACRO_ACTIVE_INITIALIZATION_AND_LAST_STARTED.md).

## Node and voice domains

Nodes: next word0, previous4, public ID8, internal chain-head IDc. Free root
8041ef28, active root8041ef2c, public counter8041ef30. Voice V: successor IDec,
predecessor IDf0, identityf4, local node pointerf8, public/root node pointerfc.
Voice addressing reads synthVoice8041ee90 with458hex stride and low8 of an ID.
No null/member/range/identity-generation/cycle/alias/IRQ/sndActive validation.
Node pointer equality, exact identityffffffff and chain IDffffffff are distinct
conditions. Removing references does not free a voice or clear its identity,
chain links, priority/free membership, macro/resource or hardware state.

## Complete initialization:112 nodes

vidInit sets counter0, active root0, free root80405b40, in that order. Sixteen
iterations each link seven16-byte nodes:112 nodes total, last80406230. Starting
previous0, each row stores previous at+4 and, if previous nonnull, stores current
address at previous.next. Retain current as previous and advance16. Finally
last.next=0. This is112 nodes, not128 inferred from the800hex-byte aid offset.
The first previous is0; all later previous point one record back; all but the
last next point one record forward. Nodes' public/internal ID words8/c remain
untouched. No voice pointer/link reset, active-node teardown or pool allocation.
Reinitialization preconditions and untouched aid bytes remain separate.

## Last-started reset before reference removal

voiceResetLastStarted(V) reads identity; exactffffffff returns. Read channel
byte121;255 returns. Read set byte122, retain slot=U8(identity):

- Set255: address FX table80406a40+SLOT (not channel). Read stored byte;
  only equality with slot stores255.
- Other set: address80406a80+set*16+channel; only stored byte==slot stores255.

No full-generation match, FX/channel/slot64 bound, MIDI channel16/set8 bound,
whole-table reset or child request. Channel still gates the FX route even
though it does not index that route. These are exact source comparisons/indices,
not a generic removal of all table entries containing the voice ordinal.

vidRemoveVoiceReferences retains V, first checks V.identity. Exactffffffff
returns before ALL work. Otherwise request voiceResetLastStarted(V), then read
predecessor fresh. The child is not assumed to establish subsequent node/list
validity. Branches below preserve source rereads; IDs are not compared against
neighbor records' own identities.

## Node unlink/recycle primitive

Whenever a branch below says recycle X, X is the selected V node FIELD (f8 or
fc), reread separately at the listed steps, not a permanent node snapshot:

1. Read X, capture X.previous. Nonnull: previous.next=X.next; null:
   activeRoot=X.next.
2. Reload X, capture X.next. Nonnull: next.previous=X.previous fresh.
3. Capture freeRoot; reload X and store X.next=capturedFreeRoot.
4. Reload freeRoot. Nonnull: its.previous=fresh X.
5. Reload X and store X.previous=0; freeRoot=fresh X; V.X=0.

Source copies this sequence, rather than calling a helper. No node ID clear,
heap free, active-list scan or pointer validation. Unsupported aliasing can
change later rereads or which node gets recycled. The primitive describes
exact request/store order, not an atomic operation on a stable graph.

## Complete non-root detach

If fresh V.predecessor!=ffffffff:

1. Resolve predecessor low8 against freshly loaded synthVoice; store its
   successor=V.successor.
2. Read V.successor fresh; nonffffffff resolves it against a fresh synthVoice
   and stores its predecessor=V.predecessor fresh.
3. Unconditionally recycle V.localNode f8, then return.

V.publicNode fc is not cleared here. Own predecessor/successor/identity remain.
Local-node null is not a supported no-op branch. No full neighbor-ID validation.

## Complete root promotion

If predecessorffffffff but fresh successor!=ffffffff:

1. V.localNode.internalHead=V.successor.
2. Read local/public pointers; if unequal, publicNode.internalHead=fresh successor.
3. Resolve fresh successor low8/fresh synthVoice; successor.predecessor=ffffffff.
4. Resolve successor AGAIN with fresh successor ID/base; successor.publicNode=
   V.publicNode fresh.
5. Reload local/public pointer equality. Unequal recycles V.localNode; equal
   retains that node in the active list. Then V.localNode=0, V.publicNode=0.

A surviving public node now maps to the promoted successor chain head. Local
and public equality is tested twice, not snapshotted once; alias/concurrency
preconditions remain required. No successor activation or ID regeneration.

## Complete terminal root removal

If predecessor AND successor bothffffffff, compare local/public pointers:

- Unequal: recycle local f8, then recycle public fc, in order.
- Equal: recycle local f8 once, then store public fc=0.

No null guard precedes either case. Equal null pointers still enter a primitive
that dereferences null; unequal with a null public pointer still attempts its
recycle. This source observation does not claim those states are reachable.
Neither branch clears own identity/chain fields. Callers such as voiceKill,
macro termination/start and voiceBlock provide separate subsequent state work.

## Root conversion and public-ID lookup

vidMakeRoot(V) reads local pointer, stores it to V.publicNode, rereads local
pointer, returns that node.word8. No null check or node/list/head mutation;
return can use a different pointer if intervening mutation changes V.localNode.

vidGetPublicId(raw internal ID I) captures activeRoot and synthVoice once. For
each active node, start at node.internalHead. Until exactffffffff, compare FULL
unsigned chain ID==I; equality returns that active node.publicID. Otherwise
resolve low8 against CAPTURED synthVoice and read record.successor; repeat.
Chain exhausted follows node.next. Active list exhausted returnsffffffff.
No own-identity/activity/slot validity check, sorted-ID early break, recursion/
cycle/count guard, list repair or whole chain captured snapshot. I==ffffffff
never matches because the sentinel is tested before comparison. The same
internal chain can map through multiple active nodes; first visited match wins.

## Evidence and remaining bindings

[Manifest](../../../tools/musyx_vid_lifecycle_and_last_started_reset_evidence.json)
pins five complete bodies,252 instruction anchors/one direct call. Seven
removal-branch/six last-started vectors and
112-node link/address deductions are static checks, not original execution.
No original bodies/assets committed; full fresh inspection ELFs/listings remain
`/tmp`. Byte identity/anchors do not prove runtime ownership or graph invariants.

Remaining: actual chain/node/last-started/caller writers, initialization order,
valid local/public null/equality/member/slot/ID/generation/sorted/cycle/alias/
reinit/IRQ/concurrency/rollback domains and original chain promotion/removal/
public-lookup/reservation/audio traces. EVT-01 remains OPEN.
