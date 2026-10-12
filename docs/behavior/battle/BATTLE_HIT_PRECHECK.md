# Battle hit eligibility, evasion and accuracy

Baseline: US G8ME01 revision 0, identity and evidence classes in
[ORIGINAL_BEHAVIOR.md](../../ORIGINAL_BEHAVIOR.md). Recovered 2026-10-12 from
`BattlePreCheckDamage`, RAM `0x800fbadc`, length `0x2b0`. B below means original
binary inspection; R means public naming aid; P means a proposed verification
obligation. No original gameplay capture or Switch equivalence is claimed.

The pinned [public battle damage source](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/battle/battle_damage.c)
does not establish this routine's US contract. Ignored local evidence is
`artifacts/reference/behavior/BattlePreCheckDamage.{elf,disasm,annotated}`.
Names are annotations; addresses, masks, signedness and branch order below
come from the US instructions.

## Interface and state (B)

Inputs, in register order: attacker `A`, defender `D`, target part `P`, weapon
`W`, input flags `F`. Return is an integer result code. This function does not
subtract HP or apply statuses. Its result must not be confused with the
18/19 results used by the later damage routine.

`U8`, `S16` and `U32` denote unsigned byte, signed halfword and unsigned word
loads. `kind(D)` is the pointer at `D+0x10`. `status(D,n)` calls
`BtlUnit_CheckStatus`; its own lifecycle/validity rules remain a dependency.
All masks are ordinary hexadecimal masks, not PowerPC MSB-first bit indices.

## Ordered decision tree (B)

Every return stops subsequent checks and their random draws.

| Order / instruction region | Condition and effect |
| --- | --- |
| 1, `0x800fbb04` | If D or P is null, return **1**. This is a precheck return, not proof a null target can be damaged. |
| 2, `0x800fbb18` | If `U32(W+0x74)&4`, write `U32(A+0x27c) |= 8`. This mutation occurs even if a later check rejects the hit. |
| 3, `0x800fbb30` | If `U32(P+0x1ac)&0x4000` and not `U32(W+0x74)&0x40000`, return **2**. |
| 4, `0x800fbb50` | If `U32(W+0x74)&8`, return **1**, bypassing all remaining checks. |
| 5, `0x800fbb64` | If status 18, or `U32(D+0x104)&0x18`, return **4**. |
| 6, `0x800fbb9c` | If `U32(P+0x1b0)&2`, not `U32(W+0x78)&4`, and `U8(A+0x302)==0`, return **5**. |
| 7, `0x800fbbc8` | If `U32(P+0x1ac)&0x40`, return **4**. |
| 8, `0x800fbbdc` | If `F&0x100000`, return **1**. This bypass is later than the preceding target/counter restrictions. |
| 9, `0x800fbbec` | For each copy counted by `U8(D+0x2e1)`, draw `irand(100)`; if result ≥90, return **3**. |
| 10, `0x800fbc24` | For each copy counted by `U8(D+0x2e2)`, draw `irand(100)`; if result ≥75, return **3**. |
| 11, `0x800fbc58` | Only if `S16(D+0x10c) <= U8(kind(D)+0x0c)`: for each copy counted by `U8(D+0x2e0)`, draw `irand(100)`; if result ≥67, return **3**. |
| 12, `0x800fbca0` | If status 7 is active and neither status 1 nor status 2 is active, draw `irand(100)`; if result ≥50, return **6**. |
| 13, `0x800fbcf4` | Initialize accuracy to `U8(W+0x10)`. If attacker status 3 is active, replace it with integer `accuracy*50/100`, truncated toward zero. |
| 14, `0x800fbd18` | If the global battle word at `+0x182bc` is nonzero, multiply accuracy by binary32 **0.5**, then truncate toward zero. Constant at `0x804221c4` is `3f000000`; r2 base is `0x80427440`. |
| 15, `0x800fbd5c` | Draw `irand(100)`. Return **1** if draw < accuracy; otherwise return **2**. |

The routine assumes valid A/W on paths that dereference them. It does not
validate array ownership or weapon fields. Do not generalize its null D/P
early return into a safe interface for every caller.

## Naming and random-stream implications (R/B)

Public unit/status headers associate `+0x2e1` with Pretty Lucky, `+0x2e2`
with Lucky Day, `+0x2e0` with Close Call and `+0x302` with Spike Shield.
The threshold at kind `+0x0c` is named danger HP. Status IDs are 1 Sleep,
2 Stop, 3 Dizzy, 7 Dodgy, 18 Invisible. These labels aid reading; the numeric
fields and conditions above remain the US contract. The producer and lifetime
of battle `+0x182bc` have not been traced here, so this document does not assert
that every occurrence of a visually similar stage effect enables it.

Each equipped copy gets its own draw, in the order Pretty Lucky → Lucky Day
→ eligible Close Call → eligible Dodgy → accuracy. Successful earlier evasion
prevents later draws. Replacing this with one aggregated evasion probability
would change both outcomes for a given seed and subsequent gameplay randomness.
See [RANDOMNESS.md](../meta/RANDOMNESS.md): modulo reduction is not perfectly uniform.
Thresholds 90/75/67/50 must be preserved rather than approximated percentages.

Final accuracy always draws, including accuracy 0 or 100. No cap to 100 is
applied here. If both accuracy reductions apply, they happen sequentially:
accuracy 99 becomes 49 then 24. Sleep/Stop suppress the Dodgy draw, not the
preceding badge checks. The charge-related write at step 2 precedes evasion.

## Reimplementation verification obligations (P)

Use an injectable sequence of RNG returns to check every boundary: 89/90,
74/75, 66/67, 49/50 and accuracy−1/accuracy. Check repeated copies, early exits,
and exact consumed-draw counts. Cover danger threshold equality, Sleep/Stop
with Dodgy, both bypass flags, and a rejected hit that still writes A+0x27c.
These are derived cases, not tests already run against the original executable.

Still open: all event callers and their reaction to codes 1–6, counter
resolution, attack-specific overrides, status helper behavior, RNG seeding,
and matched original captures. This local contract alone cannot establish
complete hit resolution or combat equivalence.
