# Paper-group binding: reuse searches,flags and failed lookup

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
animPoseSetPaperAnimGroup0x80040e04,size0x288. Used directly by
[partner paper wrappers](../partner/FIELD_PARTNER_PAPER_REQUEST_WRAPPERS.md).
Animation pool addressing/loaded resource chain is described in
[selector contract](ANIMATION_SELECTOR_AND_RESTART.md). No runtime observation.

Input ID r3,name pointer r4,B r5. Capture byte-offset O=0x170*ID and current
record R from initial work/pool. B is tested only as zero/nonzero. Unlike the
animation selector,name admission is a NULL comparison;no numeric selector mode.

## Null name

Write R+90=−1. Then reload current work/pool,load record at CURRENTbase+O+ec,
clear ONLY mask1 and store there. Return. Ordinary R+0 flags2/4,paper index+94,
timestamp+88/+8c and other fields remain unchanged locally. No release,
reference decrement,resource deallocation or Main request occurs.
If work/pool can change,R and the reloaded record need not alias;preserve that
load ordering instead of assuming a single permanently captured pointer.

## First and fallback searches

Capture current R+c as mode M. First scan signed i from0 while i<CURRENT W+14;
reload W each loop test and compute candidate from its current pool and retained
byte cursor (initial0,increment0x170 on rejection). A candidate is eligible iff:

- Candidate word+0 mask1 SET.
- Candidate word+4 masks1 AND2 SET.
- Candidate word+c equals captured M.
- strcmp(candidate resource D+4,requested name)==0.

The resource D is resolved through candidate+10 and CURRENT W+0 exactly as in
the selector contract. It compares the group/resource name at D+4,not the
animation name array. First matching candidate wins. After exiting,reload W+14;
if i equals that CURRENT count,use−1;otherwise retain i. Count or pool mutation
across callbacks is a source/lifetime domain:the local comparison is equality,
not a hardened i>=count rule. Store result R+90.

If stored linked ID is SIGNED negative,repeat the scan from0 with the same
active/word+4/name predicates but candidate+c must equal CONSTANT2 rather than
captured M. Use the same final current-count equality rule and publish R+90.
A successful first pass skips fallback. Both failures produce−1 in the ordinary
stable-count domain. It does NOT allocate a missing group or reset to record0.
No candidate busy/reference acquisition or ownership write is present.

## Publication even after failure

Non-null input ALWAYS proceeds after the search:

1. Store signed−1 R+94.
2. Get [clock quotient](ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md) according to
   CURRENT R+c;store timestamp LOW R+8c BEFORE HIGH R+88.
3. B!=0:OR mask2 into R+0,reload and OR mask4. B==0:clear mask2,reload and
   clear mask4. Other flag bits survive.
4. Reload CURRENT work/pool;OR mask1 at CURRENTbase+O+ec.

These writes occur even if R+90 remained−1. A caller may subsequently reach the
paper-animation setter's unguarded linked-record access;the failure is not
silently repaired here. The function does not itself call Main or release a
paper-pose handle. Wrapper paper resource entry/release and this reusable group
binding are distinct operations,not one ownership abstraction.

## Used bindings and boundaries

Twenty direct DOL references are pinned by
`python3 tools/verify_animation_selection_time.py`. Used partner On800bf168 and
Off800bf02c,pipe setup/reinit and Mario On/Off are included;message playback
800844e0/80084544 also calls it. NPC/battle variants have their own call sites.
This census is not proof that every group/name/state is reachable in US play.

Implementation checks should separate null name from failed nonnull name;
first-pass same-mode wins over mode2;flags must be simultaneously satisfied;
failed nonnull still resets index/stamps/flags/ec;B0 andBnonzero write distinct
masks;no invented allocation/free/default pose. Complete pool initialization,
resource loaders,group/name domain,linked lifetime,Main/draw/clock/numeric and
original paper visuals remain open. This file closes the local binding body.
