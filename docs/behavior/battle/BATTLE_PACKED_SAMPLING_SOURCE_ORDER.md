# Native candidate sampling and complete default takeoff composition

Implementation: `include/battle/battle_packed_sampling.hpp`,
`source/battle/battle_packed_sampling.cpp`, `tests/battle_packed_sampling_tests.cpp`.
Contract: [BATTLE_ENEMY_CANDIDATE_SAMPLING.md](BATTLE_ENEMY_CANDIDATE_SAMPLING.md).
Original complete core800ff684/bd4,entry800ff5c0/c4 and forward801254a0/54
were inspected read-only;ELF/disassembly stay /tmp/pm-native-sampling.*,
/tmp/pm-native-sampling-entry.* and /tmp/pm-native-forward.* outside history.
Static source/local native binding does not establish original executed states,
valid source domains,PowerPC FPSCR or complete scene fidelity.

## Metadata,records and geometry

Raw/private BattleCandidateRecords now have byte/half/word writers. Private
writes preserve existing bytes outside selected fields;raw RAM uses the shared
native address-space services. No reconstructed DamageUnit/position snapshot
supplies admission or geometry fields. Valid source count/linked part domains
remain necessary:there is no local74-row capacity clamp. Private accessor bounds
are native provider invariants,not reproduction of invalid original stack writes.

Entry writes weapon,actor,low8 belong,A,C,low8 direction before current-work/
actor lookup. Flags27c bit10 swaps exactly one side bit through TWO distinct
word stores/rereads and returnsQ1;otherwiseQ0. Update does not repeat that swap.
Core reads initial A BEFORE current work,including on the high-bit early countff
path. That path leaves selection/rows/permutation alone. Normal construction
scans64 registry slots,skips nonzero GetStatus27/flags104 mask40,and retains
kind-pointer/head/unsigned kind.b4 count in original order.

The local geometry detail extends the earlier symbolic scale notation:
part.bd facing and part.source4 pointer are captured BEFORE world-position call
at800ff718/720. Unit114 is live:optional base-Y FMA and X/Y/Z each reload it at
800ff748/784/7b8/7d0. Do not cache one scale for the whole part. The independent
world X/baseY/Z survive hit-coordinate writes. X188+194 is rounded then
multiplied by fresh scale before signed-facing conversion/FMA. Original5.0
8042220c is loaded before hitX FMA and z+5 computation;Y/Z offset sums and
fresh scales feed separate FMA operations.

Unit row half is stored before the retained definition pointer's word0 read;
then ordered hit integer outputs are stored. Bias0 and marker-1 are written,
unit/part flags reread,optional300*direction added,and signed forward half
multiplied by retained facing. BaseY truncation precedes bias read/add;z+5
truncation precedes bias publication. Sort X converts a reread bias then adds
world X;X/Y/Z sorting words are sign-extended low16. Forward helper reads
original -800/800/40 words80422668/6c/70 and preserves ordered comparison,
rounding/division/truncation,upper-only48 cap and signed-direction inversion.
Unused row+a/b and+1e/f remain untouched.

## Filtering,compaction and sorting

Unit/part queries in A/C/current-row same-unit/occlusion passes use retained
core work and GetPartsPtr on that fetched unit. Only occlusion PEER lookup uses
BattleGetUnitPartsPtr and its fresh work recapture at800fffffc. This distinction
extends the earlier generic “registry/part helpers” wording.

Every enabled admission/attribute check is ordered and short-circuits,reading
needed kind/belong/ID/flags from live native fields. Late A is reread at800ffb3c;
C is reread after the optional Monosiri service. Nonnull Monosiri route rereads
unit kind and calls lookup again before word+4. That undocumented lookup remains
a required provider for C1;no null-result fallback is synthesized.

Same-unit flagged-part suppression runs after both admission passes. Occlusion
retains signed lanes/sort positions,negative-direction word negation,peer-part
mask40/400000 and actor-pointer exception. Actor and peer-unit lookups BOTH
occur before checking actor flags27c;the peer lookup is not skipped by a flag
short-circuit. Equal sort X does not block. Every rejected row shifts later rows
using four ordered two-word preloads/stores then the ninth word,not an opaque
whole-buffer replacement. Local count decreases without intermediate public
count changes. Final signed count initializes permutation;captured direction
selects nested exchange comparison,including equal-key swaps for direction<=0.

## Native defaults and evidence

Native Takeoff backend now defaults to both native sampling and native choice.
An explicit sampling override remains available;its absence invokes recovered
sampling rather than a fixed target. Default ordinary takeoff uses null weapon,
exact masks and choice4,so it needs no Monosiri lookup or RNG provider. Numeric
providers remain explicit and source records must be provisioned. Backend and
runtime outlive registration;each call retains its own private candidate bytes.

Acceptance fixtures exercise metadata swap/no-swap,high-bit early preservation,
all A/C rejects,self exception,GetStatus27/unit/part skips,Monosiri double/null
queries,base/hit/sort separation,padding,ordered compaction,lanes/directions/part
bypass,equal-key permutation,same-unit body suppression,captured facing with
live scale changes,raw native buffers,and actual GetTakeoffPosition registration
executing both native defaults over native event/unit/kind/part records. The
complete integration yields requested-target X80/actor-home Y7/requested Z0 for
its authored source state;this is not evidence of original Crump positions.

Remaining:original construction/writers/domains,complete Monosiri body/source,
numeric/RNG services,original motion/event graph and outer battle/Game/platform
composition plus original/hardware validation. Source request is under EVT-01
in [DOCUMENTATION_REQUESTS.md](../../DOCUMENTATION_REQUESTS.md).
