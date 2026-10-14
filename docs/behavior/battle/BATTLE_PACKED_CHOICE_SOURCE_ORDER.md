# Native candidate choice over live records

Implementation: `include/battle/battle_packed_choice.hpp`,
`source/battle/battle_packed_choice.cpp`, `tests/battle_packed_choice_tests.cpp`.
Contract: [BATTLE_ENEMY_CANDIDATE_CHOICE.md](BATTLE_ENEMY_CANDIDATE_CHOICE.md).
Complete original800febc8/9d8 inspected read-only at /tmp/pm-native-choice.*;
GetMaxFp80126b14/44 inspection stays /tmp/pm-native-max-fp.*. Original binaries
remain outside source history. This is static/local binding,not original
execution,complete encounter selection or PowerPC numeric fidelity.

## Corrected source evidence

The earlier prose reversed mode4/8 coefficients. Original constants804221f4
and804221f8 are0.1 and0.4 respectively. Mode4 is0.1+0.4*(N-rank),mode8 is
0.5+0.4*rank. FP deficit still uses0.1. Existing verifier checks these original
words and the complete chooser hash;source loads/operations disambiguate their
roles. Corrected semantic/native cases give two-row weights99/50 and55/90.

Item142 calls CanActStatus8012724c at800ff1e8,not recovery-step80127890.
The provider is renamed canAct. Native default checks statuses1,2,9,27 in
order then U138 bit1,without a HP query. Null unit yields1 through these
composed null-safe leaves;it does not establish a valid original actor domain.

Initial output row's permutation byte is read at800fec14. After unit/part
outputs,the bonus index is read AGAIN at800fec58. Both choosers now preserve
that reread;output aliases can change the bonus owner. Final row output loads
unit then part around their separate stores;it is not an upfront pair snapshot.

## Record and numeric ownership

BattleCandidateRecords gives one original/private byte view with base address.
Native RAM uses the existing address-space providers. Private Takeoff storage
uses explicit byte/half/word accessors over its existing byte array,including
raw weapon word0;no DamageWeapon/DamageUnit snapshot is manufactured. The
semantic weapon pointer in the older container is independent and unused here.
Private access outside the array and weight indices outside0..73 assert native
provider invariants;they do not reproduce original invalid stack writes or add
valid-count clamps. The final draw's index74 row read remains supported.

Capture BattleWork before initial outputs-1 then0. Signed count<=0 skips actor,
weights and RNG. Positive count retains the actor lookup even for mode4,emits
initial physical row,zeros74 weights,then rereads initial bonus permutation.
Each loop reloads signed count and permutation,fetches candidate through retained
work,and applies every enabled modifier in original order with intermediate
truncation. Kind,belong,HP,maxHP,kind-pointer danger and weapon fields are read
from native records at their branch use,not merged into a unit snapshot.

Source constants are read from804221f0..80422208 for enabled operations.
Signed-word conversion,multiply/add/subtract/divide,FMA,negative multiply-
subtract and truncation are required numeric platform providers. Host fixtures
inject ordinary float/std::fma arithmetic;they do not prove PowerPC rounding,
NaN/exception/FPSCR behavior. No denominator or weight clamp is synthesized.

Weapon dispatch retains initial weapon pointer but repeats buffer weapon/item
lookup on the full-FP branch. Status chains short-circuit. Initial flags104
weight1 still proceeds into item dispatch. FP deficit requests max,current,
max,current in order,uses signed word subtraction and fused0.1*deficit+1 before
its multiply/truncate. Native FP getters load kind,current work,own signed half
first;partner224..230 then finds Mario through retained work and reads Mario's
half. Missing Mario is not replaced by partner's own FP. Native CanAct/GetStatus/
FP providers use actual bytes;weighted RNG forwards explicit runtime irand.

All74 weights determine deterministic maximum/physical ties or signed word sum
and weighted subtraction. Draw0/prefix boundaries and row74 remain original
local behavior. Takeoff backend now supplies this native chooser by default;
its adapter's integer return0 is ignored and conveys no original scalar result.
Sampling is still a required backend provider. Native Takeoff has no shared
candidate replacement or fixed target fallback.

Acceptance:empty/high-bit counts,both rank modes,every modifier/ordered combo,
post-output bonus reread,live count,output/part aliases,status branches/CanAct,
flag1 plus deficit,repeated FP queries,fresh weapon item,partner FP ownership,
physical ties/draw0/prefix/row74,private and native RAM buffers,and actual Takeoff
core chooser binding with no RNG in mode4. Source state and providers are authored
fixtures. Next:complete native sampling/source geometry,original RNG seed/call
interleaving,full Jump motion/events/outer battle/Game and original execution.
