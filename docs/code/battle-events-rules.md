# Battle events, rules and resource side effects

This guide complements [battle controllers](battle-control.md),
[battle entry](battle-entry.md) and the [event VM](event-vm.md). Original evidence
remains in [behavior contracts](../behavior/README.md). Hexadecimal offsets/masks
below name retained source fields; native adapters do not invent original heap,
SDK numeric or complete event scheduling fidelity.

## Callback ownership and operand domains

[battle_event_callbacks.cpp](../../source/battle/battle_event_callbacks.cpp)
binds original US function tokens to explicit implementations. Registrations
retain runtime/services/backend references; owners must outlive registrations
and pending calls. A copied data-child backend descriptor is temporary, but
its callbacks/user and VM creation/schedule still retain their actual owners.
Numeric userdata is separate from sound/eligibility backend userdata. Float,
double, truncation, FMA, division and libm operations require the runtime's actual
platform callbacks; an assertion marks an admitted invariant, not a successful
missing service or zero-on-unmapped read.

Takeoff defaults use the private candidate-record sampler/chooser. The chooser's
adapter scalar zero is ignored; caller output writes carry the selected IDs.
Turn defaults are real packed rule, move-reset and effect-record/heap-3 deletion
implementations. The immediate command-weapon binding aborts on an unsupported
optional branch, requiring parent admission before it can be reached.
JumpSetting resolves actor/count/speed/gravity in order, then obtains current
battle and writes unit `16c/170/174` (count and raw IEEE speed/gravity).

[battle_event_utilities.cpp](../../source/battle/battle_event_utilities.cpp)
keeps EVT completion 2 / continuation 0 distinct from output Booleans 0/1.
Guard-move waits on battle `ef4` mask 1. FlagQuery uses a **raw** mask/output
slot. Frame-to-millisecond conversion resolves only its frame input, multiplies
1000 with 32-bit wrapping, interprets signed and divides by fixed 60 toward
zero. It does not substitute current GP simulation rate or render FPS.

[battle_data_child.cpp](../../source/battle/battle_data_child.cpp) resolves
selector, translates it, captures current battle/unit, then resolves key.
Unit `+18` points to eight-byte key/value rows terminated by key zero. A null
or missing script completes EVT 2. Otherwise it creates an actual waiting child
with flags zero and translated owner, clears parent opcode and returns 255
**without deleting the parent**. The source scheduler/child links prevent replay;
this helper ignores firstCall and supplies no separate continuation.

[battle_first_attack.cpp](../../source/battle/battle_first_attack.cpp) scans for
the first alliance-byte-1 actor. No match leaves the last registry lookup token,
which may be null or a nonenemy; it is not a generic missing-target fallback.
Modes 1–6 use Mario→candidate, 7/8 partner→candidate, others candidate→Mario.
The scalar is the last original getter token, not a validity predicate. Native
attacker/target outputs may alias, and write order remains attacker then target.
The EVT ignores that scalar, walks target body, rereads target ID afterward and
stores raw output ID then part-definition ID. Target absence is not guarded.

[battle_hit_code_events.cpp](../../source/battle/battle_hit_code_events.cpp)
resolves actors/part/weapon while leaving flag/pattern masks and destinations
raw. CheckDamage provider false returns 0 without writing output or probing
parent existence. Negative result returns 255; nonnegative writes output, then
checks parent ID and returns 2 if live / 255 otherwise. Damage services may
remove the parent. This wrapper has no retry cursor or rollback.

Other leaves differ in null-unit handling: marker `1fe` ORs halfword mask 1
only for a unit; damage getter writes signed byte `270` only for a unit, leaving
a missing unit's output untouched. Other flag/attribute/pattern/code leaves
require admitted actors. Unit flags `1c`, attributes `104`, pattern equality
`274`, code-mask `278` remain separate. GetHitCode returns low 8 bits unsigned.
ResetFacing reads signed alliance `c`, its eight-byte battle-row direction `a`
and stores direction `189`.

## Action-record rules

[battle_general_rule.cpp](../../source/battle/battle_general_rule.cpp) captures
battle/descriptor, queries Mario **and the unused party result**, then reads rule
byte `18`. Rule zero stores outcome 6; >67 leaves outcome `19` unchanged. This
is judgment only: no clear, increment, reward, message or persistence occurs.

| Rule region | Input and output ownership |
| --- | --- |
| 1–57 | Exact unsigned action-record byte lists, except 19/20 signed current Mario HP; scalar helper outcome writes captured descriptor |
| 58/59 | Unsigned turn byte and strict `>` / inclusive `<=` thresholds; direct captured-descriptor write |
| 60–65 | Exact ordered byte sums; zero-turn helper owns its own independently captured descriptor write |
| 66/67 | Exact ordered sums ending with `16f39` / `16f38`; direct 6 if zero, else 1 |

The scalar helpers independently reacquire current battle/descriptor. N compares
threshold `1b` then `1a`, producing 1/2/6; U compares `1a` then `1b`, producing
6/2/1. Equality counts. The parent writes the scalar outcome to its **original**
descriptor even if the helper saw another owner. Zero-turn helpers instead write
their own descriptor without copy-back. These outcome codes retain source meaning;
they are not Boolean success or invented challenge names.

