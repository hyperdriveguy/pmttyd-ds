# Game listener setter and scoped sequence camera bindings

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete psndSetPosDirListener800dc380/28
and camGetPtr8000f2fc/14hex. Scoped sequence regions ONLY:
seq_gameMain800074ec..80007528, seq_battleMain80073de4..80073e38 exclusive.
Read [calc3D](GAME_CALC3D_SPATIAL_RESULT_AND_ANGLE_TABLE.md).
Offsets/addresses below hexadecimal; coordinate constants explicitly decimal.
Static publication/source order, not original frame/audio observation.

## Complete setter and alias-sensitive publication

Input position-word pointer P in r3; heading in f1. psnd803df6e8 is the target.
Load P.word0 (X), then P.word4 (Y); store X to psnd+30, Y to psnd+34.
ONLY AFTER these stores load P.word8 (Z), store to psnd+38. Finally stfs f1
at psnd+3c. No null, finite, alignment, bounds, angle conversion/normalization,
IRQ, camera query, spatial recalculation or audible backend request.
Return r3 remains the input pointer, without an independent success assignment.

The first two words are captured together but Z is a later read. If input aliases
psnd+2c, that Z read addresses newly written psnd+34; it sees input Y rather
than the earlier contents. Do not replace with a whole-vector snapshot under
an unsupported alias guarantee. This is a derived alias invocation; the scoped
original callers below pass separate stack arrays.

Position words preserve raw bits, including nonfinite float encodings. Heading
uses stfs; caller/math floating register semantics remain prerequisites. Neither
an atomic four-field snapshot nor coherent publication against concurrent
calc3D calls is established by this body.

## Complete camera pointer lookup prerequisite

camGetPtr(rawID) computes wrap32(rawID<<2), adds it to camPtrTbl803c95f8,
loads/returns that word. No mask, validity/null guard, fallback or camera update.
The supplied table occupies34hex bytes (13 pointer slots); it is BSS and its
runtime pointers are not supplied by immutable DOL initialization bytes.
Input4 reads table.word10 at803c9608. Camera pointer allocation/table writers,
valid extent/null invariants and full camera state remain open.

## Scoped field sequence arguments

At800074ec request marioGetPtr8005e0c0; returned pointer is not used by this
argument region. Request camGetPtr(4), capture camera words+c/+10 to stack+14/+18,
then read camera.word14 and store stack+1c. These three position reads use the
FIRST camera pointer; they are not an atomic camera snapshot.
Request camGetPtr(4) AGAIN. Load its float+114 as heading; invoke
psndSetPosDirListener(&stack14,heading) at80007524. No cross-lookup pointer equality
check or heading normalization. Source camera base may change between queries.
This closes the concrete local position/heading arguments, not all field camera
update/source conditions or seq_gameMain's preceding event/sequence gates.

## Scoped battle sequence arguments

At80073de4 request marioGetPtr, then camGetPtr(4); both returned pointers are
unused by this region's position construction. Load original literal0,400 decimal;
store stackXYZ=(0,0,400). Load global battle pointer at8041eb90, then its wordef4;
test bit100. If set, use fadds(400,200), store Z600. Otherwise leave Z400.
No battle pointer/null/float/position check or copied battle-unit coordinate.

Then request camGetPtr(4) again; read fresh float+114 as heading. Invoke
psndSetPosDirListener(&stack10,heading) at80073e34. The first camera result does
not supply XYZ or heading; do not infer that its earlier getter return is reused.
The bit100 writer/meaning and surrounding sequence state/admission remain open.
Only this original request region is closed, not the whole battle sequence body.

## Direct caller census, evidence and remaining work

Whole-DOL aligned direct-call scan finds exactly TWO setter calls:80007524 in
seq_gameMain and80073e34 in seq_battleMain. This census does not cover indirect,
REL or dynamic routes and does not prove those are the only shared-state writers.
No unsupported per-render-frame or physical-input/audio sample cadence is inferred.

[Manifest](../../../tools/game_listener_source_evidence.json) pins two complete
bodies, two scoped argument regions, linked marioGetPtr identity and original
12-byte battle literals:6 spans/46 anchors/8 resolved direct calls. Original
constants independently decode0/400/200; the alias example is static deduction,
not original execution. Full fresh ELFs/listings remain `/tmp/inbox-listener*.elf`
and game/battle listener-site files. No original body/asset exported to history.

Remaining: camera pointer/position/heading producers and finite/domain invariants,
whole sequence state gates/cadence, battle flag100 and other listener/shared-result
writers, indirect/REL/dynamic paths, original spatial/audio observations. Two local
bodies and scoped direct argument bindings are answered; full runtime remains OPEN.
