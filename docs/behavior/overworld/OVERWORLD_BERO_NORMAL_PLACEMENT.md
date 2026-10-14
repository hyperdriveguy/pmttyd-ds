# Bero normal entry placement

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes `bero_normal` in the [unaddressed-functions
register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

Fingerprint: entry 80079b74, `.elf` 976 bytes, 202-line `.disasm`, 11 `bl`
(5 distinct absolute targets + 6 same-TU). No `evidence.json`/`us-usage.json`
row. Callee identities: `marioGetPtr` 0x8005e0c0, `evtGetValue` 0x80031260
([integer operands](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md)),
`evtGetFloat` 0x80030840 ([float operands](../event/EVENT_FLOAT_OPERAND_ROUTING.md)),
`hitObjGetPos` 0x8001449c, `hitCheckFilter` 0x8001577c (hitdrv.o, 296 bytes,
positional), `sin` 0x80272410 / `cos` 0x80271ea8 (MSL double, `frsp`-rounded
per [axis rotation](../display/ANIMATION_SDK_AXIS_ROTATION.md)),
`bottomlessBero` 0x80096de4 and `bottomlessOn` 0x80096d34 ([bottomless
chain](OVERWORLD_BOTTOMLESS_EVENT_CHAIN.md)).

## Corrected complete producer contract

Use [initial-position publication and actor writes](BERO_INITIAL_POSITION_LOCAL_PUBLICATION_AND_ACTOR_WRITES.md)
for the complete body and caller pipeline. Fresh source review supersedes the
older six-input,mode!=1 and floating-residue interpretations in this document.
The helper consumes three signed XYZ words through callerLW6 and duplicates
them into two tuples. It uses callerLW5 direction, adds180 only for mode==1,
keeps the fixed30 start radius and uses30+distance for the final displacement.
Nonzero distance runs the original floor ray; zero-distance sentinel behavior
keeps distinct starting/final tuples. Explicit global stores publish both tuples,
then actual final XYZ are loaded for the recovery callback; they are not residual
trig registers. The callback stores finalY+37. Signed int conversion uses
2^52+2^31, not2^52 exactly.

The event caller separately READFs the starting tuple into encodedLW10..12,
sets Mario and both party slots from those quantized words, then resets camera
road state. No native cardinal bridge or static source table establishes the
full original world state/house05 bank. Exact SDK numerical equivalence, current
HitObj matrices/floor filters and original-runtime traces remain open.
