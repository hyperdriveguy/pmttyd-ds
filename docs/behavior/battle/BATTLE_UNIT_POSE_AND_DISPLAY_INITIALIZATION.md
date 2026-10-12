# Complete local unit pose lookup and animation resource entry requests

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete GetPoseNameFromType8012772c/80,searchPoseTbl80103cd0/44,
btlDispEntAnime801011fc/90,BattleStatusEffectInit8019673c/4 and
BattleStatusIconInit80253e84/2c. Read
[full unit entry](BATTLE_UNIT_ENTRY_COMPLETE.md),
[geometry/body lookup](BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md) and
[part lifetime](../unit/UNIT_PART_DEFENSE_AND_LIFETIME.md).
`python3 tools/verify_battle_unit_entry_complete.py` pins all bodies and callers.
Static original DOL evidence;animation/model/pose decoder,actual display and
original resource/runtime/language/table data remain separate requirements.

## Unit pose lookup:category fallback and first encountered record

GetPoseNameFromType(P,requestedType) loads P.word1bc as table pointer BEFORE
checking type category. No P null guard,even for unsupported type. Interpret
requested type SIGNED32 and choose fallback:

| Requested type | Fallback |
| --- | --- |
| <=28,including negatives | 28 |
| 29..39 | 39 |
| 40..69 | 69 |
| >=70 | Return0 after initial pointer load |

For supported categories,scan records of {signed type word,name pointer word}
with stride8 while pointer!=0. At each record:if signed record type>=70,return0
as terminator BEFORE comparing requested/fallback. Otherwise if type==requested
OR type==fallback,return its name pointer;else advance8. No table length,cycle/
name validation or secondary search. The first FALLBACK record can precede and
win over a later exact requested record. This is not “first search exact,then
fallback.” A matching null name returns0 immediately.

Entry requests type65 then43 on its retained first-body part;BOTH use fallback69
and update U2d8/U2dc independently. Source table/order/model meaning and actual
kind-specific names remain binding data,not invented animation constants.

## Separate searchPoseTbl policy used by display resource entry

searchPoseTbl(table,type):null table→return0. Otherwise inspect at most28
records ascending,stride8. At EACH record compare signed type to supplied type
OR exact28;first match returns name pointer. No >=70 sentinel/category-fallback
logic. No match among28 returns0. The bounded count28 is not proof the caller's
allocation always contains28 readable records. Current caller asks exact28,
so both comparisons target28;it does not ask Entry's43/65 or fallback69.
Keep these two lookup functions distinct.

## btlDispEntAnime(U) full local requests

Read U.parts14;scan linked P.next0 until null. For EACH part:

1. Read P.poseTable1bc. If null,store P.poseID1c0=-1;advance next.
2. Otherwise read P.source4 then source.word8 animation-name pointer A.
   If A null,store poseID=-1;advance next.
3. Retain A. Call searchPoseTbl(P.poseTable,28). Pass its returned pointer to
   strcpy(P+1c4,result) WITHOUT result-null/length/capacity check.
4. animPoseEntry(A,mode1);store returned word to P.poseID1c0. No failure test.
5. Read P.next0 live and advance.

No unit/null/list-cycle/count check,allocation rollback or kind/type substitution.
The table and animation-name null paths leave the prior P1c4 bytes untouched
(constructor's earlier byte0 still applies). The nonnull path copies a SOURCE
name before model-resource entry;it does not request immediate event execution
or animated frame advance. strcpy and animPoseEntry/model/texture/data parsing
are source/platform services requiring valid pointers/extent and original reuse.
Failed pose IDs,actual rendering cadence and pose/event lifetimes remain open.

Part1bc is copied from source part.word48 in Entry;part.source+8 supplies A.
These are borrowed original data pointers. The earlier Entry cached U2d8/U2dc
lookups happen BEFORE this display entry loop. No resource getter replaces the
source/pose table pointers. Retaining original pose names and model/texture
assets is mandatory;do not fill missing data with invented poses.

## Early status display initializer boundaries

BattleStatusEffectInit is ONLY blr. BattleStatusIconInit calls
memset(U+348,0,7a0);no pose/model/texture allocation or icon queue registration.
Full entry invokes these after early registry publication and before other
identity/geometry/stat writes. The containing unit allocation isb34 bytes;
348+7a0=ae8,so this clear is a subrange,not a clear of the whole unit. The
remaining effect/icon engine and status record lifecycle are separate work.

Acceptance:all signed requested-category edges,early unsupported-type pointer
read,null/sentinel/table first fallback-before-exact;display lookup28 bound
versus unit lookup69 fallback;part pose-table/name null branches,-1 IDs,unchecked
copy then resource entry mode1/live-next order,original borrowed pointer sources,
no-op effect initializer/icon-clear extent and early publication. Concrete
kind/source tables,decoder/renderer/pose ownership,heap/alias/null/string/cycle/
numeric domains and original spawn/display/damage/death traces remain open.
