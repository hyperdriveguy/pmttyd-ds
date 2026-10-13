# Switch 1.0.0 static research record

Switch extraction, licensing and static-analysis research recorded in the asset pipeline document, moved to the Switch collection on 2026-10-12; content is verbatim. No Switch asset is converted into or linked into the native 3DS runtime by this research.

## Switch 1.0.0 static research (2026-10-12)

The user selected the Switch remake as the mechanics reference and subsequently
restricted work to static analysis. Original GameCube conversions and their
licensing/provenance above remain separate. No Switch asset was converted into
or linked into the native 3DS runtime by this research.

Read-only source:
`/home/carson/games/roms/Paper Mario The Thousand-Year Door [BASE][0100ECD018EBE000][US][v0].nsp`.
SHA256 `04fba23ab27de88276d2a04eaf69619cd09044acb41de62c1cb2f89a8de110e4`;
CNMT application0100ecd018ebe000/version0, NACP1.0.0. Detailed member identity,
content checks and reproduction are in [Switch baseline](BINARY_BASELINE.md).

Local extraction paths (all ignored):

- `local-assets/switch/nca/`: original PFS0 members, including private ticket.
- `local-assets/switch/exefs/`, `romfs/`, `meta/`, `control/`, `other/`:
  decrypted archive sections; no original input overwritten.
- `local-assets/switch/decoded/`: selected Zstandard-expanded data ELFs and
  uncompressed main NSO; `main.elf` is an inspection-only segment wrapper.
- `artifacts/reference/switch/`: hashes, table/relocation reports, integrity logs,
  disassembly and an isolated private key mapping. Keys are never published.
- `artifacts/reference/hactool/` and `Unsimplifier/`: pinned public source tools;
  exact revisions/licenses in the baseline document. No public editor UI or
  arbitrary mod was used to mutate the reference game.

Original game code, text, models, music and other assets remain the property of
their rights holders; possession and extraction do not grant a redistribution
license. Do not commit/distribute the NSP, extracted originals, keys, firmware
or complete story text. Research commits contain prose, independently authored
inspection tools and tests, not game payloads. hactool is ISC; Unsimplifier is
Unlicense. Public layout labels are distinguished from verified native semantics.

The user's local prod.keys was used for read-only decryption. The supplied
 title.keys lacked the game's rights entry; the archive's own common ticket
provided an isolated mapping. Original key files were not altered. Before the
static-only instruction, isolated Ryujinx files/captures were written under
`artifacts/ryujinx/`; no gameplay was reached and the normal user profile/saves
were not changed. Firmware.23.0.1.zip was supplied, but successful installation
was not established. No firmware is required for continued static recovery.

HSV conversion and its floor dependency add two US fingerprints and24 instruction
anchors. Read-only checks cover255/360/60 binary32 normalization constants and
floor's binary64 1e300/zero probe. Authored contracts describe byte order,
single-precision/fused operation boundaries and exponent/mantissa branches;
no original executable is linked, distributed or executed. Local disassemblies
and reports remain ignored; no asset conversion or licensing change occurs.

Font setup/mapping research adds four US function fingerprints and24 anchors,
seven original basename pointers and five name/path bindings. Read-only complete
BFN hash checks cover f/papermarioset_US.bfn(66560 bytes),
f/papermarioset_EU.bfn(66560) and f/papermarioset_JPN.bfn(264896). Local descriptor
metadata is ignored artifacts/reference/behavior/font-data.json; no font payload
was exported or converted by this step. Authored contracts record source paths,
block/texture dimensions and setup's two ordered width mutations, including the
US/EU fallback alias. No asset redistribution, license change or executable
linking occurs; original fonts remain subject to original rights.

Message-width/icon-dimension research adds four US function fingerprints and24
anchors. Checks pin all seven US character-spacing words (the pinned Japanese
source declares a shorter table), three icon name/scanf bindings and twelve
halfword remaps. Authored contracts cover byte scanning, wrapping/max width,
LF suppression/count, fused icon arithmetic and directory/TPL dimension lookup.
No original code or icon payload is exported/linked, no conversion or asset
license change occurs; inspection ELFs/disassemblies/reports remain ignored.

