# Original US Voyage logo and local ship callbacks

These used-domain providers come from read-only US disc0 revision0 `rel/muj.rel`.
`tools/verify_opening_voyage_callbacks.py` pins the exact original spans for
`logo`0x3b94/0x34c, `draw`0x3ee0/0x1f0, `pressStartGX`0x40d0/0x4d8,
`unk_muj_000045a8`0x45a8/0x1e8 and `anm_yure`0x338a8/0x210. Static source
inspection is not a Dolphin runtime observation or original SDK numeric parity.
The owning Voyage schedule is separate from these callbacks; see
[opening flow](NATIVE_OPENING_FLOW.md).

The logo event allocates0x14 work bytes: resource,+4 Y offset,+8 alpha,+c state,
+10 counter. Every invocation first waits for original `mariost.tpl` async
admission. State0 allocates the file and initializes offset/alpha/counter0.
State1 uses curve11 with duration180 at60Hz. It increments the SAME counter
before alpha interpolation0→255 and again before Y interpolation-50→0. The
last pair therefore uses179/180, leaving truncated alpha254, offset0. After
counter180, it waits global-frame modulo15==0 before state2. This double
increment must not be rewritten as a conventional three-second fade.

State2 requires source GF0 (global flag) plus keys0x1300: A,B or START. It
writes GF1=1, clears counter and enters state3. State3 again double-increments
for alpha255→0 and Y0→100 over duration120, then state4 frees file/work,
writes GF2=1 and returns2. Native `OpeningVoyageLogo` advances once per
simulation tick, exports acceptance separately from completion and admits
resources only after the actual native original texture import. The voyage
owner supplies the original GF0 eligibility; early/unrelated presses cannot
start the departure or satisfy source completion.

`draw` binds camera8 and the source window texture manager, preserves and
restores viewport/projection/camera state, and selects original language
palettes21,21,22,23,24,25. US language0 uses21. Its source position is
(0,100+Yoffset,0), rotation0, scale1 and white RGB/runtime alpha. Native
`frontVoyageLogo` reuses the byte-derived original `title-logo` palette21 and
adapts the440×238 raster and608×480 source viewport by half size. It preserves
the source opacity and Y movement. Original SDK/GX camera-manager bit parity
remains outside this bounded image-plane adaptation.

Although `pressStartGX` retains an original palette15 mask, palette6 scrolling
pattern, white/runtime alpha and horizontal texture scroll globalFrame/-200,
there are zero relocation references to its text section1+0x400c anywhere in
this original REL. `draw` does not invoke it. Helpers retain its source unit
quad/UV behavior, but the used Voyage scene must not invent a visible prompt
registration. A development control hint is not evidence of an original draw.

`unk_muj_000045a8` reads global WORD GW0/1/2 through evtGetFloat as a point
and local WORD LW0 as theta. It computes phi=(5*GW0)/-6000 degrees, constructs
T(0,-6000,0)·Rz(phi)·T(0,6000,0), transforms the original point, writes LW7/8/9
and replaces LW0 with theta+phi. All four writes use evtSetFloat signedQ1024
publication. `openingVoyageLocalTransform` preserves that
matrix order and source float/radian constant; the Voyage geometry owner uses
those published translation/rotation results for `S_fune_m` and
`S_umi_l_siro_b`. GW1/2 must retain the owning script state rather than inferred
field position. `anm_yure` alternates curve11±500/2000ms, divides its local
angle by100, invokes this callback, publishes ship translation/rotation,
updates its original sound position and translates the foam owner. Original
sound/spatial/SDK services remain distinct consumers, never fabricated cues.

The `muj_20_init_evt` prologue branch starts `fune_eff` at0x34668, then writes
GW0=-2000, GW1=0 and GW2=-3000 at0x34674/0x34680/0x3468c before spawning
kaji/wakka/chimney/screw/yure/prologue at0x34698..0x346c0. Float literal words
are respectively0xf22b3a80/0xf24a7a80/0xf21b9a80, encoded from the original
float-immediate namespace base-230000000 and signedQ1024 payload. The approach
later writes GW0 from its-1800→0 curve after the initial4000ms wait; the prefix
must retain the earlier-2000 seed and-3000 Z. `fune_eff`0x4790/0x2a0 creates two
original effects, retains the original ship's center and updates their
positions/routing; it does not initialize these global words. Its source
callback and the entire init event are separately hash-pinned. Actual effect
rendering/spatial sounds are separate consumers outside this callback provider.
