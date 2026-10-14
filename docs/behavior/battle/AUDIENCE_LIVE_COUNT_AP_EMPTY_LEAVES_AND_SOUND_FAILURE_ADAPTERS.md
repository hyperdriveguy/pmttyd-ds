# Audience live count, AP empty leaves, and sound failure adapters

2026-10-14 US G8ME01 disc0 revision0. Read-only original/source-pack/native
inspection. Companion to [live audience services](AUDIENCE_LIVE_SOUND_WINDOW_THROW_AND_DISPLAY_ADMISSION.md).
Owned documentation only; no native/shared documentation changes or commit.

## NoiseMain live total is left then right, independent of cached totals

BattleAudience_GetPPAudienceNum801a2144/34 invokes LEFT801a2090 FIRST, retains
that result, then RIGHT801a20b4 and returns wrapping32 sum. It reads no
A13784 cachedcount. The L/R wrappers pass column0/10 respectively to
BattleAudience_GetPPAudienceNum_RL_Sub801a201c/74. That helper first requests
BaseGetPtr (returned pointer unused), then visits10 rows×10 columns in order,
slot=row*20+startcolumn+column. It invokes
BattleAudience_GetPPAudienceNum_Sub801a1fa0 for each, incrementing its result
count only when the scalar equals1.

Sub **captures actor GetPtr(slot) BEFORE GetExist(slot)**. GetExist queries its
own fresh global owner and tests flag1 set, flags100/2 clear. If returned LOW8
is0, Sub returns0 without state read. Otherwise Sub reads captured actor
BYTE19 and excludes states0d,11,0c; every other state returns1. If a provider
mutates global BattleWork between these requests, captured state and fresh
existence can refer to different owners; do not fold them into one unchecked
cached pointer. Ordinary native stable owner can still follow the same order.

This equals the eligibility used by source sound L/R counting, but wrapper
order differs from explicit NoiseAlways/Clap R-then-L. SoundMain NoiseMain
must call the real total L-then-R, not sum current cached right/left values
from CtrlProcess. Ctrl cached classification excludes0c/0b instead; e.g.
state0b exists for PPcount but not Ctrl cache, state0d exists for Ctrl cache
but not PPcount. Both algorithms are valid for their actual callers.

## AP-source flag2 admission and bit1 record predicates

ApSrcCtrlProcess801a4998/574 captures A. If A.WORD0 bit2 clear, it returns at
801a49c0→4ef0 with no per-record/audio/pouch requests. Actual Init memset
initializes that bit clear; retain real subsequent producer history.

With bit2 set, the actual ordered prefix is:

1. Scan100 records at A10e7c+i60hex; count **record WORD0 bit1 only**.
2. psndSFXChk800d95a0(A08), even when count0. If returned-1 skip volume;
   otherwise request psndSFX_vol(handle A08,
   truncated(single(64+single(single(63*float(count))/100)))) (source constants
   80424a44=42800000,804249b4=427c0000,804249bc=42c80000;
   output truncation/narrowing belongs to public accessor).
3. If signed A137c4>0, call ApSrcEntry801a4754. Only return1 decrements that
   pending count. If prior activecount0 and a fresh Chk(A08)==-1, request
   psndSFXOn800d98bc(original key802fa6e4), storing returned raw handle A08.
   This producer path can allocate records even when the starting bank is empty.
4. Otherwise (A137c4<=0), scan100 again until first record WORD0 bit1 set.
   All100 bit1 clear admits the source termination leaf:
   reread/clear ONLY A.flags2; Off(A08); store A08=0; store A137c4=0;
   call BattleStatusWindowAPRecoveryOff800f767c; return.
5. Non-terminating branches enter the100-record active pass. Each iteration
   calls pouchGetPtr800d5eb0 BEFORE reading its record flags, then skips the
   state/motion graph if WORD0 bit1 clear. Bit2 does not serve as activity here.

Record flags2-only therefore count as inactive in both scans and the active
loop. Termination clears A bit2 but leaves record bit2 untouched. No whole
record memset occurs. This is a useful branch distinction; do not require
recordflags==0 or synthesize an all-flags clear.

Actual APRecoveryOff sets current B274c=0 then invokes
BattleStatusWindowCheck800f7514. That helper captures current B and decides
wanted-open0 if B273c or2740 nonzero; else1 if B274c nonzero; else1 only when
B2748 AND B2744 are nonzero. It calls statusWinCheck8013c1f0 in either outcome:
wanted1/currentclosed opens8013c3b8; wanted0/currentopen closes8013c390. Thus
even AP empty termination has a real independent HUD/window owner; retain that
provider if not yet bound, rather than completing with the A bit clear alone.

The admitted fresh native path A.flags2clear does not execute any of these
audio/HUD leaves. An actual later bit2 producer must enter the retained graph.

## Active Ctrl motion and AI entry ABI

