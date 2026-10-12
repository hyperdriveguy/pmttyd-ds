# Icon dimension lookup and texture fallback

US G8ME01 revision0, recovered2026-10-12. Evidence B: original DOL
iconGetWidthHight0x8001a19c,size0x100 and TEXGet0x80037b5c,size0x10.
The spelling is the external symbol annotation. This dependency supplies
[message width](FONT_MESSAGE_WIDTH.md) and [embedded icon records](MESSAGE_EMBEDDED_OBJECTS.md);
it does not by itself establish icon loading, drawing or animation behavior.
No original execution or atlas equivalence comparison was performed.

## Interface, readiness and binary directory (B)

Inputs are r3=width output halfword pointer,r4=height output halfword pointer,
r5=icon ID. Outputs are written without null checks; no semantic scalar return
is established. Runtime globals are icon_tpl0x8041e630,icon_bin0x8041e634,
icon_tpl_ok0x8041e638 and icon_bin_ok0x8041e63c. Names come from annotations;
addresses/load behavior are confirmed directly in instructions.

Both readiness words must be nonzero. If either is zero, the routine sets its
record pointer to null and proceeds to load halfwords at null+2/null+4. It does
not return zero dimensions or defer a request safely. These are unchecked
accesses; the actual processor/address-map result remains unexecuted.

On the ready path, let B=icon_bin. Compare low16(original ID) to0x49 and0x67.
Those IDs are remapped through unsigned halfword tables selected by configuration
word+0x16c, with no index check:

| Original ID | Table address | Six declared halfword entries |
| --- | --- | --- |
| 0x49 | 0x80415040 | 0x49,0x4a,0x4b,0x4c,0x49,0x4d |
| 0x67 | 0x8041504c | 0x67,0x68,0x69,0x6a,0x67,0x6b |

Other IDs retain the incoming word until directory index narrowing.
Read offset=u16(B+2*low16(selected ID)); record=B+offset. There is no count,
resource-length or directory-offset bounds validation. Read width from record+2
and height from record+4, and store them in that order. These are dimension
fields, not a multiplication of atlas cell size by a scale parameter.

## Both-zero fallback and original-ID distinction (B)

Reload the width output. Only when it is zero, reload the height output and test
zero. Thus fallback occurs only when both stored dimensions are zero; one zero
with the other nonzero is preserved. Output pointers are not checked for aliasing:
if both are the same pointer, the height store overwrites width before those
reloads, changing the fallback decision. Ordinary callers inspected here use
distinct local halfwords.

Fallback calls TEXGet(icon_tpl,low16(original ID)), **not the remapped ID**.
TEXGet reads a pointer from palette+8 and returns that pointer+8*index, without
checking palette count/index or modifying data. Dereference returned entry+0
to obtain the texture header. Store width=u16(header+2), then reload entry+0
and store height=u16(header+0). No second fallback, null-header handling or
rescaling appears. Aliasing output storage with source/palette storage is not
protected; the stated order matters for such nonordinary inputs.

The helper does not initialize resource pointers or readiness flags, set a GX
texture, consume RNG or transform dimensions with font scale. Binary directory
and texture-palette formats/ownership are separate contracts; the fallback's
simple entry indexing does not prove an entire palette loader equivalent.

## Evidence and remaining work

Whole-function fingerprints and independent anchors pin readiness, special-ID
routes, offset lookup, output order and both-zero fallback. Direct checks pin
both six-entry tables; BSS pointers/flags are runtime values and are not treated
as constants from ISO data sections. Original icon payload extraction is not
performed by this step, and no asset conversion/license change occurs.

Recover icon_bin/icon_tpl loading and relocation, readiness callers, valid
ID/configuration census, texture header contents and complete draw/animation
consumers. Compare both-zero/one-zero behavior, remapped binary lookups versus
original-ID fallback and distinct/aliased outputs against original execution.
Then validate the [width arithmetic](FONT_MESSAGE_WIDTH.md) and record geometry
with actual icons. Static contracts alone prove neither rendered dimensions nor
complete-game equivalence; no Switch behavior is asserted.
