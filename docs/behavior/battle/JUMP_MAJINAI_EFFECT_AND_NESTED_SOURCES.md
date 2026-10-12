# Jump Majinai effect-name polling and complete nested source graphs

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete _battle_majinai_effect80114614/b0,effSetName8005ebec/7c,
effNameToPtr8005e928/98,btlevtcmd_ftomsec80103dc0/50;complete Jump child
80363414/ac and sound root80306874/10c. Verify:
`python3 tools/verify_jump_majinai_child.py`. Read
[power-check graph/state](JUMP_MAJINAI_POWER_CHECK_CHILD.md),
[Jump motion callback](BATTLE_JUMP_EVENT_CALLBACKS.md),
[run-root ownership](../event/EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md) and
[USER_FUNC protocol](../event/EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md).
Renderer/audio/announcement/transitive domain bodies remain separate.

## Effect callback ignores its script actor operand

Retain event E and callback firstCall F. Freshly read global B and call
BattleGetMarioPtr(B) on EVERY invocation,including polls. Its returned unit
pointer is not subsequently used. The script's raw operand-3 is NOT read,
resolved or translated by this callback;do not use it as an effect location.
There is no argument-pointer access or unit-position read. Keep the Mario query
service request even though its result is unused.

For F!=0,copy the three raw words at802ef5d8 to local vector storage;load them
as binary32 values(0,10,0). Call effMajinaiEntry(integer0,f1=0,f2=10,f3=0) at
8011466c;store returned pointer/word to E78 at80114670. Then call
 effSetName(word(E78),string802ef624) with original name `mjef_coinup`.
No allocation/null/duplicate-name safety guard. effMajinaiEntry's full identity
is pinned,but constructor/resources/update/draw behavior is not recovered here.

After optional creation,name setting and on EVERY poll,read word E78. If exact0,
return handler2 WITHOUT a name lookup. Otherwise call effNameToPtr(`mjef_coinup`):
nonnull returns handler0,zero returns2. It does NOT compare the looked-up pointer
with retained E78. Another active same-name effect can therefore keep the event
waiting after its original object vanished. Conversely a stale nonzero E78 with
no current same-name object completes. No pointer-clear,delete or cancel request
is made by this callback. E78 aliases the general wait-count/scratch word;its
current use is an effect handle gate,not a frame countdown.

USER_FUNC supplies F=1 only during initial setup and F=0 on retained polls.
Calling it with F!=0 repeatedly recreates/relabels effects by its local policy;
there is no independent E.marker/phase guard in this body. Handler0 retains
cached callback/argument state;handler2 lets dispatcher fetch the next command.
The containing child has no timeout or effect-specific frame duration.

## Name-table services and weak duplicate behavior

Both helpers capture effect-manager pointer80415ee0 (r13-7040),then SIGNED
count word manager0 and record-array pointer manager4. Scan index0..<captured
count with record stride28 hex=40 bytes. Record is admitted for name comparison
when its ENTIRE flags/allocated word0 is nonzero,not a specific active-bit mask.
Call strcmp(record+18,name),return0 equality;skip all word0==0 records.
Captured count/array remain fixed throughout the scan. No manager/array/name
null guard,index/capacity bound or later current-manager reselect.

NameToPtr returns FIRST matching record pointer;no match or signed count<=0
returns0. It makes no mutation. Equal-name records are resolved by slot order,
not retained handle identity,newest object,object kind or render visibility.

SetName(effect P,name) performs the same search and breaks on first equality,
but neither that match nor exhaustion changes its final operation:ALWAYS
strcpy(P+18,name). It does not delete/clear/rename the existing matching record,
reject duplicates,assign a unique suffix or truncate to16 bytes. Original
record stride leaves16 bytes from18 through27,while strcpy has no local length
check. The actual supplied11-character name plus null fits that nominal span;
other names/source memory domains remain separate. There is no local P-null
check before the copy. Its scalar return is not a validated success boolean.
strcmp/strcpy identities are linked;full name/backend lifetime is not a private
map owned by this one script.

## Complete Mario jump child80363414/ac

The12-command nested script is:

| Header | Operation / operands |
| --- | --- |
| 80363414 | CheckActStatus(-2,L0) |
| 80363424 | IF_EQUAL(L0,0) |
| 80363430 | DELETE when equality passes |
| 80363434 | END_IF |
| 80363438 | SetFallAccel(-2,raw encoded floatf24a7c80) |
| 80363448 | GetPos(-2,L0,L1,L2) |
| 80363460 | GetBodyId(-2,L6) |
| 80363470 | AnimeChangePoseType(-2,L6,44) |
| 80363484 | JumpPosition(-2,L0,L1,L2,30,-1) |
| 803634a4 | AnimeChangePoseType(-2,L6,35) |
| 803634b8 | DELETE |
| 803634bc | END_SCRIPT terminal |

CheckActStatus's normalized output comes from the complete
[CanActStatus contract](BATTLE_MOVE_COUNTS.md),not a plain HP>0 test. Skipping
jump does not skip the containing power child's later announcement/wait.
Encoded fall parameter decodes to binary32-0.5 under the delivered float reader;
retain raw encoding and callback semantics rather than treating it as a new
measured gravity. JumpPosition uses the already recovered local motion state;
its count30/option-1 are requests,not a measured animation/render duration.
Child inherits actor/local/message state and copies back on deletion.
Pose callbacks/render animation binding remain required.

## Complete independent sound root80306874/10c

