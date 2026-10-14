# Battle audience sound bank and ending clap ownership

US G8ME01 disc0 revision0, read-only original DOL audit, 2026-10-13.
Scoped completion of the retained EVT-01 ending SoundStop dependency, following
[ending tail](../battle/UNDERGROUND_TRIO_NO_LEVEL_UP_ENDING_TAIL.md). This closes
record allocation/initialization, slot1 producer and stop ownership; actual
positional backend playback and nonempty audience provisioning remain separate.

## Backing storage and initial state

BattleInit800f8f04/138 calls BattleAlloc800f8934(size19098hex) at800f8f20,
publishes returned BattleWork pointer B to global8041eb90 at800f8f28, then
memset(B,0,19098) at800f8f38. This is the original map-heap battle allocation,
not a dedicated sound allocation. Init later calls BattleAudience_Init801a83f8
at800f9020.

BattleAudienceBaseGetPtr801a86a0/0c reads CURRENT global8041eb90 and returns
A=B+2858. BattleAudienceSoundGetPtr801a85bc/40 retains its integer slot argument,
queries that base, and returns A+133fc+slot*24(hex). No slot range/null guard.
There are24(decimal) sound records, stride36(decimal). Slot1 is B+15c78;
sound bank is A+133fc..1375b. Any native packed BattleWork provider must map
those addresses or provide equivalent typed records, rather than alias a
smaller buffer's unrelated bytes.

Audience_Init captures A, queries window, then pouch pointer, and at801a842c
memset(A,0,13914hex). This covers ALL sound records including slot1 flags0,
handle0 and every other byte0. Its later801a8584 call SoundNoiseAlways801a0b88
initializes slot2;801a8594 calls SetVol(2,60,180). These are not slot1
producers. Other transitive audience setup calls have their own contracts.
The zeroing is a valid creation state; it is not proof of ending-time silence.

## Packed sound record fields

Offsets hexadecimal relative to SoundGetPtr result R. Unlisted bytes retain
actual source contents; there is no reconstructed C++ padding requirement.

| Offset / width | Used owner |
| --- | --- |
| 00 WORD | flags; bit1 means installed active sound for Stop/Check |
| 04 WORD | positional SFX handle returned by Sound1/2 backend |
| 08 BYTE | authored crowd/key category |
| 0c signedWORD | lifetime countdown, negative means unbounded |
| 10 signedWORD | final fade countdown threshold |
| 14 signedWORD | elapsed external volume interpolation |
| 18 signedWORD | external volume interpolation duration |
| 1c BYTE | original backend volume copied on successful creation |
| 1d BYTE | current base volume |
| 1e BYTE | current volume multiplier(percent) |
| 1f BYTE | retained multiplier start |
| 20 BYTE | retained multiplier end |

## Concrete slot1 producer: clap

BattleAudienceSoundClap801a0cec/148 arguments are lifetime prefix C and fade
threshold F. It captures C,F, queries audience base, queries SoundGetPtr(1),
then requests GetPPAudienceNum_R801a20b4 followed by _L801a2090; N=R+L.
At801a0d38 it requests SoundStop(1), BEFORE selecting/starting replacement.

Each count wrapper calls RL_Sub801a201c with base column10 or0. That helper
queries audience base and scans100 positions per half in row-major order,
index=20*row+baseColumn+column; it counts ONLY GetPPAudienceNum_Sub801a1fa0
returns exactly1. Sub first obtains the record, then GetExist801a29c0; low8
existence0 returns0. Nonzero existence additionally excludes BYTE record19
states0c,0d,11; other states return1. It does not use cached total audience
amount.

Key/category choices (source strcpy into local stack buffer):

| N | Source key pointer/string | R08 |
| --- | --- | --- |
| 1..49 | 802fa5c4 SFX_AUDIENCE_CLAPPING1 | 1 |
| 50..99 | 802fa5dc SFX_AUDIENCE_CLAPPING2 | 2 |
| 100..149 | 802fa5f4 SFX_AUDIENCE_CLAPPING3 | 3 |
| >=150 | 802fa60c SFX_AUDIENCE_CLAPPING4 | 4 |

At801a0dd0 it ALWAYS calls Sound1(localKey,rightCount,leftCount), then stores
returned handle to R04 at801a0dd4. Only handle exactly-1 skips the following
writes. Every other handle executes, in order: R00 OR1; R0c=C+F;
R10=F; psndSFX_get_vol800d9558(R04); returned byte→R1d then R1c;
R18=0;R14=0;R1e=100. Remaining flags and fields survive.

Important empty-count source boundary: N0 does not take ANY key strcpy branch,
yet still calls Sound1 with the uninitialized local stack key at801a0dd0.
There is no source N0 return or silence gate in this body. Sound1's computed
volume0 does not prevent the earlier sound start request. Negative N has the
same unassigned-key issue. A native empty-audience adapter must expose this
unsupported key/backend domain; it cannot justify handle-1, flag0 or skipped
clap by claiming the original initializer remains authoritative.

## Sound1 request and backend

