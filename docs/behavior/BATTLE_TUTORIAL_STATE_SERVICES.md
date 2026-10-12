# State services used by the harbor tutorial phase event

US G8ME01 revision0,recovered2026-10-12. Evidence B:complete DOL bodies
GetUnitWork0x8010f828,size0x84;SetUnitWork0x8010f7a4,size0x84;
CheckPhase0x80113708,size0x70;get_turn0x801137b8,size0x34;
onoff_battleflag0x801137ec,size0x60. Callback names have btlevtcmd_ prefix.
They underpin [the original tutorial graph](HARBOR_BOSS_TUTORIAL_EVENTS.md).
Static behavior only;no original execution is claimed.

## Per-unit work read/write

Setter resolves actor operand0,translates it with BattleTransID(event,value),
resolves index operand1,then value operand2. Fetch the translated actor through
BattleGetUnitPtr(sharedBattle,id). Store value at unit+0x218+4*index;return2.
No local first-call gate,index/null check,clamp or float conversion occurs.

Getter resolves actor/translation,then index;capture output operand2 raw. Fetch
unit,read its word+0x218+4*index and write it through evtSetValue(event,rawOutput,
word). Return2. This is unit-owned state,not the script's local variable array.
The tutorial's indices0..3 are supported concrete callers;unchecked out-of-range
indices are not invented game mechanics. Lifetime and initial allocation clears
still require the unit-entry consumer.

## Turn and exact phase equality

get_turn writes sign-extended halfword(sharedBattle+0) through evtSetValue using
its raw output operand0;return2. It does not increment the turn.

CheckPhase loads output operand0 and comparison operand1 **raw**,then calls
BattleGetSeq(sharedBattle,4). Compare its returned32-bit word with the raw second
operand;write boolean1/0 through evtSetValue and return2. The comparison is exact
word equality,not a bit-mask test or evtGetValue resolution. Tutorial constants
0x04000001 and0x04000003 must remain distinct. Their scheduling/state producers
are part of the battle controller,not this callback.

## Battle flag update

onoff_battleflag resolves operation operand0 through evtGetValue,but takes mask
operand1 raw. When resolved operation==1,OR mask into sharedBattle+0xef4;
otherwise AND with complement(mask). Return2. It does not replace the entire
flag word or resolve mask variables. Tutorial init supplies(1,0x1000),setting
that bit. Its full UI/combat meaning requires actual flag consumers;do not name
it an action-command restriction solely from this scene.

## Two original REL feedback helpers

The boss's _mario_ac_success_check at REL file0xa564,size0x3c reads unsigned byte
sharedBattle+0x16f58. _mario_ac_failed_check at0xa528,size0x3c reads+0x16f59.
Each gets its raw output operand from event+0x18 and calls evtSetValue,then
returns2. Neither resolves an input unit,inspects the current move result,
clears that byte or applies an input window itself.

Their ADDR16_HA/LO relocations load shared pointer0x8041eb90;their REL24 call
relocations target evtSetValue0x80030cfc. Inspection-only ELFs use file offsets
as display addresses;raw unrelocated branch targets are annotated accordingly
and are not runtime VAs. Two full helper hashes and relocation identities pin
this interpretation. Counter writers/reset/overflow and whether values aggregate
multiple moves remain open. The script only tests each byte for nonzero.

Five new function fingerprints/24 anchors and original REL hashes/bindings
validate these local services. No executable/data linking or dialogue rewriting
occurs. Close state producers and original phase/input/output traces before
claiming complete tutorial or battle equivalence.
