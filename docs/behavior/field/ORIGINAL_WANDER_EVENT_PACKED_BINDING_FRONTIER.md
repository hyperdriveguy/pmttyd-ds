# Original wander event packed binding frontier

US G8ME01 revision0 static read-only DOL/REL evidence, 2026-10-14.
This supplements the [complete four-phase driver](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md)
with source request/publication boundaries needed by the
[retained setup world](NATIVE_REGISTERED_NPC_SETUP_WORLD.md).
It supplies no native implementation, runtime observation or hardware claim.

## Actual regular callers and event work ownership

Original loaded gor kuribo4_regl9052389c (REL43924/4c) executes
get_position(me,LW0,LW1,LW2), then
urouro_init_func(me,LW0,LW2,70.0,30.0,0), then urouro_main_func(me),
RETURN/END. Radius70 is encoded f24b9280; speed30 f24af280.
Generic gor_villager_urouro uses radius100; it is not the actual radius for
these two selected setup rows.

borodo1_regl905241a0 (REL44228/68) first SWITCHes story GSW0 operand
f5de0180. CASE_BETWEEN215,261 has no body; CASE_ETC executes the same
get_position/init/main sequence with radius70/speed30/flags0. The following
END_SWITCH/RETURN/END remain original commands. Preserve the original inclusive
range dispatch rather than invoking wandering unconditionally for Borodo1.

The 44-byte work pointer is **EventEntry+c4**, which already aliases LW10
(entry+9c+4*10). It is not userWork+154, actor storage, an opaque unmodeled
entry extension or a native map keyed only by actor. Retain its actual guest
pointer and memory aliases. Main reloads that pointer on every invocation.
Original event locals can be copied by constructors/copyback under their own
contracts; do not infer unique allocation ownership from this pointer.

Init calls _mapAlloc8002ff58 with heap root loaded from8041e808 and size44.
The [actual map allocator](../event/EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md)
rounds payload to64, zero-fills that payload and performs its cache requests.
The caller publishes returned pointer to entry+c4 even when0 and immediately
dereferences it; there is no local failure branch or old-pointer release.
Native unavailable allocation may hold before this publication, but must name
that adaptation. Do not substitute the distinct SDK event-entry allocator.
Entry deletion has no automatic map-buffer free. Map bulk-clear/release is a
separate [memory lifecycle](../memory/MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md).

## Init argument and publication order

Both functions ignore their first-call argument. Init80237878/15c captures
entry+18 argument-array pointer once, then performs these ordered requests:

1. Read argument0 and evtGetValue; immediately call evtNpcNameToPtr and
   retain actor A. Actor resolution precedes every remaining operand request.
2. Separately read/evtGetFloat argument1 X,2 Z,3 radius,4 speed; retain each.
   Then read/evtGetValue argument5 flags. Reads are interleaved with getter
   calls, so a getter can change a later raw operand before its read.
3. Load heap root8041e808; request allocation44; publish result to entry+c4;
   reload entry+c4 into retained work pointer W.
4. Store W+0 flags, W+4 phase0, W+8 count0, W+c flips0, in that order.
5. Request sysMsec2Frame(6000), store result W+10; request9000, store W+14;
   request1500, store W+18. Each request reads fresh GP frame rate.
6. Store retained X to W+1c; read fresh actor A+90 Y; store it W+20.
   Store retained Z W+24, radius W+28, speed A+1b0.
7. Read fresh A+0, OR00400000, store A+0. Return2.

Thus initializeNpcWander's native semantic work={} and actor snapshot are
useful pure mechanics under stable inputs, but do not expose this actual
allocation/entry-alias/operand/read-store protocol. The actor Y read is after
the three frame-rate providers, not at initial name resolution.
The [name resolver](ORIGINAL_NPC_FLAG_POSITION_YAW_EVENT_LEAVES.md) special
tokens and unavailable sentinel domains apply unchanged.

## Main request and mutation boundaries

