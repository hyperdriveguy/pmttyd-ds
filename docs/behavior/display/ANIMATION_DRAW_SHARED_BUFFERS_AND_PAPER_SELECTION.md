# Draw-time shared buffers and paper-animation publication

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete wrapper
animPoseDrawMtx and inner _animPoseDrawMtx. Entry notation from
[wrapper/admission](ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md). No execution.

## Ring publication

Wrapper admits this preparation only when captured linked Q+4 mask2 SET.
Capture signed counts A=word[Dq+f0],B=word[Dq+f8],capacity C=word[W+f0].
Let total=A+B using32-bit add. If signed total>C,skip ring setup and continue
paper selection. Do not invent allocation/fallback/error handling. Otherwise,
if signed current cursor W+f4+total>C,write cursor0. Exact equality retains it.
Then,with fresh W reads at every step:

1. W+e8=1.
2. W+f8=word[W+ec]+12*current cursor.
3. Advance cursor by CAPTURED A,store W+f4.
4. W+fc=word[W+ec]+12*CURRENT cursor.
5. Advance cursor by CAPTURED B,store W+f4.

No negative-count/capacity/address-overflow guard,alignment or cache clear is
local. Oversize skip preserves previous e8/f8/fc/cursor. The initialized ordinary
capacity is1024 from [animInit](ANIMATION_POOL_AND_TANGENT_INITIALIZATION.md).
Main uses these alternate global outputs iff e8!=0;source/ownership/capacity
invariants are not established by the ring publisher alone.

The inner service uses the same request graph on a late cache-miss branch,
but clears W+e8 after its admitted Main call even if total>C skipped setup.
Wrapper's linked cleanup differs:it clears e8 only AFTER the inner request and
only if linked Q+4 mask2 is SET on the final reread.

## Paper selector resolved into linked ordinary animation

Read parent P+94 as raw selector K. K==−1 skips selection;otherwise reread
parent link P+90 and current work/pool to capture selector record Qs/resource Ds.
Use SIGNED K encoding:negative means strcmp scan through64-byte name slots
Ds+1ac,count Ds+148;first equal selects index. Nonnegative K<count selects K.
Missing name/out-of-count selects0. There is no bound/validity guard for a
negative string pointer,count<=0 or selected row0. This is the same encoding as
[ordinary setter](ANIMATION_SELECTOR_AND_RESTART.md),but−1 is a special skip here.

If selected index equals CURRENT Qs+14,skip ordinary restart. Otherwise store
Qs+14;Qs+90!=−1 AND Qs+0 mask2 SET causes OR mask4 into Qs+0. Obtain the
[clock quotient](ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md) from CURRENT Qs+c,
then store LOW Qs+1c,HIGH Qs+18,signed−1 Qs+3c,float0 Qs+40,word0 Qs+80,
float0 Qs+84. Unlike the pre-display force1 setter,equal index does not restart
at this first step. No exported setter call occurs;the code is inline.

Regardless of K==−1/equality/change,still on wrapper's Q+4 mask2 path:
obtain ANOTHER clock quotient from captured Q+c and unconditionally reset
captured Q+1c/+18/+3c/+40/+80/+84 in that order. Then read parent P+88/+8c
and copy LOW to Q+1c BEFORE HIGH Q+18,overwriting that quotient. Finally call
animPoseMain(CURRENT parent P+90) at8003b8f0. This consumes parent paper time
through linked ORDINARY timestamp/index and initializes its caches.

Qs and Q normally alias,but are captured at different points;do not assume that
under work/link mutation domains. The second reset occurs on EVERY admitted
wrapper preparation,even K−1 or an equal selector. It is not a once-per-tick
request. Parent paper timestamp and linked ordinary timestamp remain distinct
storage until this explicit copy.

## Inner cache-miss binding

Inner captures current/next sample indices and fraction BEFORE the late
P+3c==−1 check. On cache miss compute ID literally:
ID=high32(0xb21642c9*(P−CURRENTpoolBase))>>8 (unsigned operations).
Valid aligned pose pointers give the pool index;there is no pointer/alignment
validation. If P+4 mask2 SET,re-resolve CURRENT resource counts and run the ring
publisher,call Main(ID),then clear CURRENT W+e8. CLEAR mask2 simply calls Main.
It retains the previously captured resource/T/indices/fraction for later traversal;
no automatic recapture after Main. Source/callback/pool invariants must justify
that retained state in an implementation.

`verify_animation_draw.py` pins the full wrapper/inner reset/ring/Main graph.
Review equal-cursor-capacity,oversize preserved outputs,negative count domain,
K−1 skip versus unconditional second reset,equal selected index,changed index,
borrow-free timestamp copying and live Qs/Q differences. These are static
acceptance requirements,not original execution or full resource parity.
