# Native Harbor field start, battle admission and return

`NativeHarborFieldSequence` composes the existing whole original
`FieldNpcBattleBinding` Start8004ef54/b8, Wait8004eed8/7c and
GetRegularId8004f894/58 bodies with a private native frontend sequence owner.
It does not acknowledge original Sequence Main Init/Exit callbacks or mutate
Game's separate original sequence controller. The original source bodies,
discarded extra operand read and return0/2/1 behavior remain documented in
[native battle callbacks](NATIVE_HARBOR_BATTLE_CALLBACKS.md) and
[field entry/return](FIELD_BATTLE_ENTRY_AND_RETURN.md).

The actual field VM, initialized same-VM NPC registry and its setup/regular-event
owners must be bound before admission. Source field context GP/F/mode and the
intrinsic VM/facade registration are checked before borrowed access. The caller
supplies a dedicated context registration whose lifetime also covers the
borrowed setup/backend owners; a Game registration alone is insufficient when
that smaller context can retire earlier. Setup and VM remain stationary until
the binding is destroyed. Retained tickets do not pin those borrowed objects.
Intrinsic VM proof expiry rejects before any borrowed VM read. Context teardown
and event retirement are quiescent; attempted retirement during invocation
poisons further admission without resetting or replaying source work.

The private native sequence begins in field2 by explicit native constructor
policy. Start's actual original SeqSet4 call requests battle4. Until the entire
source invocation completes, no ticket is published. A held source request
locks its actual EventEntry address/ID/function, including before the original
body returns; another caller cannot steal its partial invocation. Successful
source return0 is retained through any final metadata hold, never called again.
External original child requests are guarded before/after callbacks. They must
retain their own partial effects. Recursive entry is rejected.

Only the actual completed Start invocation seals a copy-safe ticket. It records
the same state owner and actual source field NPC; no integer result, event ID,
visual slot or polling-based ready flag can create a ticket. The VM manager
must be idle before the frontend consumes it. SeqGet remains field2 until
`entered(ticket, actualBattleIdentity)` admits a genuine tutorial facade/runtime
identity; it then becomes battle4. Entry is idempotent only for the same actual
runtime, including shared control-block identity.

## Actual completed return

The actual HarborTutorial facade alone can create `NativeHarborBattleIdentity`.
Its private closures pin the actual runtime and check the facade registration
before accessing it. Replacement rejects old admission even if packets retain
the old runtime. The completion closure locks that runtime's actual retained
`NativeHarborCompletion`, requires its current Done receipt, and compares the
entire packed pouch, unchanged numeric result and shared runtime control block.
Public receipt fields alone cannot prove teardown. An otherwise matching
manually constructed receipt is rejected before real completion; a forged
shared pointer control block or altered pouch is rejected afterward as well.

After actual presentation/audio retirement, real pouch export/commit and field
presentation return, the caller invokes `returned(ticket, receipt)`. Only then
is unchanged original result written to actual F+30 and native SeqGet switched
from battle4 to field2. Source ending uses original field result1/3/4/5, not
the initial conclusion classification0 and not result+1. The present actual
completion owner supports victory1 only; other results therefore remain
unavailable despite the source range check.

This does not acknowledge the original field Wait. That whole body still
checks field2 AND the nonzero actual result, calls real Mario CtrlOn, current
party and global controls, BGM stop and Mario Post, then returns2. Held BGM/Post
children retain their source prefix; no repeated control decrement occurs.
Original beforebattle dialogue, branches, source sailor ID queries/deletions and
the afterbattle child remain owned by their actual VM scripts.

## Required production context and remaining integration

The Encounter facade needs its own fresh copy/move registration. Before any
Game callback borrow, its context must compare the calling facade with its
captured facade identity and validate both host and facade proofs. A copied
Encounter sharing a runtime must not inherit the original Game's callbacks,
even while the original Game remains alive. The sequence owner's context proof
does not itself implement this higher-level facade routing policy.

Actual field Prepare/Post can use
[the typed native transfer adapter](RETAINED_MARIO_BATTLE_TRANSFER.md).
Actual same-VM sailor constructors and regular IDs are provided by
[the native source sailor owner](NATIVE_HARBOR_SAILOR_CONSTRUCTION.md).
Required remaining consumers are the native Encounter context/router, genuine
battle-info area/configuration ownership, actual field controls/XYZ/direction,
sound flags/BGM retirement and the postbattle dialogue/movement/provider graph.
Game/Renderer/Audio production hooks and normal entry activation are excluded
from this package. Missing providers must retain the original source request;
the existing development fieldBattleServices booleans do not satisfy it.

## Focused proof

The test executes actual original source resource/Entry constructors in the
same VM, uses genuine event contexts and original Start operands, and composes
real source Start/Wait with actual typed native Mario/party mutations. The
operand-cursor setup is an explicit TEST prefix, not dialogue execution proof.
It verifies held Prepare, sealed entry, manager admission, rejected foreign
caller, recursive resume, source result publication, retained BGM hold and
actual Post/control order. Completion admission uses the real retained teardown
owner in an explicit TEST concluded domain with test presentation/audio hooks;
it does not bypass a lethal root in gameplay or prove a Crump playthrough.
Retained owner after actual VM destruction rejects under sanitizer. Full native
build, whole Game integration and emulator round trip remain parent integration
checks on a coherent complete source/header graph.
