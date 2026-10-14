# Original title BGM group and detached-stream ownership

US G8ME01 revision0 `L_titleInit`80009ae8..80009ccc, `titleMain`80009224,
`psndBGMOff_f_d`800dabac, `psndBGMOn_f_d`800daf24,
`psndBGMMain`800db7cc, `SoundSSMain`800dd2e0 and
`SoundSSFadeoutCh`800ddf54. The shared
[title verifier](../../../tools/verify_opening_title.py) pins31 body/dispatch/
constant anchors, including original BGM_TITLE1 table row80315af8 (volume88).
Original executable, stream and decoded PCM bytes remain outside source history.

Ordinary previous sequence3 title entry requests Off(512,3000,0) and
Off(513,3000,0), then On(512,BGM_TITLE1). Source512/513 select group0/group1;
bit0x200 takes the ordinary integer fade-off branch, while missing bit0x100
means no fade-in request. Title starts at original volume88. A new group fade
uses original integer publication baseVolume*remaining/duration, with180 ticks
at the fixed native60Hz adapter. `psndBGMOff_f_d`800dace8..800dacec recognizes
already-off state2 and returns before resetting its duration. Thus an existing
book6000ms fade continues its360-tick duration/remaining counter instead of
restarting a fabricated3000ms envelope. Portable snapshots retain original
base/duration/remaining separately from the truncated current audible volume.

Detached replacement streams do not belong to the current BGM group record.
Their750ms SoundSS fade continues its original floating volume/decrement
publication and zero-state retirement. Off(group1) affects the current market
owner, not the detached book stream. The actual original stream allocator
800db578..800db598 scans channels0..2; fallback800db59c..800db5e0 scans fading
channels before retry. There are three source stream voices, not merely two
BGM group records.

All three voices can be reachable in this ordered opening: an A exit about two
seconds into final book adjustment starts the two-second dmo mapChange white
fade; the four-second market request can arrive during that fade. Title entry
then overlaps title(group0), market(group1) and the newly detached book tail.
`OpeningTitleBgm` preserves the actual incoming demo snapshot, integer group
fade and detached float envelope independently. It does not drop a live tail.
The native consumer maps those three requests to fixed MusicStream owners on
NDSP1/0/18, with three independent files/cursors and three fixed wave buffers
per owner. It adds24KiB PCM storage. Live queued ndspWaveBuf structures are
never moved/copied/reassigned; an owner is cleared only when its actual source
snapshot retires it. Original streams/loop intervals are reused; no generated
audio or assumed fade curve is introduced.

Source title state20 idle return requests Off(512,1000,0); integer title volume
88*remaining/60 falls to0. Original group0 starts no additional replacement
track during that fade. The actual final black/frame completion permits the
subsequent fresh-demo owner. Native stream/header admission remains distinct
from source audible timing or platform parity; its query checks the current
admitted title PCM owner. Muted audio descriptor admission still needs an
explicit resource service and never falls back to always-ready.

Host regressions cover ordinary3000 fade, source6000 preservation, source1000
idle off, original88 volume and all three simultaneous owners with independent
integer/float endpoints. Native/emulator audio mix parity, original file-menu
BGM and original sound effects remain separate consumers.
