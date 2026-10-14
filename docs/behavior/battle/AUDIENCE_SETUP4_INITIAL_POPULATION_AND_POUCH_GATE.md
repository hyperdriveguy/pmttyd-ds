# Setup4 initial audience population and pouch gate

2026-10-13 US G8ME01 disc0 revision0 read-only DOL/REL evidence. This resolves
the requested initial-crowd producer behind [ending clap ownership](../audio/BATTLE_AUDIENCE_SOUND_BANK_AND_ENDING_CLAP.md).
It does not establish an invented nonempty crowd or full crowd simulation.

## Population invocation and input owners

BattleAudience_Init801a83f8 captures A=BattleWork+2858, zeros A/13914 at
801a842c, then calls BattleAudienceSettingAudience801a60c4 at801a8430.
SettingAudience captures current B, obtains A, reads I=B2738 and S=I+c,
retains parameter pointer T=S+ac, then captures original pouch P via800d5eb0.
T is SETUP information, not selected stage+ac. Field npcSetupBattleInfo copies
configC1c→Sac and generates Sb0 ranges; its exact source and portable owner are
[the field setup authority](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md).

The actual tik setup4 config C=tikREL28450/44 has C1c=0. Its16 byte-pair
ranges C20..3f are zero except index10=(5,5),13/14/15=(1,1). Source setup uses
all16 original random(bound1) requests despite equal range endpoints. Do not
replace the setup's RNG/history with a guessed crowd count. Config SHA256
`a347603436ca794d0e72fb2575f0c61088e6b3f8a2525acd25f69235a34e3d64`.

## Capacity, desired amount and actual initial amount

SettingAudience801a6140 reads captured P signedHALF88(rank). Branches rank0,
1,2,3 write A13790 capacity50,100,150,200 respectively. Other ranks leave its
prior value. At801a61a8 it queries CURRENT pouch anew, reads U16 P8c, and
if zero sets capacity0 at801a61c0. Capacity50 is not proof of50 actual people.

Next it reads retained P signedHALF8a(level), index=min(level-1,64) using signed
comparison against65, without a negative clamp. The65-float table at802f9eac
supplies level1 baseline8. It stores baseline→A13778 at801a61ec.

If B19078 mask10000 is set, desired A1377c is signed B19084 converted tofloat,
upper-clamped to capacity. Otherwise:

1. pouchGetAudienceNum800d3bac returns current FLOAT pouch84, retained Q0.
2. irand(10),irand(10), in that order; D=first+second-13.
3. pouchGetAudienceNum AGAIN returns Q1.
4. Source fsubs/fmuls/fmadds store
   A1377c = singleFMA(0.009999999776482582,
                     single((Q1-baseline)*D),Q0).

The float literal is80424998 (r2-10920), not a fixed number of people.
For T.word0 mode0/1 the extra A13780 is0; mode4 multiplies desired by the
source extra ratio; modes2/3/5/6 use capacity-desired. Actual trio T.word0=0
therefore extra0. Other modes retain separate branches.

At801a66c8..dc it truncates single(desired+extra) to signedinteger N. Negative
N becomes0 at801a66e8, then N>capacity becomes capacity at801a66fc. This N,
not rank capacity or raw P84, feeds the type-allocation and entry loop.

## Fresh supplied prologue snapshot does not prove nonempty

Original pouchInit800d59c8 zeros its complete5d4-byte record. It explicitly
sets FLOAT84=0 at800d5d84, HALF88=0 at800d5d8c and level8a=1 at800d5d94.
HALF8c retains memset0; no subsequent local pouchInit write changes it.
The [authored prologue snapshot](UNDERGROUND_TRIO_DROP_MATERIAL_AND_ENTRY_POUCH.md)
grants6,32(hex),9; their closed mutation branches do not write8c. This is
source evidence for a supplied fresh snapshot, not proof no later original
story callback ever grants a different value before a particular save.

For that supplied snapshot rank0 initially produces capacity50, then actual
8c0 overrides capacity0. The desired amount still consumes its source RNG,
and type setup still executes, but the final clamp forces N0 for every finite
ordinary desired result. No source argument permits changing8c or supplying
10/50 audience merely to select a clap key. To claim nonempty actual story11
requires a real preceding pouch8c producer and live supplied value.

