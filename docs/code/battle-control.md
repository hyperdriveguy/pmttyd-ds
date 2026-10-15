# Battle controllers and field handoff

[Battle entry](battle-entry.md) covers packed unit construction and startup
channels. This guide covers the outer sequence parents, phase tables, conclusion
and return adapters. Addresses/offsets below are hexadecimal; counts and timing
units are stated separately. Original recovery evidence remains in the
[behavior contracts](../behavior/README.md). A completed parent invocation is
not proof that a battle, teardown or field-return graph is fully integrated.

## Engine work and completion

[battle_main.cpp](../../source/battle/battle_main.cpp) calls these eleven source
services in order on every successful upper-engine invocation:

| US function token | Service |
| --- | --- |
| `800f7b28` | Unit broken check |
| `8011a43c` | Private pad manager |
| `8011f1ec` | Sequence manager |
| `800fa5b0` | Action command manager |
| `80149798` | Stage object Main |
| `801aa168` | Icon Main |
| `80139150` | Stage Main |
| `802168a4` | Action command help Main |
| `801a836c` | Audience Main |
| `80203aa0` | Break-slot Main |
| `801024f0` | Display Main |

The battle token is captured owner context, not an invented source argument
for every child. This immediate API has no continuation: a provider failure
retains prior effects and returns no engine result. Admit the full service set
before entering rather than replay a completed prefix after a late failure.

After requests, manager channel zero equal to 4 returns engine result zero.
Otherwise both GP masks `0x1000/0x2000` with manager channel 2 force battle flags
`0x60`, manager 3 and end channel `0x07000000`, but still return engine result 1.
The lower wrapper separately captures its outer battle before the upper engine
recaptures the global. It ORs end mask `0x80000000` into **outer** battle `+ef4`
only after engine zero; callbacks may separate these owners. A null outer owner
is a no-work completion. Neither that bit nor conclusion performs field return.

## Retained field sequence parents

[battle_sequence_entry.cpp](../../source/battle/battle_sequence_entry.cpp) uses
native cursor phases separately from source work `w[1]` state and `w[4]` counter.
A false service return holds the captured child; a successful call's original
integer result remains in `call.result`. Some source false results finish this
visit for polling, which differs from a provider hold inside the same visit.
The caller retains stable services/guest ownership and the same cursor.

The GP pointer cell is `80415fd0`; its field pointer is at `+168`. If GP loads
successfully and the field-pointer read holds, the getter keeps that captured
GP. Color/vector/yaw payloads represent source stack contents through native
fields; the adapter never invents guest stack addresses.

Init copies GP `+c` to process word `8041e968`, acquires field/NPC and preserves
separate GSW0 reads for the 109–110 / 402 music exclusions. Otherwise it requests
BGM Off 1024 before BGM On group 1 using NPC `+2f0`. Environment Off 1024,
rumble Off 0, event stop 4 and sound mask `0x80` follow. A fresh field-type query
and captured debug flag gate early Mario/party control Off. Init does not itself
allocate BattleWork or complete unavailable transitive services.

| Source state | Local behavior / transition |
| --- | --- |
| 0 / 10 | Fade type 22 with source RGBA and raw 1000 / 200 milliseconds; increment state, reset counter for 0 |
| 1 | Independently query DVD root for normal/Luigi audience file polling; increment counter only after both succeed, then query fade |
| 2 | Control routing, Mario mask-1 clear, field `570` counter reset; can enter 3 this invocation |
| 3 | Source encounter wait, rank-selected primary map and descriptor secondary map; can enter 4 |
| 4 | Real `battle_init`; source return zero falls into 5, nonzero yields |
| 5 | End-bit check, counter/state/debug update and listener tail; yields without executing 6 |
| 6 | Increment counter, require signed value >30, request source field/debug sequence |
| 7–9 / outside table | Genuine no-work return |
| 11 | Wait for fade, clear fresh GP `+c`, request debug sequence 5 |

State 2 still invokes the pouch getter whose result is discarded. State 3
captures field/NPC/descriptor across pouch lookup. Field `550 !=0`, `558` in
1–2 and `554 !=0` gate the delay: increment then freshly reread signed `570`,
waiting below 200. These are source visit counters, not measured milliseconds.
Pouch rank `88 <=2` selects primary string `802c2ac4+8*rank`, otherwise
`802c2adc`; descriptor `+4` supplies the secondary name. The `bmapLoad` scalar
return is ignored rather than treated as generic Boolean readiness.

State 5 always runs its listener tail, including while engine end is false or
a state mutation selects 6/debug 11. It queries Mario and camera 4, constructs
source listener `(0,0,400)`, adds source 200 to Z if current global battle mask
`0x100` is set, queries camera 4 again for fresh yaw `+114`, then submits sound
listener data. The optional addition rounds binary32 before submission.

State 6 increments the unsigned counter first and uses a signed >30 check;
it has neither listener tail nor GP `+c` override polling. Both GP masks
`0x1000/0x2000` select sequence 5; otherwise sequence 2. Parameter is 1, data 0.
These helpers do not synthesize teardown or replay an old initialization after
victory.

## Field battle initialization and retained storage

[battle_field_initialize.cpp](../../source/battle/battle_field_initialize.cpp)
captures field `+20` as information storage and field `+8` as NPC. It requests
camera 4 mode 2, optional debug messages (with original `gon_11` fallback), map
allocation, global battle zero, animation battle refresh, field-NPC release,
fresh GP `+14=1`, event reinitialization and sound mask `0x10` in source order.
Setting one mode field cannot stand in for those actual children.

