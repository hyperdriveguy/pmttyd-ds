# Battle add_zoom and nomove_y event ABI

2026-10-14. Read-only original US G8ME01 disc0 revision0. Complements
[existing mode/speed/zoom/axis request contract](../battle/BATTLE_CAMERA_REQUESTS.md).
Offsets/addresses are hexadecimal. No new camera update or matrix behavior is
implied by these immediate event requests.

`evt_btl_camera_add_zoom8012b994:80` takes two operands. It resolves priority
through evtGetValue80031260 first, then delta through evtGetFloat80030840.
At8012b9c4 it converts delta with fctiwz, extracts the integer word, XORs sign
bit80000000, and converts back through original double bias802f02b0
(`4330000080000000`), using fsubs to obtain binary32 delta. It calls
`btl_camera_add_zoom8012ba94:20(priority,deltaFloat)` at8012b9f8 and returns2
regardless of leaf admission. Finite representable delta truncates toward zero:
2.9→2,-2.9→-2. Original out-of-range/nonfinite fctiwz semantics require their
platform provider; do not execute an undefined C++ cast or silently clamp them.

The leaf freshly loads global BattleWork8041eb90 and compares signed priority
against signed B275c. Lower priority returns without a write. Admitted reads
FLOAT B2778, adds incoming f1 with **single fadds**, and stores FLOAT B2778.
It does not truncate a direct leaf caller, clamp zoom, change mode/flags/speed,
rebase time, or raise priority. Arithmetic preserves old value as first addend;
negative zero/NaN details remain platform numeric domains. This differs from
set_zoom only in the leaf's addition rather than direct store.

`evt_btl_camera_nomove_y_onoff8012b254:58` also takes two operands, but both
use evtGetValue: priority first, onoff second. It calls
`btl_camera_nomove_y_onoff8012b2ac:38(priority,onoff)` and returns2. Its
already recovered leaf rejects lower signed priority; admitted onoff exactly1
ORs flags B2754 with8, all other values clear8. Nonzero2 is therefore off.
This event wrapper adds no blocking/poll state or output variable.

Native bindings can reuse the portable request memory/value services and the
same finite truncation provider as set_zoom; keep integer onoff evaluation
distinct from floating zoom evaluation. These leaves mutate packed request
state. Camera Main/cadence then consumes it; neither callback establishes
camera4 matrices or authorizes replacing unknown mode history with fixed views.

Evidence listings/ELFs: ignored artifacts/recovered-temp-evidence/
camera-addzoom-event, camera-addzoom-leaf and camera-nomovey. Complete hashes:

| Source body | SHA256 |
| --- | --- |
| event add_zoom8012b994:80 | 0319f001d8f19d51e9751872405e6a2b0d8c4f971e6f4188d69de432bb653aa8 |
| leaf add_zoom8012ba94:20 | b54e457e41073fa3906c1d03a10374380fcbcce366fc1704c35d3811c849103b |
| event nomove_y8012b254:58 | 3ef451dc4e11ac3f9e064fd17b5734cbe41d25aa4bccad7a26b5bfb52f5400df |
| leaf nomove_y8012b2ac:38 | af51cb7c9d785984c26653c87fc1de850c2be44685d3bab36c69d720311f708b |

Source-only documentation. No native/shared files modified and no commit.
