# Native Mario key and background preparation

Source: [key leaves and next ROAD frontier](BERO_KEY_LEAVES_FRESH_ACTOR_OWNERS_AND_NEXT_ROAD_FRONTIER.md)
and [Bero background/camera contract](../overworld/BERO_INFO_PREFIX_ASYNC_GROUPS_SYSTEM_FLAGS_AND_ACTOR_PREPARATION.md).

FieldMarioPreparationWorld supplies explicit callback admission plus existing
FieldPositionWorld storage and byte/halfword stores. Admission establishes every
addressed owner before source side effects; absent or refused admission holds.
The domain is per callback, so admitting keys never promises dimensions, actor
form or camera ownership. Services borrow live owners only for the current call.
FieldAreaEvent binds key_onoff800e7474, bgmode_off800e73ec, bgmode_on800e7410 and
cam_y_off800e7314 through that boundary. No world-wide success fallback is added.

The key wrapper captures args, performs its actual unused Mario lookup before
raw operand/resolution, then the leaf obtains Mario afresh. KeyOff captures one
actor, reads/stores main8, reads/stores wrapping U8+39, rereads signed result.
KeyOn captures one actor, reads/decrements/stores wrapping U8+39, rereads and
clears main8 only for zero, then rereads signed result again. A resolver may
replace current Mario before the leaf; later writes cannot replace that leaf's
captured actor. Actual party KeyOn/Off are source return-only bodies. No clamps,
boolean counter substitution or automatic cancellation unlock is introduced.

Background on/off first mutates main02000000 on the captured actor, then obtains
Mario afresh for signed form+3c and the source dimension/movement table. Form1 writes
1b8/1bc/184/188 as20/47/1/2; form2 writes only184/188 as1/2.25, preserving prior
dimensions. Other forms select20/37 or10/25.899999618530273 from main04000000,
then1/2.25. All stores use exact original binary32 words. The follow helper gets
camera4, reads/writes its actual U16+0 mask10, then gets Mario afresh and
reads/writes display+4 mask20000. Background on disables follow; off enables it.
Standalone cam_y_off performs the same disable helper, without main/dimension
writes. No camera reset, view/target blend or timing changes are implied.

Game::fieldMarioPreparationWorld admits only key callbacks. It uses the existing
fieldMarioBattle main/keyDepth owner; fieldPositionWorld gains only the existing
main-word view. Writes maintain the native fieldKeysOff projection and existing
acquisition/reward locks. Item acquisition's KeyOff/On now delegates to these same
memory-order leaves instead of repeating its own flag/counter arithmetic. No
Bero-only key state is created. Existing unrelated main-word views are outside
the newly admitted key domain; background cannot use this partial adapter.

Tests exercise every256 starting counter byte for both directions, signed wrap,
exact wrapper/leaf lookup order, resolver actor replacement, retained leaf owner,
post-store counter reread, every background form/small-flag combination, untouched
form2 dimensions, camera/display stores and fresh actor after the main write.
A real Game test interleaves item and direct key requests, proving shared nested
ownership; native background admission is explicitly refused. An unchanged
original gor02 parent/attached Bero child with a fully supplied synthetic actor
image advances through key/background to actual set_prev_party800e3058. Its
zeroed GP/actor fields are explicit fixtures, not certified Game/save state.

The native prefix probe uses an actual Game object for key ownership and begins
with an item-owned key hold. Genuine Bero adds the second hold and stops at
bgmode_off; releasing the item and direct owners restores the same counter.
It does not claim background is integrated into native collision/rendering or
that the complete area parent finishes. Game's separate Bero/Trouble VMs still
need coordinated shared-area lifetime and real GP/checkpoint state.

Next establish original Mario form/dimension/movement owners and consumers, camera4
follow flag publication, previous-party allocation/art/setup and the genuine
attached initial_pos in this shared scheduler. Preserve source side effects
while later callbacks hold. Actual original traces/raster and hardware timing
remain unverified; this adds no draw calls or asset conversions.
