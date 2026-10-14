# Field item95 fanfare BGM routing and queries

US G8ME01 disc0 revision0; readonly DOL/stream recovery2026-10-14.
Audio providers for [finite item95 acquisition](../field/GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md).
These are source requests/state queries, not a native audible-parity claim.

## Query returns are not “is playing”

Original BGM group records begin803dffc8,stride38hex(56),selected by
argument&15. Group firstword is current BGM table index,-1 whenunassigned.

`psndBGMChk`800dab84/28 returns **-1 iff groupword0==-1**,otherwise0.
Source itemseq tests this return against0,therefore its psndBGMChk(1)
condition is **group1 vacant**,not “fanfare currently playing.” A portable
callback namedbgmPlaying that returns true whileassigned inverts original
resume timing. Preserve rawreturn or an explicitlynamedvacant query.

`psndBGMChkSilent`800dab44/40 returns0 if groupword0==-1;otherwise1 iff
unsignedgroupbyte26hex==8. This byte is the secondary-volume/muting state,
not currentaudiblevolume==0 or absence ofqueuedPCM. State8 can still be
interpolating toward a nonzero target. Source itemget stores mainflag8
when this original group0query is nonzero.

## Exact wrappers and finite requests

psndBGMOff800daefc/28 calls Off_f_d(flags,1000ms,0).
psndBGMOn800db7a0/2c calls On_f_d(flags,name,1000ms,750ms,0).
These durationarguments are sourcevalues; originalfade conversion uses
GPword4 and SoundSetFadeTime,not a newlymeasuredtimer/hardwareFPS.

| Itemget request | Source group/behavior |
| --- | --- |
| Off(3800) | group0; secondarymute bit800,volume-tableindex3,target40 |
| On(211,KEYITEMname) | group1,force-replace bit10; actualrow217 |
| Chk(1) nonzero | group1unassigned; permit group0restore once |
| Off(201) | group1,ordinaryfade-off bit200,1000ms |
| On(a0,NULL) | group0,continue-existing bit20 + secondary-unmute bit80 |

Off3800 lacks primaryfadeoff200/400: it does not erase the field BGM index.
At800dae00..ae88,when secondarybyte26 already8 it returns without restarting.
Otherwise setbyte26=8,byte27=target40,half28/2a=converted1000ms duration,
then set/clear groupflags40 according to rawflag40 or GPmainflag1. Target
selection uses original SDA table at80416520,index3 word40; it is a volume
percentage envelope request,not silence0 or streamstop. Preserve retained
field streamcursor while this source group remainsassigned.

On211 resolves string against262 original16-byte BGM rows and selects217.
It bypasses same-ID reuse through bit10 and replacesgroup1's oldowner with
normal750ms detachedtail policy. Stream allocation uses originalthreevoices;
resourcefailure/othergroupoccupancy remain admissionowners. No fade-in100bit
is requested. Table217 volume80,pan64,byte14=0,byte15=1 are sourceparameters,
not a guessed equal-volume fanfare. On_f_d invokes psndBGMMain before return1.

On(a0,NULL) resolves numeric0 then bit20 admits and continues **existing**
group0stream/song (SoundSSContinueCh/SoundSongContinueCh),without starting
an empty-name song. Resetsprimaryfade state to0/count1; withsecondarybyte26==8,
bit80 switches it to16 and computesunmute duration/remaining using retained
byte27percentage. Withoutflag40,it clearsgroupflags40 forordinaryunmute.
Exact volume publication/countercadence remains psndBGMMain's existing owner.
Do not restart mapmusic from sample0 or import a different arrangement solely
because the nativeconsumer received NULL.

Itemseq withfreshmain1003: whenSilent(0),setmain8; Off3800 happens onlywhen8
clear. **KeyItemFanfare On211 still occurs regardless of8**,inside the outer
!main8000 branch. In substate3,firstVacant(1) setsmain2000,Off201,then if8
clearOn(a0,NULL). Lateracknowledgment does not repeat those requests once2000
set. Ifack arrives beforegroup1vacant,it instead issuesOff201/conditional
On(a0,NULL) then; source windowack is not blocked untilfanfareends.

## Sound-control flag80 is global, not a per-stream mute

psndSetFlag800dc110/8c ORs requestedmask into globalhalf803df73e.
It then tests **accumulated**bit1 andbit2,not requested80 alone.
Ifbit1set,requestsBGMOff_f_d(0,1000,0),then(1,1000,0).
Ifbit2set,requests corresponding environmentOff_f_d for0/1.
Fresh80 withthosebitsclear adds noextraoff request. Do not fabricate
“mute every stream” from flag80's numericname.
psndClearFlag800dc0f8/18 AND-clears requestedbits only; noresume/restartcall.
Other consumers ofglobal80 and inheritedflags remain separate soundowners.

## Original resource and backend retirement boundary

Originaltable row217 at80316858 bytes:
802ce1e8 /80000000 /802ce1fc /50400001.
Namepointer802ce1e8 namesBGM_FF_GET_KEYITEM1; filenamepointer802ce1fc names
**sound/stream/ff_get2_32k**. Streamflag80000000 selectsSoundSS path; source
SoundSSPlayCh appends/owns its actualstream/header mechanism.

ReadonlyISOfile sound/stream/ff_get2_32k.stm,offset1336940168,size128928,
SHA256847b529744401aa5590b821ff7502a65b1ac59045306dc8792d6d44b4e05f7dc.
Originalheader hasstereo32000Hz,83888samples/channel (2.6215s decodedextent).
Bothchannelheaders setloopflag1,startnibble2/endnibble17682hex; decodedstart0,
end83889 beforedecoderclamp tosamplecount83888. This staticheader is **not**
proof that originalfanfare'sactualSoundSS/requestloopowner plays once or
retires exactlyatPCMextent. Preserve originalloop/requestsemantics; don't
make groupvacant merelybecause a guessedone-shotduration elapsed.

psndBGMMain800db7cc inspectsactualSoundSSGetFlagCh forcurrentstreamowner.
Ifbackendflagresult0,sets groupword0=-1 and skipslatergroupupdates. Onlythat
state publication (or actualexplicitretirementowner) establishes queryVacant.
Header parsing/completion/requestloop controls need their recoveredstream
services; original MusyX/NDSPbufferdrain/fieldmusic ownership remain providers.
Native PCM decode may reuse exactreadonlybytes withoutresampling,keeping
loop/control boundaries explicitlyadmitted ratherthan hiding them in tone
or Switch-track substitutions.

## Evidence and remaining integration

Fresh ignored inspections underartifacts/reference/goomba3-services:
psndBGMChk/Silent,Off/On,Off_f_d/On_f_d,Main,Set/ClearFlag. Original16-byte
row,stringpointers,mutevolumetable and actualSTMheader rereadreadonlyISO.
Existing [threevoice/detachedowner contract](OPENING_TITLE_BGM_OWNERS.md)
owns sharedstreamcapacity/replacementfade; item95 doesnot redefine it.

Next bindoriginalff_get2resources,SoundSS/requestloop/retirementproviders,
retainedgroup0secondaryvolume/resume,actualglobal80consumers andhardwareaudio.
No code/sharedindex/progress,useremulatorprofile/saves orsourceassetcopyedited;
no originalruntime or nativeaudibleparityclaim. Earlier “bgmPlaying” labels
must be read through the corrected vacant-query contractabove.
