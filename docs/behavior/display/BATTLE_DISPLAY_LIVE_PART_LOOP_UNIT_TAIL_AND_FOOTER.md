# Battle display live part loop, unit tail and footer

2026-10-14, US G8ME01 disc0 revision0, fresh read-only DOL inspection.
EVT-01 continuation after [shadow and part-tail admission](BATTLE_SHADOW_REQUEST_OWNER_AND_DEFERRED_PREPARATION.md).
This recovers execution order; no original runtime trace or native completion
is claimed. Addresses/offsets/masks are hexadecimal unless stated otherwise.

## A completed part advances its live link

At80102f70 pass current P to801261f8; after return,80102f78 reads **fresh
WORD(P0)** into current P. Nonnull branches80102f80→80102860. That destination
is the part loop, after the unit alpha/HPgauge/status/scale prefix. It reads
P1c0 and tests **exactly−1**; equality jumps80102f78 to advance that skipped
part's fresh link. This is not a test of every negative signed handle.

A valid linked next part therefore executes its own source pose selection/
rate/modes/phase/Main/queue/shadow/sound graph once. It does not rerun the unit
prefix, repeat the preceding part's phase increment/Main, re-register the
preceding sprite/shadow, or revisit registry index lookup. Native pending
providers must retain current unit, part and continuation site. Resuming from
the top of displayPrefix loses this source ownership. A native typed missing-
pose binding can remain explicitly unavailable even when source handle is−1;
that constructor gap must not be called original hidden-part admission.

Actual Mario has three linked source parts; Goombella has one. Constructor
allocation/order/bindings remain the existing [part owner](NATIVE_BATTLE_POSE_RESOURCE_OWNER.md)
contract. This loop does not reduce Mario to only its visible primary body.
Live links are dereferenced only after their current part tail completes;
precollecting a vector of all parts changes mutation/lifetime semantics.

## Used automatic extra-part branch (P204 mask2 clear)

Source8010286c..878 reads P204. Mask2 clear calls _GetStatusPoseType80103084(U)
at80102880; this is an actual provider, not an indication the part cannot be
rendered. Its complete priority/status selection and the subsequent name
lookups are already in [automatic selection](../battle/PART_PENDING_POSE_MODES_AND_AUTOMATIC_STATUS_SELECTION.md).
Mario kind224 and Goombella225 take the player-range gates. Enemy kinds1..215
search the part table directly and bypass those player-only overrides.

Automatic paths converge on CheckStatus(U,19), else CheckStatus(U,20), choosing
speed2/.5/1 via btlDispAnimeSpeed80100a4c. Forced mask2 set instead directly
chooses1. Each speed service writes P208 and publishes the actual rate product
P208*P20c to the bound pose. At801029f8 fresh mask1 decides whether to consume
a pending animation; clear jumps80102b68 WITHOUT SetAnim or P214/P215 rewrite.
The automatic lookup itself does not copy its result to P1c4 or set request1.

At80102b68 all valid parts, forced or automatic, execute gravityOffsetControl,
floatOffsetControl,statusPoseControl,PartsWorldPos,animPoseMain, then source
part RGB/alpha publication and attribute-dependent queue/shadow gates. The
lookup being unused by clear request1 does not suppress these providers.

The automatic display lookup is specifically searchPoseTbl80103cd0: null
table returns0; otherwise initialize CTR to decimal28 and scan eight-byte
records. At each record, first exact requested-tag match OR exact tag28 returns
the stored name pointer immediately. Only after neither match does the pointer
advance8 and CTR decrement; exhausted28 probes return0. A tag>=70 is NOT a
terminator here, and a tag28 record can win before a later requested tag. This
is distinct from GetPoseNameFromType's category fallback/terminator behavior;
using packedBattlePoseName unchanged for this display lookup changes source.
Fresh ISO body80103cd0:44 SHA256
559e085cb58f73fd266eda2adb12acf2a01c79f303a9785a9bdedc1126835c20.

The player override's CanActStatus8012724c:90 short-circuits CheckStatus in
order1,2,9,27; any nonzero returns0. Otherwise CheckStatusFlag(U,1) is the
last service and the result is1 iff its return iszero. Existing native
packedCanAct follows these checks and final U138 bit1 on its admitted valid
unit domain; it is this original service, not a different HP/floor/action
predicate. Fresh body SHA256
0efa9b0c5f3b59da89a48ab457f4caf227099155d3fe9db8b009e24a36f496eb.
Its null-unit guard is a native domain guard, not evidence of source call
admission for a missing unit. Complete supporting contract is
[party/CanAct](../battle/PARTY_ICON_SOURCE_AND_CAN_ACT_STATUS.md).

Original extra Mario parts IDs2/3 have attributes13010000 and P2040 at fresh
entry. Their selected descriptor remains real b_mario descriptor0 M_Z_1;
authored table type28 KRN_H_1 is only a returned pointer on this clear-request
pass, and must not trigger fabricated KRN art or an animation restart.
P215 remains initialized0, so statusPoseControl does not invent _pose_def.
Their source absolute-position attribute10000000 governs PartsWorldPos.

