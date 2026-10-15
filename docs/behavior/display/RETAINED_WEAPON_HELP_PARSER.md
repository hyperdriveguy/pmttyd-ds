# Retained original weapon help window and parser

`resumePackedBattleWeaponHelp` implements the whole US revision-0
`DrawHelp80117c1c/1d4` body. It is a retained source controller, not a new
text layout or a native font replacement. The original weapon parent requests
it at `80115ff4`, with its actual help key and optional source-produced X/Y.
An adapter must require both coordinates rather than inventing private stack
values for an unwritten parent output.

## Source and original presentation

`tools/verify_battle_weapon_help.py` opens the supplied US revision-0 ISO
read-only and checks all 468 original function bytes, source r2 startup,
original RGBA, all eight float literals and the exact fallback string.
The body SHA-256 is
`8b902f288bcc368e1f4351fb248dcb8d80a65b43effe9aa9e47e16614539681c`.
Original `artifacts/weapon-help-window-source.txt` is an ignored disassembly
witness. No ISO, original compiled code or extracted message is committed.

The controller preserves this call order:

| Source child | Operands and role |
| --- | --- |
| `8007b44c` | Window type 0, private original color `ffffffcf`, actual X/Y, original width 400, height 50, final float 10 |
| `80077b4c` | Font initialization |
| `80077844` | Original integer argument 0 |
| `80077758` | Original float scale .8 |
| `80081b8c` | Original help-key message lookup |
| `80077758` | Before each nonempty displayed row, original scale .8 |
| `800773d0` | All three private rows, at X+20 and Y minus 4/28/52, even when empty |

Every source literal is loaded from actual supplied memory at its original
stage. Held child calls retain copied operands and call identity. Later
source stages reload their literals freshly. The outer native owner must
supply complete window/font/message children with their original side
effects. Capturing a request or vector does not complete native presentation.

A null **successful** message lookup takes the actual original guest string
at `802ef788`, displayed at X+10,Y-4. Its 25 source bytes are
`82b982c282df82a282cd814082c882a282cc82be8149814900`.
This original Shift-JIS fallback is passed unchanged; it is not replaced with
invented US dialogue or silently treated as empty text. A missing message
provider remains pending and does not select this branch.

## Byte parser and private stack

Only byte offsets 0,48,96,144 of the original 192-byte local are initialized
to zero. `BattleWeaponHelpBytes` preserves all remaining cells as optional
unknown bytes. Written source bytes become known; no initialization, padding,
string terminator or row truncation is added.

The parser first loads a source byte solely to compare it with `<`. On that
branch it increments the guest pointer while scanning to `>`; a NUL inside
this scan does not end the scan. After either branch it **reloads** the byte
at the current guest address. A zero writes the real destination terminator;
a newline writes zero, advances the row base by 48 and resets the destination;
otherwise the original byte is copied. Both pointers advance with original
32-bit wrap. Adjacent tags are not normalized: `<a><b>X` produces `<b>X`.
High bytes and all other tokens remain raw. The linked font child owns their
actual original glyph/control interpretation.

The source has no 48-byte row clamp or 192-byte buffer bound check. Long rows
can overwrite later row heads. The controller retains those in-buffer writes.
An escaping store requires the explicitly provided `stackStore` service with
its exact source frame offset and byte. Without a genuine source-frame alias
owner it holds at that original store; it neither drops the write nor reports
an input validation success. An owner that supplies this service is responsible
for actual aliased stack effects, saved state and restoration. The host spill
fixture demonstrates protocol/order only, not PowerPC stack-corruption parity.
No normal native spill service is installed by this batch.

All three font calls borrow the same stable `BattleWeaponHelpBytes` owner
with offsets 0/48/96. A linked font reader must stop or wait when a needed byte
is unknown; `.byte()` never synthesizes zero outside known cells. It must not
replace the source parser with a normalized string or change original message
control semantics. Only the first three rows are drawn, although parsing still
writes later rows exactly as the original body does.

## Ownership and native frontier

The cursor pins a strong immutable service lifetime. The actual allocation,
queued callback/frame/camera/resource predicate guards every external child
before and after invocation; false permanently retires a held controller.
Service identities, lifetime control-block identity, argument bit patterns and
private request objects remain stable across waits. Reentry and reset during
a provider invocation are rejected. A whole child returning unavailable resumes
at that child without replaying completed window/font effects or parser stores.
Original guest byte access is optional, so an unmapped unterminated tag holds
its exact source read rather than reading invented padding.

`800773d0` is the scalar `FontDrawString` wrapper. A read-only survey shows its
complete 0x68-byte body builds a position and calls `80077034` (vector/pitch
font). It is different from matrix-font `80076cdc`; this module does not pretend
that the two consumers are interchangeable. The current frontier requires a
real scalar font/window service with the actual font globals, glyph resources,
control parser, current camera and native ordered draw admission. Matrix-font
work remains independently reusable only where original source shapes agree.
No normal registration, readiness mutation, fallback camera, placeholder art,
touch-only control or fabricated glyph acknowledgement is introduced.

Original message/window/font resources remain owned by their original rights
holders. Source tracking grants no new license. This batch adds no texture
conversion, asset pack, dialogue rewrite or native activation.

## Validation

Strict host tests hold every source byte-read and linked child boundary, then
compare original call order and coordinates against uninterrupted execution.
They cover fresh double loads, adjacent markup, high bytes, missing tag ends,
empty/three/four rows, partial initialization, long rows and explicit source
stack escape waits, original null-search guest fallback, held copied arguments,
late scalar reads, missing providers, service replacement, reentry and sticky
lifetime retirement. A scalar-font **fixture** reads only known bytes; it is
not a successful native font binding. ASan/UBSan (leak scan disabled), strict
GNU ARM syntax and the read-only original ISO verifier pass. Root integration
owns full `make test`/`make all` on a consistent frozen snapshot.
