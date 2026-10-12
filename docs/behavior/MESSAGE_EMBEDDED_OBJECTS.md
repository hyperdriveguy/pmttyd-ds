# Embedded message icons, poses, paper and annotations

US G8ME01 revision 0, recovered 2026-10-12. B evidence is static original DOL
code/data and a bounded read-only US resource census. See [parser records](MESSAGE_PARSER_RECORDS.md)
for the shared 24-byte record layout and state names D/W/T/V/X/Y/H/A/B/F.
No original execution, rendering equivalence or Switch equivalence was measured.
Original text, pose and icon data remain reused assets, not committed extracts.

## Resource coverage versus reachable runtime input (B)

The 260-resource, 13,186-pair census found 394 exact `icon` tag lexemes, five
`anim`, and 280 `select`. Every select has four whitespace-separated fields;
278 have four integer lexemes, one has a final `%d` placeholder and one has an
initial `%d` placeholder. There are zero exact paper, /paper or r names and
zero R or /r prefixes in this US resource scan. This is bounded lexical evidence,
not a scanner execution or census of DOL/REL-generated text. Tags synthesized
by substitution or game code and resources in other languages remain possible.

The select call's five `%hd` format and four explicitly prepared pointers remain
literal binary facts. Four valid numeric fields exhaust static argument text
before a fifth conversion can consume input; this explains the ordinary resource
shape without proving the library/ABI failure behavior. Placeholder resources
need their substitution callers traced before runtime argument values or
conversion counts are asserted. See [resource metadata tooling](MESSAGE_RESOURCES_AND_LOOKUP.md).

## Icon name resolver and original US IDs (B)

msgIconStr2ID at 0x80081a1c, size 0x84, scans 18 eight-byte (name pointer, ID)
entries at 0x8030f4b0 in order. Exact strcmp match returns ID, otherwise -1.
No null guard or case folding. msgAnalize inlines this scan for named icons.
The US table differs from the pinned Japanese source; reuse the US IDs below.

| Name | US ID | Name | US ID |
| --- | --- | --- | --- |
| PAD_A | 0x6c | PAD_B | 0x6e |
| STICK_LEFT | 0x80 | STICK | 0x81 |
| AC_ON | 0x9d | BUTTON_L | 0x86 |
| BUTTON_R | 0x88 | HM | 0x1a7 |
| PAD_X | 0x70 | PAD_X_ON | 0x71 |
| PAD_Y | 0x72 | PAD_Y_ON | 0x73 |
| PAD_Z_OFF | 0x8a | PAD_ST_OFF | 0x8c |
| black_key | 0x138 | ANM_PAD_A | 0x20e |
| ANM_STICK_RIGHT | 0x20f | ANM_PAD_START | 0x210 |

## Icon argument routing and record geometry (B)

At 0x80082ac4, inspect the first argument byte with signed classification.
ASCII '0'..'9' chooses sscanf `%d%f%d%d%d`; otherwise choose `%s%f%d%d%d`.
Established outputs are ID/name, scale Q, horizontal offset J, auxiliary integer
Z and horizontal advance N. A leading minus or plus therefore takes the name
path, not the numeric path. Conversion counts are unchecked; `%s` is unbounded
into a 32-byte stack region. Missing/invalid arguments are not safe zero defaults.

Named arguments use the resolver above; unknown names are changed from -1 to
ID 0. Exact name HM also selects a special glyph path after the name lookup.
Other named/numeric inputs append a record:

- +0=full parsed/resolved ID, +4=opcode 0xfff8.
- Pass low 16 bits of ID to iconGetWidthHight(0x8001a19c), obtaining unsigned
  halfword width w and height h. Its atlas/range/failure contract remains open.
- +6=halfword(trunc(float(X)+J+0.5*w*Q)). The binary performs `0.5*w` first,
  then a single-precision fused multiply/add with Q and X, then adds J.
- +8=halfword(trunc(float(Y)-h*Q)), with a single-precision fused operation.
- +a=low halfword of Z, +c=T, +10=Q. +14 is untouched.
- Increment count, T+=V, X=halfword(X+N+40). Input remains at the parsed end
  pointer and normal post-command LF suppression applies.

This path does not update maximum line scale L or inherit F into payload +0.
Neither actual icon width nor Q determines horizontal advance here; N+40 does.
Do not replace it with a generic proportional inline-image rule.

For exact HM, language 0 uses kanjiSearch(0x94b5); other languages call
hankakuSearch(-48), a literal signed argument distinct from ordinary US byte
lookup. Use kanjiGetWidth(low16(glyph)) and advance
`step=low16(trunc(A*(width+4)))`. Emit an ordinary glyph: +0=F|8, +4=glyph,
+6=X, +8=halfword(Y-trunc(10*B)), +c=T, +10=A; +14 gets dynamic style only
if F bit 1 is set. Update L from B, increment count/T, then X=halfword(X+step).
Parsed Q/J/Z/N are not applied to this path. It emits a glyph, not 0xfff8.

