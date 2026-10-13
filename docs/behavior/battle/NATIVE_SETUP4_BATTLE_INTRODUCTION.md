# Native setup4 battle introduction

Original [setup4 data](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md) and
[phase/command ownership](UNDERGROUND_SETUP4_INTRO_PHASE_STAGE_AND_PLAYER_COMMANDS.md)
now feed a separate battle-introduction owner. The field Goomba scene retains
its own VM at the original battle_start boundary. Native field-battle services
publish sequence4/result0 and disable Mario/party controls with a semantic NPC
owner token3. Field checkpoint coordinates/story remain unchanged during the
introduction; native battle rendering uses separate actor positions.

Read-only tik.rel group/kind/part records provide four actual source units,
HP/FP, XYZ, unit/part/counter flags and elemental defense arrays. Fresh source
registry gives Frankly ID6; the scoped native introduction preserves that ID.
Original Frankly init/entry/phase scripts execute without command rewriting,
retaining callback requests and raw phase0x04000001/turn1 gates. Entry placement,
R_1/S_1 changes and 45-callback horizontal move advance before source first-turn
messages092_01..05. Counted move performs its first step on admission, leaves Y
unchanged until snap, and completes with source advance+yield. Native arithmetic
is a scoped linear bridge; full packed-unit/sound/heading services remain open.

Source event registrations and wait-event requests are retained. The native
introduction sequences Frankly init→entry→phase; complete system/Mario/partner/
opponent init, concurrent firstAct entry barrier, phase fanout and actor command
selection are not yet the original battle scheduler. Full original battle entry
and phase lifecycle remain required. Status and camera requests are retained;
native camera uses the field camera renderer as an adapter. The five exact US
messages retain explicit battle Frankly attribution. Common print battle mode3
and used evt_msg_toge selector1/mode0 are bound as one-shot event placement.
The PAD_Y_ON icon is imported from the original115 descriptor/TPL. Window
animation continues through closing after the event finishes.

Original bti_01 and stg_00_1 geometry/textures are imported and drawn through
citro3d. Recovered source visibility now selects A1/isi_1/isi_2/B_mati/C subtrees,
retains authored ancestor matrices and bakes source additional translations.
Bti escape_wall/if_body/if_wire are hidden in this no-ironframe setup.
Both maps filter opaque/translucent passes. Dynamic intro scales, stage object
lifecycle/depth/material sorting remain unfinished. Battle homes
use recovered Mario(-100,0,-5),Goombella(-140,0,5); actual opponent homes retain
source group records. Native battle views do not claim the complete original
stage pipeline or physical3DS performance parity.

After the five messages, the native owner stays at an explicit development
player-command boundary. Original Jump/Hammer/Headbonk attack graphs, target
selection, spike counter instructions, airborne damage/fall, enemy turns, result
producer and field wait_battle_end return remain unbound. No Crump battle or
manufactured victory advances093/094/094_01 or story12.

Tests use original packs to check source kinds/HP/flags, Frankly init/entry,
first-turn and later-turn gates, exact five message order, status/camera requests,
retained field owner/control/result state and checkpoint coordinates. Isolated
Azahar capture follows the actual field dialogue into the battle introduction.
It supports presentation only; source-state assertions live in portable tests.
