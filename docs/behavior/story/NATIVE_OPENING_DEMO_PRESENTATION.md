# Native opening demo marketplace presentation

US G8ME01 disc0 revision0; static source inspection, not Dolphin observations.
Original dmo.rel demo_evt spans0x23d4..0x3b18,392 commands,
SHA256 `3e3f3b88dd5e9c8268dfdb58cf0df3120b0548814225041879830c3e637bc6c0`.
Original buttonCheck spans0x2354..0x23d4,12 commands,
SHA256 `c178ed3cafa4d6bcc0df930a54bec44c1f412b19ce91bbb8903d1de791601e33`.
Original dmo_00_init_evt spans0x3d40..0x4170,63 commands,
SHA256 `517da311a31b22dbb73a74189d18fe40d7bcef08de5734b13a44f445a5fbc77a`.
See [opening flow](NATIVE_OPENING_FLOW.md) for the unchanged37 payload IDs.

## Source final input guard

The original demo does not automatically request title after op_12. Its root
plays the chest-opening/effect/reaction requests, waits2000ms, then loops with
WAIT_FRAME1 while WW10==0. Independent buttonCheck first waits for fade-end,
polls original button-trigger bits masked0x1100 (A/START), sets WW10=1 and
requests a500ms fade/type17 plus title sequence3. The root later deletes that
child and requests its own2000ms fade/type17/title sequence3. Exact competing
fade/sequence scheduling still requires full original event ownership.

The native port completes every authored message, chest/effect/reaction request
and final wait, then automatically requests its two-second white exit. This is
the user's presentation preference; the original input guard above is retained
as source evidence. A/START still skips earlier; B does not request an exit.
Both automatic and manual exits require the entry fade to finish and an actual
completed opaque GPU frame before Title admission. Original timed message and
control bytes remain intact.

## Portable source-request scene

OpeningDemo.market owns three actors (Peach,merchant,golden chest), engine XYZ,
yaw, current pose requests/start ticks, camera eye/aim, message gates and
source effect requests. The init requests are Peach(−2,0,550)/yaw90,
merchant(−145,0,450)/yaw90 and chest(−125,0,450). Source camera starts at
(0,385,1473)/(0,100,0). After book24, source adjust0→1/3000ms precedes1000ms,
Peach's move to(0,0,450) at source speed60,1000ms and op_00.
`OpeningDemo.music()` changes from book5 to marketplace6 after that initial
3000+1000ms interval, matching the source BGM_DEMO2 request owner; source
book BGM's6000ms fade is consumed by the source-verified portable clock and
two independent fixed native PCM stream owners.

The eight root camera requests retain endpoints and requested durations:
(0,100,834)/(0,50,437)/800ms;
(0,200,837)/(0,200,437)/4000ms;
(0,89,739)/(0,44,442)/0ms;
(−100,89,739)/(−100,44,442)/1000ms;
(−120,89,739)/(−120,44,442)/1000ms;
(−450,250,1030)/(−135,100,482)/0ms;
(−90,89,739)/(−90,44,442)/0ms;
(−85,50,590)/(−85,28,442)/15000ms.
All source interpolation selectors are11. The native camera uses linear
interpolation; selector11 numeric equality is not claimed. revealProgress
retains the0→1 request; original book/paper adjustment snapshots now feed
the mixed native market/book composition.

Original Peach P_B_1→P_B_2 and autotalk P_T_7, P_H_1→P_S_7, and final
P_B_3→P_B_4 requests preserve their explicit wait order. Peach's tribe changes
come from DOL rows0x80332b28/0x80332b7c/0x80332bd0/0x80332c24: standard
P_S_1/P_T_1/P_W_1,standard2 P_S_7/P_T_13/P_W_7,carrying
P_S_8/P_T_14/P_W_8,and anxious P_S_4/P_T_4/P_W_4. The merchant tribe at
0x80332c78 requests S_1/T_1/W_1, but original c_nazo's descriptors are
Z_1/S_1/T_1/A_1. Its unavailable W_1 resolves to original descriptor0 Z_1;
the raw actor request remains W_1. See [source name lookup/fallback](../field/NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md).