Main802372a8/5d0 re-reads entry+18 and argument0, calls evtGetValue and
evtNpcNameToPtr, retains actor A, calls marioGetPtr8005e0c0, then reloads
entry+c4 to W and reads W+4 phase. The Mario pointer and W survive the selected
phase; a later provider must not substitute another actor/Mario/work pointer.
Every normal local exit returns0. This is genuine original continuing work,
distinct from an unavailable native service that must resume within one visit.

The existing four-phase contract describes branches and formulas. Additional
observable boundaries for a packed adapter:

- Phases0/2/3 read their budget to form half+1 **before** irand; they read that
  budget again after irand to form half+result. Do not cache it across RNG.
  Phase0 stores initial count, reloads count, then stores its multiple20.
  Phase0 clears flips before its separate irand60. Phase2 clears flips after
  yaw/heading stores and only then reads work flag10000 for optional irand1000.
- Both random-yaw phases load fresh actor yaw after irand60. They publish
  A+144 yaw, read it back fresh, then publish A+1ac movement heading. The
  anchor turns and Mario-contact turn likewise read back published yaw.
- Phase1 retains its first distance result but makes four separate paper-end
  requests at the reached gates. Each request reloads A+104 pose handle.
  Source Mario-touch service is requested only after the second ready result;
  Mario travel/root values are fresh reads within those conditional tests.
- Phase1 decrements/stores W+8, reloads it for the exact-zero gate and may
  publish phase2 before any movement/sound. Earlier movement components survive.
- Speed/dt cap and displacement follow the existing source SP formulas.
  For X, load movement heading, compute radians, request sine, round and store
  A+1b4. Then **reload movement heading** for a separate radians computation
  before cosine and A+1bc publication. A mutating sine provider can therefore
  change the subsequent cosine input.
- Only after both component stores, call psndGetFlag800dc0e8. Request
  animPoseGetMaterialFlag8004069c (fresh pose handle) only if sound flag100 set.
  Then load unsigned A+1e8 period, fresh A+1b0 speed and A+1dc elapsed for
  threshold testing. These reads do not precede the sound/material providers.
- On threshold entry read signed A+1d8 toggle, choose the corresponding fresh
  A+1e0/+1e4 sound pointer, and for nonnull name call searchPSSFXList800dc650,
  then psndSFXOnEx_3D800d9808 with root pointer A+8c and255,255,0,8.
  Only afterward reset A+1dc elapsed0, reload toggle, apply byte-sign-extended
  1-toggle and store halfword A+1d8. Null name still reaches reset/toggle.
- The final elapsed gate reloads A+1e4 and, when null, signed A+1d8. When
  reached it reloads actor dt A+1a4 and elapsed A+1dc before SP fused
  multiply-add1000*dt+elapsed. It must observe completed audio-provider effects.

An adapter may admit stable synchronous providers for a whole visit, or use
an explicit continuation through these requests/publications. A false provider
must not repeat an already consumed RNG draw, allocation, audio submission,
count decrement, flag mutation or component store. Whole semantic actor/work
snapshot writeback is insufficient when public services can mutate aliases.

## Required owners and native reuse boundary

The actual registry binding can supply bounded original actors/strings and
actor reads/stores; the same actual VM supplies checked operand routes,
entry/LW10 aliases and map-heap memory. The shared-session RNG must be the
same owner used by numeric NPC setup, never a fresh per-actor seed. Clock
budget conversion uses live GP+4; callback frequency remains source scheduler
visits, not render frames or an unconditional60Hz counter.

The retained FieldPosePool/BattlePosePool has actual per-handle scalar state:
paper readiness uses pose flags+0, retained+74 and target+7c. The material
flag producer and normal facing Main must remain actual services; an active
CPU pose or animation clip-end does not prove paper readiness. Actual Mario
owner must supply Mario pointer, current root, travel heading and the previous
NPC scan's retained touch actor. No current-frame geometric touch guess.

System angleABf/distABf/compAngle/reviseAngle, sine/cosine and instruction
rounding remain explicit numerical services. Existing npc_wander helpers and
field native movement/sound adapters are bounded semantic implementations,
not proof of Gekko/libm numeric parity or complete source alias ordering.
Original audio requires sound-name lookup, global sound flag/material state
and original spatial request routing; a generic native footstep is insufficient.

