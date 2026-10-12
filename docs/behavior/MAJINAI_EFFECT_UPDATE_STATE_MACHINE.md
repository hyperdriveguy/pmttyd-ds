# Complete Majinai effect update state machine and cleanup

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete effMajinaiMain8025c5ec/394. Verify:
`python3 tools/verify_majinai_effect_update.py`:26 complete/linked identities,
338 instructions and five spans. Resource/animation/matrix/display/driver body
hashes are identities,not newly recovered backend semantics. Read
[draw/material leaves](MAJINAI_EFFECT_DRAW_AND_POSE_REQUESTS.md),
[construction](MAJINAI_EFFECT_CONSTRUCTION.md),
[soft/hard deletion](EFFECT_SOFT_AND_HARD_DELETION.md) and
[name-wait caller](JUMP_MAJINAI_EFFECT_AND_NESTED_SOURCES.md).
Static evidence;no original update/frame/resource/runtime trace.

## Entry captures and payload interpretation

Input is effect record E. Retain E and payload D=word(E+c). No null/active/
payload/record-count guard. Copy source zero-vector80302f80 into stack storage,
then overwrite with D.X4,Y8,Zc and copy those raw coordinate words to a SECOND
local vector for later display-depth query. The depth vector is captured BEFORE
phase callbacks;it is not recomputed from post-callback payload coordinates.

Capture GP pointer at8025c648,then read E.flags0 and captured GP14. Normalize
GP14 nonzero to1,else0,retaining that context for phase0 resource calls. E.context4
written by pool acquisition is NOT read. Later phase material paths reload GP
independently. D.state18 is signed word,D.alpha14 signed word,D.pose1c signed
ID with exact-1 sentinel,D.scale10 binary32. D.word0 (constructor K) is not read
by this local Main or Disp. Position and scale are not locally mutated here.

If E.flags has numeric04,perform soft cleanup BEFORE state dispatch. Otherwise
read state and dispatch exact0/1/2/3. Every other signed state takes the common
draw-queue tail without normalization. A transition uses a LIVE state reread+
wrapped1,not assignment of a branch's expected next literal,and does not
redispatch the next phase within this invocation.

## Soft-request cleanup and normal-expiry cleanup

Soft branch8025c680 clears numeric04 only from captured flags and stores E0.
Read D.pose1c;if !=-1,request animPoseRelease(pose). Then request mapGetWork,
read U16(map+4),clear numeric08,store U16(map+4),call effDelete(E) and return
WITHOUT display queue. It does not fade colour/alpha,clear D.pose,run phase3
actor material-Off requests or reset map colour1e. Do not add those restoration
requests to a generic destructor. Under ordinary flags7,clear04 leaves3 and
hard deletion releases heap3 payload;if unusual flags04 alone become0,the
hard leaf's zero-flag gate can skip free. Preserve the actual leaf request/gates.

Normal expiry after phase3's fade/material/alpha work requests the same pose
release-unless-1,map halfword08 clear,hard deletion and return-without-queue,
using a NEW mapGetWork call. This cleanup does not perform the soft branch's
E0&~04 store. Effect-specific update consumes the soft request through hard
release;the deletion leaf itself did not schedule this invocation.

## Phase0: asynchronous group admission and pose setup

At8025c6e0 request animGroupBaseAsync(name80302f8c=`MOBJ_EFF_majinai`,
captured contextBool,0). Return exactly0 exits WITHOUT state change or draw
queue. Any nonzero result,including an invalid negative domain value,proceeds:

1. animPoseEntry(same name,same captured contextBool);store returned ID to D1c.
2. RELOAD D1c and request animPoseSetAnim(ID,string80428b2c=`A_1`,1).
3. RELOAD state D18,store wrap32(state+1).
4. Execute common display queue tail;do not execute phase1 now.

No returned pose-ID failure/null check or frame reset beyond those services.
Resource acquisition/animation loading and actual admission latency remain backend
requirements. First successful phase0 can queue a draw with pose-1;Disp skips
it using its own sentinel guard. Later phase loop queries do not supply that guard.

## Phase1: map darkening,actor material bit and progress threshold

