# Original field BGM envelopes and native item-get audio

Finite source normal group1 Off201 and group0 Off3800/On(a0,NULL), plus clean
SoundSS detached flag20 tail. Source [gain/fade arithmetic](FIELD_KEY_ITEM_FANFARE_STEREO_GAIN_AND_GROUP_FADE.md),
[boundary/float ordering](FIELD_BGM_SECONDARY_BOUNDARIES_AND_DETACHED_TAIL.md),
[plaza row67 owner](ROGUEPORT_PLAZA_INIT_BGM_AND_ITEMGET_RESUME_OWNER.md),
and [global initialization/slew](FIELD_BGM_GLOBAL_PERCENTAGE_INITIALIZATION_AND_SLEW.md).
These providers do not make the Goomba3 acquisition playable by themselves.

`FieldBgmEnvelope` binds actual base/global percentage and bypass metadata.
Source fresh normal On counters initialize1. The positive duration admission
performs single(milliseconds / single(1000 / GP)), signed32 truncation, low16
narrowing and signed16 reload. Nonpositive narrowed/signed32-overflow values
are refused; positive low16 wrapping is retained. There is no clamp introducing
a fabricated source tick. Full source unsupported primary states and mixed
primary/secondary fade modes need separate owners.

`main()` is one admitted psndBGMMain invocation. It publishes low8 integer
primary volume, quantized secondary factor and signed global percentage before
advancing counters. Off201 repeated while state2 preserves counters. Base80/D60
publishes80..1, then marks group vacant and requests detached100ms tail. It
never emits primary0 before detaching. Off3800 retains state8 at target40%;
D60/R24 produces factor50 and base80 volume31, or original plaza base70 volume27.
Resume resets normal primary counters1, switches8 to16 and seeds from the
source retained40%/new duration. R59 publishes before increment to60; the next
R60 publication clears16. Fast flag40 advances5/clamps the endpoint. Native
ordinary A0 passes no fast flag. Silent query tests assigned group/state8.

`FieldStreamFade` handles clean source20: captured float current/float count,
minimum denominator1 from ordered count<1, subtraction/clamp, integer mix and
PREVIOUS-volume-zero stop. Start1/100ms/GP60 computes step0.1666666716337204;
six single subtractions publish integer0 while retaining the owner, then the
seventh admitted call stops. Gate denial preserves state. Source flags40/10,
IRQ/device/decoder buffers and special-value domains remain separate services.

Native `fadeKeyItemFanfare` starts original primary envelope;
`tickKeyItemAudio(GP)` performs group0 before group1 BGMMain, then clean tail
SoundSSMain. Each call is an admitted simulation/audio invocation, independent
of rendering. Logical fanfare slot becomes vacant at detach, while its existing
file/buffers/physical slot are retained by a separate tail owner. The first tail
subtraction can happen in the same outer call. Natural producer/drain retirement
and explicit backend release remain serialized with refill/teardown. Fresh
provider admission refuses another start during its clean tail; general source
forced replacement/arbitration is still unbound.

`retainItemGetFieldMusic` attaches source metadata to the existing live stereo
field slot/track. Repeated retention preserves state; slot replacement clears
binding. Caller must supply original area/context/row/global state, not infer
these from a native sound-toggle boolean. Source normal plaza story0..55 uses
row67/base70/pan64/bypass0. Fresh completed source psndInit and its admitted flags
establish global100; imported/live state retains its actual percentage. Exact
source signed-byte slew/update gates are documented but not implemented here.

`muteItemGetFieldMusic` uses wrapper1000ms and source3800 counters.
`resumeItemGetFieldMusic` uses wrapper1000ms/current GP, resets counters and
performs synchronous BGMMain before returning. `beginKeyItemFanfare` also runs
synchronous BGMMain, matching source On_f_d; its native default GP60 is an
explicit adapter and callers may supply the rate. Synchronous calls do not also
advance SoundSSMain. Resume contains no file open/seek/startMusic; the authored
stream cursor stays retained. Source row67 pan64/source volume mixes use exact
single3c010204 without the ordinary native .65 adapter multiplier after binding.
The clean live source Continue10 clamp does not change the published current
volume; complete pause/combined stream flags and device-continuation behavior
remain unbound.

Final GameCube `romfs/rogueport.wav` is original stg_gor2 PCM3284008frames,
SHA256d83abec5e38338a1740e5644523bcbddf34fc879d0af4a6906a6aeba7897fc6d.
Makefile's final audio_map/config step overwrites an older build_assets gor1
intermediate. Configured Switch track selection remains its existing presentation
adapter; original-mode isolated validation explicitly uses SOUNDTRACK=gamecube.
Area identity, late-story music branches, source scene/global flags and original
message/event dispatch must be admitted by their callers. Source backend natural
vacancy can precede final audible samples; native drain/source-device scheduling,
output-mode/aux/gain parity and physical3DS performance remain unverified.
