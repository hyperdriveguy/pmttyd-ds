# Native underground approach and Goomba trio scene

Original [field event](UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md) and its
10-command footstep child execute in the existing native event interpreter.
The new event pack retains tik.rel words/selfrelocations, original module0
callbacks and independent section addresses. Source story<=11 admits the scene
on tik_01 restore; source NPC placements/flags precede its per-frame MarioX>=-100
poll. Field controls remain available until the script keys off. Room unload
resets this owner with ordinary native restore.

Native services bind named NPC pointers to separate actors3/4/5, party0/1 to
Goombella/Frankly, explicit extparty speaker and original Mario/party positioning
and facing. Source parallel run/move/hop events retain distinct event IDs and
local/shared variables. NPC run/damage pose requests are retained by their actor.
Original086/087/088/089_kur/090/091/092 text/speakers are used unchanged. Script
camera requests and Mario e_mario M_N_7 use original assets. Unsupported effects
and SFX retain requests rather than fabricate story dialogue.

Original c_kuribo provides fifteen used KUR/PTK/TGK stay/talk/walk/run/damage
poses. Native animation sampling and citro3d batches draw these meshes and
textures; original flags determine hidden positions. General glyph countdown
selects source talk poses only while requested and NPCflag0x200 admits them.
No partner jump pose is inferred from the NPC no-hit hop. Temporary Frankly
can now render/follow underground and receives source party1 positions during
key-off. Full party stop/selector/restart/previous-state ownership is incomplete.

The source callback sets tik battleinfo4 then waits at original battle_start.
This is [three variants plus Frankly's teaching unit](../battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md),
not the Crump harness. The actual battle/coordinator/attack/counter/result services
remain unbound; postbattle093/094/094_01 and story12 remain blocked. The last
Frankly message resource tik_04 is loaded for its later source continuation.

Portable original-data tests pin initial positions, inactive crossing, inclusive
X threshold, seven message order, actor/party poses and positions, four camera
requests, setup4 and retained story11. Native isolated captures inspect all
three original variants, Frankly, source text, camera changes and battle boundary.
Capture is a manual presentation check; host state asserts source sequence.
Neither proves complete original-runtime, battle or handheld performance parity.
Native movement/hop arithmetic, NPC/material/shadow sorting, effects/audio,
manager cadence and force-restart clocks remain explicit adapters/dependencies.

Reproduction: build DEBUG_SHORT_BOOT=1 BUILD=build-trio-verify
TARGET=artifacts/ttyd-underground-trio, make the native development fixture with
tools/make_underground_trio_fixture.py, then use tools/emulator_smoke.py with that
binary/fixture, tests/azahar_underground_trio.py and a separate artifacts output.
