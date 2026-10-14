# Field BGM secondary boundaries and detached tail

US G8ME01 disc0 revision0; read-only source clarification2026-10-14.
Follow-up to [item95 gains/group fade](FIELD_KEY_ITEM_FANFARE_STEREO_GAIN_AND_GROUP_FADE.md).
Offsets hexadecimal. Numerical examples are finite source-derived cases under
ordinary binary32 rounding, not observed audio or hardware timings.

## Secondary volume publishes before counter changes

In psndBGMMain800db7cc, secondary state is groupbyte26,duration signedhalf28,
remaining signedhalf2a,target percentage unsignedbyte27. For state8 or16,
read signedremaining R and signedduration D,compute signed32 R*127/D with
truncation toward0,then **low8** for secondary factorQ. ApplyQ to primary
byteB with trunc(B*Q/127),low8; then globalpercentage with trunc(value*G/100),
low8; publish streamvolume. No counter increment/decrement occurs before this
publication. NeitherQ nor output is a saturating0..127/0..255 clamp.

After primary fade processing, reload secondary state. State8:
compare current signedR to targetT=trunc(signedD*unsignedpercentage/100).
R<=T preserves R and state8. R>T and groupflags40 clear decrements signedhalf
by1. With40 set subtract5,storelow16,reload signedhalf,recomputeT from fresh
D/percentage,and clamp resultingR belowT toT. State8 is **not** cleared when
its target is reached; Silent query remains true for assignedstate8.

Then reload secondary state again and process16 only if exactly16. State16:
if signedR>=signedD,clear statebyte26=0,without changing R. Otherwise add1
(or5 if40set),storelow16. Only the +5 branch rereads/clamps overshoot toD.
The +1 branch does not clearstate when incrementfirstreachesD. A laterupdate
publishes that retainedstate16/RD endpoint before clearing it. Stable8 cannot
fall into16 during the same call; a mutatingprovider belongs to its source
fresh-read domain and must not be silently snapshotted in a general emulator.

For fixed D60,percentage40,B80,G100 and groupflags40 clear:

| State/inputR | Q | Publishedvolume | Counter/state after call |
| --- | --- | --- | --- |
| 8 /24 | 50 | 31 | R24,state8 |
| 8 /25 | 52 | 32 | R24,state8 |
| 16 /24 | 50 | 31 | R25,state16 |
| 16 /25 | 52 | 32 | R26,state16 |
| 16 /59 | 124 | 78 | R60,state16 |
| 16 /60 | 127 | 80 | R60,state0 |

On(a0,NULL) itself calls psndBGMMain before returning success. WithD60,
savedpercentage40 it seedsR24/state16; the synchronous call can publish31
and leaveR25 before the next outer sound tick. Omitting this call moves the
whole source envelope by an update. Original GPvalue/sourcecadence remain
providers rather than hardcoded render-frame timing.

## Primary fade completion captures the last published stream value

Off201 normal1000ms/GP60 seeds primary state2,D60,R60. Each BGMupdate
publishes low8(trunc(80*R/60)) before decrements. InputsR60..1 end with
published1 atR1. That same call decrementsR to0,requests
SoundSetFadeTime(0,100),SoundSSFadeoutCh(currentvoice),then publishes
primary state0/groupindex-1. Source does not issue streamvolume0 first.
This is the finite live,row217,secondaryfactor127/global100 domain; other
source factors may produce a different actual last streamvalue.

SoundSSSetVolCh stores each requested low8 volume as binary32 into stream
float10(current) andfloatc(target). Therefore the finite lastrequest1 leaves
currentfloat1 before detachedfade construction. Stereo pan64 retains both
channel gains1. Retain that **actual** currentfloat as the tail's start;
do not restart a detachedtail fromtablebase80 or cached .65 platformgain.

## Exact floating detachedfade construction

SoundSetFadeTime stores100 as unsigned16 sharedhalf20a. Admitted
SoundSSFadeoutCh800ddf54 reads unsignedduration, signedGPword4 and uses:

```
interval = single(1000.0f / single(signed32(GPword4)))
count = single(single(unsigned16(duration)) / interval)
denominator = ordered(count < 1.0f) ? 1.0f : count
step = single(fresh stream.float10 / denominator)
```

Store step tofloat14,then freshlyreadflags andOR20. There is **no** integer
conversion,countdown,decrement,currentvolumechange or free in this callback.
Constants1000/1 at80421910/14 are447a0000/3f800000. Don't reassociate to
D*GP/1000 or replacecount with signedintegerfloor.

For ordinary GP60,duration100,startfloat1:
interval16.66666603088379,count6.0,step0.1666666716337204.
Source step uses the captured actualfloat,not the mix's truncated integer.
An inactive flags0 voice receives no fade-state writes.

## Tail update and final stop ordering

SoundSSMain admits a row only with initialmask8 set andmask2 clear; pending
stopmask4 has priority. Otherwise source processes flags40,then20,then10.
Finite unmuted fanfaretail has only20 among those modes. For20:

1. Freshly read flags,then currentfloat10; if exactly0,clear20 and execute
   original inline stop immediately. No subtraction/mix on this prezero path.
2. Otherwise single subtract step14,store/reread currentfloat10.
3. If orderedresult<=0,clamp topositive0. Do not clear20 yet.
4. Issue original inline mix through signedfctiwz of channel gains. Then
   check any fresh10 mode; clean finitefanfarerow hasnone.

Finite six subtraction values fromstart1 are approximately
.8333333135,.6666666269,.4999999404,.3333332539,.1666665822,0.
All six convert to integergain0 in this tiny-tail domain,while voice/resources
remainretained. The seventh admitted update seesprezero and stops.
These are source-derived invocation counts; they are not a measured100ms
speaker fade or evidence every tail begins1. Other40/10 modes can subtract
or add again in the same invocation and change stop timing.

Inline stop publishes rowflags0 before deferredDVDclose and primary/secondary
MusyXfree calls,with sourceIRQcounter/marker order retained. See
[complete stream-main contract](GAME_STREAM_MAIN_ORDERED_FADES_AND_INLINE_STOPS.md)
and [fade construction](GAME_STREAM_FADE_STOP_AND_DEFERRED_CLOSE.md).
Groupalreadyvacant must not retain ownership ofthis detachedvoice; its actual
stream owner controls the remainingtail. A replacementgroup may useanother
oforiginalthreevoices whiletailresources arelive.

OriginalouterpsndMain calls BGMMain beforeSoundMain→SoundSSMain. Therefore
construction during groupfadecompletion can be followed by the first tail
subtraction in the **same** admitted outersoundupdate. Rendering timestamps
alone do not establish this cadence. Pause/init/IRQ/async/device buffering,
other sharedSoundSetFadeTime writers and arbitrarymutatingproviders remain
separate source/native integration concerns.

## Evidence and scope

Fresh psndBGMMain instructions800dbb4c..dbc2c pin bothsecondary boundaries;
existing source disassembly underignored artifacts/reference/goomba3-services.
Fresh arithmetic examples retain binary32 rounding for step/subtractions.
Linked complete SoundSS contracts pin gate/fade/stop/IRQ/release order.
No code/sharedindex/progress,sourceassets,userprofiles or saves changed.
Native provider binding and physical New2DS XL audio validation remain open.
