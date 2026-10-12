# Battle input update order, snapshots and event sampling boundaries

US G8ME01 disc0 revision0, documentation agent ID1, recovered2026-10-12.
Evidence B: complete local battleMain (0x800f904c,size0x48), BattleMain
(0x800f8e28,size0xdc), BattlePadManager (0x8011a43c,size0x50) and the previously
inspected BattleActionCommandManager (0x800fa5b0,size0x7c). Also checked the
main-pass prefix in marioStMain, snapshot writes in BtlPad_WorkUpdate and event
execution-credit loop in evtmgrMain. Fingerprinting those containing bodies does
not constitute full main/sequence/hardware recovery. No original execution or
measured impact tick is claimed.

The [physical producer](PHYSICAL_INPUT_PRODUCER.md) closes its local raw-read,
edge/repeat and GP publication contract. The [pad/defense document](BATTLE_INPUT_AND_DEFENCE.md)
retains the local history shifts,virtual thresholds,repeats,defense windows and
reset policy. This document connects their caller order without turning the
native harness's scheduling into original evidence.

## Original main-pass prefix

marioStMain at0x8006f904 begins by testing GP word+0x10. While nonzero it calls
the yield service at0x8029ccf4 and retests. Once zero,call makeKey at0x8006f928.
The important subsequent direct-call sites are:

| Site | Call and consequence within this function |
| --- | --- |
| 0x8006f928 | makeKey:raw read/copy,GP edges,repeats and analog publication |
| 0x8006f92c | seqMain:sequence transitions/current main |
| 0x8006f930..0x8006f93c | cardMain,bgMain,shadowMain,animMain |
| 0x8006f940 | battleMain:general/private battle input and battle subsystems |
| 0x8006f944..0x8006f950 | camMain,effMain,dispMain,winMain |
| 0x8006f954 | evtmgrMain:event eligibility/credits and script callbacks |

The full existing ordered list is in [the original reference](../ORIGINAL_BEHAVIOR.md).
This is a function-pass order,not proof of one invocation per rendering frame,
uninterrupted hardware polling or complete pause semantics. Earlier sequence and
intermediate service calls can mutate state;their complete effects must not be
assumed absent. marioStMain's later suffix is not newly specified here.

## Distinct battle wrapper and engine update

The lowercase **battleMain** captures shared BattleWork pointer W. Null skips all
work. Otherwise call uppercase **BattleMain()**. If it returns0,OR0x80000000 into
the captured W's flags word+0xef4. A nonzero return leaves that word unchanged on
this wrapper path. It does not clear a previously set bit or locally call BattleEnd.
The lowercase function itself is a wrapper,not the pad updater.

Uppercase BattleMain captures shared W and makes these requests in exact order:

| Order / site | Function request |
| --- | --- |
| 1 / 0x800f8e40 | BattleCheckUnitBroken(W),0x800f7b28 |
| 2 / 0x800f8e44 | BattlePadManager(),0x8011a43c |
| 3 / 0x800f8e48 | BattleSequenceManager(),0x8011f1ec |
| 4 / 0x800f8e50 | BattleActionCommandManager(W),0x800fa5b0 |
| 5 / 0x800f8e54 | BattleStageObjectMain(),0x80149798 |
| 6 / 0x800f8e58 | BattleIconMain(),0x801aa168 |
| 7 / 0x800f8e5c | BattleStageMain(),0x80139150 |
| 8 / 0x800f8e60 | BattleAcHelpMain(),0x802168a4 |
| 9 / 0x800f8e64 | BattleAudience_Main(),0x801a836c |
| 10 / 0x800f8e68 | BattleBreakSlot_Main(),0x80203aa0 |
| 11 / 0x800f8e6c | btlDispMain(),0x801024f0 |

No local pause/status/command-active gate precedes these calls. Many callees have
their own gates;this ordering does not close them. General pad manager reloads
the shared pointer on each channel iteration,whereas the private manager receives
the captured W. The sequential game normally relies on stable ownership,but the
local bodies do not validate or enforce pointer stability through these services.

