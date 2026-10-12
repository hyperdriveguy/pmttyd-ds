# Field party destruction, saved kind and pointer lifetime

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
partyKill at DOL 0x800be69c,size0x160 and partyKill2 at 0x800be594,size0x108.
This specifies local writes and ordered service requests;animation release,
allocator internals and original transition execution are separate dependencies.

Use this with [field callbacks](FIELD_PARTY_EVENT_CALLBACKS.md) and
[position constructors](FIELD_PARTY_POSITION_CONSTRUCTORS.md). In particular,
clear(nonzero) dispatches partyKill2(1), while kill(nonzero) dispatches
partyKill(1). These bodies establish the semantic difference.

## Shared entry and exit-hook dispatch

Argument i is a signed party pointer-table index. A negative argument produces
null;otherwise read table[i] at **0x8041ead8** (r13+0x1bb8), with no upper
bound check. Null returns integer0 immediately without slot/saved-kind writes.
Let P be the nonnull object and M be its stored owner pointer at **P+0x160**.
Both services compute the slot-address base **M+i**, then read signed byte
**M+0x245+i**. If nonnegative, read that index from the same pointer table;
otherwise use null. The subsequent read of that object's **+0x31 kind byte**
is **not null-guarded**. A valid P alone therefore does not make every owner
slot configuration a supported input. Preserve this implicit domain requirement
when assessing an equivalent implementation;do not pretend the original
gracefully skips an inconsistent linked slot.

Sign-extend the linked object's kind byte and choose the record at
**0x803143e8 + kind*0x28**. Store this pointer globally at **0x8041ead4**
(r13+0x1bb4), read callback **record+0x24**, and call it with P if nonnull
and **P+0x34 mode is not11**. Mode11 suppresses that callback only; it does
not suppress the later clear/release/free sequence. No lower/upper kind
validation occurs before table selection.

The pointer and kind used to choose this record come from M's linked slot,
not directly from P+0x31. They normally correspond under the original runtime
invariant, but this body does not check that equality. Callback side effects
are significant:partyKill rereads M's slot after the callback.

## Saved-kind difference

partyKill rereads signed **M+0x245+i** after the optional callback and resolves
that pointer-table entry again. If the resolved pointer is nonnull, copy its
raw kind byte **+0x31** into **M+0x247+i**. If null, store0 there. Thus the
saved kind reflects the linked object after the hook;it is not necessarily
the kind captured before the hook or the original P kind.

partyKill2 instead stores **0** into **M+0x247+i** unconditionally after the
hook. It performs no post-hook linked-pointer lookup for a saved kind.

Both then, in order:

1. Store byte **0xff** at **M+0x245+i**, the negative current-slot sentinel.
2. Store null in pointer **table[i]**, using the original input index i.
3. Call **animPoseRelease(P+0x0c)** at 0x8003b2d0, without a local negative
   pose-ID guard. The pose driver owns invalid-ID behavior.
4. Call **__memFree(0,P)** at 0x800300c4.
5. Return integer0.

The freed pointer is captured P, even if the linked owner slot or table changed
in the callback. The code does not clear the global current-index word at
0x8041ead0 or the global record pointer at0x8041ead4. It does not locally
export HP/FP, save a pouch snapshot, detach every event, award a reward or
recreate a party. Such effects require explicit called-service or other-owner
evidence.

## Concrete lecture composition

The [lecture field event](LECTURE_FIELD_ENTRY_AND_RETURN.md) kill(0) first
reads signed Mario+0x245 in its wrapper; if nonnegative, it passes that value
as i to partyKill. When the linked owner/table invariant holds, partyKill
saves the linked kind, writes the negative current sentinel and clears/frees
the table entry. The later previous-position wrapper can request creation
from the saved byte on its first call. This explains its intended restoration
route without assuming a battle victory result or inventing a resource reset.

The preceding wall event clear(1) dispatches partyKill2(1) after the wrapper
checks Mario+0x246 nonnegative. Under the same invariant it clears saved
secondary kind at Mario+0x248;the previous-position callback's zero-saved-kind
test will skip secondary construction until another writer supplies a kind.
This is why replacing clear with kill would alter later restoration policy.

The pinned records for kinds1 and8 at0x80314410 and0x80314528 have **zero
exit-hook pointers at+0x24** and the same init callback kuribo_init. This
establishes the local hook bindings for those records;it does not prove every
runtime lecture partner always has one of these kinds. No story dialogue or
English attribution is inferred from kind indices or animation resource names.

## Verification and remaining work

tools/verify_field_party_lifetime.py checks six containing bodies,43 instruction
anchors,three constants,five data spans and three actual field callback imports.
The destructor bodies and their used saved-kind writes are checked independently
of the callers. Metadata reports and original Gekko listings remain ignored;
no original executable bytes are included in source history.

Open:exit-hook bodies for other kinds,animPoseRelease and heap ownership;
current-index/owner-slot writers and domain invariants;all party mode updates,
resource snapshot ownership,battle teardown and original accepted/declined/
interrupted/reentry traces. These static bodies do not complete the party system.
