# Ordinary Hammer: StickKeepLeft command

US G8ME01 revision0,recovered2026-10-12. Evidence B:complete main
0x8012a17c,size0x534,result0x8012a174,size8,delete0x80129fc0,size0xc,
BattlePadCheckNow0x80119fe0,size0x28 and BattlePadMultiCheckNow0x80119fc8,size0x18.
Registration ID3 at0x80364370 supplies main/result/display/delete pointers
0x8012a17c/8012a174/80129fcc/80129fc0. Ordinary Hammer core explicitly selects
this ID. See [bindings](BATTLE_ORDINARY_ATTACK_BINDINGS.md),
[parameter/start/result callbacks](BATTLE_ACTION_COMMAND_CALLBACKS.md) and
[pad/difficulty rules](../input/BATTLE_INPUT_AND_DEFENCE.md). No original timing measured.

## State and input ownership

All offsets relative to BattleWork. Use names as local descriptions,not an
unverified complete structure layout:

| Field | Local role |
| --- | --- |
| +0x1c90/+0x1c94/+0x1c98 | Actor,setup options and special-route delay |
| +0x1c9c/+0x1cb8 | State,result bits:bit0 pending;bit1 success |
| +0x1cc4 | Mode word;bit1 permits retry/forced timeout success |
| +0x1cc8/+0x1ccc | N charge parameter and period T |
| +0x1cd0/+0x1cd4 | Suppression source/per-call latch |
| +0x1cd8 | Final-charge sound marker |
| +0x1ce8/+0x1cec/+0x1cf0 | Charge-entry marker,step count,release classification |
| +0x1d18 | Signed halfword forced-result counter |
| +0x1f20..+0x1f4b | Display/command scratch44 bytes |
| +0x1f4c/+0x1f50/+0x1f54 | Countdown C,stage S,finish countdown |
| +0x1f58..+0x1f84 | Twelve sound-handle words |

At each main call capture actor byte+0x307!=0 as automatic mode A. Clear the
suppression latch;when state!=0 and suppression source!=0,set latch1. Nonzero
latch skips state processing and returns1. This capture/latch is not recomputed
inside same-call redispatch. Normal Hammer's parameter calls write suppression0.

Held-left query is BattlePadCheckNow(0x40000),which delegates channel0 to the
multi helper and reads shared battle+0x1034+channel*0x1fc,AND mask. This is
**general battle held history index0**,not private AC trigger history. Existing
pad construction adds0x40000 when main-stick X<=-30. No new-press edge or A/B
button check exists in this main body. Masked result is treated as boolean.

## Main initialization and charge entry

State0 stores result1,sets stage0,clears44-byte scratch,sets original display
floats-300/+0 at+0x1f34/+0x1f38 and20 at+0x1f40. Clear markers+0x1ce8,
suppression source/latch,final sound marker and classification. Initialize twelve
sound handles to-1,then set state99. It does not clear N,T or mode. State99
waits;[StartAC](BATTLE_ACTION_COMMAND_CALLBACKS.md) sets100.

State100 initializes C120 and sets1000,then evaluates1000 **in the same call**.
In1000,first handle option bit0 plus actor token bit0x10. If both set and option
bit1 set,decrement special delay;nonnegative delay returns1 immediately. When
eligible for its draw,irand(100)>=0 stores result0;negative result stores2 and
increments+0x1cb4. Return0 from main on this route. Normal irand(100) does not
produce the negative branch;retain draw/failure behavior rather than inventing
random success probability. No normal state cleanup is performed on this return.

Otherwise query held-left. Left nonzero,forced counter>0,or automatic mode enters
charge:marker1,state1001,C1,S1,step count0. If none,decrement C;positive continues
waiting,nonpositive stores result0,state1003 and redispatches immediately.

## Charging order and stage progression

In1001,a positive forced counter is decremented as signed halfword;store result2,
state1003 and request same-call redispatch. **Do not return yet:**the rest of
this1001 body's countdown/stage/input code still executes and may update fields.

Decrement C unconditionally. If C<0,increment S and step count. If S>=N+2,
clear result bit0,set state1003 and classification2 (overrun). If mode bit1 set,
also OR success bit1 before clearing pending. Redispatch1003 the same call.
No success-count increment occurs on this normal overrun path.

