# Complete local audio cover-state requests from the disc gate thread

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete SoundOpenCover800e1474/20,SoundCloseCover800e1354/120 and
sndIsInstalled8028dc04/8. Verify:`python3 tools/verify_event_readiness_gate.py`.
Read [disc gate producer](../platform/EVENT_DISC_THREAD_READINESS_GATE.md). Static original
DOL evidence;no original music/stream/cover execution or playback observation.
These leaves request state changes;the downstream audio engine remains separate.

## Open request

Sound work base is803e0138. Read U16 cover-state at+214 (803e034c). If nonzero,
return without mutation. If exact0,store U16 state1,then return. No disc/audio
SDK query,stream iteration,pause/resume command,file close,effect cancellation,
volume change or sample start in this local leaf. Every nonzero state—including
2—is preserved. Scalar return has no specified contract.

## Close request and complete three-record predicate

Call sndIsInstalled first. It simply loads unsigned byte8041ee60=r13+1f40;
it does not query a device or normalize the stored value. If return0,exit before
sound-work state read. Otherwise load work.coverState214;unless EXACT1,return
unchanged. This differs from Open's “any nonzero” gate.

For state1,load stream-array pointer work.word100. No null guard. Inspect EXACT
three records at offsets0,138,270 (stride138). Candidate flag is initially0;
each record can set it1 but later records cannot clear it. For each record:

1. Read unsigned16 flags at record+0. Require bit8 SET,bit400 CLEAR,bit2 CLEAR.
   If these gates fail,skip the rest of that record.
2. If flags bit1000 set,set candidate1.
3. Read U16 record+1a. If exact2 AND flags bit2000 set,set candidate1.

The second flags predicate reuses the record's initially read flag word. This
is not a second read,despite record fields being tested separately. After all
three,write work.coverState214=2 if candidate!=0,otherwise0. No other record
mutation or stream call. Records1/2 apply the same offsets relative to their
bases;the unrolled three-record layout is directly evidenced,not a runtime
array count or native player-limit choice.

No scalar result contract,stream pointer/size validation,installed-state
recheck,asset reload,save,actual audio-resume instruction or callback occurs.
Open requests1;Close can select2 or0;it does not change an existing2. Sources/
consumers for state2,stream flags,+1a and installation byte remain required.
Do not describe these state writes as measured “music pauses immediately” or
“music resumes immediately” without downstream/trace evidence.

## Caller ordering and implementation acceptance

Disc thread reads GP10 to choose Open versus Close,THEN may clear GP10 for
card execution. No second Close request after that clear in the same iteration.
State transition/output can therefore lag its updated GP gate,according to the
next iteration and downstream audio service. It is not automatically coupled
to event deletion,render profile fields or action-command timing ticks.

Cover Open0/nonzero1/2,installed0/nonzero,Close state0/1/2;all three flag/state
combinations,bit8/400/2 admission,1000 and state2+2000 candidate tests,any-record
latched outcome,retained flags/live+1a,no stream null fallback and no immediate
SDK play/pause call. Original audio assets/music/sound effects remain reused
per project instructions. Downstream playback,state/installation writers,
platform/stream/source domains and original cover/audio traces remain open.
