# Native sound-mode loader and movement-sound record services

Implementation: `include/battle_packed_movement_sound.hpp`,
`source/battle_packed_movement_sound.cpp`,
`tests/battle_packed_movement_sound_tests.cpp`. Contracts:
[BATTLE_UNIT_SOUND_MODE.md](BATTLE_UNIT_SOUND_MODE.md) and
[BATTLE_MOVEMENT_SOUND_CONTROL.md](BATTLE_MOVEMENT_SOUND_CONTROL.md).
Read-only original inspections:BtlUnit_LoadSeMode80126588/14c,
_MoveSoundControlCore80104850/180,_UnitMoveSoundControl80104a24/54 and
_MoveSoundControlEndCore801047bc/44. Artifacts stay /tmp/pm-native-sound-mode.*,
move-sound.*,unit-move-sound.*,move-sound-end.* outside source history.

Loader now acts on native word/halfword storage,without a whole-record snapshot.
Unconditional half14=0 precedes word18=-1. Mode-2 reads only destinationc and
copies to12;mode-1 dispatches exact type0/1/2/3 source ranges;other signed types/
modes clear only the selected words/halves. Source reads/stores stay ordered,
including reads overwritten by previous destination writes. Gap8..b survives;
initial18 reset does not implicitly stop a prior voice. No source key/type is
interpreted as a generated sample or measured sound duration.

Core snapshots incoming XYZ before memory providers,as original f1/f2/f3 inputs
are already retained on call. Disassembly also exposes three discarded template
word reads at802ee494/498/49c BEFORE key queries. All three template coordinates
are subsequently replaced by incoming floats. Native records still perform those
reads,including empty-key path;templates must be provisioned by their owner.
They are not used to synthesize a position/default sound request.

First key is read once for dispatch. Second key is read only at the original
branch sites. Single-key path reads current handle and,for exactffffffff,
requests original-shaped trigger(unit,key,rawSentinel-250000000,0,XYZcopy),then
stores returned word unchanged. Unit or optional-part geometry is queried AFTER
trigger;handle is reread AFTER geometry before position request. Failed returned
ffffffff is still positioned and can trigger again later;no invented failure
classification occurs. Unit wrapper first snapshots visible XYZ and can query
it again inside retained-voice core. Part world queries compose with the existing
native absolute/relative/facing service,independent of hit offsets.

Other nonempty key combinations read signed remaining12,skip negative values,
decrement/store/reload,then on expiry increment/store/reload signed alternation14.
Selected key is reread AFTER counter writes,then its signed period is copied to12.
Key0 skips only trigger;counter/period mutation persists. Periodic trigger result
is ignored and never stored to18. Halfwords wrap before testing/parity. A retained
voice end reads18,skips exactffffffff,requests Off with the captured handle,then
storesffffffff after the callback. Keys/counters/periods remain unchanged.

Native factory binds record/geometry providers to shared EventNativeRuntime and
copies explicit playback callbacks. Userdata/runtime must outlive calls;playback
must not retain private coordinate references. Trigger/position/Off implement
required original services;no silence/fixed/generated-effect fallback is supplied.
Complete BtlUnit_snd_se_pos,psnd playback wrappers/core,macro/voice/resource
ownership still need documentation/native integration. These services are not
automatically connected to Game's provisional battle harness or original banks.

Fixtures cover all loader types/modes,source/destination/self aliases,reset/gap
order,empty-key/template reads,input capture before template callbacks,retained
trigger/geometry/handle and failure routing,absolute part geometry,alternating
expiry/period/zero-key/negative/counter-wrap,current selected-key reads,Off-before-
handle reset,unit wrapper's fresh visible queries,and actual native byte RAM/
loader/sound-factory composition. Playback callbacks record authored requests;
no original audible output/voice fidelity is asserted. ISO verifiers match the
loader/core/wrapper contracts;hardware/emulator/audio macro validation remains.

Next:bind native MovePosition using these sound services and explicit original
math providers;then Mario jump motion/transitive event/outer battle/Game flow.
Missing spatial playback contracts are requested under EVT-01 in
[DOCUMENTATION_REQUESTS.md](../../DOCUMENTATION_REQUESTS.md).
