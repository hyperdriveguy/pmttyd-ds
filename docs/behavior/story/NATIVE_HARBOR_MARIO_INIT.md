# Native harbor original Mario initialization

`HarborTutorial::beginFresh(milliseconds, carriedHp, maxHp, originalStory)` is the
explicit new-game entry adapter. Existing `begin` remains compatible for the
older isolated init/phase diagnostics. Native opening callers should use the
fresh entry and supply original HarborEncounter story4 and actual carried field
HP (current source maximum10). This creates the verified fresh source pouch,
copies the explicit carriedHP to pouch70, writes original GSW0 and starts the
actual Mario kind80372afc+bc initializer80372ec4 before admitting the intro phase.
Native allocation maps pouch84400000 and original pointer8041eb00. Source heap0/
size5d4 semantics and original global US `name_party3` message are retained.
This is explicit fresh-state ownership; it does not reconstruct old-save badges
from native Progress inventory. Parent owns carrying this source state across
field/battle. Actual packed data can be read with existing readPackedWord.

Original Mario init has20 commands. Its source callbacks install wait80417c38,
attack80417c30, damage80417c08 and confusion8034f1e8; SetEventAttack also clears
unit2ac. Registered script pointers are copied raw, not resolved as event values.
It installs jump keys802f4018/4030, walk/run keys802f4048/4060 and the original
period/delay halfwords; part1 pose-sound table80417be0 with actual reset constants;
unit work8 and part3 work0; original emblem callback; entry80372bc0; conditional
story201..212 black-Mario callback; and start-wait. Source unit and part fields
are retained in the shared packed owner and public actor projection now also
exposes entry2b8. The all-init barrier requires Mario roots to finish before the
current opening phase adapter dispatches original t1_000.

The emblem callback80145404 resolves Mario, counts actual equipped327 and328 via
source pouchCheckEquipBadge, then on the initial invocation with both counts0
returns2 at801454dc/e0 before requesting any PaperPoseRead. This verified fresh
branch is bound. Other emblem/model-reload branches remain guarded until their
native paper ownership exists. GSW0=4 naturally skips201..212. A test story201
runs the original IF branch and blocks unowned80145614 rather than silently
claiming black-Mario initialization or admitting the intro.

The source default attack80417c30 is only RETURN/END. It remains installed;
normalJump must come from original command weapon+b0 selection/STATE6. Wait
80417c38 invokes original AnimeChangePoseFromTable(-2,1), then returns. The
actual wrapper801064d8 calls btlDispPoseAnime801010f4: packed part204 first
clears2/10, sets1, clears80 and clears4, retaining that source write order.
The fresh automatic-pose presentation projection selects original table28
M_S_1; original Mario table803728b8 entries28 and43 both point80422ef0
M_S_1. The current initial/intro bank therefore needs no M_N_H clip. Broader
automatic status/damage poses still need their original display controller. Damage80417c08 sets unit/
part variables and runs common original child8034cc60. Installing this real root
does not claim the child services or CheckDamage are complete.

Sound banks now execute their original storage writes. Actual movement/jump
services retain source trigger/position/off requests in `soundRequests`, carrying
source key pointer, exact extracted name, actual owner, source position and
simulation timestamp. Native request handles are allocated within this owner
for retained-voice lifecycle; they are an explicit audio adapter, not original
hardware playback handles or evidence of audible playback. Parent audio owns
consuming these requests and mapping original sound resources. Generic jump
sound requests retain their original caller ordering.

Validation exercises fresh pouch with carriedHP7/max10, all original installed
scripts/banks/part work/pose sound fields, source no-emblem callback completion,
absence of story201..212 work at4, positive branch blocking at201, original
Crump movement sounds and unchanged source result. Eighteen read-only source
body/script/constant hashes are pinned by verify_harbor_mario_init_services.py.
No asset changes are needed beyond the already imported original DOL data pack.

Next: actual first-turn/phase/command roster, original selected normalJump and
complete counter/calculator/status/damage/reaction/retaliation/controller graph;
Crump death004/005 and genuine BattleStart return. Source result remains0.
See [fresh pouch](../battle/BATTLE_FRESH_PACKED_POUCH.md),
[counter/hit events](../battle/BATTLE_PACKED_COUNTER_AND_HIT_EVENT.md) and
[physical defense](NATIVE_HARBOR_PHYSICAL_DEFENSE.md).
