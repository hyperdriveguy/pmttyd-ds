# Demo fades, title, audio and golden-chest effects

Implementation: [demo exit](../../include/story/opening_demo_exit.hpp),
[title](../../include/story/opening_title.hpp),
[demo BGM](../../include/story/opening_bgm_scene.hpp),
[title BGM](../../include/story/opening_title_bgm.hpp),
[prompt coordinates](../../include/story/opening_title_prompt.hpp),
[chest effects](../../source/story/demo_chest_effects.cpp),
[effect API](../../include/story/demo_chest_effects.hpp).
Evidence: [fades](../behavior/display/NATIVE_OPENING_DEMO_FADES.md),
[ordinary title](../behavior/display/NATIVE_OPENING_TITLE.md),
[audio owners](../behavior/audio/OPENING_TITLE_BGM_OWNERS.md),
[effect requests](../behavior/effect/NATIVE_DEMO_CHEST_EFFECTS.md).

Fresh demo entry waits resource admission, then type9/500 ms black fade with
actual zero-alpha presentation. Only then can A|START (`0x1100`, not B) request
exit. Configured type17/500 or 2000 is distinct from actual dmo mapChange override
type10/2000, F0-white RGBA, sequence3/title. Sparse ticks replay binary32
elapsed+=1000/60: entry driver ends only ratio<0, exit only ratio>1. Equality is
still in progress. Driver completion and actual final opaque presentation are
separate gates; headless callers must explicitly supply real admission evidence.

Title implements the ordinary demo-return route, not all title/file/card branches.
Admission methods receive claims from actual resource/presentation owners; they
are not timers or proof of GPU completion. Muting and audible playback availability
are distinct from validated music-resource readiness. One tick is one supplied
60 Hz simulation invocation; BGM/prompt/fades advance before state admission.

| State | Reached work / gate |
| --- | --- |
| 0 | Wait music/resources, then begin ordinary curtain entry |
| 1 | Wait actual type34 final pose/scale, then overlap white-in and logo motion |
| 2 | Curves10/11 logo300->120/copyright-300->-170, sample0..60; transition >60 |
| 3 | Another strict >60 delay, then prompt/input; input precedes idle timeout |
| 10/11/12 | Request63, wait final pose, request26, wait final pose, complete files path |
| 20/21 | Request33 and independent black fade; actual final black presentation completes idle return |

Curtain34/Z_2 has zero animation span but still needs 90/180 native ticks, loop
completion and actual scale1 presentation. Used other spans are 33=160, 63=80,
26=60. A descriptor with one key need not have zero span. Changing curtain resets
its admission/publication. Idle timeout is strict >19*60 after input eligibility;
A|START wins on that boundary. Idle black completion need not wait the earlier
160-frame paper. White500/black1000 fades retain repeated float steps and strict
endpoint tests.

Prompt amplitude changes by .1; alpha is clamped truncation of
`amplitude*459*sin(pi*counter/120)`. Original disp2 counter is draw-owned; here it
is explicitly simulation-owned. Pattern texture6 translates S by counter/-200
under the unchanged original US mask. Source unit quad scales288x32 at0,-122;
this helper does not generate a replacement pattern or acknowledge actual drawing.

Track4/5/6 are native catalog title/book/market slots, not original BGM enum IDs
or hardware voices. Demo group fade is integer base90 over360 ticks (6000 ms).
Replacement at240 ticks starts market77 without mode0x201's absent fade-in bit
0x100, and detaches current book volume30. Detached tail subtracts30/45 in repeated
binary32 steps with separate zero-state retirement; do not combine it with the
integer group envelope. Title retains incoming fade base/duration/remaining
rather than resetting an existing 6000 ms fade to default3000/180 ticks.
Title88 idle fade uses1000 ms/60 ticks. Three snapshot owners preserve possible
simultaneous title/group/detached overlap; snapshots are requests, not audible
playback or NDSP-buffer readiness.

`PMDEFX01` exports 692 native payload bytes after magic: 11 converted vertices,
16 deterministic five-word ray rows, 25 float constants and two colors. A native
vertex is24 bytes; original N64 vertex is14, with signed16 XYZ/ST and fraction5
ST divided32 during conversion. Exact layout/EOF and finite floats are checked
before process catalog replacement. This is a bounded demo resource consumer,
not the global effect pool or complete type validation.

REL request offsets `0x3994/0x39d4` create crystals2/6 at scale.5, durations0/60,
camera5; `0x3a24` creates radiation3 later. Creation runs the first Main update
immediately, following event-before-effect source ordering. Sparse updates replay
all ticks; draw never ages effects. Type2 sentinel100 never decrements, uses
215+15*sin alpha and two rotating texture109 quads. Type6 decrements from60 on
creation, multiplies scale by1.1, uses alpha160/texture110 mask and deletes only
below0. Sixteen-step age/lifetime fade preserves integer operation order.

Radiation has16 deterministic rays with life32+2*i, no RNG; age/life<5 times50
controls scale. Life<=0 resets before the same update's Z-angle addition.
The used branch ignores request scale.5. Matrix order is T*RX*RZ(z+45)*RY*S;
ordinal width factors and original triangle alpha gradient remain. At most19
read-only draw records cover three crystal layers plus16 rays. Camera4 yaw is
bridged into camera5 requests; trig, matrices, outer GPU state and hardware parity
remain native dependencies. See [demo/book](opening-demo-book.md).
