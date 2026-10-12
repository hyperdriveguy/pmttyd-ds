# Battle pad sampler/reset owners and tutorial history injection

US G8ME01 disc0 revision0, agent ID1, recovered2026-10-12. Evidence B: static
DOL opcode18 references,initialized-data candidates and module0 import streams
in all29 shipped RELs,plus complete small reset helpers and one relocated gor
helper. Names/owner intervals come from the supplied US symbols. No original
execution or complete indirect writer graph is asserted.

Reproduce the ignored metadata census:

```sh
python3 tools/input_ownership_audit.py --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/input-ownership-audit.json
```

The tool checks six DOL bodies,30 instruction anchors,two data bindings,one REL
helper body/relocations,two actual script calls/masks and the demonstration event
hash/selected branch words. It annotates direct references with containing symbol
intervals and REL imports with section/file offsets and owner candidates. A
symbol owner is an annotation;computed calls,interior aliases and unrecognized
functions are not ruled out. Direct counts include nonlink branches,though the
sampler/reset sites below are all branch-and-link calls.

## Actual direct sampler census

| Target | DOL direct caller site/owner | Meaning from recovered local body |
| --- | --- | --- |
| BtlPad_WorkUpdate0x8011a0ac | 0x8011a464,BattlePadManager | Four general pads,channels0..3,one update each per manager call |
| Same | 0x800fa5cc,BattleActionCommandManager | Private W+0x1d1c,physical channel0,even with no main callback |
| BattlePadManager0x8011a43c | 0x800f8e44,BattleMain | General updates before sequence and private manager |
| BattleActionCommandManager0x800fa5b0 | 0x800f8e50,BattleMain | Private update before selected AC main |

No audited module0 REL imports target either sampler or manager. This confirms
the known direct ownership routes across the shipped import census;it does not
prove there are no indirect sampler calls or writes to pad fields.
[Cadence/snapshot rules](BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md) specify the bodies.
[Physical GP production](PHYSICAL_INPUT_PRODUCER.md) is upstream and separately
owned. Event execution credits do not themselves shift the pad histories.

## Four DOL reset sites and one additional tutorial import

BtlPad_WorkInit0x8011a38c has these four direct DOL call sites and one shipped
module0 REL24 import:

| Site | Owner | Destination/reset context |
| --- | --- | --- |
| 0x8011a4b0 | BattlePadInit | Each of four general buffers |
| 0x800fa644 | BattleActionCommandManagerInit | Private W+0x1d1c |
| 0x800fa180 | BattleActionCommandResetDefenceResult | Private W+0x1d1c after clearing W+0x1cb0 |
| 0x800fa404 | BattleActionCommandCheckDefence | Private W+0x1d1c on every local defense return |
| gor section1+0x116ec,file0x117b8 | _config_key_record@unit_lecture_christine.o | Private pad reset followed by injected trigger-history word |

WorkInit's preserved release array remains as documented in
[local pad/defense](BATTLE_INPUT_AND_DEFENCE.md). Do not replace the owner-specific
resets with a single reset of both general and private histories after every hit.

### General and AC manager initialization bodies

BattlePadInit0x8011a48c,size0x4c iterates four general pads at
sharedW+0xf2c+0x1fc*c,reloading shared W each iteration,and invokes WorkInit.
The sole direct DOL caller is BattleInit at0x800f8f50. This is distinct from the
per-pass sampler at0x800f8e44. Full BattleInit/sequence allocation lifetime is
covered only to the previously recovered extent.

BattleActionCommandManagerInit0x800fa62c,size0x4c takes W explicitly. First
WorkInit its private pad. Then store0 into callback words+0x1ca0,+0x1ca4,+0x1ca8,
+0x1cac in that order,and signed word-1 into+0x1f18. There is no local callback
invocation/free,override-counter reset or general-pad reset. Complete caller
lifetimes and delete-callback ownership remain dependencies.

BattleActionCommandResetDefenceResult0x800fa164,size0x30 reads shared W,stores0
at+0x1cb0,then WorkInit(W+0x1d1c). It has no local first-call/command-active gate
and does not modify shared override counter+0x1d18 or general history. The full
CheckDefence body clears its result at entry and resets the private pad on exit;
its conditions/results remain in the separate defense contract.

## Complete relocated tutorial key-history writer

