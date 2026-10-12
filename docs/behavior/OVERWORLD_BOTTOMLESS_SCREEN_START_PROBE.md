# Original bottomless recovery start screen probe

US G8ME01 revision0, read-only mot_bottomless80091904 entry and projection
service reconstruction. This closes the ordinary valid return-position domain,
not the full damage/recovery event lifecycle. Read
[the camera projection contract](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md).
No original execution or measured camera height is claimed.

## Exact entry input and loop

On pending motion-entry bit0 of Mario+0c, mot_bottomless clears that bit,
sets retained base speed+180 to0, air counter+50 to0 and phase+44 to0
(80091940–968). It reads return-position Y atMario+e4. In the ordinary domain
returnY>=**-2000 engine units** (wordc4fa0000 at804209a4), choose:

    X = Mario.e0  # retained return X, not current root X
    Y = Mario.e4 + 100
    Z = Mario.e8  # retained return Z, not current root Z

The retained target loads are800919fc–a04. Offset100 is word42c80000 at804209b8,
added at80091a08–34. The source then repeatedly copies that point and calls
marioGetScreenPos80058960 at80091a54–a7c. It truncates projected SX/SY toward0
using fctiwz (80091a88/8c), and calls marioChkInScreen80058928.

marioChkInScreen accepts **inclusive** SX0..600 and SY0..535. It ignores projected
Z and does not reject a point from view-space depth alone. While accepted,
increment Y by**10 engine units** (word41200000 at804209c4), store the new
point at80091aac–ab4 and repeat. Stop at the **first** point classified outside;
write that Y directly to Mario root+90 at80091abc–acc. No Q1000, safety margin,
physical-body height, inverse camera-top formula or source iteration cap is used.
Do not replace this with a fixed recoveryTopHeight.

A finite native engine*.01 adapter starts at returnY+1.0 and increments.1 while
projecting the target's retained returnXZ. Projected screen integers/bounds
remain original screen-space quantities or a clearly described projection
adapter; they are not 3DS viewport0..400/0..240 checks.

## Which camera and which frame

marioGetScreenPos explicitly gets **camera4** at80058980–88, installs that
camera's projection+15c/type+19c through GXSetProjection, obtains the current
GX projection and **current retained GX viewport** snapshots, then calls
GXProject802ba010 with camera4 view matrix+11c and the supplied point
(8005898c–9cc). The leaf does **not** install camera4's viewport itself.

The ordinary manager calls camMain before EVT and Mario; see lifecycle/order.
Thus normal entry consumes camera4's already built view/projection for this
callback, while that view was computed from the prior Mario callback's retained
follow/contact state. The loop does not recompute the road/camera for each new
Y, does not read cameraAt as a top-height proxy, and does not use Mario's
cached+168/+16c projection words. Event/GX viewport mutations are separate
state inputs; ordinary full-frame viewport adaptation must be labeled as such.
No render-frame vs simulation-tick substitution is implied.

## Recovery camera publication is not a solver freeze

marioStMain calls camMain at8006f944, Mario at8006f95c, and npcMain at8006f964;
there is no recovery-motion admission around those calls. camMain invokes each
installed camera callback and rebuilds its view. cam3dMain's whole callback skip
is **camera flags2** at8000ea88/8c. Neither mot_bottomless nor ordinary mot_damage
writes that camera flag. Retaining a recovery frame for all ticks is unsupported.

The original table80310030 has 12-byte entries: **motion31=mot_damage8013ae04**,
32=mot_damageToge, **33=mot_bottomless80091904**, 34=mot_forceReset, and
29=mot_dokan8014a564. The original water controller is mot_damage, not a symbol
mot_water. The camera's motion29 gate therefore does not apply to bottomless.
Its entry calls marioNoUpdateCamPos801a8f34 at8013ae90. That helper sets
**Mario display10000**, which suppresses ordinary follow ec/f0/f4 and depth64
publication at801a9100–108. It does not suppress camMain, road solving or the
three shift approaches preceding that guard. Camera therefore continues from
retained or explicitly published targets while root Mario moves independently.
This corrects the older MECHANICS statement that water freezes the field camera.

| Owner/phase | Explicit follow publication while display10000 is set |
| --- | --- |
| mot_damage sink/hold/rise (0/1/10/11) | None; retain ec/f0/f4. |
| mot_damage12 | f0+=Mario2c0; move rootXZ; ec=rootX,f4=rootZ (8013b350–380). |
| mot_damage14, zero-distance path | f0+=Mario2c0; ec=rootX,f4=rootZ; no rootXZ move (8013b3e0–3fc). |
| mot_damage13/20/31/32 hold/fall/hurt | None until unlock. RootY/floor are not replacements for retained f0. |
| mot_bottomless0 | Move rootXZ; ec=rootX,f4=rootZ; retain f0 (80091b38–58). |
| mot_bottomless2/11/12 fall/hurt | No explicit ec/f0/f4 writes. |

Mario2c0 in phase12/14 is a **followY delta**, not fall/rise velocity:
(chosen returnY−retained Mario2b8)/travelTicks at8013b2c4–2f4. The zero-distance
branch uses10 ticks and the same division at8013b320–344. set_damage_root_ypos
8013b600 takes a caller-supplied Y and stores it to2b8 at8013b618; callers must
retain that actual admission height. A rootY/floor substitution at recovery
start is not implied by this leaf. Native engine*.01 scales both Y operands,
then divides by the source tick count; no extra.01 multiplier on the quotient.

