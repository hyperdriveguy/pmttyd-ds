# Native NPC actor event leaves

Implements the five [original flag/position/yaw leaves](ORIGINAL_NPC_FLAG_POSITION_YAW_EVENT_LEAVES.md)
through the [registered NPC setup bridge](NATIVE_REGISTERED_NPC_SETUP_WORLD.md).
This is actual retained actor/VM integration, not a new semantic actor catalog.
It does not complete the original area parent, NPC Main/query/facing/render or
unbound case/map/hit/wander services.

## Source mechanics and native suspension

`fieldNpcActorEvent` dispatches original US identities800500b0 (flag),80053158
(set position),80052e98 (get position),80050988 (yaw) and8005088c (left/right
yaw). It captures the original argument-array pointer and resolves arguments
in source order. Raw get-position destinations remain raw. Complete synchronous
stores preserve source order: previous XYZ first, fresh paired previous XY
reads before current XY stores, then late previous Z read/current Z store.
No extra velocity, pose, elapsed-time, collision or actor+4 writes occur.
Get-position reads each fresh component immediately before its output service,
so an earlier output alias can affect a later component.

The bounded native cursor retains successfully evaluated arguments, target,
output index and captured output value across unavailable providers. It belongs
to one event pointer/ID, original function and provider owner. First-call restart
resets native state; source mechanics have no first-call branch. Real source
completion returns2 and clears it. False providers publish nothing. Resolve
success admits stable full actor storage through synchronous effect publication;
removing facade mappings or recycling actors without retiring referencing
scripts violates that lifetime domain. Position stores cannot suspend halfway
through the admitted six-store sequence. Output setters may suspend separately
without repeating earlier outputs or rereading their captured values.

## Actual actor and operand ownership

`FieldNpcSetupBinding::invokeActorEvent` requires its same retained VM, admitted
GP mode and an actual active context entry. It supplies the real ordinary36 or
alternate16 actor allocations, not renderer models. Ownership is checked by
slot/range in constant time; the existing facade binding retains their complete
stable spans. `me` reads fresh event+170. `master` and `slave_0..3` read the
actual context actor's +32c/+330..33c association. These context targets need
owned storage, without an invented active1/query/render gate: clearing active1
and then setting position through `me` is valid. Ordinary names use release
lookup and its fresh GP-selected registry/active1 rules.

For operations that dereference an actor, negative party/Mario sentinels and
unowned exhausted lookup pointers hold. Invalid flag selectors still evaluate
all three arguments and resolve names, but neither dereference nor store target
flags. They can therefore complete even for a sentinel or unowned returned
pointer. Association resolution still needs its actual owned self dereference.
Bounded strings are32 bytes; this is native admission, not a measured source
limit. No Mario/partner dispatch is invented for these five callbacks.

Checked operand services call the existing evtGetValue/evtGetFloat/evtSetFloat
routers once per admitted request. Missing memory or exceptional fctiwz input
marks the request unavailable. A float output defers its one possible word
store until its reads/conversion succeed. Context/entry/global identity reads
are checked too: an earlier UW output can really overwrite GP80415fd0, making
the next output hold without an assertion. Restoring the real GP resumes the
next captured output without replaying the first. Actual setter namespace
asymmetry, encoded work quantization and GF/LF bit behavior remain unchanged;
NaN is not rejected universally for no-store/flag destinations. Partial source
stores remain visible on subsequent holds; no transactional rollback is added.

## Actual camera4 and arithmetic

Optional retained `CameraManager` comes from the session owner. Left/right
reads slot4.yaw freshly at its camera boundary, only with a published view.
The [camera4 contract](../camera/NATIVE_FIELD_CAMERA4_PUBLICATION_AND_BATTLE_INHERITANCE.md)
documents source+114 mapping and native field/battle ownership. No synthetic
boot yaw or movement heading substitute is supplied. The leaf itself does not
invent a field-mode rejection of genuine retained battle camera4.

Selector conversion admits finite signed32 values before target resolution.
The arithmetic uses original packed100/5 constants, float `std::fma`, float
division, double libm fmod360, binary32 rounding, original NaN-to-zero and
post-negative-add >=360-to-zero guards, followed by signed remainder. Selector0
may yield -90; it is not rewritten as270. Negative zero is retained by the
revise step. This is native libm arithmetic, not proof of Gekko/FPSCR bit parity;
ARM fixtures check the stated ordinary numeric cases. No animation-facing or
paper-turn interpolation is requested by the yaw stores.

## Area dispatch, data closure and validation scope

The retained Area routes these identities through the installed setup adapter.
Instant completion uses a stack cursor and allocates no map node; only a native
hold retains state. Actual event retirement and room retirement erase cursors
before releasing their bindings. Normal Game activation still needs the earlier
source parent services and shared session owners.

`field_npc_actor_assets.py` adds nine hash-pinned unchanged DOL string/numeric
views (128 payload bytes) to the existing private door pack. They include the
original `me` pointer8041fd80 required by DOL npc_init_evt80415e18. Previous
script closure had the script but omitted that literal. Packs retain original
addresses/bytes, no PPC or fabricated mutable globals, and stay outside history.

Host and ARM fixtures exercise original fifteen-row setup with real registered
actors/poses and the existing explicit test RNG. DOL npc_init_evt now genuinely
executes its flag callback and returns; chusan3/roten3 retain their original
RETURN/END completion. Other unbound callbacks hold. Remaining init roots are
explicitly retired in the scheduling fixture before proving two regular roots;
this still does not claim all init callbacks ran. Separate fixtures cover actual
inactive `me`/master stores, real quantized LW outputs, negative sentinel holds,
invalid flag selectors, camera absence/yaw, corrupted input and aliased outputs.
Portable checks additionally mutate previous fields and output aliases to prove
paired/late reads and retained raw destinations. These are source-order/provider
fixtures, not Dolphin observations or new GPU/physical3DS performance evidence.
