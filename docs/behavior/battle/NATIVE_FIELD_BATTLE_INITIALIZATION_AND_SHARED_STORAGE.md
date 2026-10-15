# Field battle initialization and shared storage

`initializeFieldBattle` implements the complete US `battle_init800738ac` parent,
called by [sequence Main state4](NATIVE_BATTLE_SEQUENCE_ENTRY_AND_FIELD_BINDING.md).
Complete original listing/hashes and static review remain private under
`artifacts/battle-sequence-init-ownership-evidence/`. This is static recovery;
validation fixtures are not an original authored encounter trace.

The retained cursor captures the current GP before GP168, then F8 and the pouch
getter result. Captures survive unavailable children and memory reads. Camera
mode4/2 follows the pouch capture. A fresh current GP0c selects the debug message
branch: nonzero calls the actual debug-data getter and msgLoad(pointer-or-original
`gon_11`,0); zero skips that branch. Map allocation precedes GLOBALB=0, full
animation battle initialization and field-NPC release. Only then does a fresh GP
receive word14=1, followed by event-context reinitialization and sound flag10.
These children never receive successful defaults.

Information construction preserves original initializer/setter call order,
Mario party getter→party pointer→signed kind byte→kind conversion→captured-pouch
halfword at14*kind, then the eligibility mask2 gate. A blocked party skips only
its setter. F1c mask1 suppresses first attack. Otherwise F10 has exact equality
routes20000..1000000, special10000000 loads F14, and all other bit patterns map0.
The setup value is ADDRESS N230. N2f4/2f5/2f6 are independently read and stored
at information18/1a/1b. Lower BattleInit receives captured F20; only acknowledged
completion returns source integer0. No unavailable child advances sequence work.

`BattleSequenceBinding` nests this cursor within the existing shared sequence
continuation and exposes its pending child. Both initial entry and initial
battle_init admission require GP168 to equal the same NPC owner's fourth580-byte
allocation and F8 to name an active aligned field actor. This is native domain
admission; it adds no original field write. The retained owner protects the
infallible Information initializer's supported memory span.

The camera child changes only the existing shared camera4.mode halfword to2.
It preserves lens, view, projection and publication history, and never runs boot
initialization. Information leaf setters write the actual shared F20 storage.
Animation refresh, NPC release, event reset, sound children, party getters and
lower BattleInit retain their real provider boundaries. In particular, setting
animation mode1 alone cannot complete animPoseBattleInit: original scratch and
selected-pose refresh allocations are still required.

`BattleSequenceStorage::cold` is an explicit native guest allocation policy,
not recovered SDK heap startup or original numeric heap identity. The caller
supplies stable arena/pouch guest addresses and arena size. Storage owns the
primary arena, original map globals8041e808..e81b, pouch pointer8041eb00 and
BattleWork pointer8041eb90. Pouch bytes alias the actual
`Game.progress.originalPouch`; another pouch cannot be attached. Cold creation
initializes native primary-header state once; attaching/rebinding never clears
allocator history. Mapping failure rolls back this facade's partial aliases.
Reattachment verifies and reuses the complete original constant view already in
the VM; partial/conflicting bytes are rejected without reset or overwrite.
Owner survives facade retirement through BattleSequenceProcess. Stable Game/VM
and the same actual pouch owner remain required when reattaching.

The map child pre-admits ordered, mapped headers in that arena and sufficient
free space for the aligned payload AND required terminal32-byte header, plus
real platform flush/barrier callbacks. It then runs existing original
N_battleMapAlloc: publishes length64000, actual best-fit primary allocation,
split/zero/cache operations, fresh length reload and nested battle-header stores.
No fabricated payload pointer, successful allocation or separate copied B is
substituted. Existing eventMapAllocate/eventMapFree/freeEventBattleMapPool operate
on those same bytes for lower allocations and paired release. Callers own
retirement order: release all users and the nested pool before primary storage.
This implementation does not infer that an arbitrary arena size supports every
future battle resource. Corrupt, cyclic, overlapping or unsupported headers hold
before source allocator effects; preflight does not repair them.

The native ordinary-game bootstrap remains incomplete. It must connect the
persistent field VM, original tagged tik_01 actors, raw-name/setup4 and authored
battle_start before any legacy action snapshot. Shared storage admission and a
complete parent do not complete the animation/resource graph or lower BattleInit.
Never replay this initialization after legacy victory. See the
[request inbox](../../requests/EVT-01.md) and [progress](../../PROGRESS.md).