Sound1 8019fcec/5c calls GetInfo1 8019fbe8 with right,left and addresses of
local volume/XYZ, then psndSFXOn_3D800d9848(key,&XYZ), retains handle and calls
psndSFX_vol800d97d0(handle,localVolume), returning that retained handle.
The volume0 path still calls psndSFXOn_3D first. GetInfo1 N0 writes volume0 and
neutral X; Y/Z are fixed source values. Positive-count formula and pan use
source floats/integer narrowing in the complete listing; nonzero numerical
providers must preserve that calculation rather than fabricate volume.

Existing [start wrappers](BATTLE_POSITIONAL_SFX_START_WRAPPERS.md),
[slot provisioning](POSITIONAL_SFX_SLOT_PROVISIONING.md),
[slot accessors](BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md),
[backend dispatch](POSITIONAL_SFX_BACKEND_DISPATCH.md) and
[Off](BATTLE_POSITIONAL_SFX_OFF.md) own real handle semantics. Public positional
SFX records at803df988 have stride28hex and are distinct from the audience
records. Neither fabricated handle0 nor an unconditional successful stop
provides actual playback ownership.

## Stop and ending history

BattleAudienceSoundStop8019fd68/4c calls SoundGetPtr(slot), reads R00, and tests
bit1. Clear skips backend and all record writes. Set calls psndSFXOff800d95f8
with R04, then RELOADS R00 and clears only bit1. R04, category, lifetime,
volume/counters all survive. Backend may mutate other state before the flag
reload. No local output is a completion/poll result.

Original btlseqEnd calls Clap(-1,0) at802156e8 (state06 after reaction barrier)
and802158a4 (EXP08 sound sequence). The ending does NOT leave slot1 untouched.
Other loaded DOL direct clap callers are Audience_Case_Appeal8019f25c,
Audience_Case_AcrobatGood8019f5c8 and sac_wao80248d90. A DOL direct-call census
of SoundGetPtr finds Clap as the only constant slot1 producer; parameterized
Stop/Check/SetVol and sequential SoundMain can also touch it. This census does
not exclude REL/indirect access.

Ending state1c calls SetVolAll(0,30). SetVolAll8019fddc loops slots0..23,
calling SetVol8019fe28 regardless of active flag. Each copies R1e→R1f,
stores low8 target→R20, R14=0,R18=duration,R10=0; it does not clear R00 or
stop R04. State1f later calls Stop(1). Silence after state1c cannot be inferred
from request target0 alone.

SoundMain8019f97c first calls NoiseMain8019f7b4, gets slot0 and walks all24
records WITHOUT an R00&1 admission test. For each, psndSFXCheck800d95a0(R04)
return-1 clears bit1 and skips; otherwise lifetime0 invokes Off then clears
bit1; otherwise it processes volume ramps/positive lifetime. For Clap(-1,0)
R0c=-1 does not decrement. A successful real clap handle therefore persists
until backend finishes or an explicit source stop. Source backend liveness,
manager cadence and producer history are required to establish currentflags.

## Evidence / delivery scope

Complete raw DOL body SHA256 (addresses/sizes hexadecimal):

| Body | Address/size | SHA256 |
| --- | --- | --- |
| BattleInit | 800f8f04/138 | 616f72f3468ba55fb0ac5dc234c5b950eb85fed4c8caafc20cca55bb73aa48c7 |
| Audience_Init | 801a83f8/1c4 | a926eae5ff02962f79d66e063dc5a345edfceb4e58130f38a13e65db79d25d99 |
| BaseGetPtr | 801a86a0/0c | 17096f3ef21a7a0336e1d70a08dc681fa1d734a641d0eef0da749a779d749f79 |
| SoundGetPtr | 801a85bc/40 | b5cb35bbd5cda0492e956f42a0e5df6e743e988f41b7d1f720a963e6e2b794cf |
| SoundStop | 8019fd68/4c | 10b948e9b75435fcd4ea27d33f5d9714b1b52fe59d5aeb33a4243c7def54e5ca |
| SoundClap | 801a0cec/148 | 64bbd2c5ba9d96fd426ece9617123064b2ebc8bcaa744bfb4a406adb679e9f83 |
| Sound1 | 8019fcec/5c | 55ff9d0c5738ce89e4fe57b47c5d27900dc099cbdeacef4d57a25ad3c7247b73 |
| SoundGetInfo1 | 8019fbe8/104 | 015372ad3e3459acf08bd9d5b0f3b753d0fa6c071bec2349fd8c23406de5580c |
| SoundMain | 8019f97c/1e8 | f09a66ac8c4e0784251d8efd78676ea0d83d5148daae30c1b8e169f2ad9ddf9a |
| SoundSetVol | 8019fe28/4c | 5ed4ebd79198572ac18db0fe0d2a74dfcc97fb8af766a849b1a3871c3638a464 |
| SoundSetVolAll | 8019fddc/4c | ee6aabae0e9d8dcab32f7f778e9570f97ba6398d50125d88d0ed43ce4ff8ce6c |

Complete inspection listings/ELFs under ignored artifacts/recovered-temp-evidence/
audience-*. Source initialization/slot layout, constant slot census, local
clap/stop/volume ordering independently checked against PPC. Documentation only;
no native edits, shared index/progress/inbox edits, make rerun or commit.
Fresh initializer flags0 is closed. Current ending flags/handle require actual
producer/backend lifecycle; zero-crowd key remains an explicit source domain
boundary, not an invented successful silent record.