[battle_turn_rule.cpp](../../source/battle/battle_turn_rule.cpp) uses turn byte
`16f57`, limit `1b` first then fresh limit `1a`. Inclusive comparison or limit
255 admits each branch. First writes 2 for zero sum / 1 otherwise; second writes
6 for zero, or 1 only if current outcome lacks mask 2. The second can override
the first. General rules 58/59 do not use the 255 shortcut. Preserve listed read
order and repeated/nonmonotonic fields rather than deduplicating them. Individual
counter caps do not cap combined sums to 100 or one byte.

## Resource payment and recovery

[battle_weapon_cost.cpp](../../source/battle/battle_weapon_cost.cpp) captures
base FP cost before equipped-count callback, then rereads item ID to select the
exact exponential / linear badge-ID membership. Exponential cost is
`base*2^(copies-1)`, including the source copies-zero exponent -1; caller pow and
truncation retain numeric ownership. Linear cost uses wrapped word multiplication.
Nonzero cost subtracts reduction with word wrap then minimum 1; zero stays free.
Costs at least 1000 cap to 999.

Affordability captures available FP **before** cost callbacks and queries AP only
if FP suffices. AP uses hundredths of displayed SP: requirement `100*apCost`.
Payment does not recheck affordability. It captures work, obtains FP/cost, writes
FP, then freshly reads unit kind. Mario/partner cost records receive **low byte**
of cost at `16f4b/16f4d` and increment the next byte; AP cost is read after these
callbacks. Source byte additions wrap before capping at 100; count saturates at
100. Records can differ from the full signed resource debit. EVT resolves actor
then weapon before translating actor.

Pouch AP current/max at `7a/7c` are signed big-endian halfwords. Add with word
wrap, narrow to 16 bits, then clamp negative to zero and above maximum. Clamping
before narrowing would change behavior. Equipped badge count scans all 200
signed halfwords at `38a`, including ID zero matching empty slots.

[battle_badge_recovery.cpp](../../source/battle/battle_badge_recovery.cpp) has two
operand protocols. Amount query translates a **raw** actor, caps HP/FP Drain only
above dealt-HP total, and writes zero for a missing unit; negative dealt HP can
produce a negative result. Recovery resolves actor, then discarded part, then
amount after capturing work. Actor -1 skips part/amount evaluation and completes.
Attribute `40000000` bypasses HP gauge bookkeeping only; ordinary recovery records
before/after, mirrors `202/206` to `200/204` and ORs damage flag 1. FP delegates
to shared resource ownership rather than duplicating HP gauge fields.

## Weapon aftereffects

[battle_weapon_aftereffect.cpp](../../source/battle/battle_weapon_aftereffect.cpp)
is whole dispatch without continuation. A late unavailable request preserves
prior RNG/side effects; pre-admit providers rather than replay the prefix.
Weights initially load `b6/b5/b7/b4` for the shared draw, then fresh cumulative
`b4/b5/b6` subtraction selects A1+A2 / A1 / A2 / none. After the initial draw, force word `19078` masks
`10/20` seed those choices before cumulative selection; RNG callbacks may change
these fields. Stage byte `180f8` masks `4/2` suppress A2/A1.
Existing A1 with mask 8 clear allows a 100-roll against weapon `b8` or fresh force
`40` for DestroyB, possibly restoring A2. Dispatch order remains A1, A2, B.

Three subsequent 100-rolls occur even when force masks will select the effect:
weapon `b9/ba/bb`, force `200/100/80`, then nozzle direction / nozzle work mode 1 /
iron-frame checks. Final stage mask `20` permits object-fall; force 1 skips only
that final RNG, otherwise roll against `bc`. No invented visual event replaces
the original source service address.

## Battle hit snapshot and query admission

[battle_hit_registry.cpp](../../source/battle/battle_hit_registry.cpp) owns an
explicit imported battle-map snapshot, not inferred live field history. Private
little-endian `PMBHIT01` admission bounds are 1–8 maps, 4096 total objects,
32767 triangles per object / 100000 total, 64-byte terminated nonempty names,
cull 0–3 and finite geometry/radius. These are native limits. Duplicate map-local
node/name or trailing bytes reject. Failed load preserves the previous snapshot;
success replaces object storage and republishes active order.

Publication keeps objects in original sequence when parts exist and flags `81`
are clear. Flag mutation does not rebuild active order automatically; attributes
and optional mobj flags are read live. Invalidation holds rays while retaining
storage. A successful replacement/destruction invalidates borrowed object pointers.

Only the original filter `8002d380`, downward `(0,-1,0)` direction and finite
nonnegative-length domain are admitted. Every active tagged owner needs known
mobj flags **before broad phase**, so an unresolved distant owner still makes
the whole query unavailable. Tagged mobj mask `800` filters the object. It does
not authorize choosing a farther floor or returning Miss for unknown evidence.

Broad phase uses a midpoint sphere with radius `length/2+object radius`. Source
edges are `v2-v0`, `v0-v1`, `v1-v2`, preserving rounded/FMA order. Cull 1 requires
upward normal and nonnegative plane numerator; other cull modes require strict
normal-Y × numerator >0. Edge tests include zero with no invented epsilon.
Distance must be strictly below the running limit: endpoint and later equal
hits do not replace the first winner in object/triangle order. Orient by plane
sign and normalize only the winning normal. Nonfinite arithmetic or zero
normalization means Unavailable, not Miss or an upward-normal fallback. Native
finite SDK adaptation does not establish paired-single/rsqrt/FPSCR equality.