CtrlProcess801a5ca8 captures A then scans all200 actors in ascending order.
Actor WORDflags10 set owns motion regardless of GetExist:
FLOATd0=single(d0+dc); FLOATdc=single(dc+e4); when fresh d0<=0 stores
d0=+0,dc=+0,e4=+0 in that order. It does not clear flags10 here, and calls no
motion service for this branch. Numeric fields are original actor offsets,
not unit packed battle position fields.

Flags10 clear calls GetExist(slot). LOW8zero skips. Otherwise BYTE1b type>12
skips; types0..12 dispatch via8039cd54 to13 distinct call sites801a5e5c,
68,74,80,8c,98,a4,b0,bc,c8,d4,e0,ec. ALL13 targets are the original
BattleAudienceCtrlProcessKinopio80192a0c, with ordinary r3=slot index.
Its generic state/type-specific transitive graph remains that real provider;
do not invent one callback per race or assume a Toado-only actor admission.
Fresh source flags0 skips all motion/AI before reaching the real throw tail.

## Original type0 item table is present in the current native source pack

Original type0 row803b4de4/20hex bytes:
00000000803b4828803b4c044120000041c80000404000003f8000003f800000.
Its WORD8 is the actual item-table pointer803b4c04. That table60hex bytes is
12 pairs of signed WORDs:

```
(121,10),(123,10),(124,10),(148,50),(149,10),(150,1),
(151,5),(154,10),(132,10),(129,5),(130,5),(0,0)
```

Actual ItemOn only reads firstWORD for the used zero-record rejection path;
121 is nonzero and passes table eligibility before seat/GetExist rejects.
Do not rewrite pointer0 or fabricate an empty list from actor absence.
Read-only PMEVT001 segment verification of romfs/trio-attack-events.pmevt
found both source addresses in original segment803041e0 size790912 and
compared their bytes to original ISO. The builder tools/trio_attack_assets.py
imports original DOL data sections7..12, with only the documented mutable-tail/
manager exclusions, so these addresses are already available to native reads.

## Current nonnegative audio mocks versus an explicit failed request

Read-only source/battle/trio_attack.cpp audit identifies:

- Movement playback trigger enqueues the original key, returns uint32(0);
  its position/off adapters are empty.
- MarioJump play callback returns0; original jump motion discards that return,
  so changing only the unavailable-request result cannot alter jump motion.
- HammerSoundSink enqueues key and returns0; source Hammer stores it in its
  handle work array and tests-1 before subsequent per-stage sound requests.

Source failure is raw-1, not0. These mocks do not provision public slot0,
generation0 or its backend; returning0 does not establish an original live
handle. A native unavailable request adapter can truthfully supply-1 while
retaining the attempted key/history, but that is a declared backend/service
adaptation, not proof original __psndSFXOn would have rejected this request.
Original On allocates a public pending slot/encoded generation BEFORE actual
backend start, so an absent NDSP sample alone does not imply source On failure.

Changing returned handle affects source consumers. Continuous one-key motion
stores-1 and retries on the next control invocation, where the old fabricated0
instead triggered position updates/stop requests. Hammer stores-1 and preserves
the real failure-domain stage admission; source stages normally advance, but
later repeated-stage/lifecycle tests must not assume request-history identity.
Jump ignores its return locally, making this specific substitution motion-
neutral. No global claim of unchanged source trace or original audio is justified.
The preferred faithful provider is the separate persistent40-slot original
registry with real pending/start/update/stop ownership and an explicit native
voice projection. A failed-request adapter remains useful bounded progress
if its consumer effects and unbound source On admission are recorded.

## Evidence and validation

Complete ignored source listings are audience-live2-* in
artifacts/recovered-temp-evidence/. Original body SHA256:

| Body/address/size(hex) | SHA256 |
| --- | --- |
| PPtotal801a2144/34 | 0ab51292ab15cec40d156533d8c019415e26fb3d64429e61e7ee2afcfa7ee4cf |
| PPleft801a2090/24 | 26c8c347827f7f658fe15fc41442d79c487e2828cc82a3b4b6b1cb158c296198 |
| PPright801a20b4/24 | 06c8b223ecadab8adadf34b8eb26cb10818279b1dcb7220061fd815415406857 |
| PPrl801a201c/74 | 95fe94aff68132e76744cb73b61d1f671e7b048a932b0c927bf6dfeda5252046 |
| PPsub801a1fa0/7c | a025411304a9eb0c7244a2a237530021609b7f931cd67de7e6e8353f7252c0b4 |
| APctrl801a4998/574 | 18b4d7706db37eb21d93b760a52fbb1a9f606830f81ddf6a23d09452efbd78b2 |
| APoff800f767c/2c | 98b9c0a0dce5f4efc3b5ab7aba0b7bff7e84373cee0a788260fb59b7bf57d775 |
| WindowCheck800f7514/b8 | f8480d86b324df66a420f7f89bf775729bfe32591b87370c36290fc6f0763cd5 |

Item table803b4c04/60 SHA256
c2485b912a51d8a465f116951b5f19499d09198e2c74e7fb12e2861fbaee0c49.
Read-only original/static pack checks and scoped diff check; no native test,
emulator, playback-equivalence, shared edit, or commit claim.