After all eleven calls,read BattleGetSeq(W,0). Exact4 returns0. Otherwise read GP
word+0. If both masks0x1000 and0x2000 are set,read BattleGetSeq(W,0) **again**;
if that second result is2,OR0x60 into W flags+0xef4,request BattleSetSeq(W,0,3),
then BattleSetSeq(W,7,0x07000000). Return1. All remaining routes also return1.
Do not rename these GP bits as complete pause/escape policy without writer and
caller evidence. Their test occurs **after** pad/AC/stage/audience/display requests.
Moving it to an early whole-battle freeze would change the local call behavior.

## Four general histories and one private history

BattlePadManager iterates channels0..3. For each,reload shared W,pass
`W+0xf2c+0x1fc*c` and channel c to BtlPad_WorkUpdate at0x8011a0ac. Increment
channel and pad offset after the update. There is no local null check, input
availability test or skip for unconnected channels. Each invoked updater performs
its own three history shifts. Channel0 new-trigger history begins at W+0xf44;
its current/held history begins at W+0x1034. Channel selection is independent of
unit IDs or active partner identity.

BattleActionCommandManager(W) always first calls BtlPad_WorkUpdate(W+0x1d1c,0).
This private pad's new-trigger history begins at W+0x1d34. It updates even if no
main action-command callback is installed. Thus inactivity of an attack command
does not locally disable defense input sampling. Defense reads this private
history;ordinary TimingA and Hammer readers documented earlier use general pad
history/current masks. The buffers must not be merged merely because both sample
physical channel0 in this path.

After that private update,read main callback W+0x1ca0. Null skips the rest of the
manager. Nonnull calls it with W. A return0 clears W+0x1ca0;any nonzero result
leaves it. Then read display callback W+0x1ca8. If nonnull,queue dispEntry(camera8,
layer1,depth900.0,displayCallback,W) at0x800fa614. This display request can still
occur on the call that clears the main callback. Conversely,an initially null
main callback skips display even when a display pointer exists. Callback changes,
registration and actual draw execution remain separate service lifetimes.

The sequence controller runs **between** general and private updates. It can
initialize commands or mutate state. It must not be moved after both updates
without establishing that its observable effects commute. Original per-callee
resets/status gates and command setup paths remain dependencies.

## Complete write map of the12-byte battle snapshot

BtlPad_WorkUpdate copies its previous12-byte current snapshot into the prior
snapshot first,then performs the history shifts. For requested channel c,it
reads GP and writes current fields as follows:

| Pad-relative bytes | Original incoming source/write |
| --- | --- |
| +0..+1 | Low16 bits of GP word+0x1328+4*c |
| +2 | GP byte+0x13b8+c, main X |
| +3 | GP byte+0x13bc+c, main Y |
| +4 | GP byte+0x13c0+c, C X |
| +5 | GP byte+0x13c4+c, C Y |
| +6 | GP byte+0x13c8+c, left trigger |
| +7 | GP byte+0x13cc+c, right trigger |
| +8,+9 | Explicit zero writes (raw analog A/B are not copied) |
| +0xa,+0xb | No new write in this updater;retain previous contents |

Axis interpretation is signed where virtual directions compare them; trigger
bytes are retained bit-for-bit. The12-byte stride does not imply12 newly supplied
bytes or a freshly copied hardware error field. WorkInit explicitly clears bytea but PRESERVES byteb in each snapshot.
Fresh object/allocation zeroing is separate from either local body. Do not invent
error metadata for them or discard retained values during every update.

Physical new/repeat/released masks come from GP+0x1338/+0x1348/+0x1368+4*c;
these were already produced by makeKey. Virtual edges/repeats use this pad's own
prior combined mask and repeat state,as specified in the local pad document.
Both general channel0 and private pad receive the same GP source on the ordinary
stable-input pass,but their prior snapshots/history/reset state can differ.

## What this establishes at a script defense call

On the direct main chain,makeKey publishes GP before both battle-pad updates;
general pad updates precede sequence control;private update precedes the AC main
callback;the later evtmgrMain can resume the Crump script and reach
ResultACDefence. Its private history index0 therefore refers to that latest
private **pad update**,including the current GP press edge,provided intermediate
services have not reset/overwritten the pad. This proves an inclusion boundary
within the inspected path,not an exact hardware poll-to-impact latency.