If not overrun,let k=S-2. Access handle[k] unchecked. When k<N-1 and handle-1,
request psndSFXOn("SFX_AC_PI1") and save returned handle. Otherwise,for
k>=N-1 and handle-1,request "SFX_AC_PONE1",save handle and set final-sound
marker1. Reload C=T and end this pass through the state. The second branch is
not simply k==N-1 for arbitrary parameters. No handle bound/range validation or
sound completion polling occurs locally. Original normal script supplies N4.

If decremented C>=0,query held-left. Nonzero held-left with automatic mode clear
continues charging immediately. Otherwise evaluate release. A stage increment
and release check therefore **cannot** occur in the same pass through1001's
body:increment takes the C<0 branch and exits that pass. Reloading T does not
also decrement it on that pass.

## Release result and difficulty

Set local success=false. If S!=N+1,mode bit1 clear sets classification1;mode bit1
set preserves old classification. If S==N+1,compute signed elapsed E=T-C,
call the attack difficulty getter,and load release allowance W from table
0x80416df8 indexed difficulty*4:

| Difficulty | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| W | 80 | 70 | 60 | 40 | 20 | 10 | 5 |

E<=W sets classification3 and local success=true. E>W with mode bit1 clear sets
classification1;with bit1 set preserves old classification. Equality is accepted.
This compares elapsed after the current call's decrement,not the old countdown.
Getter indexing has no local clamp;the existing setter normally supplies0..6.

Automatic mode ends only when local success is true:OR success bit1,clear
pending bit0,set1003 and redispatch. When local success false,it continues live
even if released. Ordinary mode with local success true performs the same
success mutation/end. Ordinary mode with false success ends by clearing pending
and setting1003 **only when mode bit1 is clear**;with bit1 set,it keeps running.
The branch does not explicitly clear an already-set success bit. Normal Hammer
sites set mode0,so unsuccessful release ends that normal branch. These routes
neither increment+0x1cb4 nor clear input history locally.

N4 means eligible release stage5 and overrun stage6. For normal T and countdown
cadence,progression occurs when decrement produces a negative counter,so a
reloaded T yields T+1 passes before next progression. Do not replace this with
T passes. These are derived invocation rules;actual attack animation alignment,
parameter branch selection and clock/pause cadence remain unmeasured. In the
ordinary positive-period path with no overrides or suppression,target-stage
release checks occur at derived E1..T;the next pass overruns instead. Thus the
local eligible release span is min(T,W) calls,not necessarily the full allowance
W. For example,T25 and difficulty3 allowance40 yield25 eligible checks by this
static deduction,not40 measured attack frames.

## Finish states and event completion

Redispatch resets the local repeat flag at its loop head,then rereads state and
suppression latch. State1003 performs a local busy bdnz loop when S-2>=0,
then sets1004. That loop has no function call,no sound-stop request and no
simulation yield. It is not a multiframe delay or evidence of audio cleanup.
State1004 has no handler and waits for an external state write.

Delete callback only sets state1005. On1005,set finish countdown60,state1006
and exit without decrement. Subsequent1006 calls decrement;nonpositive changes
to1007,then a later1007 call clears all four callbacks. Unlike TimingA's finish
initialization,1005 does **not** fall through to its decrement. Normal return is1
even after this cleanup;the main callback has already been cleared explicitly.
Unhandled states otherwise return1 with no local change.

Result callback simply returns the full stored result word. Event ResultAC
ignores its register result and waits for pending bit0 to clear before returning
completion2. Thus start and result can appear adjacent in the original Hammer
stream without making the command instantaneous. Damage,animation and script
continuation are downstream consumers,not established by these local functions.

## Evidence and remaining work

Eleven new function fingerprints and48 instruction anchors pin main branches,
input query,parameter/start/result wrappers and tiny callbacks. Checks also pin
seven allowances,registration,display floats,two sound keys,eight parameter
sites and two mode0 sites in the previously hashed Hammer-core stream. Original
reports/listings remain ignored;no executable linking or dialogue rewrite occurs.

Recover full conditional Hammer script/weapon data,SetDifficulty and output
readers,automatic/suppression/forced counter writers,display/effect/audio service
ownership,and event result/damage/stop behavior. Verify original held/released
inputs on stage boundaries,allowance equality,timeout/retry,forced routes,
same-call redispatch and completion counts. Bind Crump/tutorial exceptions
separately. This local mechanic contract does not close combat or full-game
requirements,and no Switch behavior is inferred.
