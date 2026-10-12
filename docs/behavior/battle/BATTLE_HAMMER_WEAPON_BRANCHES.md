# Ordinary Hammer weapon selection and difficulty branches

US G8ME01 revision0,recovered2026-10-12. Evidence B:previously hashed original
Hammer-core stream0x8035d6d4,size0x14d4;three weapon records;two wrapper scripts;
complete GetWeaponActionLv0x801058e0,size0x48,AcSetDifficulty0x80107b38,size0x80,
check_battleflag0x8011384c,size0x50. Branch interpretation is supported by
[original integer IF/switch behavior](../event/EVENT_INTEGER_BRANCHES.md),not public
symbol names alone. No original execution or full attack/tutorial closure claimed.

## Branch variables and mask2 gate

The relevant raw event operands are0xfe363c80 (local slot0),0xfe363c8c (slot12,
weapon pointer),0xfe363c83/84 (target unit/part slots) and0xfe363c8e (slot14).
These retain their original encodings;portable event variables must resolve the
same namespaces rather than treating those words as addresses.

check_battleflag(event) reads destination operand0 raw and mask operand1 raw.
Test shared BattleWork+0xef4 AND mask;write boolean1/0 with evtSetValue and
return event2. There is no evtGetValue for the mask. The core uses mask2 at
0x8035d6d4,0x8035d884 and0x8035d9b0. Repeated queries are separate reads of
current battle state,not a single cached flag.

At entry,when mask2 is set,the original IF_NOT_EQUAL block installs default
Hammer pointer0x80353de8 into slot12,asks GetFirstAttackTarget for target/part,
and may jump to label99 when target==-1. It requests camera mode7,homing/zoom/
speed,home/facing/position and waits30 event frames. When mask2 is clear,the
ELSE block obtains the selected command weapon/enemy,has the corresponding
no-target exit,declares the attack,waits guard movement,pays weapon cost,runs
actor data event7 and a power-up child,and requests camera mode11/homing/zoom.
These are script requests;their callback bodies and exact first-strike gates
still need full recovery. Describe mask2 as the **GetFirstAttackTarget branch**
until battle-state writers establish its complete meaning/lifetime.

Later,mask2 clear performs the setup immediately in the parent stream under
IF_EQUAL at0x8035d894. Mask2 set takes a separate movement block under
IF_NOT_EQUAL at0x8035d9c0 and places the same setup policy inside the inline
region beginning0x8035db28,ending0x8035dc40. That region waits an event-derived
movement-frame operand before setup. Its actor motion/coroutine scheduling is
not established here;do not turn textual address order into a measured start tick.

## Pointer switch maps charge periods

Both setup policies read weapon action level into slot0,set actor difficulty,
then SWITCH the resolved slot12 weapon pointer. CASE_EQUAL resolves each literal
pointer and matches identity;the matched case prevents later cases/default from
running. No switch break is needed between these original cases.

| Weapon pointer / symbol annotation | Charge period T | Case sites | Parameter sites |
| --- | --- | --- | --- |
| 0x80353de8 marioDefaultWeapon_Hammer | 25 | 0x8035d8c8 /0x8035db5c | 0x8035d8d0 /0x8035db64 |
| 0x80354da8 badgeWeapon_ConfuseHammer | 30 | 0x8035d8f8 /0x8035db8c | 0x8035d900 /0x8035db94 |
| 0x80354ce8 badgeWeapon_TsuranukiNaguri | 30 | 0x8035d928 /0x8035dbbc | 0x8035d930 /0x8035dbc4 |
| Other weapon reaching this core | 40 | Default0x8035d958 /0x8035dbec | 0x8035d95c /0x8035dbf0 |

Symbols are research annotations,not asserted US menu display names. Every
parameter variant supplies N4,three following zero values and three preserve
sentinels. Each switch then sets raw mode0 and SetupAC(-2,3,1,0). The two
policies select the same command and data rules but occur in different event
contexts. See [callback stores](BATTLE_ACTION_COMMAND_CALLBACKS.md) and
[charge/release rule](BATTLE_HAMMER_STICK_KEEP_LEFT.md).

The default branch is relevant only for weapons that actually enter this core.
Do not assign period40 to every other weapon in the game;many have different
attack events/commands. Nor should semantic similarity or item ID substitute for
pointer identity when an event deliberately uses another weapon object.

## Base difficulty and weapon event links

GetWeaponActionLv resolves operand0 through evtGetValue,loads **unsigned byte**
weapon+0x6e,then writes that value to raw destination operand1 via evtSetValue.
Return2. Null/malformed weapon pointers have no local fallback.

AcSetDifficulty resolves actor operand0 then base operand1,in that order;then
BattleTransID(event,actor),BattleGetUnitPtr(battle,translatedID),and
BattleActionCommandSetDifficulty(battle,unit,base). Return2. The existing setter
stores low8(base) at+0x1cbc,but adjusts the full incoming integer by subtracting
unit byte+0x305 and adding byte+0x306,clamps0..6,and stores+0x1cbd. The helper
window subsequently uses the adjusted difficulty. Do not sign-extend weapon+0x6e
or use its menu order as difficulty.

All three pointer-case weapon records above have original+0x6e byte3. With no
modifier bytes,the command therefore uses difficulty3,whose release allowance
is40 calls. The default Hammer's period25 caps its ordinary eligible target-stage
release checks to25 despite allowance40;the two named cases cap to30. These
are derived local invocation counts,not video frames or measured impact windows.
Modifiers can reduce the allowance;see the seven-entry original table.

Each weapon's+0xb0 event pointer is independently checked:

| Weapon | Script pointer |
| --- | --- |
| Default Hammer / ConfuseHammer | 0x80416c40 marioAttackEvent_NormalHammer |
| TsuranukiNaguri | 0x80416c94 marioAttackEvent_TsuranukiHammer |

The first28-byte wrapper assigns slot14=0;the second assigns slot14=1. Both
then request child execution of core0x8035d6d4,followed by explicit event-delete
opcode2 and end marker1. They are four-command streams with retained raw words
and hashes in the ignored wrapper inventory. This identifies wrapper→core data
links;the downstream meaning of slot14 and command-menu weapon-selection
consumer still need recovery. Do not call the entire weapon attack equivalent
from these pointers alone.

## Coverage boundary

Six new function fingerprints and40 instruction anchors cover these wrappers
and the used integer branch helpers/inline dispatcher regions. Independent
checks pin eight VM targets,actual conditional/case operands,weapon difficulty
bytes/event links and both wrapper streams. Full Hammer core hash was already
checked;original executable/data/reports remain ignored.

Next:remaining parent/inline waits,hit precheck/damage/result/stylish/stop branches,
menu→weapon/unit binding,flag/automatic/forced-result writers and original
input/animation/state traces. Bind Crump's tutorial separately. Exact original
US story conditions/dialogue remain mandatory;this improves a real ordinary
mechanic dependency without closing full combat/story/game requirements.