The request to establish a nonempty fresh crowd therefore has no such proof
in the recovered path. Explicit snapshot8c0 proves desired initial count0;
the zero-total-weight arithmetic domain below remains separate before every
final record can be claimed unchanged. Ending Clap zero-count unassigned-key boundary remains real; it is
not solved by substituting rank capacity for actual crowd.

## Type/placement producers and scoped binding boundaries

For mode0, SettingAudience's default type-weight path consumes original
irand(100), a24-iteration permutation of12 categories (two irand(12) calls
per swap), category-specific irand(10) for selected nonzero setup weights,
then tie-break/random sorting and final seat permutation. Raw category
properties are32-byte rows803b4de4. Pair categories count two seats per member.
The function's allocation list at stack34 is initially zeroed; desired N is
apportioned to categories and rounding adjustment before seat construction.
Zero weights include raw source divw denominator0 domains; do not replace the
entire source RNG/numeric graph with a fabricated successful population.

For ordinary mode0 it expands allocated category counts into a local seat-kind
list, retains its actual length L, then performs150 swaps using TWO irand(L)
requests per iteration. Even L0 still executes those300 requests with bound0.
The entry loop at801a6ad0 only runs while index<L. L0 calls no audience Entry;
records remain zero from Audience_Init. Nonzero path queries original
NoUsedFCHaitiRand801a21a0, then Entry801a3400(slot,low8type,0); the loop stops
on allocator return-1. These allocation/random providers must be bound for
positive-count correctness; their body/numeric closure is a next request.

AudienceGetPtr801a8664 maps slot→A+1bc+slot*134(hex), no bounds guard. Original
GetExist801a29c0 queries base then record, returns1 only when record WORD0 has
bit1 AND lacks masks100 and2. PPAudienceNum_Sub801a1fa0 additionally excludes
record BYTE19 states0c,0d,11. Sound counts are computed live by this policy,
not by cached A13784 totals.

Entry801a3400 captures slot,type,appearance; queries base and stage-world
service80139864 before obtaining record. For single-seat types (type row
flag1 clear) it rejects occupied flag1 or reserved100, then writes flags5.
For paired types it requires even slot, rejects occupied/reserved, writes85
before the placement/init helper801a31a8. Actual type/body/state/model placement
belongs to that helper; assigning recordflags1 without it invents the source
population. These positive-entry services are outside the proven initialempty
scope. Initial empty is not blanket ending-time emptiness: later audience
producers, phase/turn target updates and actual canonical pouch lifetime must
remain their existing owned requests.

## Exact bounded zero-count side effects and remaining arithmetic domain

With setupmode0, pouchrank0/level1/84zero/8c0, B19078 overrideclear and
B19088 selected-type override0, the used default setup RNG order remains:

1. Two irand(10) requests for desired amount.
2. One irand(100) selects desired category cardinality1..12 through table
   802f9fb0/2c weights20,40,20,5,3,2,1,1,1,1,1. Strict draw<cumulative wins;
   residual draws select12.
3. Twenty-four permutation swaps, two irand(12) each (48 requests).
4. If sole allowed type10 occurs among admitted selected categories,
   one irand(10) supplies its weight5*(draw+1); its type-row flag1 is clear.
   All other twelve weight entries are0. Types13..15 setup bytes exist but
   are outside this12-category allocation.
5. Pairwise original category sorting compares all66 pairs; equal weight
   issues irand(2). Allzero gives66 such requests. Sole positive type10
   gives55 equal pairs; its11 comparisons do not request tie RNG. It sorts
   an index list without rewriting the compared weight array.
6. Special guest random20 path is gated by retained default-mode r29=1 and
   actual allocated-seatcount!=0, hence skipped when that count is proven0.
7. Seat-list shuffle still calls irand(L) twice per150 iterations. For L0
   there are300 service requests with bound0. The complete irand80005da0/44
   local branch returns0 for argument0 WITHOUT calling rand; these requests
   consume no underlying RNG state. Do not conflate request count with draws.

