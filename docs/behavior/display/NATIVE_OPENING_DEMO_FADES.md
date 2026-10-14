# Original fresh-demo entry and title exit fades

US G8ME01 revision0 source anchors verified read-only by
[the verifier](../../../tools/verify_opening_demo_exit.py). No original executable
or texture bytes enter source history.

`demo_init` (dmo.rel1c44..1d64) waits for actual `demo.tpl` file admission, then
writes gp+108=9 and gp+10c=500. `seq_gameMain`80007458..80007460 reads those
enter-fade fields and calls `fadeEntry`. Color8041f4a0 is000000ff. Source type9
uses `fadeMain`800120b8: repeated f32 elapsed +=1000/FPS, alpha ratio
1-elapsed/duration; only ratio<0 sets the original finish flag. At60 simulation
ticks per second the500ms request finishes on tick31 with repeated float
rounding. Source `disp_texture`80011ee8..80011f00 truncates255*ratio.
The native adapter queues a black rectangle over the original608x480 content
viewport adapted to304x240 at native x48; it admits only after actual original
front-end textures succeed and acknowledges zero-alpha completion after actual
frameEnd/frameSync. Elapsed time alone never releases `buttonCheck`.

`buttonCheck` starts with `evt_fade_end_wait`, then tests the original A|START
mask1100. It writes WW10=1, requests mapchange(1,17,500,-1,-1), and publishes
sequence3/title. At the final main-event gate, `demo_evt` additionally deletes
the button child and publishes mapchange(1,17,2000,-1,-1)/sequence3/title.
`seq_mapChangeMain`80007cd8..80007ce4 unconditionally requests17/2000 when the
previous map prefix is dmo, overriding the child500. `fadeEntry` normalizes17
to10 and colorf0f0f0ff (8041f77c); `fadeMain`800121c0 advances elapsed/duration
and finishes only ratio>1. The repeated f32 clock finishes on tick121 at60Hz.
Only actual opaque final top presentation/frame completion permits portable
`finished` and `Frontend::demoComplete`; B does not request exit. The input
trigger is consumed in Demo and cannot also select a title/menu item.

`OpeningDemoExit` retains source request IDs, configured/actual durations,
sequence/destination, entry/exit driver state and separate presentation gates.
A headless consumer must supply explicit renderer acknowledgments; it cannot
use a readiness timer. The original portable narration/actors stop on exit
request; independent BGM fade clock continues. Existing original37-message
sequence regression validates final A/START exit request and delayed publication.

The conditional mapChange restoration fade28 is not this fresh-demo producer:
80008480 requires global bit0 and800084ac additionally tests swByteGet(4).
Earlier investigation incorrectly generalized that restoration branch. No
left/right curtain is authored or exported for fresh-demo entry.

Original title entry21/34/20, curtain/input gates and idle return are now bound;
see [title ownership](NATIVE_OPENING_TITLE.md). Remaining: source
scheduler-wide autonomous actor/effect ownership during mapChange, and measured
native screen adaptation parity. This increment does not claim those services.