Call mapGetWork,retain M. Read U8(M1e),subtract5 as signed word,clamp lower
bound64. Load raw template000000ff from80428b14,replace its three leading bytes
with LOW8(clamped value),then store the entire WORD to M+1e. Thus the map colour
is (v,v,v,255);it is not three isolated map-byte stores. Read U16(M4),OR08,
store U16(M4). All other halfword bits remain. No upper clamp is needed for
ordinary U8-minus5,but source operations remain exact.

AFTER those map writes,reload CURRENT GP pointer and GP14 for actor material
requests. GP14==0 obtains marioGetPtr once and visits five WORD pose IDs at
Mario+22c,+230,+234,+238,+23c ascending;call animPoseSetMaterialFlagOn(ID,800)
for EACH,without ID validity/sentinel tests. Nonzero GP14 gets current global
battle Mario,BodyPartsId,GetPartsPtr,reads part.word1c0,and requests On(ID,800)
once. Neither branch checks missing Mario/part/null or mirrors the retained
phase0 context/effect.context4. Linked unit/part contracts define their queries.

Then request animPoseGetLoopTimes(D.pose1c). Compare returned binary32 f1
against exact3f6e147b at80428b30 (approximately0.930000007). Only ordered
>= passes `fcmpo;cror2,1,2;bf2`;unordered does not advance if exceptions permit
continuation. Below threshold queues draw with no state change.

For passing progress,read current D.scale s and coordinate fields and request
 effStardustEntry with integer arguments(0,16,60),float arguments:
X, fusedSingle(55*s+Y),Z,single(45*s),same size copied into fifth float argument.
Constants55/45 are425c0000/42340000;preserve one fmuls and one fmadds rather
than separate multiply/add rounding. Ignore returned effect pointer. Then LIVE
state reread/increment and common queue tail. This local graph does not establish
Stardust resources/update/emission/deletion behavior or observed particle count.

## Phase2: animation completion gate

Request animPoseGetLoopTimes(current D1c). Compare ordered>= exact binary32
1.0 at80428b3c,with the same CR tests. On admission LIVE D18 reread/increment;
otherwise no state write. Both paths queue display. No map colour/material/alpha
update in this phase. The transition does not execute phase3 during this call.

## Phase3: map brightening,actor material off and alpha expiry

Call mapGetWork,retain M. Read U8(M1e),add5 and clamp upper bound255. Use
separate source template000000ff at80428b18;patch three leading bytes and
WORD-store M1e,then read/OR08/store U16(M4). This is the brightening counterpart
of phase1,not a restore of a saved original map colour.

Reload CURRENT GP14 and request actor material Off(ID,800) using the same
field five-ID versus battle Mario-body-part route and read order as phase1.
It uses current actors/poses,not a retained list from the earlier On operations.
Then read SIGNED D.alpha14,subtract5 with wrap32,store,reload and test signed<0.
Negative result invokes normal cleanup above;nonnegative result queues display.
There is no alpha clamp or phase increment. Under initial alpha255 and no
external writes,the first51 phase3 invocations publish250..0 and can draw;
52nd publishes-5 and cleans up with no draw. These are derived UPDATE CALL
counts,not measured frames or wall-clock lifetime. Wrap/extreme source values
follow word arithmetic rather than a native <=0 timeout policy.

## Common display queue and remaining obligations

Nonterminal phases/default states pass the BEGIN-captured vector to dispCalcZ,
then request dispEntry(4,2,callback8025c4c0=effMajinaiDisp,E,returned float depth).
One queue request per admitted update tail;no immediate call of Disp and no
local renderer/driver frequency. Resource-not-ready and either cleanup exit
omit the queue. Main has no scalar success contract. Map/provider/GP changes
between requests are not forced into one stable frame snapshot.

Acceptance covers soft branch before phase,distinct cleanup requests,all signed
state cases/live increments,no same-call redispatch,resource0 versus nonzero,
original group/animation names,three-byte scratch patches/four-byte map stores,
map flag08 preservation,current GP/field five poses/battle one pose,exact.93/1
ordered/unordered gates,fused Stardust parameters,alpha0 still queued versus
negative expiry,52 derived calls,early coordinate depth snapshot and all queue
omissions. Main/Disp's local requests are delivered;driver/camera/matrix/resource/
pose/particle/render/scene ownership,numeric exceptions and original traces
remain EVT-01/JUMP-01/DMG-02 requirements.