A13794 receives the final12 WORD type weights by memcpy at801a6ae8 even
with desiredcount0. Capacity, baseline, desired and extra writes above remain;
there is no early capacity0 return from SettingAudience. Audience_Init later
recounts halves and updates its cached totals/window amount, starts slot2
noise requests and invokes NumToTarget; those transitive lifecycle services
remain their existing contracts, not implied by population-prefix closure.

If type10 was admitted, summedweight>0. N0 division numerators0 yield all
allocationcounts0; residual allocation and guest branch skip, L0, no Entry.
In this bounded domain all200 records A+1bc+i*134 retain every byte0 from
memset, including WORD0 flags0 and BYTE19 state0. GetExist then returns0
before stateclassification, and live R/L soundcounts are0.

If type10 was NOT selected, all12 weights and summedweight are0. Source
801a6758 unconditionally executes divw(0,0) per category before listlength
construction; it does not skip arithmetic just because desiredcount0.
Exact original Gekko zero-divisor quotient is not established by the local
PPC listing. Therefore capacity0 alone does not close universally unchanged
records through the whole original function. A native zero-quotient/empty
population numerical policy must be explicitly labeled an adapter until
original processor/result evidence is supplied. Do not invent a positive
population to avoid this source arithmetic boundary.

## Unassigned clap key versus a verified invalid-name failure

[Complete positional key provisioning](../audio/POSITIONAL_SFX_SLOT_PROVISIONING.md)
distinguishes signednegative string pointer and numeric key. An actually valid
NUL-terminated name that matches none of4242 table rows resolves K0; row0
at80316b28 has WORD4=0 and therefore original __psndSFXOn returns-1.
Raw numeric key0 takes the same content gate. Row0 raw20bytes are
804218c800000000804218c80000000000000000.

That is an established failed-key domain; it does not establish original
Clap's uninitialized local stack bytes as a valid, unmatched name. The local
key address itself is a normal string pointer, but its content is unspecified
when crowd0. It might fail lookup, accidentally match, or leave the valid
string-reading domain. Native unavailable/uninitialized-key request failure
may therefore explicitly return-1 as a backend adapter. The original Clap
wrapper then legitimately stores that supplied handle-1 to R04 and skips
activeflag initialization. The choice of backend failure is the adapter,
not proof of original stack contents or original audio silence. Preserve
original request/source addresses and this limitation for later recovery;
do not substitute an invented SFX key or positive handle.

Additional complete irand80005da0/44 SHA256
0523fae52d8e314f0e46dafe5c3f04f5c4c90ced7628011fdb0cf4efa4ba0daf;
row0 80316b28/14 SHA256
32d7470af440ef3b49893395cf85a064c58d5d3506e13a47f679d2388944ce51.

## Evidence and validation

Complete raw DOL SHA256, original address/size(hex):

| Body | Address/size | SHA256 |
| --- | --- | --- |
| SettingAudience | 801a60c4/a4c | 21467d6044dd1773f586a7721fd0064ab1bfca57de5f8ba9495831c045cf2e8e |
| pouchInit | 800d59c8/4e8 | c6e29f88e574c1d5caa1aaf5c7a73a53bc2d952cbad00a9e21ab2bfd3113def1 |
| pouchGetAudienceNum | 800d3bac/0c | 2295cb31b0022a43c846eeabe45ab19049fdfafcd9f0535fed9a05759f362c93 |
| AudienceEntry | 801a3400/1d0 | a79390037fb66521479fcfcd921cee24de4523a32e63a452101d9d9f3885b5c1 |
| AudienceGetPtr | 801a8664/3c | b9357ba56748bd7cca833c66858c0d7c8552b2f84196298ab4e054eefcc73d56 |
| AudienceGetExist | 801a29c0/64 | 21d4c42ea7469e96002e8a917d4e5ed2736c391d5dcbed4200a0e2ebc445e95d |

Complete listings under ignored artifacts/recovered-temp-evidence/audience-initial*.
Checked source config/ranges, capacity/8c gate, amount/RNG order and original
zero-entry loop admission. Scoped diff check passed. Documentation only;
no native/shared index/progress/inbox edits, build rerun or commit. No
nonempty crowd, current sound handle or playback outcome inferred.