Most critically these event bodies produce components only. Binding them
does not supply actual npcMain dt/history/component consumption, map-side/
gravity, automatic pose, candidate stop/start, paper facing or rendering.
Existing native catalog's Goomba wander/render state is separate from the
new retained actor registry. Bind the real owners rather than execute a second
parallel simulation and copy approximate positions between them.

## Actual regular scheduling, pause and replacement

Setup stores the new regular ID at actor+11c after creating an independent
evtEntry(script,0,20). The actual constructor stores flags21, typeef, current
global event speed, credit0 and byte10=1 supplied by setup. Flag20 enables
append to an already active manager schedule; it is not an instruction to
execute immediately or a persistent every-render-frame tick. The
[constructor](../event/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) also performs
actual global system-level run checking, which may stop the root.
The [manager](../event/EVENT_MANAGER_COMPLETE_LOOP.md) dispatches only the
matching live ID with active1, flags92 clear and sufficient accumulated credit.
Thus observed creation of two roots proves neither wrapper execution nor
allocation, initialization, continuing main polls or movement.

Actual regular wrappers first get XYZ and init; init return2 can reach main
in the same admitted dispatch. Main return0 retains that USER_FUNC and work;
the wrapper's trailing RETURN/END is not reached by ordinary continuing main.
Do not call init again on every poll. On a new source main visit, operand/name/
Mario/work resolution runs again, while an unavailable provider continuation
within one native visit must preserve already completed effects.

Candidate admission occurs later than npcMain's movement/automatic-pose/history
work, and stops regular+11c with actual StopID before A. It does not delete the
root, free/reset LW10, clear driver phase/count, undo that tick's components or
pause NPC dt/pose. Candidate loss with mainflag00400000 restores saved yaw+148
to+144 and calls StartID on the same ID/work; without00400000 it instead posts
actor+140 trigger10000000. Skipped query/candidate gates need not perform
candidate-loss cleanup. Talk stage0 stops regular again; completion resumes
only under the original candidate-state gate. See the
[complete candidate/interrupt contract](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md)
and the existing driver contract's paused-clock follow-up.

npcCheckInterrupt800440f8, called at8004acc8 before the later candidate scan,
consumes actor+140 trigger10000000 on the next admitted pass. With nonnull
actor+124 script, this creates a **new** regular root, then deletes the old
ID, installs the new ID+11c and resets its separately listed NPC work/speed.
It does not StartID on old work or restore yaw. The replacement constructor
clears locals, so a later executed init allocates/publishes new LW10 work.
No per-entry map-free is implied; room map-heap lifecycle remains required.
Other prioritized interrupt bits can replace regular with other scripts under
their own gates; do not unconditionally restart wandering from npcMain.

## Evidence

Fresh ignored artifacts/npc-wander-bridge-evidence/init.elf/init.txt and
main.elf/main.txt were inspected with tools/inspect_dol_function.py using
artifacts/reference/us_symbols.csv and the read-only ISO. Exact original
caller listings/hashes are in callers.json from tools/rel_event_inventory.py.
No original assets/imports, code, build, profile or save changes were made.

Original DOL body SHA256: init80237878/15c
e7c3b97afbc2491f6e10e0c5f9d15e5ebe1967aefa80bc28c030703825d3d6f8;
main802372a8/5d0
e9e96d828393a9aaaca60e6825701c169c0d0ba7fd05a6cd455f95fccc31f8c2.

Caller original-span SHA256: kuribo4_regl
054b549e779cd6a9c7bbf52f8b2d5a1e41d2fe053a726eda4e0206231080fd34;
borodo1_regl2eb25ad89e667ed27f75af8d0e4a62e8c7125bfd4f7d7318df547c5322fe2938;
generic23c48a8c42386fe13eac63d26820766f899b1fd9be292b59f6870a12da2ab3d3.
