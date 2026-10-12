# Complete battle fortune countdown and mode producer

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete BattleMajinaiCheck800f7774/150. Read
[pouch initializer/call bindings](BATTLE_FORTUNE_INIT_AND_CALL_BINDINGS.md),
[mode1 consumption](JUMP_MAJINAI_POWER_CHECK_CHILD.md),
[first-process caller](BATTLE_TURN_FIRST_PROCESS_COMPLETE.md) and
[original RNG](RANDOMNESS.md).
Verify:`python3 tools/verify_battle_fortune_producer.py`:eight complete/linked
hashes,108 instructions,SDA span and two scoped DOL direct calls.
Static original instructions;no original activation/turn/RNG-interleaving trace.

## Capture order and three distinct pouch bytes

Call pouchGetPtr first;retain returned P. THEN read global battle B from
8041eb90. Read U8(P+5b8) as remaining count. The countdown at5b9 is read as
U8 for stores/arithmetic and S8 for branch tests;selected mode5ba is U8.
Do not combine these into one turn counter or interpret their bytes uniformly.
No P/B null,source lifetime,descriptor or value validation is supplied.

If remaining count is EXACT0,return word0 immediately without reading5b9/5ba,
writing any state or drawing RNG. B was still read before the count guard,but
is not dereferenced on this path. In particular,stale countdown/mode survives.
Check never decrements remaining5b8;the separate Done service owns that mutation.

For nonzero remaining,read countdown byte C at5b9 and test S8(C)<0 FIRST.
This test precedes any selected-mode read. All C128..255 therefore take the
negative branch,including initialff. A nonzero selected mode does not suppress
that branch;negative countdown takes precedence over mode preservation.

## Negative countdown initialization and immediate decrement

Perform these exact operations at800f77b0..d4:

1. Store byte P5ba=0 before RNG.
2. Call irand(5) once;retain returned word R.
3. Store LOW8(R+6) to P5b9.
4. RELOAD U8(P5b9),subtract1,and store its LOW8 to P5b9 AGAIN.
5. Return word0. No setup-flag read or mode draw.

For ordinary irand result0..4,the intermediate count is6..10 and final stored
count5..9. Both stores matter;do not collapse the published intermediate and
reload into an atomic R+5 assignment. Retained P receives later writes even
if an RNG adapter changes current pouch pointer. Mode is cleared even when
an old value was nonzero. Remaining count5b8 is preserved.

This describes a delay in Check calls,not a measured number of rendered frames
or wall-clock seconds. The source caller's actual invocation conditions are
separate. Global shared RNG state advances once;there is no extra probability
draw here.

## Nonnegative countdown: existing mode versus ready selection

For S8(C)>=0,read U8(P5ba). If nonzero,return that exact unsigned byte without
countdown decrement,setup read or RNG. Values1..255 are returned as-is;there
is no mode-range normalization or generic bool conversion.

For mode0,store LOW8(C-1) to P5b9,then RELOAD the byte and interpret S8.
If reloaded value>0,return0. Otherwise store P5b9=0 and proceed to mode
selection. C1 becomes0 and selects immediately;C0 temporarily becomesff,
then its negative reread also selects after the explicit0 store. C2 becomes1
and returns0;C127 becomes126 and returns0. There is no initial C<=0 check
that skips the decrement/store/reread sequence. The C used for decrement is
the captured byte from before mode read,not a new pre-decrement read.

The selection source is captured B,then pointer D=word(B+2738),then
pointer S=word(D+c),then flags=word(S+4). Test numeric mask0x20 (from
rlwinm0,0,0,26,26 at800f781c). These pointer reads happen AFTER countdown
has been explicitly stored0. No null guard or source pointer recapture after
RNG. This is a local setup-record flag path;full descriptor/scene writers and
concrete encounter values remain separate. Do not substitute unrelated B.ef4
flags,rule byteD18 or a port's tutorial enum.

## Exact inline mode thresholds and raw return

Call irand(100) ONCE through the selected branch. Compare the signed returned
word against these strict thresholds;store the resulting byte to retained P5ba:

| Setup flags0x20 | Returned R range under ordinary0..99 domain | Mode |
| --- | --- | --- |
| Set | R<45 | 1 |
| Set | 45<=R<90 | 2 |
| Set | R>=90 | 3 |
| Clear | R<30 | 1 |
| Clear | 30<=R<60 | 2 |
| Clear | 60<=R<80 | 3 |
| Clear | R>=80 | 4 |

This is inline comparison code,not an external weighted table. Only one mode
byte is written. After writing it,RELOAD U8(P5ba) at800f78ac and return that
byte,not the pre-store branch literal. An observable aliased store/reload must
remain live. Remaining5b8 and countdown0 are preserved after selection;Check
makes no Done/EndCheck call,cost/damage/event/message/effect/save request.

The verified integer RNG returns0..32767 and uses modulo reduction for bound100.
Threshold intervals therefore do NOT prove exact30%/45% measured rates. See
conditional residue enumeration in the companion contract. Negative/out-of-range
injected RNG results would still follow the signed compares;they are not clamped
locally. Seed/writer/replay/interleaving and actual source domain remain open.

## Connection to delivered consumers and implementation checks

Mode1 is tested by the ordinary Jump power-check child;its true path writes
battle18ff9=1,then Done decrements remaining and resets countdownff/mode0.
Mode2 is tested by move controller state2 for its defence-event request.
Other mode consumers and raw writers are not inferred here. Check can return
nonzero repeatedly without consuming count,while consuming services reset state.

Acceptance:count0 stale state preservation;all signed countdown boundaries
0/1/2/127/128/255;negative mode-clear before irand5,two countdown writes and
reread;mode-nonzero priority only after nonnegative test;mode255 raw return;
ready0→ff→0 sequence before setup dereference;captured pouch/battle versus
current global changes;exact mask20 and every threshold edge29/30/44/45/59/
60/79/80/89/90;one draw only on the taken ready/negative path;final mode reread.

The complete local producer is delivered. Pouch/source assignment/save/reset,
setup descriptor/flag writers,full mode consumers/turn/scene ownership and
original activation/RNG/timing traces remain EVT-01/JUMP-01/STATUS-01 requirements.
