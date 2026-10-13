# Complete battle unit setup:startup actors and repeated snapshot imports

Complete static US G8ME01 Btl_UnitSetup800f8aac/0x2f8.
[Opponent/equipment tail](BATTLE_UNIT_SETUP_OPPONENT_LOOP_AND_EQUIPMENT_MASK.md)
completes the same body.Native request:
[EVT-01](../../requests/EVT-01.md).
[Verifier](../../../tools/verify_battle_unit_setup.py) pins the full190 instructions,
seven linked service identities,and five source spans.No original execution,
complete source setup/end/query-domain or native Game claim.

## Capture and startup partial writes

Retain passed battle B.Load S=WORD(B+0x2738),retain D=WORD(S+0x0c),then
G=WORD(D+0x18).These pointers are captured BEFORE pouchGetPtr(),whose
returned P is retained through the final equipment scan.No initial null guards.
Perform these direct stores in order before the first unit-entry request:

- HALFWORD(B+8)=2,HALFWORD(B+0x10)=1,HALFWORD(B+0x18)=0.
- BYTE(B+0x0a)=1,BYTE(B+0x12)=0xff,BYTE(B+0x1a)=1.
- WORD(B+0x0c)=0,WORD(B+0x14)=0,WORD(B+0x1c)=0.

No whole-record clear:interstitial bytes remain untouched by this caller.
Do not attach guessed affiliation/status names to these literal setup fields.

## Two fixed entries and explicit Mario reimport

Request BtlUnit_Entry(0x80416a80),then BtlUnit_EquipItem(returnedU,3,0).
No returned-unit check and no retained pointer for this first actor beyond the
immediate service call.The original first setup points to definition803ade74.

Then request BtlUnit_Entry(0x80416a50),retain U;request EquipItem(U,3,0),
then SetParamFromPouch(retainedU) at800f8b38.The second fixed setup points
to80372afc,unitdata_Mario.Startup r13=8041cf20 gives these addresses via
signed displacements-25760/-25808,respectively.All96 bytes of the adjacent
48-byte setups are pinned rather than generated substitutes.

[Complete entry](BATTLE_UNIT_ENTRY_COMPLETE.md) already calls
SetParamFromPouch in its own body.This explicit post-equipment import is an
ADDITIONAL request,not duplicate evidence to eliminate.Fresh caller/service
owners and equipment changes can affect its result.No import for the first
fixed actor is requested directly by this caller after its equipment call.

## Optional initial party:nonzero is the only kind admission

AFTER fixed entry/equipment/import services,reload CURRENT WORD(B+0x2738),
then WORD(S+4) as party kind K.Exactly0 skips the party branch.EVERY nonzero
word derives wrapped source address80363790+48*(K-224),with no signed
224..230 guard.That differs from complete BattleChangeParty's range check.
Do not replace malformed/out-of-domain kinds with a default party.

Request BtlUnit_Entry(thatSetup),retain U.No null guard.Request
GetHomePos(U,&SP10,&SP0c,&SP08),then SetHomePos(U,-140.0,privateY,5.0).
Reload constants/privateY and request SetPos(U,-140.0,privateY,5.0) separately.
The original binary32 constants are80422178=-140.0 and8042217c=5.0,from
startup r2=80427440 offsets-21192/-21188.Use those values,not a provisional
native battle layout.PrivateY is the home getter result,retained across services.

Request EquipItem(retainedU,5,0),then SetParamFromPouch(U) at800f8bb4.
No party-table/menu construction,pose/event wait,or selected-party field write
occurs in this branch's own instructions.Entry already performed its own import;
this caller repeats it AFTER both position setters/equipment request.

The seven original party setup records are pinned at80363790,linking valid
kinds to the [switch source table](PARTY_SWITCH_SPAWN_HOME_AND_SNAPSHOT_EXPORT.md)
and [all key1 callbacks](PARTY_WEAPON_CALLBACKS_COMPLETE_SHARED_PROTOCOL.md).
Static definitions do not prove global actor/table readiness in an actual battle.

## Captured versus fresh owners and implementation requirements

D/G and P from entry remain retained for opponent/equipment stages;initial
party K uses a fresh setup root after actor services.Unit entry/import helpers
also capture their own global owners.Preserve these differences;do not flatten
setup to one immutable structure or cache every import under passedB.

Implement exact startup partial stores,source-address requests,unchecked
results,additional import calls,nonzero-only party admission,homeY retention,
and original coordinate setter order.Reuse original definitions/data/control
streams.[Import/export contract](../unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md)
specifies raw30-byte snapshot/gap/typed resource behavior.Actors/source/event
readiness,teardown import paths,query domains,Game/platform/original runtime/
full-game requirements remain open.
