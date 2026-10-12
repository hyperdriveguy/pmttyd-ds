# Fortune count initializer,call bindings and exact RNG accounting

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete pouchMajinaiInit800d31d4/30 and event wrapper
 evt_pouch_majinai_set800f08ac/30;two DOL direct caller sites and full producer
linked in [BattleMajinaiCheck](BATTLE_FORTUNE_PRODUCER.md).
Verify:`python3 tools/verify_battle_fortune_producer.py`. All execution/timing/
probability observations remain separate from static source deductions.

## Initializer only assigns when unsigned old byte is signed-less than request

Input is signed word N. Read CURRENT pouch pointer P0 directly from8041eb00,
then old=U8(P0+5b8). Compare SIGNED word old with N. At800d31e0,bclr4,0
returns when the less-than condition is FALSE:old>=N. That early path makes
NO byte writes and does not read another pouch pointer or reset selected mode.
There is no scalar success contract or cost/inventory/save update.

If old<N,perform:

1. Store LOW8(N) to P0+5b8 at800d31e4.
2. RELOAD current pouch pointer P1 at800d31f0;store byteff to P1+5b9.
3. RELOAD current pouch pointer P2 at800d31f8;store byte0 to P2+5ba.
4. Return with no specified scalar result.

Those are three pointer captures,not one atomic pouch update. P0/P1/P2 may
differ under an observable provider;no equality/consistency or rollback guard.
The comparison uses the full signed N before byte narrowing. Thus old10,N5
and old10,N10 preserve all three fields;old10,N11 updates count11/resets ff/0.
N=-1 is rejected for every U8 old,while old255,N256 is admitted and stores0.
This is not generic max(old,U8(N)),saturating count,unconditional clear or a
strict guarantee byte count never decreases. Names do not establish that input
N is a paid field fortune duration;caller/source domain remains separate.

The event wrapper captures argument pointer E18,reads raw argument0,resolves
through evtGetValue(E,raw),passes the returned signed word to pouchMajinaiInit,
ignores its result and returns handler2. It does not read callback firstCall,
store output,create an event,read/charge coins or normalize N. Original field
script/REL caller operands and any payment/upgrade conditions remain open.

## Direct original call sites and first-process ordering

A scan of aligned linked-branch words in all seven DOL text sections finds:

| Target | Direct site | Containing caller |
| --- | --- | --- |
| pouchMajinaiInit800d31d4 | 800f08c4 | evt_pouch_majinai_set800f08ac/30 |
| BattleMajinaiCheck800f7774 | 8011b21c | btlseqTurnFirstProcess8011b1ec/f0 |

The verifier recomputes this exact two-site set. This is not a census of raw
pouch writes,REL imports,indirect calls or script opcode91 references;those
are distinct source requirements. An original resource can call the event
wrapper indirectly even though the DOL direct initializer census has one site.

The complete [first-process contract](BATTLE_TURN_FIRST_PROCESS_COMPLETE.md)
calls JudgeTurnRuleKeep FIRST,then increments/stores/reloads signed halfword
B0,then requests BattleMajinaiCheck only if the reloaded value>=2. This check
precedes clearing battle18ff0/18ff4 and incrementing battle record16f57. It is
not gated by that separate capped byte counter and not called on every action
selection. The explicit B supplied to first-process can differ from global B
captured inside the producer;producer also selects its own current pouch.
Original outer turn frequency and scene invocation are still unmeasured.

## Branch-specific random draw budget

The full producer makes no hidden extra draw. It calls original irand as follows:

| State at call | Draw request | Writes / return |
| --- | --- | --- |
| Remaining5b8==0 | None | Preserve countdown/mode;return0 |
| Remaining nonzero,S8(countdown)<0 | irand(5) once | Clear mode;publish R+6 then reread/decrement;return0 |
| Nonnegative countdown,mode!=0 | None | Preserve state;return exact mode byte |
| Mode0,post-decrement S8(countdown)>0 | None | Keep decremented byte;return0 |
| Mode0,post-decrement S8(countdown)<=0 | irand(100) once | Set countdown0;select mode from setup mask20 thresholds |

Each positive-bound call consumes exactly ONE underlying rand invocation.
Done,EndCheck and initializer consume none. Replacing modulo irand with a
uniform host distribution,rolling every turn regardless of state,or generating
a separate delay and probability on the same initialization path would alter
shared RNG interleaving. Store order/state capture is just as important as the
returned category for deterministic replay.

If enumerating each possible15-bit rand output0..32767 ONCE,the modulo residues
have the following derived counts (not empirical probabilities):

| Bound / policy | Residue or mode | Number of possible15-bit outputs |
| --- | --- | --- |
| irand(5) | residues0/1/2 | 6554 EACH |
| irand(5) | residues3/4 | 6553 EACH |
| irand(100),mask20 clear | mode1 /mode2 /mode3 /mode4 | 9840 /9840 /6548 /6540 |
| irand(100),mask20 set | mode1 /mode2 /mode3 | 14760 /14738 /3270 |

These counts derive from modulo100:residues0..67 have328 preimages,68..99
have327. They do not establish equal-likelihood outputs in an actual seeded,
interleaved run,or observed activation rates. The original seed/state/caller
schedule remains necessary evidence for execution comparison. The threshold
code itself,not rounded percentages,is the implementation requirement.

## Acceptance and remaining source ownership

Fixtures should preserve old>=N no-write paths,equal/negative/256 requests,
full-word comparison before narrowing,three current-pouch pointer reads and
split-owner stores,event wrapper raw input resolution/no output,exact two DOL
sites and prior halfword>=2 gate. Producer fixtures need every draw/no-draw
branch,intermediate countdown stores,signed mask/threshold behavior,raw active
mode preservation and full global RNG sequence rather than isolated random
category assertions.

Field fortune purchase/assignment/payment/dialogue,REL/event wrapper callers,
pouch count/mode/raw writers/save snapshots,setup flag source values,other mode
consumers and original turn/activation traces remain open. These local routines
do not create a field fortune transaction or authorize invented story dialogue.