The supplied gor helper _config_key_record is file0x11784,size0x54,SHA-256
`ef58d190898204409380f0c4e2eb7e14b2093b464c56aa117b526e9a998dc791`.
Its DOL-global HA/LO relocations at0x1179e/0x117a2 bind0x8041eb90,the shared
BattleWork pointer location. The REL24 call at0x117b8 binds WorkInit0x8011a38c.
Unrelocated branch targets printed from the temporary inspection ELF are not
runtime destinations;use these original relocation bindings.

The complete callback behavior is:

1. Read event argument-span pointer at+0x18.
2. Read shared BattleWork W and capture operand0 **raw**,without evtGetValue.
3. Store word4 to W+0x1cb0.
4. WorkInit(W+0x1d1c).
5. Store the raw operand to private pad+0x18,that is W+0x1d34,new-trigger history
   index0.
6. Return callback2 immediately.

It ignores the first-call flag and has no local yield,unit lookup,physical poll,
held/repeat synthesis or general-pad write. Passing an encoded local-variable
operand would store its encoding bits,not its resolved contents. The injected
history therefore need not agree with the private current held snapshot,which
WorkInit has just zeroed. Do not reconstruct all press edges solely from held
changes after this explicit script writer runs.

The prewritten result4 is not an unconditional final guard result:the subsequent
CheckDefence entry clears W+0x1cb0 before evaluating the injected history and
normal eligibility/modifier/override rules. The history write,not a bypass of the
checker,is the concrete input effect established here.

## Shipped demonstration-script branch binding

Two self-ADDR32 script imports bind this helper:

| USER_FUNC request / function-pointer site | Raw operand0 |
| --- | --- |
| 0x67c20 /0x67c24 | 0x100,the checker A trigger mask |
| 0x67c34 /0x67c38 | 0x200,the checker B trigger mask |

Both are in lecture_zutsuki_attack_event@unit_lecture_christine.o,gor file0x67680,
size0x8e8,162 commands,SHA-256
`5d1b151dce5e580660d7c60201a78a9c3429f6631a1426fd664d3e7808867282`.
Only the following decision slice is specified here;the whole stream's other
movement/lecture conditions are not claimed recovered:

- At0x67bfc request GetUnitWork(actor selector-6,slot1,local0).
- SWITCH(local0) at0x67c10;CASE_EQUAL3 at0x67c18 invokes the A-mask helper.
- CASE_EQUAL7 at0x67c2c invokes the B-mask helper.
- END_SWITCH at0x67c40.
- ResultACDefence(target local3,weapon local12) at0x67c44,import0x67c48 binds
  0x80107694.
- CheckDamage(owner-2,target local3,part local4,weapon local12,flags0x100,
  result local5) at0x67c54,import0x67c58 binds0x8010a78c.

No wait/pad-sampler request appears between a selected injection and these two
callbacks in this slice. With normal handler2 completion,they can be reached
in the same interpreter invocation. This is a scripted tutorial-history writer,
not evidence of a new physical button sample. Other work values inject neither
of these masks through the two shown cases.

Actual unit-work producers,selector-6 translation,lecture event selection,
weapon eligibility and branch reachability remain open. It must not be conflated
with Crump's **normal_attack_event** at0x653c8,whose separate physical-history
boundary was documented previously. Presence in shipped gor and helper allocation
are strong leads,but do not establish when every demonstration mode executes.

## Implementation obligations and remaining audit gaps

Keep explicit history injection as a distinct original script service,with raw
argument semantics and private-only reset. Preserve reset→injection→defense order
and independent general state. A controlled test can call the helper with0x100
and verify only latest private trigger history is injected after initialization;
a second call with0x200 replaces it rather than accumulating both. Check that
physical sampling has not been fabricated between adjacent immediate commands.
Defense outcomes still require actual target/weapon/modifier services.

The census also covers event-speed/system-level references and direct r13-relative
speed-global stores;see [system-level/speed](EVENT_SYSTEM_LEVEL_AND_SPEED.md).
Audit misses computed/indirect calls,non-module0 dependencies,pointer/bulk/indexed
field writers and script services reached through unresolved selection. Recover
reset caller lifetimes,complete lecture mode graph,other pad-field writes and
original normal/tutorial/multihit traces before closing AC-01/02/03. All extracted
inspection artifacts stay ignored;original code and dialogue are not distributed.