Original scan-runtime research adds three US function fingerprints and28 anchors
for sscanf, __StringRead and __sformatter. Read-only checks hash the complete
256-byte classification table and check its six whitespace codes. Authored
contracts cover ABI, EOF/pushback, counters, output storage and string loops;
scanf grammar/numeric/multibyte internals are explicitly open. No original code
is executed, compiled or exported into source history; local ELF/disassembly/
metadata reports remain ignored. No message/asset conversion or license change.

Scanf-specific parse_format adds one namespace-qualified US fingerprint and24
anchors. Read-only checks pin its40-byte initializer, all52 dispatch targets via
a complete table hash and ten width-digit classification codes. Authored grammar
covers suppression, wrapped widths, storage selectors and acceptance bitmaps;
no original parser implementation/format payload is copied into source. Its ELF,
filtered symbol CSV and disassembly stay ignored. No asset conversion, original
execution or license change occurs.

Unsigned32 scan research adds __strtoul identity and20 anchors. Read-only checks
hash all17 state targets and the complete256-byte uppercase table. Authored
contracts record signs/base inference, lookahead/width/count, sticky overflow
with wrapped magnitude and unconditional unread. No public strtoul or executed
conversion claim is implied. Original disassembly/reports remain ignored; no
asset conversion, original executable linking or license change occurs.

Unsigned64 scan research adds __strtoull identity and24 anchors, with a full
hash of its separate17-entry state table. Authored contracts record division
threshold, word products/carries, overflow/count/sign outputs and invalid-entry
uninitialized-register unread. Local ELF/disassembly/report outputs stay ignored;
no original execution, asset conversion, native linking or license change occurs.

Multibyte runtime research adds mbtowc and is_utf8_complete fingerprints and24
anchors. Authored contracts record permissive continuation-bit checks, exact
halfword masks, incomplete/error routing and size1 scan-consumer composition.
No message bytes are reinterpreted/rewritten, original code linked or asset
converted; original ELF/disassembly/report artifacts remain ignored. Licensing
is unchanged and static checks do not establish executed decoder equivalence.

Floating scan entry/special-value research adds the containing __strtold
fingerprint and24 selected anchors,42-byte zero initializer, two token bindings
and two special binary32 words. Its finite states/conversion tail remain open;
whole-body identity is not complete behavioral recovery. Authored contracts
record callback/width/sign/count and no-unread special exits. No original code
linking/execution, data conversion, message rewrite or license change occurs;
local disassemblies/reports remain ignored.

Decimal floating scan adds24 anchors within the already-fingerprinted __strtold,
initial decimal-point pointer/byte and four binary64 range constants. Authored
contracts cover retention/scale/exponent/count/handoff and local range/sign exits;
hex states and __dec2num remain open. Original disassembly/reports stay ignored;
no executable linking/execution,asset conversion,message rewrite or license change.

Hexadecimal floating scan adds32 anchors within the existing __strtold identity
and checks all22 hex digit classification codes. Authored contracts cover nibble
packing,exponent/count/normalization and direct binary64/range/sign construction,
including static uninitialized/nontermination edge deductions. No original code
execution/linking,asset conversion or message rewrite occurs; local disassembly/
reports remain ignored. Numeric execution equivalence and licensing are unchanged.

Decimal-to-binary entry research adds containing __dec2num identity and24 anchors,
three binary64 sign/zero constants and complete lowercase-table hash. Entry/
special-record contracts are authored;finite approximation/refinement remains
open despite whole-body identity. No original code execution/linking,asset
conversion or message rewrite occurs;local ELF/disassembly/reports stay ignored.
Original rights/licensing remain unchanged.

Finite decimal preparation/approximation adds20 anchors within existing
__dec2num identity,eight binary64 chunk powers and three base/bias constants.
Authored contracts distinguish private record mutation and approximate candidate
from still-open refinement/final rounding. No original code execution/linking,
asset conversion or message rewrite;ignored inspection artifacts and original
rights remain unchanged.

