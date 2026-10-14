# Retained ordinary battle part display

2026-10-14. Implements the ordinary local body of original US
btlUnitPartsDisp80101774 against the
[source matrix/material/history contract](ORDINARY_BATTLE_PART_CALLBACK_MATRIX_MATERIAL_AND_HISTORY.md).
This is callback continuation work; original raster equivalence and a completed
native ending draw are not established.

`BattlePartDisplayCursor` captures the callback's part, initial unit and battle
owner. SDK/scalar/material/callback/draw requests remain pinned while unavailable;
following source requests read their live inputs. Negative signed pose handles
return before matrix work. Special part flag20000000 holds after the source's
initial16 SDK calls. Ordinary rotation orders0..5 and the owner04000000 camera
correction path have explicit source graphs. Invalid rotation orders skip the
switch's three rotations rather than selecting order0.

The initial unit graph, ordinary part graph, inclusive90..270 negative unit-Z
rebuild and history copy retain SDK call boundaries. All three pose passes use
the same un-biased drawing matrix, revised incoming angle and original scale2.
Only the history matrix receives the final world-Z minus5 translation. Material
processing retains source color gates, sequential poison red/blue arithmetic,
per-channel unit fade reads, live handles and independent pre/post callbacks.
History is embedded at part+21c: ten68-byte records move descending, then the
new matrix/angle/color/header values publish only after all draws and post-hook
complete. A held final Copy never repeats the preceding shift.

The native camera queue binds this driver only to its actual borrowed80101774
invocation. It uses the live VM actor memory, current camera, source angle table,
status predicate and pose pool. Actual pose colorF0 and flagsE8 update through
the recovered leaves; no host pointer is cast from a source word address.
Unlinked ordinary pose model flags admit or reject each pass using the original
wrapper rule. Admitted inner geometry is a service boundary at8003bc68; linked,
missing and other unbound pose paths hold at8003b4d8. An unavailable service does
not acknowledge the queue or publish history. Generic driver draw providers
represent the entire wrapper; the native bridge performs its finite admission
before forwarding an admitted inner draw.

Numeric adapters use ARM/libm sine/cosine and the existing explicit matrix
operation order. Float/gravity phase offsets use live unsigned phase halves and
native sine-degree adapters retaining the source rounded multiply by
6.2831854820251465 then division by360 before double sine and float return. Exact original SDK paired-single/trig rounding,
linked/pool poses, arbitrary overlapping Copy memory, nonzero part hooks,
special20000000 matrices and inner pose geometry remain open. No full callback,
GPU parity, field return or prologue acceptance is claimed from a held native
inner draw. The documentation request remains open for those dependencies.

Validation covers full local graph completion with explicit host providers,
held SDK/material/pass/history requests, fresh next-call handles and hooks,
poison color sequencing, six rotation orders, mirror rebuilding, signed zero,
negative handles and the explicit special-path hold. Runtime integration uses
the real finishing camera queue and pose owner, verifies material stores,
retained queue ownership and unchanged history at the inner renderer boundary.

The next graph already has source contracts: [inner state and buffers](ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md),
[wrapper admission](ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md), and the linked
facing/group/shape/array contracts. Its first native dependency is
mapTestXLU8001ca2c against actual current-map group state; later traversal must
use the retained current camera and actual indexed source geometry. Unlinked
pose admission alone does not establish an empty model or authorize an idle
bake as a replacement for original material/geometry requests.

Private read-only evidence: `artifacts/battle-display-queue-evidence/floatOffset.dis`
(80100ed0:64, raw body SHA256
`4cd8d914eda9ea0a87f04771dc11911c5c62421a23491a397072b3bbefb01cdb`),
`gravityOffset.dis` (80100ffc:64,
`ef020a7432713eebe97ab5b9c30dc2732a78dcf8ddaafe9c1df0e7e9e5aeea33`),
`GetSin.dis` (80125cd4:34,
`2438796048394f29f154dc21c9a0620236c7dec53740e525c529cd659f80777f`),
and `mapTestXLU.dis` (8001ca2c:a8,
`5c65a0530038f9e33f75b326aa97bcb531167a3aa931feae8eae5a275a4a0718`).
Hashes identify original instruction bytes, excluding disassembly formatting.
