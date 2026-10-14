# Used original opening voyage scene

US GameCube G8ME01 revision0, static source evidence plus native simulation and
GPU resource consumers. The source `muj_20_init_evt` and `evt_prologue` include
more than a ten-second native input delay. [Read-only verifier](../../../tools/verify_opening_voyage_requests.py)
pins seven whole event hashes,15 used requests and the source map-play body/
completed-pose branch. [Callback contract](NATIVE_OPENING_VOYAGE_CALLBACKS.md)
pins the original logo and local ship transform; no unused prompt callback is
invented as a visible scene.

Source init34674/80/8c seeds GW0/GW1/GW2=(-2000,0,-3000), then spawns the
five ship animation subevents and prologue. The first animation requests a
translation with no extra Y rotation; `anm_yure` waits1500ms before its first
curve11 sway request and original Y rotation180. The native
[scene owner](../../../include/story/opening_voyage_scene.hpp) retains this prefix.
Its source finite schedule is:

| Source request | Relative source time |
| --- | --- |
| BGM_OPENING1 and ENV_OPN_MUJ1 | after500ms |
| Original logo callback and first GW0 travel sample | after another3500ms |
| GW0 curve11 -1800→0, with GW10 per-sample delta | 10000ms |
| GF0 input eligibility | after the final travel sample's original WAIT_FRAME1 |
| Optional BGM_EVT_SAILING1 | five1000ms sibling waits after that travel finishes, unless GF1 was accepted |
| Original A/B/START logo acceptance | source state2, GF0 nonzero and keys0x1300 |
| GW0 curve11 0→1800 | 10000ms from acceptance, runs in parallel |
| BGM/ENV fade off and sys_01 map request | 4000ms after acceptance |
| Source map exit17/800 | native white opacity and actual final opaque-frame acknowledgment before heading |

Counter/start/keys use simulation ticks with a60Hz millisecond bridge; GPU
acknowledgments cannot replace simulation time. The logo retains its original
double counter increments and global-frame modulo15 eligibility owner. Ship
wheels/chimney/screw advance by the original2×GW10 and source wrap request;
local root placement is the original translated Z rotation around -6000Y,
followed by Q1024 publication. Camera source eye(0,367,2093) and
at(0,343,-115) replace the former native(0,500,1400)/(0,0,0) snapshot.
Camera4 uses original default25FOV with the explicit handheld304×240 crop of
its400×240 projection, retaining the original effective19/15 aspect.

Original `yoru_hiru` is requested with map-play **mode2**, which backdates its
start timestamp by1000×duration/FPS at80029fdc..a038. It therefore supplies
its completed original250-frame sky pose immediately. Mode1's loop bit is a
different branch. The native exporter now retains that actual completed sky
pose, rather than displaying the unrequested initial night/arch orientation.
The original sun's additional engine Y=-260 composes in local coordinates
through the already rotated original parent; it raises the displayed sun
instead of subtracting an invented global viewport offset. Runtime wave/cloud
clips have disjoint mesh ownership and retain their original resource data.
The original `nami_02` overlapping mesh channel and full map-animation ordering
remain separate; ordinary `nami` wave and `kumo` material channels are retained.

Original geometry and textures remain private local ISO derivatives; there is
no `b/muj_20.tpl`, so original map sky geometry owns this scene. NPC/lighting/
multilayer GX material services, full environmental/MusyX/spatial sound and
original BGM six-second fade consumption, generic image projection, exact
runtime parity and hardware profiling remain incomplete. A matched isolated
Dolphin fresh file now confirms the authored small hull and world cinematic
crop; [observations](../../MECHANICS.md) record the bright original daytime sky.
The opt-in native S_hai01 bake consumes original named pointLight9 RGB20/30/87,
intensity1.5, no distance falloff and original S8/64 normals with normalized
inverse-transpose directions. The first static-light bake was darker than the matched original. A bounded
original live-MEM1 observation and original mapMain light-color publication now
close the missing completed yoru_hiru light-color track: record+28 selects
104-byte keys, final RGB publishes54/104/202 and intensity1.5. The native
converter consumes that original track, preserving raw-source provenance rather
than adding a palette. General dynamic lighting/GX numeric state remains open. The Voyage-only world crop is the measured half-size
EFB48..432 adaptation, not complete camera8 lifecycle ownership. These
are explicit remaining services, not claims that the whole scene or remake is
bit-for-bit equivalent. Actual native resource failures stop progression.