Decimal infinity-candidate research adds24 anchors,complete36-digit boundary seed
hash/NUL and maximum-finite fallback bits. Authored contracts cover local record/
compare/retention/sign flow;general refinement/ties remain open. Original seed
payload is retained in the read-only ISO;no native linking,original execution,
asset conversion,message rewrite or license change occurs. Inspection reports
and disassembly remain ignored.

Decimal equality/refinement admission adds __equals_dec identity and24 anchors
for complete equality,initial converter comparison and first upward neighbor.
Authored contracts distinguish sign-ignored/trailing-zero equality and raw-word
neighbor from still-open later search/ties. No original routine execution/linking,
asset conversion,message rewrite or license change;inspection artifacts remain
ignored and whole-body identity is not complete conversion equivalence.

Decimal neighbor/refinement research adds32 anchors within existing __dec2num
identity. Authored contracts cover both search loops,four subtraction requests,
record equality/parity ties and asymmetric selection. Local containing-body
recovery is distinguished from open __minus_dec/pow and execution equivalence.
No original code execution/linking,asset conversion,message rewrite or license
change;ignored inspection artifacts remain local.

Decimal subtraction adds __minus_dec fingerprint and24 anchors. Authored
contracts record complete copy/alignment/borrow/correction/normalization flow,
including pointer-dependent tail correction and unchecked zero/alias edges.
No exact-distance/execution equivalence is inferred. Original disassembly/report
artifacts stay ignored;no native linking,asset conversion,message rewrite or
license change occurs.

Power entry/special research adds pow and containing __ieee754_pow fingerprints,
24 selected anchors and three special doubles. General finite/large-exponent
core remains open;identity is not full behavior coverage. Authored contracts
record parity/shortcut/errno order and decimal base-five caller domain. Original
ELF/disassembly/reports stay ignored;no original execution/linking,asset conversion,
message rewrite or licensing change occurs.

Power large-exponent research adds20 anchors within existing __ieee754_pow
identity and a full48-byte/six-coefficient hash. Authored contracts retain raw
high-word thresholds,original signed-base reload,fused operations and low-word
split;general logarithm/common tail remain open. No original execution/linking,
asset conversion,message rewrite or licensing change;local inspection reports/
disassembly remain ignored.

Power general-log research adds24 anchors within existing helper identity,
complete reference/correction and scale/coefficient region hashes plus signed
conversion bias. Authored arithmetic preserves fused boundaries and integer
low-word splits. Common exponentiation/range tail remains open;no accuracy or
execution equivalence is inferred. Original inspection artifacts stay ignored;
no executable linking,asset conversion,message rewrite or license change.

Power common-tail research adds32 anchors within existing helper identity and
full104-byte/thirteen-constant hash. Authored contracts cover complete local
product/range/reduction/polynomial/scaling/sign sequence;helper and composed
numerical execution equivalence remain open. No original code linking/execution,
asset conversion,message rewrite or license change;local artifacts stay ignored.

Square-root research adds sqrt/__ieee754_sqrt identities and24 anchors plus
remainder-path one constant. Authored contracts record complete local special/
errno,integer normalization/extraction/correction and output bit flow,without
executed numerical/FPSCR equivalence. Original ELF/disassembly/reports stay
ignored;no executable linking,asset conversion,message rewrite or license change.

String-pitch drawing adds three US fingerprints and24 anchors,two zero-vector
wrapper templates and half/default pitch constants. Authored contracts preserve
byte/tag versus LF behavior,matrix requests,pitch/advance and outline/raw-color
restoration with known inner RNG counts. No original execution/linking,asset
conversion,message rewrite or license change;inspection artifacts remain ignored.

Affine matrix research adds three US helper identities and24 instruction anchors,
with positive-zero and Unit01 constant words checked read-only. Paired-single
instruction semantics were cross-checked against Dolphin's public GPL-2.0-or-later
interpreter source;no interpreter implementation was copied or linked. The
behavior document cites that reference and distinguishes original words from
emulator interpretation. Original ELFs/disassembly/reports remain ignored.

