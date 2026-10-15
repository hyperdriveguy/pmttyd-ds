# Damage calculators and hit orchestration

See [battle core](battle-core.md) for status/resource helpers,
[packed state](packed-battle-state.md) for counters/prechecks and
[battle events/rules](battle-events-rules.md) for event wrappers. These local
calculators and the synchronous hit controller have different callback domains
from the [retained owners](battle-retained-owners.md).

## Typed calculators and callback ordering

Typed unit/part/weapon fields preserve recovered byte/word domains, not a complete
BE guest-memory access trace. Callbacks may mutate their owners. HP calculation
captures element before its power callback, then reloads special flags and other
modifier inputs afterward. Defender flag 2 adds incoming 0x1000; default code is
`flags|30`. Missing HP callback, part attribute 0x20000000 and pre-callback
AllOrNothing failure return zero. If the callback instead adds a failing
AllOrNothing condition, the later defense-only route remains active through
subsequent defense/output modifiers; it is not the same early return.

Attack modifier order is mode-1 bonus 3, additive badges, AllOrNothing, independent
peril/danger contributions (5×MegaRush and 2×PowerRush), IcePower, status 16,
statuses 10/12, drain/trade penalties, then statuses 11/13. Peril and danger can
both contribute. Individual status stages floor at zero independently. The
status-16 route also ORs attacker flag 8. This layer performs no direct resource
application or hit event; callbacks are not assumed pure.

Element indices 0..4 select hit codes 23/24/26/25/27; other indices use code 23
while defense-table bounds remain caller-owned. Defense attribute 3 bypasses
all defense modifiers. Others retain DefendPlus, DamageDodge under guard flag
0x40000, status flag bonus, status strengths, mode-2 Mario bonus, extra defense
and positive-defense piercing in source order.

After defense, defender trade modifier precedes sequence decay, guard subtraction,
elemental IcePower, defense trade, DoublePain and LastStand. Sequence decay keeps
a minimum of one only while its input is positive. DoublePain uses `copies+1`,
not repeated doubling. LastStand uses wrapped addition followed by signed division
toward zero, `(damage+copies)/(copies+1)`. Add/subtract/multiply helpers wrap each
32-bit intermediate rather than only the final result.

| Defense attribute | Final local effect |
| --- | --- |
| 1 | Add one before final floor |
| 2 | Nullify |
| 3 | Return negative floored amount with code mask 0x2000, including zero |
| 4 | Nullify unless incoming high bit is set |

Ordinary zero restores default code 30, unlike attribute 3. Positive HP may mark
code 0x10000 according to source flags/attributes. FP skips code 0x2000 and has no
incoming-0x20000 AllOrNothing bypass. After its power callback it applies only
guard subtraction and floor; positive FP can replace low code 30 with 23 while
preserving high bits. Emergency revival only marks code 0x800: Mario 222 and
partners 224..230 query inventory item 151, other kinds check held item 151.
It neither tests HP nor consumes/heals/executes revival.

## Synchronous hit controller

`checkPackedBattleDamage` requires admitted callbacks which actually complete
source work. Its void/int callbacks have no native unavailable result or saved
suffix. Source returns 18/19/21/-1 must not be reinterpreted as generic readiness.
The caller still owns the distinct PreCheckDamage path and original negative
result/calling-ID handling.

Null defender returns 18 before effects. A supplied part is published at defender
+0x258. Superguard result 5 is tested before counters/calculators, with source nice
mode 2, position offset (-60,+45,-10), effect payload 0.75, sound/audience and
sequence publication before zero direct damage. Weapon modes 2/3 return 21;
mode 4 can execute the attacker's explicit script, otherwise the source self-hit
is one HP, then return -1.

Counter admission zero skips hit-body work but still reaches the sequence tail.
Ordinary guard independently queries action possibility/result; reusing the
superguard reads would hide callback changes. HP and FP share the code output;
weapon status, reaction and pattern rewrites follow both calculators. Enemy
no-damage/damage audience requests use original kind range 1..215 and source
pattern/status predicates, not inferred battle outcomes.

Pikkyoro independently captures current B even with a null flags pointer. Weapon
mask 0x800 selects enabled bits 0x100..0x1000; multiple candidates draw rank then
scan high-to-low 4..1 with default index zero. Zero/one candidate consumes no RNG.
It ORs the resulting 0x01000000-shifted choice without clearing prior choices.

Status-9 strict `irand(100)<0` remains a draw even though ordinary nonnegative RNG
cannot take it. Patterns 14/16 also draw after an attribute has already forced
pattern zero; the incoming-0x20000 gate is applied afterward. Eliminating either
apparently useless draw changes shared RNG history.

Counter tail slots 1/6/7 independently add wrapped `hp*50/100` to slot 10. Only
zero becomes one; a negative word is not clamped positive. Four flag slots emit
statuses 4/9/8/6 using the original unit/part operands. Optional bomb/sound/fog
precede StopAC on captured B, Help0, self damage and possible defender event.
Return -1 preserves this original control path. No event completion, effect or
resource write may be fabricated to admit an unavailable native integration.

## Dive interpolation

Dive curve 0 is linear, 1 quadratic-in and 4 quadratic-out. Squares and
`count-elapsed` wrap as source words before signed float conversion; replacing
them with floating squares changes overflow behavior. Count/convert/divide domains
remain caller invariants. There is no endpoint clamp or count-zero fallback;
unsupported IDs abort. These curve parameters establish no renderer timing or
original SDK/Gekko arithmetic parity.