mot_damage clears display10000 on zero-HP admission to33 at8013b58c and on
positive-HP phase32 completion at8013b5d0. mot_bottomless clears it at80091d30
on zero HP and80091df0 on positive return/completion. mot_bottomless itself
never sets it; retain the actual incoming flag, rather than inventing a phase
mapping. When it is clear, ordinary marioCamMain publication later that same
callback can overwrite the phase0 XZ write with root plus camera shifts and
rootY+offset9c+vertical shift. With it set, explicit writes survive for the next
camera callback. marioCamMain skips only marioUpdateCam for motion29, not33;
the recovery-domain marioMain special dispatch reaches marioCamMain at8005acb4.

No recovery owner here writes collision target f8/fc/100. Retain its actual
last contact, including any downstream source service write; do not substitute
root/floor/return marker. Display40000 clearing in marioCamMain is a separate
contact-publication marker, not a clear of that target.

Water entry separately calls npcStopForEvent80043cd8 at8013af5c, and completion
calls npcStartForEvent80043bc8 at8013b5d4. Stop iterates current field NPCs with
flags2 and10000000, sets jumpFlags10000 at80043d34–3c, stops their event and
sets animation rate0. It is not a skipped npcMain manager, nor a rule stopping
every actor or partner. The partner owner remains a separate contract.

## Water admission height and travel callback boundaries

Ordinary marioChkGnd checks current motion!=31, nonnull support1e8 and its
hitObjGetAttr bit200 at8008f928–944. It passes **current rootY+90** to
set_damage_root_ypos at8008f960–964, then requests motion31 at8008f968–96c.
The selected support/surface height is not the passed argument. This is after
that caller's preceding contact/root processing. Its optional preceding
marioSetFall at8008f95c does not change the argument's source: the following
instruction still explicitly loads current rootY.

marioLandOn's ordinary water branch likewise tests motion!=31 and attr200 at
80092de8–df8, loads current rootY at80092dfc, calls set_damage_root_ypos at
e00, then directly switches current motion31, invokes prior motion's post
callback and sets pending-entry bit1 (80092e54–94). Special main10000000 and
other branch gates remain as their source inputs. set_damage_root_ypos copies
that argument unchanged to2b8. Capture this actual admission rootY before the
sink starts; do not recapture the subsequently sunken root or substitute the
floor. NoUpdateCamPos does not assign f0=2b8: it retains actual incoming f0.

The damage phase table8036ed4c pins control flow:

| Trigger | Same-callback work and next phase |
| --- | --- |
| Pending entry | Setup0, then fall through phase dispatch8013af60 and phase0 sink/integration on this callback. |
| Phase0 root below sink threshold | Set1/count40, branch end8013aff8; no hold decrement yet. |
| Phase1 count reaches0 | Set10/count0/rise speed20, branch end8013b024; rise starts next callback. |
| Phase10 | Increment count; at2 sets11; both branches fall into rise8013b06c on this callback. |
| Phase11 prepares nonzero travel | Set12/delta/count;8013b310 branches endb5d8. First follow increment/XZ move/decrement next callback. |
| Same preparation with zero distance | Set14/count10/delta;8013b34c branches endb5d8. First follow increment/decrement next callback. |
| Phase12 travel completes | After this callback's increment/move/decrement, set13/count1, branch endb3c0. |
| Phase13 count reaches0 | Set20 and branch endb3dc; fall starts next callback. |
| Phase14 completes | After follow increment/decrement, set20/fall tuple and branch endb434. |
| Phase20 lands | Set31/count50 then fall throughb554 decrement on this callback, leaving49. |
| Phase31 positive HP after hold | Set32/count12 then fall throughb5b8 decrement, leaving11. |

These boundaries belong to original motion31; native display phase names are
not source phase numbers. Bottomless motion33 phase0 entry/travel is separately
specified below; it must not inherit water's preparation pause accidentally.

## Immediate continuation and entry effects

After the first outside Y, source sets horizontal recovery speed+2c4 to8,
prepares word+48 from signed16(trunc(distance(currentRootXZ,targetXZ)/8)), and
heading+2c8=angleABf(currentRootXZ,targetXZ) at80091ad0–b10.
There is no immediate rootXZ snap to the return point here. The same callback
continues through phase dispatch at80091b14. For ordinary phase0 it moves
rootXZ at speed8 using that heading, writes camera-follow ec/f4 from moving XZ,
and decrements word48 (80091b38–b64). Vertical fall/grounding/hurt admission
belongs to later phases and the existing recovery contract.

The horizontal preparation and same-callback dispatch are relevant to a native
start-probe replacement: it must not accidentally add a stationary entry tick
merely because screen probing was factored into a separate helper.

## Low-return sentinel branches

When returnY<-2000, the body checks exact map names from GP+12c:

| Map | Probe X | Base Y before +100 | Probe Z |
| --- | --- | --- | --- |
| win_03 | 250 | 13 | current rootZ |
| win_01 | 220 | 1 | current rootZ |
| tik_01 | 100 | 120 | current rootZ |

These constants/strings are source-pinned. They are not Rogueport fallback
positions. With the low sentinel on another map, this source span reaches the
probe with caller-retained/unassigned locals; that exceptional domain is not a
license to invent a gor return point or height. The bounded ordinary contract
requires an actual valid return position, not an unknown legacy/harness marker.

## Validation and numeric boundary

Run `python3 tools/verify_overworld_bottomless_probe.py`. It pins entry/service/
bounds bodies, point/loop/root writes, constants and exact map-name bytes.
Finite derived classifier cases cover inclusive edges and truncation of
slightly negative projected coordinates. They are reconstructed source rules,
not original execution. SDK projection, current GX viewport provenance and
termination for arbitrary/nonfinite view/input states remain runtime/numeric
dependencies. A native adapter may report an unsupported projection state;
a guessed fixed camera-top height must not be labeled as this source loop.