US completion/usage audit reads original DOL text/data and all29 shipped REL
module0 import streams read-only. Ignored us-usage.json contains annotations,
counts,relocation locations and20 command registration entries,not executable
or dialogue bytes. Its addresses/names derive from the existing GPL-3.0 symbol
metadata;no upstream implementation or original code is linked or distributed.
No asset conversion or license change. Static references are not execution proof.

Gekko disassembly correction (2026-10-12):independent tools/gekko_disasm.py
repairs LLVM paired-single mislabels and is integrated into function inspection.
354 instruction rows in47 ignored original listings repaired;386 inspection
ELFs unchanged. No ISO write,asset conversion,original execution or story edit.
Encoding facts consulted GNU opcodes and Dolphin primary references;links and
upstream licenses are in GEKKO_DISASSEMBLY.md. No upstream implementation copied
or linked;original text/ELFs and metadata repair report remain ignored.

Ordinary attack research adds six function identities,32 anchors,seven TimingA
threshold/window tables and pointer list,registration/two floats,and independent
Jump/Hammer-core event hashes/four setup records. dol_event_inventory.py reads
US ISO DOL data read-only;ignored reports retain raw operands/annotations,not
dialogue contents. No original code linked/distributed,no asset conversion or
story rewrite;existing original rights and reference licensing remain unchanged.

Hammer ID3 research adds eleven function identities and48 anchors,seven release
allowances,registration,two display floats/two sound keys and eight parameter/two
flag records in previously hashed US Hammer event data. No new extraction or
asset conversion,ISO write,game execution,native linking or story rewrite.
Original reports/disassembly remain ignored;existing licensing is unchanged.

Hammer weapon-branch research adds six function identities and40 anchors,used
VM targets/condition/case operands,three weapon difficulty/event references and
two wrapper streams. Existing read-only DOL inventory outputs retain raw data
only locally;no original code/dialogue distributed or linked,no asset conversion.
Interpretation remains static;public names are annotations and licensing unchanged.

Harbor story binding research reads rel/gor.rel directly from the read-only ISO
through rel_event_inventory.py,without writing an extracted module. Local reports
retain raw commands,self/module0 ADDR32 bindings,foreign/BSS identities and
resource identifiers;no loaded VA inference or event execution. Seven span hashes
and original msg/US/gor_00.txt identity checked;fresh message census preserves
text/token payload hashes without exporting dialogue. Only IDs/offsets/actor
bindings/authored behavior notes committed. No asset conversion,original code
linking/distribution or licensing change;symbol annotations retain existing
reference provenance. Full original story/transcript/conditions still incomplete.

Field/NPC battle research adds ten function identities and48 anchors;seven
harbor configuration/prolog spans and actual setup/group/unit relocation chain
plus zero DOL item table are checked. No new extraction/asset conversion,ISO
write,emulator execution,native linking or story edit. Registration pointers and
unit stat interpretations remain bounded by documented consumers;original files
retain existing rights and all inspection artifacts stay ignored.

Harbor boss tutorial research adds five DOL state-service identities/24 anchors,
seven whole event spans and two60-byte REL feedback helper hashes/relocations,
plus exact six US message-key uniqueness and enemy defense/power bindings. REL
inspection ELFs/listings are ignored and use file offsets explicitly;raw branch
targets are unrelocated,annotated with original relocation destinations. No
original executable linking/distribution,ISO write,asset conversion or story
rewrite;existing original/reference provenance and rights remain unchanged.

Feedback writer/stage-gate research adds four identities/48 anchors,original
quarter/half/signed bias and four ordinary attack declaration records. Connects
REL tutorial readers to DOL declaration counts and one proven battle flag
consumer. No original execution,ISO/profile write,asset conversion,executable
linking/distribution or story rewrite;all original artifacts remain ignored.

Unit entry/stat/spawn research adds five containing-body identities and32 anchors,
original boss HP5/FP0 data and tutorial getter/DIV/helper-spawn bindings. Large
entry body coverage is explicitly partial despite whole-body fingerprint. No
new extraction/asset conversion,ISO write,original execution/native linking or
dialogue rewrite;artifacts ignored and provenance/rights unchanged.