Power-check clears L0/L1/L2 BEFORE RUN_EVT. The root receives copied locals,
actor/type/priority and shared user pointers per RUN_EVT,with global speed,
credit0 and no relationship links. Flags0 means no current-table append;
execution begins through the next priority rebuild. Parent does not wait for
this root and parent deletion does not discover it through a brother link.
There is no root ID output,join,explicit sound stop or cancellation here.

The17-command script has five successive triplets below,then DELETE at80306978
and END_SCRIPT at8030697c. Each triplet computes L10 through ftomsec,waits using
WAIT_MSEC(L10),then calls evt_snd_sfxon_3d(key,L0,L1,L2,0):

| Convert / wait / play headers | Frame-count input | Converted integer ms | Original key pointer/name |
| --- | --- | --- | --- |
| 80306874 /80306884 /8030688c | 29 | 483 | 802c12e4,SFX_EVT_BTL_INCANT_CUBE1 |
| 803068a8 /803068b8 /803068c0 | 200 | 3333 | 802c1300,SFX_EVT_BTL_INCANT_CUBE2 |
| 803068dc /803068ec /803068f4 | 90 | 1500 | 802c131c,SFX_EVT_BTL_INCANT_CUBE3 |
| 80306910 /80306920 /80306928 | 80 | 1333 | 802c1338,SFX_EVT_BTL_INCANT_CUBE4 |
| 80306944 /80306954 /8030695c | 5 | 83 | 802c1354,SFX_EVT_BTL_INCANT_CUBE5 |

ftomsec captures E/A,resolves RAW operand0 through GetValue,multiplies resolved
word by1000 with word wrap,reads RAW output operand1 AFTER resolution,then
signed-divides wrapped product by HARDCODED60 with truncation toward0. Write
that result through SetValue and return2. It does not query GP frame rate,
render count,OS clock or original measured display timing. WAIT_MSEC later uses
its own lifetime/tick/division protocol;table ms values do not prove audible
wall-clock intervals. Audio source/macro/spatial mixer and sampled timing remain
open. Preserve these five original keys/requests without replacement tones.

## Extra source spans,registry entries and acceptance

Existing three-stream pack omits both nested sources:Jump80363414/ac and sound
80306874/10c,plus each original trailing scanner word. Add these alongside
Mario key7 child80416bd0/8 when binding the full conditional request graph.
Read the power-check source without substituting a callback-success shortcut.
Compared with the earlier72-address direct registry,these nested scripts add
three identities:CheckActStatus80105cb4,ftomsec80103dc0 and
 evt_snd_sfxon_3d801521dc. Others already occur in that scoped census. Actual
callback effects,source reachability and source-owned table/registry binding
still require the linked contracts and missing backends.

Acceptance:every firstCall/non-firstCall branch,unused actor/Mario query,
constant vector/name creation,E78 versus any same-name record,zero handle bypass,
first-match/duplicate behavior,SetName's unconditional copy,global count/array
capture,CanAct gating,child copy-back,independent-root locals/append/lifetime,
all five keys/header/count conversions,wrapped signed ftomsec and real WAIT_MSEC
semantics. No original fortune activation/effect/audio/runtime trace is claimed.


## Reproducible additional source packaging

Using the existing read-only source packer,explicitly select all six names
(explicit selections replace its defaults):

```sh
python3 tools/pack_event_scripts.py --output /tmp/jump-six-sources.pmevt \
  marioAttackEvent_NormalJump marioAttackEvent_MajinaiPowerUpCheck \
  btldefaultevt_SuitoruBadgeEffect subset_attack_ready_jump_event \
  marioAttackEvent_MajinaiJump evt_majinai_effect_se_btl
```

ID2 reproduced /tmp/id2-majinai-jump-six-sources.pmevt:6 events,5 merged segments,
5020 bytes,SHA256ed911bc8cb183d68dfb4e0f2f19a83f47640db4d94c640e4198fe712e3127a0e.
All three Majinai ranges including trailing scanner bytes match original pinned
spans. Adjacent parent/jump ranges share the identical overlap word and merge;
no rewriting occurs. This source artifact stays outside history;the native
packer defaults are unchanged. Packaging does not supply a bound registry,
fortune producer,effect/audio/announcement backend or original execution proof.


[Complete Majinai constructor](../effect/MAJINAI_EFFECT_CONSTRUCTION.md) and
[pool/map publication](../effect/EFFECT_POOL_ACQUISITION_AND_MAP_PUBLICATION.md) now
close its called construction/acquisition/map helper requests,32-byte payload
fields,heap3 and exact map+1e word write. Update/draw/delete/resource/platform
engines and original lifetime/frame behavior remain open.


[Complete soft/hard deletion](../effect/EFFECT_SOFT_AND_HARD_DELETION.md) and
[caller/name lifetime](../effect/EFFECT_DELETE_CALLERS_AND_LIFETIME.md) now pin the
shared release leaves. Soft deletion keeps mask02 records name-visible after
OR04;hard deletion clears only flags after heap3 free. Effect-specific update/
04 consumption and render/frame/source ownership remain open.


[Complete Main/04 consumption](../effect/MAJINAI_EFFECT_UPDATE_STATE_MACHINE.md) now
supplies effect-specific soft cleanup and normal fade/lifetime;
[Disp/pose requests](../effect/MAJINAI_EFFECT_DRAW_AND_POSE_REQUESTS.md) supplies full
local draw sequence. Actual driver/animation/resource/matrix/render/scene
services and original timing/visual traces remain open.
