# Key-item fanfare stereo gain and group fade

US G8ME01 disc0 revision0; read-only DOL recovery2026-10-14.
Complements [routing and resource](FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md)
and [container retirement](FIELD_KEY_ITEM_FANFARE_CONTAINER_LOOP_AND_RETIREMENT.md).
This pins original control/mix requests, not measured loudness or hardware parity.

## Row217 volume and original dual-channel pan

Row217 at80316858 supplies volume80,pan64,surround0. Original On stream
path calls SoundSSSetVolCh(voice,80),SoundSSSetPanCh(voice,64),then
SoundSSSetSrndPanCh(voice,0). Original type2 owns separate primary and
secondary MusyX stream IDs for the authored stereo channels.

SoundSSSetVolCh800ddc1c low8-truncates requested volume and stores its
binary32 value to stream row+10 and+c. On an admitted active state it
forwards the retained pan through the same dual-channel split as
[pan/surround setters](GAME_STREAM_PAN_SURROUND_CACHE_AND_REQUESTS.md):

- Pan P<64: primary=W; secondary=single(single(P*W)/63).
- Pan P>=64: primary=single(single((127-P)*W)/63); secondary=W.

The source divisor is **63**, exact constant8042190c=427c0000.
Both63 and64 therefore retain full W on both sides. For row217 P64/W80,
primary80 and secondary80 are correct; division by64 would invent asymmetric
78/80 gains. Signed fctiwz truncates each channel gain before child requests.

Primary sndStreamMix receives original primary ID,gain80,pan0,surround0,aux0;
secondary receives original secondary ID,gain80,pan127,surround0,aux0.
Preserve separate authored PCM channels and hard-pan requests. Do not collapse
stereo to mono and then apply a single centered pan formula.

[MusyX hardware mix](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md) publishes
effective volume through exact single constant3c010204 (approximately1/127),
with effective pan/surround and aux fields supplied separately. Source80 maps
to single(80*that constant). No extra .65 master multiplier occurs in these
owners. A native .65 mix gain is a separate platform volume preference/adapter;
it must not be described as this recovered original request.

Stereo output mode retains pan0/127. Original mono mode can override both
requested pans to64 through CheckOutputMode; output-mode preferences and
hardware pan/DSP gain consumers remain distinct owners. Thus diagonal NDSP
left/right PCM mix at80/127 is a bounded stereo adapter, not proof of complete
MusyX output-mode/aux/mix or physical loudness equivalence.

## Original Off201 group-envelope arithmetic

Off201 selects group1 and primary fade-off bit200. Wrapper passes1000ms.
For a live group, source psndBGMOff_f_d uses binary32 conversion shape:

```
D = signed_trunc(single(float(milliseconds) /
                        single(1000.0f / float(GPword4))))
```

Store low16 D to group half22 and reread signed16 into remaining half24;
set primary fade state byte20=2. Ordinary GPword4=60 derives D60 for1000ms;
this is a source arithmetic case, not an observed native/hardware clock.
Already state2 returns without restarting duration. State4 becomes2 without
reinitializing its retained counters. No source count clamp is added here.

On each admitted psndBGMMain call, for state2 read remainingR and durationD:
first derive primary byte volume low8(signed_trunc(baseVolume*R/D)). Apply
secondary envelope and global percentage as below, then publish to the stream.
Only **after volume publication**, decrement remaining as signed16; when
result<=0, request SoundSetFadeTime(0,100),SoundSSFadeoutCh(voice),set primary
fade state0 and group index=-1. The100ms detached stream fade/free belongs to
its stream owner; group vacancy does not imply all tail PCM is gone.

For ordinary base80,D60,R60..1 the group sequence starts80,then78,77,76,
and ultimately1. It does not publish a primary-volume0 step before detaching
on remaining reaching0. Repeated Off201 while alreadystate2 preserves its
ongoing fade. A native float interpolation80→0 followed by immediate free
would change source integer publication and detached-tail lifetime.

## Secondary attenuation and resume arithmetic

Source psndBGMMain computes secondary factorQ:
state byte26==0→127; otherwise low8(trunc(remaining2a*127/duration28)).
Then combines low8 primary byteB with Q using signed arithmetic equivalent
for the ordinary nonnegative domain to trunc(B*Q/127),low8. Globalpercentage
G is100 while group byte10!=0; otherwise it uses source shared signedbyte58.
The stream volume request is low8(trunc(combined*G/100)). Fanfare row byte15=1
initializes group byte10=ff, keeping its G100 in this local envelope domain.
Original output mute/preferences and transitive hardware routing remain separate.

Off3800 initializes secondary state8,targetpercentage40,durationD,
remainingD. Its publication occurs before decrement; decrement stops at
trunc(D*40/100),preserving state8. Without groupflag40 decrement1; with40,
subtract5 and clamp to that target. At D60,targetremaining24 produces
Q=trunc(24*127/60)=50. For field base80/global100, final combined volume is
trunc(80*50/127)=31; do not assume mathematically exact32 from40% of80.
The actual source field track can have a different basevolume.

On(a0,NULL) continues the retained stream and,when secondary state8,
switches to16,sets durationD and initialremaining trunc(D*savedPercentage/100).
Without groupflag40 increments1 after publication towardD; with40 increments5
and clampsD. Once remaining>=D on a later update it clears secondary state0.
Again preserve integer byte publication, authored stream cursor and separate
backend continue fade behavior. Neither request is a map-track restart.

## Update ownership and evidence

Original psndMain800dc48c requires nonzero init_f. It calls psndBGMMain,
psndENVMain,psndSFXMain,then SoundMain, in that exact order; SoundMain later
calls SoundSSMain. On_f_d also invokes psndBGMMain before returning success.
Therefore a private renderer-frame timer or one generic call per render is
not the original envelope ownership. Preserve source-admitted updates in the
simulation/audio service and document native fixed-rate cadence as an adapter.

Fresh original inspections retained ignored under artifacts/reference/
goomba3-services: psndBGMOff_f_d/On_f_d/Main,SoundSSSetVolCh/SetPanCh.
Constant1000 at804218cc and63 at8042190c read directly from source ISO.
Existing complete pan/MusyX/outer-sound contracts supply nested owners.
No code, assets, shared index/progress, emulator profiles or saves changed.

Remaining: native row217 resource/export/stream integration, actual source
field group0 base/cursor/global-volume state, detached100ms tail ownership,
output modes/aux/hardware pan gain and original/native audio traces.
