# Field-return sequence counter and incoming fade order

2026-10-13 US G8ME01 disc0 revision0 read-only DOL, scoped follow-up to
[field return handshake](UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md).
Exact used seq_battle states5/6 and seq_game incoming state0/init ordering.
This does not claim sequence-manager exit/init/event/frame scheduling complete.

## State5: ready, alternate override, listener tail

seq_battleMain80073b24/3dc retains incoming sequence owner Q in r30. State5
enters80073dac and calls battleSeqEndCheck800f903c. Returned0 skips the ready
writes but still executes the state5 listener tail. Nonzero:

1. Q.word10=0 at80073dbc.
2. Read Q.word4, add1 with WORD wrap, store at80073dc8.
3. Only now read CURRENT global GP pointer80415fd0, then GP.word0c at
   80073dcc/dd0. Any nonzero value (signed comparison only tests equality0)
   forces Q.word4=11 at80073de0.

No state6 same-call fallthrough. No GP0c override polling while ready check0.
GP0c and sequence counter10 are different owners.

Both ready/blocked execute identical tail at80073de4:

* marioGetPtr8005e0c0, return unused.
* camGetPtr8000f2fc(4), return unused.
* Source local listenerXYZ=(0,0,400), literals804203c4=0,804203c8=400.
* Read CURRENT global battle B, then Bef4. Mask100 adds literal804203cc=200
  to local Z (600), without changing X/Y.
* camGetPtr(4) AGAIN at80073e24, read current returned-camera FLOAT114.
* psndSetPosDirListener800dc380(&localXYZ, FLOAT camera114) at80073e34.
* Return through common epilogue80073ee0.

The first camera lookup and unused Mario lookup are real source requests,
not interchangeable with a retained camera4 pointer. The listener uses the
second camera's actual114 value. No root-position getter is used for this
state5 listener vector.

## State6: wrapping signed counter, no camera tail

At80073e3c read Q.word10; addi1 at80073e40 is32-bit modulo addition. Compare
that NEW word as signed against30 at80073e44; store the newword at80073e48
before branching. Newvalue<=30 returns directly through80073ee0.
For example old7fffffff produces80000000 (signed negative), oldffffffff
produces0; both fail admission. There is no saturation, halfword counter,
render-frame increment, or forced counter31 normalization.

Newvalue>30 reads current GP pointer and GP.word0 once at80073e50/54.
Both masks1000 and2000 on that same captured word request seqSetSeq8002e218
(5,1,0); otherwise(2,1,0). No later GP0c override appears in state6.
Each branch returns directly after seqSetSeq. State6 has NO Mario/camera/
listener tail; the state5 tail is not a common unconditional tail of this body.

## seq_game state0: typed fade inputs and fresh pointer reads

seq_gameMain800073a4/198 retains its incoming sequence Q in r31 and reads
Q.word4. State0 starts800073d4, capturing current GP pointer in r4 and GP.word0.
Specialmask10 takes:

1. Copy original RGBA WORD8041f498=000000ff to local stack colour.
2. fadeEntry80013368(type18,time900,&RGBA) at800073f8.
3. evtEntryType80037220(scriptAddress80414f28,0,0,0) at8000740c. Address is
   the original small-data resource named maku_spread; resource/event runtime
   semantics remain their own provider.
4. AFTER BOTH requests reload CURRENT global GP pointer at80007410, read
   its current WORD0 at414, clear mask10 and store at41c. Changes by those
   callbacks must be preserved; this is not clearing the pre-fade snapshot.

Specialclear does not run on the ordinary path. Ordinary path tests captured
GP WORD118 at80007424, not byte118. Exactly0 copies RGBA8041f49c=000000ff,
loads WORD f8 and WORD fc and requests fadeEntry at80007444. Nonzero copies
RGBA8041f4a0=000000ff, loads WORD108 andWORD10c, requests fadeEntry at80007460.
Type/time are full32-bit ABI words; this caller neither narrows them nor
validates sign. Do not read only the low16 time or treat type as a byte.

After every selected fade path, presets reset in this precise order:

| Address | Action |
| --- | --- |
| 80007464/70 | Reload current GP pointer; store WORD f8=9 |
| 80007474/78 | Reload current GP pointer; store WORD fc=300 |
| 8000747c/80 | Reload current GP pointer; store WORD108=9 |
| 80007484/88 | Reload current GP pointer; store WORD10c=300 |
| 8000748c..94 | Read retained Q.word4, add1 wrapping WORD, store |

Presetstores use current GP pointers after callbacks, not the original r4
capture. Sequence increment similarly uses a fresh read from retained Q rather
than literal state1, although ordinary unchanged state0 yields1. Return follows;
state1's listener/other calls do not execute by local fallthrough.

## seq_gameInit timestamp and flag publication

seq_gameInit8000753c/38 ignores incoming argument locally. It calls
OSGetTime8029d84c at80007548, returning r3 upper/r4 lower time-base words.
Order after return:

1. Store LOW r4 into global8041e5a4 at8000754c.
2. Load CURRENT GP pointer80415fd0 into r4 at80007550.
3. Store HIGH r3 into global8041e5a0 at80007554.
4. Read GP WORD18 at80007558, clear mask10, store at80007560.

The timestamp pair is separate global storage, not GP+c/GP18/GP event elapsed
time or sequence counter. Do not label two pointer-displaced stores as GP
fields. OS time-base units/high-low capture belong to
[time publication](../platform/EVENT_RETRACE_TIME_PUBLICATION.md); a native
clock bridge must document its source units/adaptation rather than substitute
an arbitrary render counter. No local sequenceQ reset or GPf8/fc mutation is
performed by this Init.

## Evidence

Complete raw original DOL SHA256:

| Body | Address/size(hex) | SHA256 |
| --- | --- | --- |
| seq_battleMain | 80073b24/3dc | 013f9d3a2a4da7f4507dd7e8268be1b37f4088a93c89eeeaeb6e41cf07116ea2 |
| seq_gameMain | 800073a4/198 | 52cd83b5a3acfdb559a7e0950b35c278de49bd97cd35d9a367dcebadd8e2606a |
| seq_gameInit | 8000753c/38 | ee640b28e51bd0bc7385f539c52bdf363daefff7215bdc70b35a7f71b60a5380 |

Inspection-only listings/ELFs under ignored artifacts/recovered-temp-evidence/
return-sequence-*. Source wrap/compare/stores/GP captures/service order,
original32-bit fade read widths and literal colour/vector values checked.
Documentation only; scoped diff check passed, no native/test/shared index/
progress/inbox edits, build rerun or commit. Actual fade/event/audio/sequence
provider readiness remains distinct from this local ordered scalar closure.
