# Native Mario motion sound posts

Source: [initial handles and post stop](MARIO_INITIAL_SOUND_HANDLES_AND_ITEMGET_POST_STOP.md),
[original idle sound cues](../overworld/OVERWORLD_MARIO_IDLE_SOUND_CUES.md),
and [complete SFX Off](BATTLE_POSITIONAL_SFX_OFF.md).

Game retains source-initialized handles27c/280/284 as `MarioMotionSfx`, all-1.
The native main binds the actual Audio post provider before every Playing
simulation visit, including after a Game restore. For item-get/stay (new motion15/0), motion0 exit cancels27c then
280; motion10 exit cancels284. The slot Off precedes each handle's-1 write.
The other supported ordinary motion rows have null posts. An unavailable
out-of-bank/backend operation retains its handle at the port frontier.

`FieldSfxNdsp` adapts already-owned native effect voices to the existing source
PositionalSfxRegistry. Producers must provide the original handle/key/descriptor
and an already-started NDSP channel. Attachment is not original On allocation,
macro execution or automatic sample selection. Native field channels20..23 are
reserved; music0/1/18, cue2..9, house10..17 and map19 cannot be adopted. Duplicate
slot/channel ownership and contradictory descriptors for one table key refuse.
Pending source slots can be attached separately without starting a backend.

Off preserves low8 slot lookup without generation validation. Inactive/-1 and
pending slots do not clear NDSP. An active effect uses ndspChnWaveBufClear on its
owned field channel, then clears registry identity/engine and the Mario handle.
Stream/song and unowned-channel backends remain unavailable. Shutdown clears
registered field voices before NDSP exits; repeated shutdown/clear is harmless.
Audio post readiness checks the selected retained handles, including pending/
inactive slots and source out-of-bank frontiers, before installing the callback.

Fresh empty handles perform no DSP work and need no idle PCM substitution.
Current native idle/fall cue On/Main/macro/emitter producers remain unbound;
this step does not make those previously silent source requests audible. New
producers must retain actual handles and backend ownership rather than cancel
by cue name. Global source priority/capacity, source position/listener, voice
completion/release, MusyX execution and all original idle audio remain open.
The forty logical source slots and four native channels are distinct capacities.

`tests/field_sfx_ndsp_tests.cpp` compiles the actual native backend against an
NDSP stub. It checks ordered calls to the exact owned channels, retained handle
clears, null posts, inactive/pending/repeated operations, source stale-generation
Off semantics, reserved-channel/stream/slot refusals and unavailable handles.
It does not prove physical DSP voice release or original audible equivalence.

Validated2026-10-14, code commit `eb9155a9`, isolated review tree
`6e163055c18fabbc1402828e9d13bc128fdd259e`: `make test`231/231 C++ and141/141
Python tests passed; `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review` passed. Independent review confirmed
Off order and found external channel collisions in the initial broad adoption
range. Final code reserves20..23, with explicit house/map/music refusal tests.
No Azahar/Dolphin run, physical DSP observation or audible-equivalence claim.

Ignored artifact `artifacts/ttyd-mario-sfx-posts-reviewed.3dsx`, SHA256
`0d69830156754e4219cae1d145133366fd91e30050943c51f9eb2e48cfb742a4`.
Logs `artifacts/mario-sfx-final-test.log`, `artifacts/mario-sfx-final-native.log`
and `artifacts/mario-sfx-source-audit.txt` identify the reviewed source.
No new assets, sample substitutes, conversion or licensing changes. Original
source initial handles and stop logic are reused; idle audible providers stay
open in the request inbox.
