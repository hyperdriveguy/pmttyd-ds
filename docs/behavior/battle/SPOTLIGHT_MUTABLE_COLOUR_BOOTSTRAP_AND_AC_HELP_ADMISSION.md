# Spotlight mutable colour bootstrap and action-command help admission

2026-10-13 US G8ME01 disc0 revision0 read-only source check. Follow-up to
[spotlight work](STAGE_SPOTLIGHT_WORK_INIT_AND_MAIN_ADMISSION.md) and
[StageMain pass](STAGE_WORK_INITIALIZATION_AND_INACTIVE_COUNTER_PASS.md).
No immutable asset is inferred for original BSS colour words; no native edits.

## WORD804295d4 is mutable SBSS2, not ISO-backed colour data

US symbol rows place804295d0 and804295d4 as separate4-byte `.sbss2` objects
`unk_804295d0`/`unk_804295d4` in battle_stage.o. Neither has an original DOL
file payload. They are globals addressed by r2=80427440 plus2190/2194.
Their values must be supplied by an actual mutable memory/bootstrap owner;
reading an adjacent ISO/file offset is not a source-backed default.

Complete __init_data8000542c/c0 first executes ROM copy table80005544. It
then walks original BSS zero table800055b0, stopping at zero length. Records:

| Original table address | Base / byte count |
| --- | --- |
| 800055b0 | 803c5360 /4fbb0 |
| 800055b8 | 8041e580 /ea4 |
| 800055c0 | 80429500 /40c |
| 800055c8 | 0 /0 terminator |

At800054c4 it memset(base,0,count). The third row covers both mutable colour
words through8042990b, proving their fresh bootvalue0. This is bootstrap
state rather than a stage-specific immutable literal or per-entry reset.
Do not reset them during ordinary reentry without an original writer.

Complete loaded DOL text census of r2-relative direct D-form load/store
operands2190/2194 finds exactly two instructions:

* batSpotEntry80137b04 reads WORD804295d4 and stores it into recordWORD28
  at80137b08. Fresh bootdomain therefore copies RGBA00000000.
* batSpotDisp80137f1c reads WORD804295d0 for its second-pass material colour.
  Fresh bootdomain supplies0 there too, if that activepass is admitted.

No direct r2-relative writer exists in that census. This does not exclude an
aliased pointer, REL or indirect service modifying the globals; preserved
mutable owner state remains necessary outside the bounded fresh path.

Actual batSpotInit first calls Entry (with its real zero-colour copy), then
writes original immutable WORDffffffff from80422ad4 into that returned record
at8013816c. Thus each of the five fresh Init records ends white even though
Entry's sourceBSS read is0. A native provider must preserve both source writes
rather than replace Entry's read with white or fabricate an ISO colour asset.

## Next service: AcHelp Main has a complete local inactive predicate

BattleAcHelpMain802168a4/64 recaptures current globalBattleWork B at802168b0.
Read WORD B19008; exactly0 returns. Otherwise read signedWORD B1900c;
exactly0 returns. Negative nonzero mode is NOT an inactive gate. Otherwise
read WORD Bef4; mask2 set returns. No other admission or SP/pouch check.

Admitted active domain requests dispEntry80010bc4(camera8,class1,
callback80216728,user0,priorityFLOAT498 at804272b8) at802168f4. It does not
invoke that renderer directly, return a readiness bool, mutate packed help
fields or clear the key/mode. Actual active display callback and text/UI/GX
provider remain their own request, not a source silent-success service.

BattleAcHelpInit80216908/4 is literally blr. The original19098-byte BattleInit
memset supplies initial B19008/1900c=0; HelpInit does not clear prior state.
BattleAcHelpSetHelp80216954/10 captures current B and stores incoming rawWORD
key atB19008. It does not set mode or call anything.

BattleAcHelpSetDispType8021690c/48 captures current B, stores incoming rawWORD
mode atB1900c at80216924, THEN always requests BattleAudience_WinSetActive801a1cdc.
Mode exactly0 sends argument1; every other mode sends0. WinSetActive resolves
the current inline audience window and stores the low8 argument to its BYTE0;
it does not change crowd amount/target (NumToTargetSub is801a1bf4). Callback changes do
not restore the earlier mode. Thus binding a mode write without its audience-window
request is not whole source SetDispType completion.

The original StopAC event wrapper requests source ActionCommandStop, then
SetDispType(0), as pinned in [Stop services](BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md).
A native packed help owner with actual retainedmode0 therefore admits inactive
Main even if the original key remains nonzero. Initial key0 is an independent
creation-domain predicate; later menu help writers cannot be ignored because
native presentation uses another UI struct. The completion pass should check
actualpackedwords and skip display only through one of these verified gates.

## Evidence

| Raw original span | SHA256 |
| --- | --- |
| __init_data8000542c/c0 | d914cfeeba969b19278cdc6b151829c1683d7852fb7cf568770d450c2eaaa8bb |
| BSS row800055c0/8 | 51e6308dd9eb2b1ee684709394fef9d1931f02b5c77fb8e838368067bb394cca |
| AcHelpMain802168a4/64 | ac32c584aebdfe843e27b481f92a6cb19d43d3f0766a769394f8135809ebb320 |
| AcHelpSetDispType8021690c/48 | b6278b065afb487c64e45d10397ca1c30d0a05798ef246b1eb8a419de7acc975 |
| AcHelpSetHelp80216954/10 | 8b8ff3af915b6a64ad1eb827a193facf1c22c28601ad1262a9d33fe05fc53a82 |

Complete listings under ignored artifacts/recovered-temp-evidence/help-used-*;
Entry/Init/Disp source lists already retained there as spot-used-*. Checked
BSS metadata/zero table, direct instruction census, used zero→white order,
complete help gates/setters and source priority. Documentation only; no native/
shared docs edits, build rerun or commit; scoped diff check passed. Mutable
colour lifetime and activehelp renderer remain explicit external providers.