Peach's source authored positions0→−55→−80 atZ450, repeated facing reversals,
merchant A_1 for700ms thenS_1, later wall visibility and chest lift/transfer
requests occur before their respective op_00..op_12 gates. The chest's source
sixty WAIT_FRAME1 transfer iterations retain ADD_F Q1024 operands
X+=0.0830078125,Y+=0.166015625. Independent Peach departure/return and chest
tracking continue while the root moves the merchant ±20 at speed40. The
controller keeps those owners independent. Original chest yaw225 and A_1
opening, crystal/radiation request order and Peach's300ms-spaced reactions
precede the final waitingForTitle state. Source crystalN64 subtypes2/6 and
radiationN64 subtype3 now have bounded portable and native consumers with
original geometry, intensity-alpha masks and independent effect clocks. See
[effect ownership](../effect/NATIVE_DEMO_CHEST_EFFECTS.md).

Movement is a native constant-speed/heading bridge using source speed operands;
Peach and the merchant now use the recovered NPC camera-relative facing matrix,
original angle lookup, retained rotation and side-dependent Z reflection, updated
once per simulation tick. This preserves the authored paper side when turning
away; the old binary yaw transform reversed it. Complete original NPC driver,
flag/status/tribe scheduling remains unfinished.
The lift's one-second interpolation and horizontal decrement are represented
by60 native updates, not proven original WAIT_MSEC callback cadence. Millisecond
waits round upward to whole60Hz simulation ticks. Sparse updates replay each
elapsed tick; repeated same-tick calls cannot advance the transfer loop.

## Book and original packs

The source book-open request after book04 interpolates0→1800/2000ms then
divides10 before book_r, followed by explicit1000ms. OpeningDemo exposes its
native0→180 curve11 bridge and postpones book05 accordingly. Source demo_scale
and four original captured-page A_1 clips now have native consumers and explicit
capture/final-pose acknowledgements. See [book presentation](NATIVE_OPENING_BOOK_PRESENTATION.md).
The source cover quads, original page-base/illustration composition and final
adjustment are now consumed natively. Page fronts require an actual completed
GPU composition copy; a missing resource cannot acknowledge capture. Source
outer paper projection and complete camera/window services remain adapters.
See [book cover/capture](../display/NATIVE_OPENING_BOOK_COVER.md).

The original Peach/merchant packs retain19/4 original clips, with descriptor0
first. Chest S_1/A_1 uses original MOBJ_GoldenTreasureBox and its texture bank.
Its wrapped UVs use the existing PMANIM4 surface atlas/subdivision path. The
pack reader/converter now retain source opaque pass1 alongside2/3; no opaque
pass1 is relabeled as2. Original pass1 requires the [source GX opaque state](../display/ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md).
The native frontend renderer consumes these pass/cull/color records, including
opaque pass1 and wrapped chest surfaces. Identical selected pose samples are
cached; root transforms continue to update independently. No substitute mesh, generated art or invented story dialogue is used.

The native renderer consumes actorPose, actor roots/yaw, tick−poseTick and
camera arrays. The dedicated original marketplace pack consumes wall requests.
Source crystal/radiation consumers now use original geometry/masks and clocks;
see [effect ownership](../effect/NATIVE_DEMO_CHEST_EFFECTS.md). Ambient source rotem/Goomba/Chu interrupted walk events, original
background/materials, complete map visibility, paper/fade/audio/effects and
hardware/original-image timing remain unfinished. tools/verify_opening_demo_scene.py
checks all three original event hashes/counts plus30 camera/placement/input/
wait/float/tribe anchors read-only. Host checks cover the full unchanged37-message
unskipped flow, automatic exit acknowledgement, authored gates/endpoints, independent transfer,
source fallback, same-tick/sparse cadence and locally packed original clips.

The dedicated [demo map visibility pack](NATIVE_OPENING_DEMO_MAP_VISIBILITY.md)
now retains the five permanent interior hides and both late wall subtrees.
The extended verifier includes51 source anchors including map alias/flags.
