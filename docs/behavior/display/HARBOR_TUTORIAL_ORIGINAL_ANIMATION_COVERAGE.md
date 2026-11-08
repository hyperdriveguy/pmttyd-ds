# Harbor tutorial original animation coverage

The native setup2 presentation reuses the existing ordinary Mario battle bank
for original combat clips absent from the dedicated intro bank. This is native
presentation selection; it does not complete source battle Main, its camera
driver, menus, results, or animation services merely from elapsed time.

`selectHarborTutorialAnimation` takes a **visual slot**, not a source registry
ID. It first looks up the requested name in that slot's dedicated pack. Only
Mario visual slot1 may then use the ordinary Mario bank. Unknown names remain
unavailable. Existing dedicated/editable clips take priority even when the
ordinary bank also contains that name. No idle substitution is permitted.

Every selection names its texture owner. The renderer uses the same selection
for endpoint notification, top-screen geometry, lower-screen replay, and message
speaker bounds. Ordinary-bank vertices use `trioMarioTexture` and
`trioMarioUVScale`; dedicated-bank vertices keep their own texture and UV scale.
The lower screen reuses the actual upper actor vertex ranges and selects the
same atlas. It performs no additional simulation or animation completion.

## Original requests

The original `marioAttackEvent_NormalJump` at80355cf0 directly requests
M_J_1B, M_J_1D, M_A_5, M_D_2, and M_W_1. Source type42 selects M_R_1 and type40
selects M_W_1 through the original Mario table803728f0. All are present in
`trio-attack-mario.pmanim` and absent from `harbor-tutorial-mario.pmanim`.
The existing ordinary bank also carries M_B_1 and M_V_1/M_V_2 for guard/victory
presentation; their availability does not prove those paths have been scheduled
by normal tutorial gameplay.

The dedicated Mario bank retains M_S_1, M_N_2, M_D_1, and M_C_3. Original
SetAnimation95544a44 and WaitAnimationEnd95544a58 run in Crump's event but select
-3 (Mario), part1, with original M_N_2 string9540a7e4. The clip is nonlooping,
start0/end18. Under the existing 60Hz presentation clock, its actual endpoint
criterion first admits at300ms and refuses299ms. Notification uses actor.id1;
the source wait checks visual slot1 and that pose's actual poseAt epoch.

Original Crump attack requests R_2, A_1A/A_1B/A_1C, D_1, S_1, and R_1; all
already exist in the dedicated Crump pack. Its death event's explicit Mario
request is M_S_1. The scoped helper bank retains PKR_S_1 and PKR_T_1. No new
source behavior or additional helper attack/victory is inferred from these
resource inventories.

## Native ownership

Dedicated maps/actors and the required shared ordinary Mario atlas are imported
before FrameBegin by tutorial presentation preparation. Only successful real
imports set tutorialReady. Generic non-trio texture retirement is bypassed while
the tutorial owns that atlas; it must not delete the texture and then silently
continue sampling the bank. The existing ordinary scene/Renderer teardown
retains responsibility for retiring the shared texture after GPU synchronization.
No second texture field or duplicate atlas allocation is introduced.

The ordinary atlas is512x512 RGBA8 (1MiB); the dedicated Mario atlas is512x256
RGBA8 (512KiB). These are native pack dimensions and byte sizes, not measured
whole-scene memory headroom. This change does not claim to diagnose a tutorial
allocation failure or remove other unused resources as a memory optimization.

## Editable sources and rights

Existing editable sources remain `assets/original/animations/harbor-tutorial-mario.json`
and `assets/original/animations/trio-attack-mario.json`, with their existing
`assets/manifest.json` animation recipes and per-asset provenance/license ledger.
The original MODEL is a/b_mario and texture source a/a_mario- in the read-only
US G8ME01 ISO. Existing original T3X conversion uses the ordinary bank's own
atlas and sidecar. No importer, converter, checked-in asset, recipe, pixel, or
compiled pack is changed by this selection. Nintendo/Intelligent Systems retain
all original rights; no redistribution license is granted.

## Validation and limits

The focused test loads the actual matching four animation packs and samples all
22 required/available named clips at start, midpoint and endpoint, checking
finite vertices/UVs, color cardinality and the existing actor arena bound. It
checks dedicated priority, Mario-only fallback, identical top/lower selection,
missing bank/name refusal, and the real intro endpoint threshold. Independent
read-only ISO proof records22 original MODEL descriptors, three TPL identities,
original gor.rel, exact Jump request operands and Mario type-table rows.

Host strict, ASan/UBSan and ARM syntax receipts are retained in
`artifacts/crump-animation-coverage/`. The companion actual source intro proof
is in `artifacts/crump-presentation-admission/`. Native tutorial GPU execution,
physical Jump/guard playthrough and normal results remain parent integration
checks; resource availability is not a source queue completion receipt.
