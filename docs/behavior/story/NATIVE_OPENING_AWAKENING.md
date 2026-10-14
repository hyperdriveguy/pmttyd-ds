# Native used awakening and prologue event owners

US revision0 gor evt_mezame at0x66838..66bc0 has92 commands and hash
`ac0a475fff5cc3b12df03184f5df405b5d9d5aea60130062f0f56581807e7955`.
mezame_init_evt at0x66bc0..66cb4 has23 commands/hash
`113a5536b690fd6b8629bdf006c44d4b1eed228e41db13f1c3ef5e2fd7bcdbec`.
[Read-only verifier](../../../tools/verify_opening_wake.py) pins those events,
three used display/motion bodies, message literals, interpolation and waits,
and camMain view translation composition. This is static evidence and native
adaptation, not an original-runtime comparison.

The initializer hides Mario/party and map roots marukongou/densha/pukupuku,
sets WW0=50 and WW1=240, then starts persistent depth, eyelid and camera-shift
callbacks. Native conversion omits exactly those source hidden map subtrees;
map animation services remain separately adapted. Camera eye/aim comes from
original m/gor_10/c through the existing authored-road solver at the retained
sys Mario root(0,0,0). It replaces the invented snapshot eye(0,200,1000).
The source camera callback captures camera4+154, then independently accumulates
`.3*sin(GP1c/50)` each simulation tick. camMain constructs translation from
camera+150/154/158 after its roll/LookAt composition at8000f79c..7cc. Native
simulation retains the Y accumulation and left-multiplies that view translation
at scale0.01. Host/native trig and global clock/cadence remain adapters.

The source awakening waits sequence2,1000ms and fade-end, then3000ms before
mac_0_012. After completion it waits2000ms before mac_0_013. All three message
calls use literal speaker0; no character label or attribution is invented.
After013 it waits300ms, requests WW1 from2400→1200 with curve11/2000ms,
then curve1 blinks1200→2400→1200→2400 at100ms each. Source publication
truncates LW0 atQ1024, divides by10, then truncates the WW destination again.
After500ms it starts parallel WW0(500→0) and root WW1(2400→0), both
curve1/2000ms. Another3000ms precedes mac_0_013_01. Its completion waits
300ms and2000ms before gor_00. Native clocks reproduce these finite requests
with simulation milliseconds and replay missed camera-shift ticks. Source
one-frame yield/WAIT_MSEC scheduling and initial sequence/fade admission remain
explicit integration dependencies; this does not measure original runtime.

mabutaDisp0x10cdc:1f0 emits two opaque black quads with original signed16
vertices(-304..304,240..240-WW1) and(-304..304,-240+WW1..-240).
The native renderer scales these directly to the centered304x240 viewport.
They cover the source world before message submission, so the first waking
messages remain visible over closed eyes.

depthDisp0x10f10:604 copies the already rendered608x480 world to RGB565,
then submits a full original quad with eight texture coordinates. Each stage
contributes1/8 of its sample. UV translations follow the recovered original
quadrant polynomial sin/cos and amplitude(WW0/100)/100. Native rendering
renders the current world into a private512x256 tiled RGBA8 target,
then submits the ordered GPU texture consumer before eyelids/messages. The original
centered304x240 world crop uses the source quad and sampling coordinates. Eight additive GPU
passes retain weights/offsets and the original quad. Capture storage, native
RGBA8/filter precision versus sourceRGB565/TEV quantization, clipping and GPU
frame barriers are backend adaptations. No previous-frame or dialogue capture
is substituted for the world. Texture/render-target admission uses actual VRAM, as required by Citro3D,
and storage is released with frontend phase lifetime. A failed target cannot
be passed to the scene backend. The already synchronized frame owner admits a
new map without a second active-frame FrameSync; phase release retains its
existing outside-frame synchronization.
Host tests check original weights/offsets, effect endpoints, request gates, sparse/dense
camera progression and the exact source quadratic midpoint rather than linear
interpolation.

## Prologue event/display boundary

`OpeningHeading` exposes real display/fade admission fields for the used
sys_01/prologue path. Original root waits3000ms before evt_start; evt_start
waits fade-end and2000ms, requests fade35, waits its completion, calls start
until actual display work reaches its ready state, then waits3000ms before
original A|B|START0x1300 input. Exit waits the renderer's actual completion.
The sequence owner cannot manufacture display/fade readiness from elapsed
time. Host providers explicitly acknowledge these services for event tests;
the native display provider must acknowledge actual resources/work/presentation.
Other chapters are outside this interface's scope.

Original sleep/find/creak/ENV services, actual sequence2/initial fade admission,
full map animation and source audio routing remain separate owners. Original
window/input adaptation is documented in
[New Game dialogue presentation](NATIVE_OPENING_INPUT_AND_FRAMING.md).
