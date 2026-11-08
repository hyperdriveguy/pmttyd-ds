# Retained battle unit setup

`resumePackedBattleUnitSetup` owns the complete US `Btl_UnitSetup800f8aac`
parent. The [startup actor contract](BATTLE_UNIT_SETUP_ACTORS_AND_REPEATED_IMPORTS.md)
and [opponent/equipment contract](BATTLE_UNIT_SETUP_OPPONENT_LOOP_AND_EQUIPMENT_MASK.md)
define the source operations. `tools/verify_battle_unit_setup.py` pins the whole
body and linked child identities.

The cursor captures the original NPC/group pointers before the pouch getter,
retains its returned pouch, and performs the nine partial alliance stores once.
System and Mario construction, equipment and Mario's additional pouch import
have independent held child requests. The optional partner uses the fresh
information pointer and every nonzero kind; malformed kinds are not normalized.
Home Y belongs to the real getter's private native output. Separate position
setters reread their original constants before each request, preserving that
request across a hold.

Opponent construction retains its definition pointer while rereading the live
group count on each loop test. The signed narrowed ordinal selects the override
array; the full parallel offset selects fallback item and metadata arrays.
Fallback storage and equipment arguments use distinct source reads. Overrides
skip equipment, while fallback requests retain their exact argument across a
hold. The resource copies occur after those children. The final scan reads 200
signed equipped halfwords from the captured pouch. Case314 performs no store;
the nonmonotonic source bit mapping is explicit.

The stable service registration and lifetime cannot change across a hold.
Reentry/reset during invocation rejects. Retiring a registration prevents later
resumption until explicit reset. Parent memory services are admitted synchronous
guest accesses; children own any partial constructor/equipment/import effects.
True means the original local return1, rather than readiness of an actor's init
event or completion of a battle.

Focused tests compare the synchronous parent for optional/absent partners and
negative/zero/positive group counts. The synchronous helper's case314 is also
corrected to perform no write, as the original branch requires. Independent
assertions count the exact mask stores, including case314's no-write behavior. Tests hold every reached child twice, check the
full call/write history, retain private home Y, exercise live count growth and
nonzero malformed party selection, and reject reentry and retired lifetimes.
Native UnitEntry/display/equipment providers remain independently required.
