# Monosiri/Tattle flag groups, persistent writes and battle-work reads

US G8ME01 disc0 revision0; complete battleSetUnitMonosiriFlag8012ed2c/110
and battleCheckUnitMonosiriFlag8012ee3c/e4, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md) with [SAVE-01](../../requests/SAVE-01.md)
shared switch-storage prerequisite. Sizes/offsets are hexadecimal unless stated.
Static source evidence only; no complete Tattle/save/runtime equivalence.

These routines do NOT populate [monosiriMessageTable](MONOSIRI_TABLE_SOURCE_AND_CONSUMERS.md).
They write/read separate global switch bits and BattleWork fields. The
[swdrv contract](../memory/SW_SET.md) proves ordinary bit indexing and initial
bank extents; use it instead of the earlier scrambled-bit interpretation.

## Complete group-aware setter

Input is a raw kind word K; retain current BattleWork B at8012ed4c before
scanning. Static battle_monosiri_same_tbl at8036e758, sizec4, is a null-terminated
pointer array of same-kind lists. Each list is itself a sequence of kind words
ending0. Scan pointer lists in original order, then words in each list in order.
A matching raw K selects that WHOLE list. A null pointer before any match takes
the single-kind route. No argument/index/table/null-domain guard or allocation. The supplied pointer
array has48 nonnull entries plus its terminator; the lists contain247 total
member occurrences, with kinds1..193. Repeated/overlapping membership is real
source data, not an invitation to reorder or deduplicate calls.

For every kind J in the selected list, or K alone on the miss route:

1. Call swSet(wrap32(4474+J)). This call reads CURRENT GP itself.
2. Set bit J%32 in retained B+163d4+4*trunc(J/32), using the same signed
   quotient/remainder/PPC slw arithmetic as swdrv. This is an independent
   retained-B bank, not a copy of GP's switch-bank word.
3. For the group route, load the NEXT kind word after these stores; stop on0.

For example, the first list is8,9,10. Raw input9 matches that list and
requests swSet4482,4483,4484 in that order plus the corresponding battle bits;
it does not start from member9 or choose the later list9,8,10. This is a static
derived case, not an original Tattle observation.

The matching element is not the first element necessarily; on match the setter
restarts the list at ordinal0 and sets all members. First matching list wins;
no deduplication across groups or short-circuit already-set check is supplied.
The group route updates the retained input register with each loaded member,
then returns after the list terminator. Capture order and raw list contents
matter when reproducing equivalent-kind registration.

## Complete checker and precedence

Input is unit U. Capture current BattleWork B, then current kind K=U.word8.
Read B.word163f4 mask2; if nonzero return1 IMMEDIATELY, skipping swGet and
other unit/work fields. Otherwise:

1. Call swGet(wrap32(4474+K)); retain its normalized0/1 result.
2. Read current U.word1c mask4. Compute/read retained B's kind-K bit from
   bank163d4 and OR its MASKED word value with the swGet result.
3. Only when U1c mask4 is set, read original kind O=U.word4 and OR its
   retained-B bank163d4 bit too. There is NO second swGet(4474+O) call.
4. Return exactly1 for nonzero combined word, else0.

Unit.kind8 and original-kind4 therefore have different persistence queries.
Neither current nor original kind is clamped or checked against231. No group
scan, table-message lookup or status/HP/AC eligibility check occurs in this
checker. Mask2 bypass is a battle-work policy whose external writer remains
open; do not turn it into a universal Tattle ownership flag.

## Bank and lifetime boundaries

For nonnegative kinds0..230, battle-work word offsets span163d4..163f0;
policy word163f4 follows them. This arithmetic span does not prove an arbitrary
kind cap or all bank initialization writers. Persistent switches4474..4704
use GP178's8192-bit bank. swReInit preserves that bank while clearing local
swdrv banks; battle teardown/card reload are separate services.

Original group pointer/list data is hashed and decoded into derived kind
metadata in [evidence](../../../tools/switch_monosiri_flags_evidence.json).
No original executable, asset or message payload is committed. Group source
ranges and caller/other writer/lifetime invariants remain separate from this
complete local setter/checker.

Validation: fresh complete setter/checker and12 swdrv listings in `/tmp`;
generic DOL metadata verification plus independent signed arithmetic and
original group-pointer/list scans. Remaining: all flag/policy/caller writers,
valid-kind domains, actual Tattle event/AC/messages/log/party/scene source
composition, save/card storage inclusion and original registration/reload traces.
Parent requests remain OPEN outside these concrete local portions.
