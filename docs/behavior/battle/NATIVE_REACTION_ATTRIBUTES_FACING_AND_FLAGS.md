# Native reaction attributes, facing, flags and data children

This continues [original hit-reaction ownership](NATIVE_HARBOR_HIT_REACTION_OWNERSHIP.md).
The common original Crump damage child now has actual attribute, facing-reset,
unit-flag and data-event services. Original scripts and HP/result ownership remain
unchanged. These services also replace the trio's duplicated narrow callbacks.

CheckAttribute8010e430/8c resolves/translates the actor, captures raw mask/output,
reloads global battle, fetches the unit, reads104 and writes any-bit overlap as
boolean. The mask is not resolved as a script variable. Its order matches the
original disassembly, including output capture before lookup.

ResetFaceDirection8010e4bc/70 captures battle BEFORE actor resolution. It fetches
that captured owner's unit, sign-extends belong byte+c, reads captured battle's
byte at8*belong+a, and stores exactly unit189. There is no part-direction write,
angle conversion or repair/clamp. The raw direction byte and neighboring188 are
preserved according to the source accesses. Full original display-facing binding
remains separately owned; this callback installs source storage behavior.

OnUnitFlag80105b40/60 and OffUnitFlag80105ae0/60 resolve/translate actor, capture
RAW mask1, reload current battle, fetch unit and request the actual word1c OR/
AND-complement update. The trio's former evtGetValue calls on these masks and the
attribute mask are removed; their variable-encoding-shaped literal bits must
remain raw. Current caller words are read rather than cached snapshots.

Tutorial RunDataEventChild8010a010 and CheckDataEvent80109f88 now bind their
existing complete source services. Data-table lookup uses actual unit18.
Absent/null data returns its source code; present scripts create real waiting
children with inherited locals/translated owner and original return255 semantics.
The callback first-call flag is not used as a completion shortcut.

## Evidence and limits

Tests cover raw-mask source domains, any-bit overlap, signed belong addressing,
pre-resolution versus post-translation battle capture, adjacent-byte preservation,
OR/off stores and the original VM reaction graph. Complete source fingerprints
for attribute/facing/flag wrappers join the three previous hit-metadata bodies.
Inspection-only source ELFs remain private; [assets](../../ASSETS.md) are unchanged.

The existing code100|18 reaction now finishes its actual source roots and returns
to source wait behavior without HP/result mutation. Tests no longer incorrectly
require that newly supported root to remain pending. The ordinary normal-element
code300|23 branch progresses through metadata and data-event requests, remaining
live at GetDamage8010f458 (signed amount270). This is a producer-service boundary,
not complete damage motion, an automatically selected hit or a playable attack.
Source damage amounts/patterns must come from actual damage application.

Full common damage child/status/animation/effect providers, original startup/
UnitEntry, source damage orchestration/turn scheduling, rotating menus and the
lower-screen audience remain required. No native input bypasses those owners.
