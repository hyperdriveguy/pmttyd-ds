# Native empty environmental sound Off

This bounded native counterpart closes the actually reached Harbor request
`evt_snd_envoff80151e0c(0x200)` using owned environmental handle state. It does
not implement whole `SoundInit`, `psndInit`, environment On/Main, fades, native
ambience scheduling or MusyX playback. Missing ambience is not empty-state proof.

## Complete recovered chain

The original US wrapper `80151e0c/2c` captures E+18, reads raw args[0], calls
`psndENVOff800d76b8/28` at `80151e20`, then returns 2. There is no evtGetValue,
first-call branch, or dependency on the integer child result. The child supplies
literal duration 1000 and flags 0 to `psndENVOff_f_d800d7470/248` at `800d76cc`.

The latter computes group from control low4, loads its unsigned handle at
`803df6e8+18+2*group` at `800d749c`, then reads an SDA lookup word at
`800d74a4`, **before** testing handle zero. The SDA table base is initialized
r13 `8041cf20` minus `6a00`, namely `80416520`. For reached control `0x200`,
group0 and table index0 select the actual word 10. A zero captured handle returns
0 at `800d74b0` with no native backend child or source store. The event wrapper
still returns 2. A live handle instead reaches the source fade/state graph;
this native counterpart holds that domain rather than acknowledging it.

Complete source bodies, original startup ABI bytes and actual table word are
pinned by `verify_native_environment_sound.py`, read-only from US revision-zero
ISO. Existing `harbor-tutorial-events.pmevt` already contains the original SDA
bytes. No new asset import, synthesized table or source-global clearing occurs.

## Explicit native constructor policy

`NativeEnvironmentSound::prepare` creates **unknown** native handle storage.
Actual native Audio startup may call `initializeNativeCold()` once, before any
ENV request, to execute these two finite native state contributions:

- `psndInit800dc5ec`: halfword zero for original `803df700` (group0).
- `psndInit800dc5f4`: halfword zero for original `803df702` (group1).

The native fields are real owned optional halfwords. This policy constructs only
those fields; it neither writes invented guest globals nor certifies the
preceding source `SoundInit800e239c`, intervening mode stores, later psndInit
children, or whole original audio startup. Its private-constructor cold proof
is weakly tied to the exact native state/control-block identity. It cannot reset
another owner. Late/repeated initialization, foreign/expired proof and retired
actual Audio host reject. Legitimate copies of the same proof may serve later
field contexts sharing that same actual native Audio owner; this is not a new
cold initialization. Never reconstruct it per Encounter/battle/checkpoint.

The binding pins an independent actual userdata/source lifetime and Audio host
registration. Host proof precedes the current callback or borrowed native data.
Root Audio must validate its actual current startup/owner identity, retiring the
registration on actual replacement/destruction before any borrowed userdata can
die. A holder must not contain a cycle through owner→binding.lifetime→holder.
An accepted unowned ENV mutation must invalidate empty evidence, never silently
reset it. Source On/fade/backend domains remain unavailable until actually owned.

## Retained source child and event adapter

The finite Off cursor admits only reached control `0x200`. It retains the
captured native handle across a held actual SDA read, then the successful table
word across the source branch. Missing source bytes do not become a constant10.
A completed cursor returns its original result without repeating any read.
A different owner or control cannot replace its held invocation. Live handles
and other groups/flags remain genuine unavailable domains.

`NativeFieldEnvironmentOff` pins the real caller VM registration and actual
shared environment owner/proof. It retains the whole wrapper's argument pointer
and raw flags across waits; subsequent arg-cell mutations do not restart source
loads. Return0 from the completed source child is an engaged result, so the
wrapper correctly returns2. Context5's actual retiring hook must call its
`retire(e)` to release per-entry metadata. Recursive invoke is rejected; recursive
retirement poisons that wrapper. No event/local/story/result stores are made.

## Fresh actual event frontier and next key child

The fresh focused fixture executes actual original NPC Entry/placement roots,
GetPos and this ENVOff, using explicit TEST native Audio constructor admission
and actual original SDA bytes. It next holds at
`evt_mario_key_onoff800e7474(0)`, command `90506518`.

Existing `fieldMarioKeyEvent` already owns the complete key wrapper/leaves.
`Game::fieldMarioPreparationWorld()` supplies actual Mario main flags and
wrapping signed byte39 key depth, including native fieldKeysOff recomputation
when flag8 changes. Its source operands must come from the actual Encounter VM
under proof-first current admission. Actual partyKeyOn/Off US bodies are
return-only and require **no party-registry lookup**. Do not replace those
source effects with the encounter's visual `keyDepth` mirror. Other preparation
callbacks, movement, camera, message/reaction/SFX and full field return remain
separate actual providers. This package adds no Game hookup or normal activation;
legacy working dialogue is unchanged.
