# Native setup2 Crump initialization and guarded tutorial phase

US G8ME01 disc0 revision0, static source plus portable original-event execution.
This is the initial tutorial service backend, not a complete playable battle or
an original-runtime timing comparison. See [source configuration](HARBOR_TUTORIAL_CONFIGURATION.md),
[boss event graph](HARBOR_BOSS_TUTORIAL_EVENTS.md), and
[separate helper roles](CHRISTINE_HELPER_ROLES.md).

`HarborTutorial` executes unchanged original Crump init (18 commands), phase
(84), Crump wait (3), NPC Goombella init (8), and NPC wait (3) with the existing
EventNativeRuntime battle context. Original branches, integer DIV, inline roots,
WAIT_MSEC/WAIT_FRAME, joins and event deletion remain VM instructions. No new
story dialogue or manually sorted tutorial message list is authored.

The converter proves setup2 → weight50 → one-unit group60c90 → setup60c60 →
Crump kind645c8, stage608f8, and helper entry647d8 → kind66d78. Fresh native
registry now retains system0, Mario1, Crump2 and spawned NPC Goombella3, with
separate Crump/Mario/helper visual slots. The original system init7-command
stream runs in that registry; see [registry scope](../battle/HARBOR_NATIVE_REGISTRY_AND_VISUAL_PROJECTION.md). It records
source pointers separately from those runtime IDs. Original Crump HP5 and helper
HP10 are imported from kinds. The source spawn result becomes Crump work0;
source helper init installs its own hooks and maxMoves0. Its phase remains0;
lecture-only coordinator, HP-skip and mode writes are absent. Crump source
position is imported from its setup, helper position/home is source(-140,0,5).
Mario position(-100,0,-5), default maxHP10 and registry allocation are explicit
fresh-prologue adapters pending complete field/pouch/entry ownership.

After init roots complete, the native entry bridge invokes Crump phase with
turn1/exact04000001. This is a scoped bridge, not recovered turn scheduling.
`dispatchPhase(turn,word)` admits another phase only after the current root ends.
The phase consumes source turn/phase, unit work and externally provided original
success/failure declaration bytes. HP <= integer DIV(maxHP,2) wins priority and
deletes immediately after t1_003. Independent success and failure branches can
then print t1_001 and t1_002 during one later eligible invocation. Their unit
work latches suppress repetition. All prints retain mode2, actual spawned
speaker ID3 and original msg/US/gor_00 bytes/tokens. Death t1_004/005 is not
admitted merely because HP reaches0.

Source interpolation mode0 delegates to the recovered built-in interpolation
function; sample truncation and 16 source queries/one-frame waits are retained.
Mario rotation requests and raw camera/audio requests are published for native
consumers. Initial waits install source wait roots; the checked fresh wait scripts
change pose then delete themselves. Scoped idle selects original type28 from the
actual kind part pose table, not from the callback's bodypart operand1. Mario
post-intro type43 also resolves its original DOL pose table to M_S_1.

AnimeWaitPlayComplete80106980 has a source one-query delay using entry+78 before
polling animPoseGetLoopTimes8004079c. The backend preserves that delay and then
waits for explicit animation-driver completion for the current pose epoch.
`animationComplete(id)` releases that exact request; simulation time alone does
not manufacture a playback result. Original b_mario M_N_2 and M_S_1, c_pkr
PKR_S_1/PKR_T_1 and c_kanbu S_1 packs support the renderer's clip-end publication.
Native MessagePlayback talk/stay requests update the current helper pose using
the source-installed pose names. Full pose-driver loop counters/rounding remain
integration work.

Limitations: this owner retains camera/audio requests without playback; no command
menu, source Mario/Crump attack, action declarations, damage/death/rewards, battle
end or field-return service is bound. Its result stays0. It never advances story5
or starts the ordinary development harness. Original setup lottery's17 RNG draws,
seed ownership, unit-entry packed services, source main/sequence order and
resource teardown remain required. The tutorial and opening goal stay incomplete.

Focused original-data checks cover kind/HP/helper identity, installed hooks,
maxMoves0, t1_000 speaker/status, actual animation acknowledgement and rotate-back,
exact phase rejection, HP-latch priority, independent feedback and latch reuse.
They are source-data execution tests, not Dolphin observations.

## Installed attack-hook admission foundation

`dispatchInstalledAttack(id)` now admits only source-installed Crump2 or NPC3
hooks after the intro ends and while no phase/attack root owns execution. This
is an explicit battle-scheduler bridge, not a gameplay development button.
The NPC's three-command attack only restarts its wait; maxMoves remains0 and no
lecture damage is enabled. Crump's11-command wrapper preserves EnemyItemUseCheck80104498. The
source wrapper resolves the owner and calls BattleEnemyUseItemCheck801f9658,
whose first branch reads unit+308 and immediately returns0 for no held item.
The converter now verifies original selected table80417920 is eight zero bytes;
fresh setup2 leaves the cleared selected item0 and consumes no item draw.
Nonzero held items remain pending. On0, the wrapper supplies original weapon
64718 to its95-command normal-attack child. Original check_battleflag reads
raw mask2 and flags1000, selects the normal target branch, then waits at
GetEnemyBelong8010eed4 pending packed group services. Crump membership1/phase04000004 and helper membership2/phase04000001 are
imported from the original entry/setup words; the helper is neutral, not a player
partner or enemy. Unknown callback requests retain owner, command, time and raw
operands before publishing the pending function address. No arbitrary callback
success is returned to reach movement/damage.

`harbor_tutorial_combat_assets.py` verifies unchanged wrapper11/normal95/damage5/
death75/NPCattack3 command spans and hashes, source data key49→death and weapon64718.
The existing relocated gor data pack already contains those streams; the small
ignored PMHTC001 mapping retains source entry/weapon addresses and53 callback
identities without another script/text copy. Generic DOL damage-child services,
packed registry migration, original Mario first-attack branch and all battle
scheduler/death/end/field services remain unbound. In particular HP0 does not
run death004/005 or publish battle result1.