Additional target/attack research (2026-10-12): seven original DOL wrapper/helper
fingerprints establish first-attack pointer pairing,body-part ID lookup,signed
side inversion and sampling interfaces. Forty selected instruction anchors
include their local contracts and relevant GOTO/CASE_OR/CASE_END regions;the
latter regions do not constitute complete new VM helper coverage. Direct harbor
REL import/result/label checks pin the normal-attack request graph and absence
of label99/END_SWITCH in its95-command span. Fresh report remains ignored at
artifacts/reference/behavior/evidence.json;fresh event inventory was generated
read-only into /tmp/pm-boss-normal-attack.json. Original executable bytes/poses/
sound resources were not converted,linked,committed or distributed. No original
execution or new licensing rights are inferred from these contracts.

Battle jump research by agent ID1 (2026-10-12) uses a separate read-only verifier,
tools/verify_battle_jump_contracts.py,while general research tooling is shared
with agent ID2. Its ignored battle-jump-evidence.json records ten body identities,
55 instructions,nine initialized constants and concrete harbor jump imports/
operands. Nine local bodies are documented completely;evtGetFloat is documented
only for used literal routes. Inspection ELF/disassembly artifacts remain under
artifacts/reference/behavior and use corrected Gekko paired-single mnemonics.
No original executable linking/distribution,asset conversion,ISO/profile/save
mutation or story/dialogue rewrite. Encoding/behavior facts do not grant rights
to distribute original resources;existing provenance and rights still apply.

Physical input/cadence research by agent ID1 (2026-10-12) documents six new full
local bodies and checked ordering/snapshot regions in prior helpers. Original
DEMOPad is BSS;no live input state was extracted or inferred from initialized
file bytes. tools/verify_input_cadence_contracts.py pins ten containing bodies
(including four prior identities),64 instructions,28 direct calls,reset-mask data,
AC display depth and the event-credit unit. Report remains ignored at
artifacts/reference/behavior/input-cadence-evidence.json;listing/inspection ELFs
remain in the same ignored research directory. SDK hardware and actual input
poll/impact timing remain dependencies. No asset conversion,original execution,
ISO/profile/save mutation,native linking or original text rewrite. Existing
original/reference provenance and distribution rights are unchanged.

Input ownership/control research by ID1 (2026-10-12):input_ownership_audit.py
reuses the read-only shipped-reference census and adds containing-owner labels,
six small DOL body checks,30 selected instructions,two data checks and the gor
_config_key_record helper hash/HA/LO/REL24 bindings. One162-command lecture event
hash and its selected injection cases/defense imports are verified;full event
semantics remain open. Ignored report is artifacts/reference/behavior/
input-ownership-audit.json;temporary unrelocated REL inspection ELFs/listings and
lecture inventory are in /tmp. REL file offsets,not provisional loaded addresses,
identify these bodies. No original executable linking/distribution,dialogue
rewrite,asset conversion,ISO/profile/save mutation or original execution. Existing
rights/provenance remain unchanged;metadata/reference licenses do not grant
original-resource distribution rights.

Helper-role/coordinator research by ID1 (2026-10-12):the read-only verifier
verify_lecture_helper_roles.py pins eight full data records,three relocated REL
leaf bodies,ten event spans/counts,23 local/ten service bindings and concrete
work/selector writes. Distinct NPC/lecture kinds are preserved even when visual
assets are reused. Twenty coordinator message/select/append sites bind16 exact
US keys in msg/US/gor_02.txt (128,972 bytes,SHA-256
1958e3d22bb27b01332e08b67f279a33f21e0e8a3aba2b947ad8c2a446144f16).
Payload hashes/speaker operands are metadata;original text/control tokens remain
in the read-only ISO/local assets and were not paraphrased or committed. Ignored
reports are christine-helper-events.json,frankli-lecture-coordinator.json,
lecture-counter-event.json and lecture-helper-roles-evidence.json under
artifacts/reference/behavior. Temporary REL inspection ELFs/listings remain in
/tmp;unrelocated targets are distinguished from original import bindings. No
original execution,ISO/profile/save mutation,asset conversion,executable linking/
distribution or story rewrite. Existing provenance and rights remain unchanged.

