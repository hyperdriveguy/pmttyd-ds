# Original ordinary title entry and file-selection input ownership

US G8ME01 revision0 source: `titleMain`80009224, `L_titleInit`80009ae8,
`disp2`80008d30, `pressStartGX`80008888, `fadeMain`80012058,
`fadeSetNarrowFast`80010cd4 and original animation loop reader/writer.
[Read-only source verifier](../../../tools/verify_opening_title.py) pins31
original body, dispatch, bank and constant anchors. Original extracted bank,
TPL, executable bytes and generated GPU packs stay ignored/outside history.

The supported source route is the ordinary demo/mapChange return to title.
Initial title work is zeroed; logoY=1000 and copyrightY=-1000 keep those images
outside the viewport. State0 waits actual source BGM admission and context2
OFF_d_maku_left_right/OFF_d_maku_center resources. Only validated original PCM resource
admission and actual GPU pack admission release this gate. A/START cannot skip it.
The native resource query opens the original title WAV and validates its PCM
header and complete declared payload length. The cached admission is independent
of mute and NDSP playback availability, matching the resource wait's ownership.
Missing/truncated assets never release the gate; this is a native PCM-resource
adapter, not an emulation of the original sound archive loader.

Source entry requests21/0 (normalized15, F0 white) and34/0 with original
center/Z_2. The ordinary title immediately calls fadeSetNarrowFast, setting
slot1 flag8. The initial34 scale field is3, but its first simulation sample is
curve12 from1.5 to1 over90 ticks. Without narrowFast the recovered source path
uses3 to1 over180 ticks. The original zero-span Z_2 descriptor has0..0,
1 nonloop key,1416 original bindings and4 textures. animPoseMain8003eed0..eee0
sets zero-span loopTimes=elapsed-start; it is not an invented completion timer.
Native scene count/loop completion and actual final scale1/pose presentation
are separate gates. The renderer evaluates original sparse geometry, retains
UV wrap/material pass/cull/RGBA and appends one unique vertex range per frame;
frameEnd/frameSync acknowledges only a successfully queued original final pose.

State1 decrements its signed initial-hide counter23 while waiting for34. After
completion it requests20/500, normalized14 white-in, and starts state2. The
original white fade overlaps logo motion; it does not serialize a second wait.
State2 samples curve10 logoY300→120 and curve11 copyrightY-300→-170 for
counter0..60, then transitions on counter>60. State3 resets its counter and
waits strict counter>60 before input. Only then does original prompt amplitude
increase by0.1 per tick, clamped1; A|START mask1100 requests file selection.
B does not select a file. Native prompt alpha retains source
clamp(trunc(amplitude*459*sin(pi*counter/120)),0,255). The original disp2 prompt
counter is render-owned; the native fixed60Hz adapter keeps it in simulation.

Accepted input publishes source state10, which requests63/0 with actual
left_right/A_1 (0..80,14 keys,nonloop,648 original bindings/one texture). State11
waits its actual final pose before requesting26/0 left_right/Z_1 (0..60,
1 key,nonloop,648 bindings/one texture). A single key still has a60-tick span.
State12 waits that final pose and only then publishes file selection. Resource
admission and final-pose acknowledgment are required for both owners; elapsed
simulation alone never selects a file. One press cannot also activate New Game.

The ignored PMTIT001 companion records atlas UV inverse scale, original pose
count and texture count. [Exporter](../../../tools/opening_title_assets.py)
retains source descriptors/geometry/sparse animation and uses original decoded
textures with the shared124px sampler-gutter atlas, then half-size Lanczos
GPU atlas conversion. Center/sides source assets are not aliases. Native outer
projection maps source64x48 curtain coordinates to the304x240 content rectangle
atx48; this is an explicit image-plane adapter, not SDK camera/depth parity.
Logo/copyright use recovered source translations with existing original raster
images. Exact title camera/display ordering remains a native visual check.

The source19-second idle return is now bound. State3 increments its idle
counter only after strict>60 eligibility and after testing A|START; input wins
at the boundary. Strict idleCounter>19*60 enters state20, which requests
33/0 original center/A_2 (0..160,161 nonloop keys,1416 bindings/4 textures) and
10/1000 black plus original BGM Off(512,1000,0). State21 samples curve11
logo120→400 and copyright-170→-400 over60 ticks and subtracts prompt amplitude
0.1/clamped0. Its fadeIsFinish reads the latest slot0 black fade; it does not
wait160 ticks for the earlier paper owner. Actual final black alpha255/frame
completion publishes sequence3/dmo_00 and begins a fresh original demo clock.
Early fabricated black acknowledgments cannot advance the scene.

The original pressStartGX two-texture consumer now uses source US mask16 and
actual mariost.tpl6 RGB pattern. Source TEV stage0 copies mask RGB/A; stage1
multiplies previous RGB by pattern RGB and previous alpha by TEVREG0 source
alpha. A separate six-vertex GPU buffer reuses the existing two-UV shader,
keeping queued title and message vertex ranges independent. Original unit
quad±0.5 scales288x32 at source0,-122; native outer projection maps it to
144x16 at128,173. Texture6 is actual64x4/RGBA8 with repeatS/T. Native64x8
minimum storage repeats the four original rows and samples inverseVscale;
source texture matrix translateS is promptCounter/-200. No generated pattern
or moving sprite substitutes for the texture transform. Exact GX/NDSP numeric
parity and camera projection remain native adapters.

[Title BGM ownership](../audio/OPENING_TITLE_BGM_OWNERS.md) now retains source
3000ms entry group requests, already-running6000 fade and detached750 float
tail, original title88 volume and1000ms idle off. Three fixed PCM owners preserve
reachable overlap without moving queued buffers.

Remaining source consumers: animated title field actors; original seq6-return
64/31 title branch; original full file-menu/card
and sound-effect services. Existing empty-slot menu remains a native adapter.
This increment claims ordinary source title timing/input/curtain/idle/prompt
and audio request ownership, not those pending services.
