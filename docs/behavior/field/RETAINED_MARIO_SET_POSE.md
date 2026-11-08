# Retained original Mario pose selection

`SourceMarioSetPose` implements whole original
`evt_mario_set_pose800e6840/154`. This is source pointer/flag selection, not a
native renderer animation, pose endpoint or MarioMain completion receipt.

It captures E+18 then Mario through8041e900 and reads motion+2e. Motions24,25,22
or21 return2 before reading the actual argument or any flag. Other motions read
the raw pose operand and resolve evtGetValue80031260 before reading main flags.
Main80000000 calls actual `toDotMarioPose80058f9c(requested)`; engaged zero
returns2 without stores, and missing child remains a true hold. A nonzero result
is written to actual Mario+18; freshly read dirty+c gains1000. This branch does
not alter table-membership flags.

Ordinary form scans actual null-terminated pose-pointer table8032dd60 in source
order with strcmp8026d464(requested,tablePose). The original table has59 nonzero
entries then zero; this is pinned source data, not reconstructed strings or a
native preferred-pose list. It writes the actual requested pointer at+18, reads
dirty+c freshly, stores dirty|1000, reads extra+4 freshly and clears10000000 on
membership or sets it on a miss. Both branches finally read extra+4 again; if
01000000 is set they freshly read dirty+c and store dirty|8000. No source pose
name, supplied pointer or flag is rewritten for native presentation.

Root binds actual VM event memory/operand services and the canonical finite
Mario view(main0/extra4/dirtyc/pose18/motion2e). Original table/string source
bytes must exist under their real immutable owner. The compare callback can use
the existing native semantic strcmp policy over actual known bytes; it must
hold on unknown input. It is not a speculative PPC strcmp LWZ trace claim.
Actual Dot-form conversion stays its own typed child; no default pointer is
returned. The root native pose consumer must observe the canonical pointer and
own real imported original pose resources separately.

Callback/user/event address+pointer+ID+command and full shared-owner identity
are fixed across holds. `current` is checked before event identity access and
each source step; loss poisons, recursive resume/reset while held rejects.
A false store has no effects; child callbacks retain partial work. The service
domain is single-threaded and stationary within each callback. A4096-phase
native work budget yields a malformed/unending table scan without claiming
source completion or inventing a terminator.

Independent full-trace tests hold each external boundary twice, cover all early
motion exits, first/middle/missing table membership, Dot null/nonzero results,
late dirty/extra mutation, reentry, callback/event replacement and current loss.
Strict host, ASan/UBSan and ARM syntax pass. The verifier pins the whole body,
GetPtr, full original table and all table target string bytes read-only from the
US ISO. No Game/main/shared file change or fresh native-script completion is
included. In the actual beforebattle script the first call is9050664c; source
next reaches GetPos and original NPC move helpers, not immediate battle entry.