After their real Main and RGB/alpha publication, attributes01000000 make the
80102c70 gate jump80102d24, suppressing ordinary part registration. At that
destination attributes02000000 make80102d38 jump80102ecc, suppressing the
fresh floor/shadow query. Thus they do **not** need an opaque queue entry or
shadowEntry before their sound tail. Mario kind224 skips the listed paper kinds
and reaches801261f8. No-table P4d0 admits its no-service return, after which
the live P0 is advanced. This is the original hidden-extra-part admission;
a generic pending wholeMain at either suppressed draw/shadow gate incorrectly
blocks the first used automatic part. Nonzero sound table or different flags
retain their real services and must not inherit this bounded bypass.

## Unit tail after the last part

After P becomes null,80102f84 reads current unit U308. Nonzero selects this
ordered U1c flag graph, using one captured flags word:

1. If40000000 set and08000000 clear, queue callback80101548.
2. Otherwise, if04000000 set and02000000 clear, skip all tail registration.
3. Otherwise, if08000000 set, skip all tail registration.
4. Otherwise queue callback80101678.

Each admitted request is dispEntry(camera4,class2,callback,userdata=U,
priority=source+0 at80422240), called at80102fbc or80102ff4. They are real
borrowed unit display callbacks, not immediate callback calls. They do not
consume a guessed item icon or arbitrary screen priority. Source U308 is an
admission operand; nonzero's identity may have additional callback ownership.

If U308==0,80102ffc reads retained captured BattleWork Bef4 and tests mask
20000. Clear skips registration; set queues callback801015ec with the same
camera4/class2/U/+0 tuple at80103020. Therefore even a no-item unit can admit
a real tail callback through this battle flag. Source ordinary Mario/Goombella
with U3080 and Bef4 mask20000clear take the no-registration path; this is a
conditional source domain, not proof those fields are permanently zero.

All branches converge80103024. Increment registry index, compare signed index
with decimal64, and if below call BattleGetUnitPtr800f88cc again through
80102530. Getter uses fresh global BattleWork; the display body retains its
original captured B for some later work and the footer. Null registry slots
still advance toward64. Do not end display after the last known native player
or skip remaining valid Frankly/enemy registry owners to reach the footer.

## Exact footer and first service

After all64 indices, first unconditional footer provider is
BattleStockExpDisp8010045c at80103030. The remaining requests are:

1. BattleStageDisp80139084 at80103034.
2. BattleCommandDisplay80122870(captured B) at8010303c.
3. BattleAudience_Disp801a6c18 at80103040.
4. BattleBreakSlot_Disp80203a68 at80103044.

The [ending footer contract](../battle/ENDING_NEXT_SLOT_DISPLAY_AND_PUBLIC_SOUND_REGISTRY.md#btldispmain-first-surviving-unit-provider-and-complete-footer-order)
already pins these services' own registration/admissions. Whole btlDispMain
completion requires their actual providers, not merely a trace entry. After
all five return, freshly read captured Bef4; if10000 set, clear/store. Freshly
read captured Bef4 again; if20000 set, clear/store. Only then return. These
flags are not cleared before the last unit-tail gate or before footer callbacks
are registered. Source flag-clear operations preserve every unrelated bit.

## Complete local BattleStockExpDisp admission

Body8010045c:7c captures **fresh global B** in r4. Compare signed Bf00 with
signed Bf04. If f00>f04, load Bf08, increment with word wrap and store;
reload counter and compare signed with4. If counter>=4, load f04, increment
with word wrap and store. There is no local f08 reset. Overflow into a negative
counter naturally fails the signed>=4 gate; native saturation changes this.

Then read captured current Bef8, test bit2. If clear return. If set call
real dispEntry(camera8,class1,callback80100258,userdata0,priority901.0),
constant80422238 bytes44614000. Scalar update occurs even when the draw gate
is clear. Queue registration returns without executing80100258. The pending
native frontier can therefore advance past this local body only if its scalar
owner and required display record really exist.

Stage/Audience/BreakSlot registration likewise survives the absence of visible
vertices; deferred draw and GX providers remain separate. The unit loop,
footer, display sorting/camera dispatch and eventual battle return are distinct
completion stages.

## Original evidence and scope

Fresh ignored listings under artifacts/battle-display-queue-evidence include
the continuation and StockExpDisp. Static identities:

| Original range | SHA256 |
| --- | --- |
| btlDispMain801024f0:b94 | 6281c04f5a9784643665fd82f9f0bbc37f053f52fb2cdf672076049a4c17802f |
| StockExpDisp8010045c:7c | 31df30597cd63cbcd3072269fe3409c9de13a925bea8be21cc838e9837d079e0 |

Native progress must retain live owner/cursor, not infer completion from an
empty presentation list. Special admitted callbacks80101548/15ec/1678,
nonempty pose-sound table, remaining registry pose constructors, every footer
provider and deferred camera/GX consumers retain their own required work.
This documentation neither modifies code nor closes EVT-01/prologue parity.