Information construction calls its initializer then mode-zero setter. Party
lookup/conversion uses signed byte `+31`; captured pouch row `14*kind`, mask 2,
gates only the party setter. Field `+1c` mask 1 suppresses first attack. Exact
field-type values `0x20000..0x1000000` map to modes 1–8; `0x10000000` reads the
explicit field `+14` mode; other bit patterns map to zero. These are equality
routes, not priority tests over a composite mask. Setup points to NPC `+230`;
NPC bytes `2f4/2f5/2f6` copy independently to information `18/1a/1b`. Actual
lower `BattleInit` is the final child. Native true represents source integer zero.

[battle_sequence_storage.cpp](../../source/battle/battle_sequence_storage.cpp)
is retained native storage, distinct from cold SDK heap startup. It owns primary
arena, original pointer cells, nested battle pool and an alias to the real pouch.
Cold construction initializes one big-endian free header and pointer roots;
reattachment must preserve bytes/history.

Native admission requires a 32-byte-aligned arena/length, at least `0x64040`
bytes, and guest spans within 32 bits. The minimum allows the `0x64000` battle
payload plus two 32-byte headers; it does not guarantee space for every future
resource graph. Headers have next word `0`, size word `4`, used halfword `8`.
`canAllocate` rounds requests up to 32 in widened arithmetic, then validates
**all** ordered nonoverlapping headers before source effects, even after finding
a fit. A free block needs payload plus a terminal header. It does not repair
corruption or replace the actual allocator's best-fit/split/zero/cache operations.

## Phase tables and move consumption

[battle_phase.cpp](../../source/battle/battle_phase.cpp) constructs two different
64-entry tables. Actor IDs at battle `120` come from each unit's stored ID;
phase-event rows at `220` use lookup slot indices. Their membership differs.

Actor construction clears unit marker `311=255` before later exclusion. State
10 needs status 20; unit phase tag `2c` must match battle channel-four word
`f18`; attribute mask `80000` excludes. Remaining moves, HP, status 27 and
alliance do not independently filter this construction. Selected units reset
word `140=-1`; linked parts with actual pose handle `1c0 !=-1` get marker
`4e4=1`.

Two stable sorts first use absolute home X ascending, then signed initiative
word `28` descending. For finite home coordinates, initiative is primary,
absolute X secondary and original registry order the final tie. Do not merge
into a comparator that changes source callback reads or unordered-value behavior.
The second table includes all nonexcluded actors regardless of phase/state.
Negative X uses `abs(X)`, nonnegative X uses single-precision `X+1000`, then
truncation and stable signed ascending sorting. It is side/position order,
not initiative or Euclidean distance.

Phase-end and selection predicates are deliberately different. End requires
state !=10, positive signed remaining byte `22` and no status 27. Selection can
admit status 27 when attribute `10000` permits; state 1 resets to 0 and skips
this selection visit. No selectable actor is a violated caller invariant,
represented by native abort rather than a fabricated valid ID.

Move consumption advances `5→6→7→0`, `8→9→0`; states 3/4 reset to 0 without
same-call decrement. At zero, status mask `02000000` clears instead of spending
a move; otherwise remaining byte wraps on decrement. Battle mask `2000` forces
remaining zero. Exhaustion marks state 10; unit `27c` mask `2000` preserves turn
byte `24` once, otherwise it clears. Eligible alliance-zero / phase `04000002`
actors publish marker `311=96` while the phase remains. This is a source byte,
not measured display-frame timing.

## Conclusion, barriers and incoming field fade

[battle_conclusion.cpp](../../source/battle/battle_conclusion.cpp) refreshes all
three eight-byte alliance rows even when battle flag masks `30` force conclusion.
Config zero concludes; config masks 1/2 select the exact status/attribute scans
commented in code. At least two concluded rows suffice. The initial end-result
region still queries Mario/alliance/enemy first, then prioritizes masks `40→4`,
`20→5`, own-group result `→3`, enemy result `→1`, mask `10→4`. No matching route
returns no selected result; this does not publish victory or field return.

Unit barriers scan every slot and all four event IDs, clearing stale IDs even
after a live one. Only complete unit readiness queries after-reaction. The
all-event barrier then scans its battle-global IDs, stopping at the first live
one; later stale IDs intentionally remain. The scans must not share a generic
short-circuit implementation.

[battle_return_sequence.cpp](../../source/battle/battle_return_sequence.cpp)
provides narrow states-5/6 and incoming-field-state-0 adapters with the same
ordering described above. Incoming GP mask `10` requests fade 18 / 900 ms and
original event `80414f28`, then clears that mask through fresh GP. Otherwise
GP `118` chooses incoming type/duration pair `108/10c` over `f8/fc`. Reset both
pairs to type 9 / 300 ms using fresh GP per store, then increment freshly read
work state. It does not enter state 1 in that pass.

[battle_ending_fade.hpp](../../include/battle/battle_ending_fade.hpp) adapts fade
10 to a native black overlay. Duration/elapsed are milliseconds; the supplied
simulation refresh rate is used directly without a zero/negative guard. Zero
duration completes immediately without changing retained source slot `28`;
ordinary ticks subtract 10 from it. The field is named `textureOffset` in the
adapter, but the used type-10 GPU path has an identity texture matrix, so this
scalar does not drive texture scrolling on that path. Ratio exactly 1 does not finish until a later tick
makes it greater than 1. Alpha truncates `255*ratio`. These scalar updates do
not perform camera/event/field-return work or prove original GPU fidelity.