Lecture player/window research by ID1 (2026-10-12):verify_lecture_player_flow.py
pins seven DOL containing bodies(four window/two timing services complete,large
menu builder partial),31 instructions,three whole403-command event spans and
three weapon/professor-table spans. Default-record substitution and mode/feedback/
AC parameter pointers are checked separately from menu availability/runtime.
Six exact US keys001/002/008/009/010/024 bind gor_02 payload hashes and original
speaker operands. No original dialogue rewrite or text extraction into history.
Ignored report is lecture-player-flow-evidence.json and original event inventory
lecture-player-events.json under artifacts/reference/behavior;inspection ELFs/
listings remain ignored. No asset conversion,original execution,ISO/profile/save
mutation or original executable linking/distribution. Existing rights/provenance
remain unchanged.

Lecture field research by ID1 (2026-10-12): tools/verify_lecture_field_flow.py
and its lecture_field_evidence.json manifest pin four original gor event spans,
346 commands,332 scoped ADDR32 relocations,22 operands,four entry/NPC links and
11 message sites in msg/US/gor_02.txt. Only identifiers,offsets,operands and
payload hashes enter source history;original dialogue/control tokens remain in
the read-only resource and must be reused unchanged. Reports lecture-field-entry,
lecture-field-callers and lecture-field-flow-evidence.json remain ignored under
artifacts/reference/behavior. No conversion,ISO modification,original execution,
emulator profile/save mutation or executable/asset distribution occurred.

Field lecture callback research by ID1 (2026-10-12):verify_field_lecture_callbacks.py
and field_lecture_callback_evidence.json pin15 DOL containing bodies,54 selected
instructions,six constants and10 actual gor imports. Twelve local bodies are
fully specified;three general variable services close only used local-coordinate
branches. Original Gekko ELF/listings and field-lecture-callbacks-evidence.json
remain ignored under artifacts/reference/behavior. Metadata and authored behavior
only are versioned. No asset conversion,original dialogue rewrite,ISO mutation,
original execution,emulator profile/save changes or original code distribution.

Field party lifetime research by ID1 (2026-10-12):verify_field_party_lifetime.py
with field_party_lifetime_evidence.json pins six original DOL bodies,43
instructions,three numeric constants,five data-span hashes and three actual
field bindings. Kind1/8 init/hook and ordinary pose-row metadata are pinned;
no animation asset conversion or resource/playback execution occurred. Original
ELFs/listings and field-party-lifetime-evidence.json stay ignored under
artifacts/reference/behavior. General field verifier accepts a manifest and
checks optional data hashes;existing callback checks retain their scope. No ISO/
profile/save mutation,original binary/dialogue distribution or rewritten story.

Field party mode research by ID1 (2026-10-12):verify_field_party_modes.py and
field_party_mode_evidence.json pin eight whole US DOL bodies,44 instructions,
six constants,three data-span hashes,two used lecture imports and original BSS
span. The scoped DOL text integer-store census found no direct r13-based stores
overlapping control-index8041ead0;alias/indexed/floating/REL/bulk writes remain
excluded. Original ELF/listings and field-party-mode-evidence.json remain ignored
under artifacts/reference/behavior. No resource conversion,original dialogue
rewrite,ISO/profile/save mutation,original execution or binary distribution.

Field driver research by ID1 (2026-10-12):verify_field_party_drive.py with
field_party_drive_evidence.json pins four whole DOL bodies,58 instructions,one
numeric constant,four data-span hashes and two used lecture imports. The15-word
mode table is additionally checked as explicit address metadata;the reusable
field verifier now validates optional data words. Kind1/8 records and four
vectors are hashed,not copied as asset payloads. Original ELF/listings and
field-party-drive-evidence.json remain ignored under artifacts/reference/behavior.
No asset conversion,original execution,ISO/profile/save mutation,original
binary/dialogue distribution or rewritten story occurred.
