# Motion pose-call-site survey (recovery slice)

US G8ME01 disc0 revision0; static survey, no execution or gameplay claim.
Answers the pose-name/condition/ordering slice of native field/AC-03 request
3 (`docs/DOCUMENTATION_REQUESTS.md`) for recovery, jump, fall, walk, and dash
motion. Entry-condition ownership stays in [recovery
poses](OVERWORLD_MARIO_RECOVERY_POSES.md); this closes the literal names and
their immediate gate order. All strings read from the read-only ISO through
the DOL section map; SDA literals and absolute/table-relative forms are
distinguished per site.

Pose-request semantics (snapshot `mario.c`, JP/US caveat): `marioChgPose`
(0x80058d84, mario.o) and `marioChgEvtPose` ignore the request when the same
name is already active, else publish it with poseTime 0 and flag updates —
so repeated per-tick calls are idempotent by design, not redundant work.

## Recovery sites

| Site | Pose (form) | Gate / order |
| --- | --- | --- |
| mot_damage 8013aea4 | KPA_D_1 (table 0x802F3838+128) | [Mario+60] == 2 (entry region, before pose setup) |
| mot_damage 8013b4ec | KPA_D_2 (table +160) | [Mario+60] == 2 (land path, after [488] park) |
| mot_damage 8013b4f8 | M_D_2 (SDA) | Land path after the [488] park and bgmode test |
| mot_damage 8013b5a4 | M_U_3 (SDA) | HP-nonzero path after the −1 penalty |
| mot_bottomless 80091cc0 | M_D_2 (SDA) | Land path after SFX 185 |
| mot_bottomless 80091d3c | M_U_3 (SDA) | HP-nonzero path after the −1 penalty |
| mot_fall 8009b000 | M_J_1C (table 0x802C4098+48) | [Mario+0] bit 18 set AND signed [Mario+60] != 0 |
| mot_jump 8009b578/8009b658 | M_J_1B (absolute 0x802C40C8) | Same bit18/[60] gate shape, two sites |

The damage/bottomless pairs are structurally parallel (KPA pair at kind-2
gates; M_D_2 on landing; M_U_3 after the HP penalty). M_J_1B's rodata
neighbors (gon_10, SFX_MARIO_AIRPLANE1, p_hikohki strings) are adjacent
bytes, not arguments.

## Walk/dash sites (SDA literals, bgmode-gated alternation)

mot_walk: M_W_1 (80087a54, bgmode-gated), M_W_7 (80087b10, bit18-gated),
M_O_1 (80087b30/80087ca8, sub-call result gates), M_W_1 (80087b48).
mot_dash: M_R_1 (800876fc/80087778), M_O_1 (80087760/8008779c/80087958),
M_W_1 (800877a8). Gates alternate on `marioBgmodeChk` == 0 versus nonzero
with sub-call (0x8005942c) result tests; exact gate order is in the
listings. These are ordinary-locomotion entries, included because the
request's table slice spans all domains eventually.

## Explicitly still open (request 3 remainder)

Domains 4..9 (NPC/switch/stand/small jump), 14 talk, 15 item, 16..17 hip,
18..20 hammer/jabara, 21..28 paper/partner, 30 grasp, 32 spike, 34 reset, 35
shadow, 36 party, 37..40 Bowser/dummy; the pipe paper (p_dokan_x) asset
lifecycle (no reference body found — asset-side work, not disassembly);
`marioChgEvtPose` call sites (none in the surveyed bodies — all sites above
are `marioChgPose`).
