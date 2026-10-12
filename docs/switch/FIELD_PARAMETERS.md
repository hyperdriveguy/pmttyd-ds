# Switch player forms and jump data

Baseline: [Switch 1.0.0](BINARY_BASELINE.md). These are original stored records,
not yet a recovered field-physics simulation. Exact position units, timebase,
mode selection, collision meanings and integration order remain unverified.

## Player records

RomFS `data/param/data_param_player.elf.zst` expands to a 4,264-byte ELF with
symbol `_ZN3wld3fld6player12s_playerDataE`, size 1,728 = 18 records of `0x60`.
The symbol begins at `.data+0`. Each row has relocated string pointers at
`+0`, `+8`, `+0x10`; the remaining words are preserved in the evidence report.
The source editor labels these strings ID, playerState and playerType. They
are valid associations in the binary; their complete runtime effects are untraced.

Selected original values, printed with short decimal notation (the report keeps
exact binary32 values/bits):

| Row / ID | +8 string | +0x10 string | +0x40 | +0x44 | +0x48 |
| --- | --- | --- | ---: | ---: | ---: |
| 0 NormalMario | mario_normal | mario | .25 | .45 | .45 |
| 1 DuctMario | mario_duct | mario | .25 | .45 | .45 |
| 2 MarioBackGround | mario_normal | mario | .15 | .27 | .27 |
| 3 MarioSlitMode | mario_normal | mario | .10 | .20 | .20 |
| 4 MarioRollMode | mario_normal | mario_rollmode | .30 | .60 | .60 |
| 5 MarioShipMode | mario_normal | mario | .20 | .40 | .40 |
| 6 MarioYoshiRide | mario_normal | mario_yoshiride | .25 | .675 | .32 |
| 7 MarioBarrelLift | mario_normal | mario | .25 | 0 | .25 |
| 8 Peach | mario_normal | mario | .20 | .40 | .40 |
| 11 Koopa | mario_normal | koopa | .25 | .45 | .45 |
| 12 KoopaActionLv0 | mario_normal | koopa_lv0 | .28 | .45 | .425 |
| 17 VivianHideMario | mario_normal | mario | .25 | .45 | .45 |

Rows 9/10 are PeachBig/PeachSmall; 13–15 are KoopaActionLv1/2/3; row 16 is
DotMario. Thus the file includes interlude/form data as well as ordinary Mario.
File presence does not establish that every listed mode is reachable.

Unsimplifier names `+0x40` runSpeed. Preserve it as an external annotation until
the Switch movement code establishes its use. In particular, .25 is not proven
ordinary full-stick movement per update, and .675/.45 does not establish an
observed Yoshi/Mario speed ratio. Fields `+0x44` and `+0x48` differ in some modes;
ignoring them would hide relevant data before their use is known.

NormalMario's floats at `+0x18..+0x38` are
`1.2, .6, 1.2, 1.1, 1.2, 0, 1.2, 1.1, 1.2`.
MarioSlitMode has `.29/.295` at `+0x1c/+0x20`, while MarioRollMode has
`.65/.1` at `+0x20/+0x24`. These are evidence for distinct stored form geometry
parameters, not a recovered collision cylinder or bounding-box algorithm.
Do not map them directly to the native prototype's calibrated sprite scale.

## Jump records

RomFS `data/param/data_param_jump.elf.zst` expands to a 4,168-byte ELF. Symbol
`_ZN3wld3chr10s_jumpDataE` is 2,160 bytes = 30 rows of `0x48`, starting `.data+0`.
A relocated string at row `+0` identifies the record. Selected first four floats:

| Row / ID | +8 | +0xc | +0x10 | +0x14 |
| --- | ---: | ---: | ---: | ---: |
| 0 default | 0 | 0 | 0 | 0 |
| 1 mario | .8 | -.065 | -.0178 | -.006 |
| 2 mario_yoshiride | .2 | -.1 | 0 | 0 |
| 3 mario_rollmode | .72 | -.065 | -.0178 | -.006 |
| 4 mario_rollmode_b | 1 | -.065 | -.0178 | -.006 |
| 5 mario_jabarajump | 1.8 | 0 | 0 | 0 |
| 6 mario_jumpstand | 1.2 | -.065 | -.0178 | -.006 |
| 7 mario_moon | .3 | -.0003 | .0029 | -.0004 |
| 8 mario_moon_rollmode | .12 | -.0003 | .0029 | -.0004 |
| 20 party | .8 | -.143 | .009 | -.003 |
| 21 party_moon | .15 | -.0003 | .0029 | -.0004 |

For mario, fields `+0x1c..+0x28` are
`-.0005, -.0035, -.01, 0`; `+0x30/+0x34/+0x38` are `-.375, 1.2, -1.2`.
For mario_moon, the corresponding four values are `.034, -.0002, -.001, 0`,
and the final triple is `-.375, 1.8, -1.2`.
Unsimplifier calls `+0x34` jumpHeight, but whether it is a clamp, target, release
parameter or another quantity is not established by reading the table.

Rows 9–19 carry koopa, level/hop/hipdrop and swimming IDs; rows 22–29 carry coin
entry/one, moon, small and big variants. The same table serves several object
classes. Applying ordinary-Mario equations to all rows without their consumers
would be an unsupported generalization.

## What can be concluded now

Player modes and jump parameter sets are explicitly distinct in the Switch data.
The GameCube DOL's ordinary jump tuple, actor dimensions and 60 Hz tick assumptions
cannot serve as binary evidence for this build. Even similar-looking numbers do
not establish identical units or execution order.

Next trace the lookup from the third player string to jump selection; determine
which values initialize velocity/derivatives, where button release is sampled,
how fall/ascent transitions are ordered, and how collision adjusts the result.
Separately trace each speed field and stick threshold/dead zone. Recover dynamic
support, walls, ceilings, water/void recovery, camera and partner movement through
their own consumers. None is established by a row inventory alone.

## Reproduction and fingerprints

Run `python3 tools/switch_table_evidence.py --output artifacts/reference/switch/tables.json`.
Every row keeps its index, `.data` offset, decompressed ELF file offset, original
string targets and raw word interpretations. This retains unsupported fields
without assigning them new names.

Decompressed ELF SHA256:

- Player: `26ea665097b82a5539b914a979c9c3575bdd4b15840b019dbcf227fa7035ceb9`.
- Jump: `4dc3e8b8e237ae96550f59dac9e92884fb1fc1fc9b5e4b388583ef6d22c559f6`.