[Crump's attack](HARBOR_BOSS_NORMAL_ATTACK.md) calls defense only after precheck1.
[JumpPosition](BATTLE_JUMP_EVENT_CALLBACKS.md) yields on ordinary landing before
the next script command. A yielded callback does not force another physical pad
sample if execution credits permit another interpreter invocation in the same
manager pass. evtmgrMain adds event timescale to credit and repeats evtmgrCmd while
credit>=1,subtracting1 each time. Interpreter return0 after callback result1
permits that loop to continue;interpreter-1 stops the event;interpreter1 stops the
manager pass. Ordinary timescale1 supplies a different opportunity from a larger
rate or accumulated credits. The timescale writers and pause/eligibility policy
still need actual event/caller closure.

There is no second physical/general/private update inside this evtmgrMain credit
loop on the inspected direct chain. Several script callbacks in that pass can
therefore read the same latest sample. Creating a fresh synthetic input sample
for every event invocation changes the original ownership boundary.

Every defense-check return locally WorkInit-resets the private pad,as documented
in [defense](BATTLE_INPUT_AND_DEFENCE.md). A second hit in the same event pass
must account for that reset;the main-chain sampler has not run again. A later
main pass can sample the GP producer anew. Other callers/resets and multihit event
branches must still be recovered before establishing all repeated-hit outcomes.

## Scoped implementation requirements and remaining gaps

| ID | Requirement | Derived acceptance boundary |
| --- | --- | --- |
| PAD-01 | Keep physical producer,general channels and private pad as separately owned state | Inactive AC main still receives a private update;private WorkInit cannot erase general history |
| PAD-02 | Preserve general→sequence→private→AC callback ordering and later event dispatch | A press published by makeKey is present in both channel0 updates before a later eligible event check,unless an explicit intervening reset applies |
| PAD-03 | Retain the12-byte snapshot write map | Updates zero bytes8/9,retain bytes10/11,and copy trigger bytes independently of signed axis tests |
| PAD-04 | Keep global scaled24/6 repeats distinct from virtual16/8 repeats | A mixed physical/stick change follows each producer's own mask/countdown rules |
| PAD-05 | Separate event invocation credits from pad-update count | Two permitted event invocations in one manager pass share the latest sample;do not shift pad history twice solely because a movement callback yields |
| PAD-06 | Bind defense-reset and multihit ownership explicitly | After one defense reset,a same-pass next check sees the reset private state until a genuine update/caller writer occurs |

These are implementable ordering/storage requirements,not a complete battle or
full-game requirements document. Original comparisons must record makeKey and
pad-update identities,GP/masks,full private/general histories,command registration,
sequence phase,event timescale/credits,callback status,precheck and defense calls.
Pause/unpause,single/mash/held/A+B,early/late and multihit cases remain unobserved.
Other direct/indirect BtlPad_WorkUpdate and reset callers,physical SDK behavior,
main-pass frequency,override writers and every actual pause producer remain open.

The [dedicated verifier](../../tools/verify_input_cadence_contracts.py) checks ten
containing-body hashes,64 selected instructions,28 direct calls and three data
bindings. Four hashes repeat previously covered helpers and are not new semantic
completion units. seq_battleMain was inspected as a discovery aid;its larger
lifecycle is not covered by the local cadence contract.


## Sampler/reset and control follow-up

[Input reset owners](BATTLE_INPUT_RESET_OWNERS.md) now supplies the complete
static direct sampler/WorkInit census and a shipped tutorial helper that can
inject private trigger history between main-chain samples. Preserve that explicit
writer rather than deriving every press solely from physical held transitions.
[System-level/event speed](EVENT_SYSTEM_LEVEL_AND_SPEED.md) supplies the local
requested-level/type-mask table and speed initialization;neither closes all
pause/UI callers or original multihit cadence.


[Complete WorkInit byte map](BATTLE_PAD_RESET_AND_RAW_PRESERVATION.md) now pins
the two snapshot loops,interleaved90-word clear,repeat-byte reset and all124
preserved bytes,including raw tail. INPUT-01's byte clarification is answered;
SDK/physical/caller/original sampling domains remain separate.