## Animation record and pose side effects (B)

Exact anim at 0x80082de0 parses `%s%s%d%d%f%d`: model name, animation name,
horizontal coordinate J, vertical displacement K, scale Q and orientation O.
The two `%s` destinations are each 32-byte stack areas without format width;
conversion counts and pose creation failure are unchecked locally.

Call animPoseEntry(model,2) and write returned ID to record +0. Call
animPoseSetAnim(newID,animation,1); set its material flag bits 0x1800 and clear
0x40. The subsequent animPoseSetGXFunc call uses the global message-work pose ID
at +0x20, not the newly created record ID: callback msgDispKeyWait_render at
0x80083360, argument 0. This distinction is explicit register flow and must not
be silently changed based on inferred intention.

Emit opcode 0xfff4: +6=low16(J), +8=halfword(H-K), +10=Q, +14=0.0 when O=0
or 180.0 otherwise. Increment count and T by V. +c is not assigned here,
and X/Y/L are not advanced by this record. Pose cleanup, animation advancement,
draw transforms and resource lifetime belong to the consuming runtime.

## Paper/NPC control records (B)

paper at 0x80082ed0 parses `%s%d` into a 32-byte name area and integer P.
Exact name me yields pointer 0; any other name calls npcNameToPtr(name) at
0x800499e0. No local missing-NPC rejection. Emit +0=pointer, +4=0xfff3,
+c=P; increment count only. Neither T nor coordinates advance.

/paper at 0x80082f4c parses `%s`, performs the same me/null versus NPC lookup,
emits +0=pointer and +4=0xfff2 and increments count only. It leaves +c alone.
Both take common post-command LF suppression. Meaning of null/me at consumption,
paper poses, restoration order and invalid pointer handling remain open.

## Annotation anchor, entry gate and literal closing behavior (B)

Exact lowercase r at 0x80082fa8 does not append a record. If W.type is not 1,
save X in local annotation anchor U; type 1 ignores it. U has no entry-time
initialization in this routine, so annotations without a preceding accepted r
depend on an uninitialized register value, not an established default anchor.
This r path uses common post-command LF suppression.

After known command comparisons fail, load only the first command-name byte
and sign-extend it. If byte is uppercase R, enter annotation processing with
prefix length 1. Otherwise accept only byte '/', then compare that same
sign-extended byte to 0x2f72. No byte value can equal 0x2f72. This path does
not load a two-byte `/r` code unit and cannot accept a conventional closing tag.
Retain the literal US behavior rather than correcting it from likely intent.

For accepted R-prefix names, W.type 1 skips annotation and returns straight
to the scanner without normal post-command LF suppression. Other window types
interpret the remaining command-name bytes (not the argument buffer) as
two-byte big-endian units. n=trunc(strlen(name+1)/2); an odd final byte is
ignored. There is no language gate or trail validation here.

## Annotation widths, centering and emitted glyphs (B)

For each of n units, use kanjiSearch then kanjiGetWidth(low16(glyph)). Sum
`trunc(0.55*(B*(width+4)))` with original word arithmetic, then retain low 16
bits as unsigned total Rw. Constant 0.55 is binary32 0x3f0ccccd.

Centering uses several separate truncation/narrowing steps:

1. Compute float P=4*A; compute `C=trunc(float(Rw)-0.55*P)` using the binary's
   fused single-precision operation. Retain low16(C) as unsigned C16.
2. Set U=trunc(0.5*((float(s16(anchor))+float(s16(X))-P)-float(C16))).
   The binary adds the two signed coordinates as integer word before float
   conversion. The stored U is subsequently narrowed as signed halfword.
3. Use vertical coordinate s16(Y) unchanged for each annotation glyph.

For each unit append +0=F, +4=glyph, +6=low16(U), +8=Y,
+10=0.55*A, +c=0. If F bit 1, also write dynamic float +14; otherwise leave it.
Advance U as signed halfword by `trunc(0.55*(A*(width+4)))` and increment count.
No extra flag 8 is forced, no T/X/Y/L update occurs, and width measurement used
B while glyph advance uses A. Return directly to the scanner, without common
post-command LF suppression. These differences matter after scaleX and on
negative/wrapped coordinates. No Western ruby-text grammar is inferred.

## Remaining consumer and full-game obligations (P)

The local branches of msgAnalize are now documented across these two parser
files, including unusual literal behavior. This is not a full dialogue runtime:
sscanf/library numeric behavior, font mapping/metrics, icon atlas geometry,
pose/NPC lifetimes, display callbacks, msgMain record playback, user input,
selection navigation and story scripts still need recovery and execution checks.

Derived cases include named/numeric/unknown/HM icons, leading sign routing,
separate advance versus image geometry, scaleX annotations, odd annotation
bytes, no accepted anchor, type 1 LF behavior, unreachable closing comparison,
global versus new pose callback ID, orientation zero/nonzero, and untouched
paper/animation schedule fields. Resource absence does not remove these code
paths from complete-game requirements; callers outside US text remain open.
