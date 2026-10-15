# Original NPC field tail and retained whole Main invocation

Source8004b1d0..8004b3e4 now follows [actor traversal](NATIVE_NPC_MAIN_HISTORY_AND_TRAVERSAL.md).
The native whole-Main cursor executes actor traversal once, then this tail,
then acknowledges completion only after the final original-field byte clear.
Repeated calls after completion publish nothing. This is one source invocation
under admitted services, not closure of field events/rendering or normal Game
activation. Standalone actor traversal still returns its unfinished FieldTail.

## Effect state and queue

Reload GP80415fd0, capture F=GP168 and E=F550. E0 nonzero is the only effect gate;
E4 is not consulted. States1/2 read counterC, increment wrapping32, store, then
RELOAD C. Comparisons are signed. Other states skip directly to field mode.
State2 count<20 requests intplGetValue(5,count,20,-600,0);20..79 writes zeroX;
80..99 requests intplGetValue(1,count-80,20,0,+600);>=100 clears E4, preserving
E0 and skipping the queue. Reached animation steps load a DISTINCT fresh zero,
store E14, then register camera9/class2/callback80047dfc/actualE with the SAME
retained zero depth. Plateau's X zero is an earlier separate load.
State1 count>60 clears E4 and skips queue; otherwise ONE zero load feeds E10,
E14 and queue depth. Do not clamp wrapped negative counts, normalize zero or
reload it after stores. Queue NaN rejection is acknowledged without a record;
missing/capacity providers hold without effects.

Native interpolation reuses cameraInterpolation modes1/5, with the audited
single arithmetic grouping and signed counter conversion; no clamp or FMA.
Mode1: A+((S*S)*D)/(N*N). Mode5: (A+D)-((R*(R*R))*D)/(N*(N*N)), R=N-S.
Every operator is rounded single as in source; original FPSCR/int-conversion
runtime parity remains a numeric adaptation boundary. Constants are original
DOL -6008041fce4,+6008041fce8, zero8041fc7c. Signs corrected in the parent/ABI
contracts remain authoritative.

## Encounter, Kpa queries and copied score position

Read signed half F0 fresh ONCE after the effect.1 requests fbatEncountCheck;
2/3 request fbatTalkMode;5 requests fbatBattleMode;4/6/other/negative skip.
After encounter, reload GP and capture F2=GP168 BEFORE marioGetPtr. Retain
returned parent Mario M. Fresh M3c==2 requests kpaJumpChk; a SECOND independent
M3c==2 requests kpaFireAttackCheck. Jump can change the second gate. These
helpers each fetch Mario AGAIN internally; do not replace that fresh lookup
with parent M or repeat the outer type gate inside the helper.

Native read-only atomic query adapters use actual VM Mario/work bytes. Jump
returns1 iff signed half50 is nonzero. Fire requires word0 mask1, then reads
actual work298+13c; returns1 only for signed kinds20..23 or40..43. Missing maps/
owner admission holds the entire atomic child without effects; this is native
availability policy, not a source pause inside the helper. Each retry admits a
fresh child lookup. No flags/work defaults or guessed Kpa state.

If jump nonzero, capture signed F257c score; only score>0 reads signed F2578
and requires>=2. Eligible score retains the captured score argument and copies
M Xword,Yword, loads original60, forms single(Y+60), then reads fresh Zword.
The semantic copied XYZ words replace source stack48..50, never fabricated guest
pointers or actor aliases. Request kpaAddScorePos with that captured score/copy.
Jump true clears F2578 THEN57c regardless of eligibility; jump false preserves.
Fire false clears F2574; true preserves. Final byte54d is ALWAYS cleared through
ORIGINAL F, even if callbacks changed GP/F2/Mario. Neighbor bytes remain intact.

## Composition, ownership and evidence

Each successful primitive/request is retained across holds; false/empty has no
effects. Whole Main stays bound to its native binding and same VM/pool aliases.
Actor scan is never rerun after the tail begins. Queue generation, source pass,
field/Mario/mapping/resource owners and borrowed userdata remain live through
holds and deferred dispatch. Encounter/talk/battle/score are mandatory real
providers; missing handlers hold. No no-op success or fabricated event/dialogue.
The announcement callback80047dfc still needs a real native consumer. Normal
Game continues its existing field path pending complete production wiring.

Tests cover signed/wrapped counter boundaries and mandatory reload, both easing
requests/signs, independent trace, every primitive hold, retained zero/negative
zero/NaN queue acknowledgment, field modes, repeated mode gates, score eligibility,
copied bits and precise final byte. Native tests continue the actual multi-actor
scan into queue/encounter/fire-map/score holds, switch Mario inside fire admission,
change GP/score/root afterward, and prove original F/captured score/copy survive.
A fresh zero-actor invocation and real Kpa jump halves/fire flag/windows/gaps/
negative-kind boundaries also pass. These fixtures verify local composition,
not original execution/story/visual acceptance.

Ignored artifacts/npc-main-parent-evidence/tail-review-notes.txt and *-tail-review
listings pin bodyb1d0/214hex SHA256
c398b572e7e513be4096fab9fe15f0051a16a2436d5c3574c0b3d50e61880bff.
Helper intplGetValue80006c8c/3b0hex SHA256
0d800da046dc09ba1fe07a6933b4a910553c1fbe7ea2f6a874395018f4b6a4ae.
Kpa helper/score identities are recorded in the audit. Static source evidence is
separate from Dolphin observations. Native core is allocation-free and reuses
existing memory/display owners, scalar ARM/VFP arithmetic and explicit provider
boundaries for future areas. Actual resource/event/render/pass owners, original
numeric/visual parity and physical performance remain required.
