# Original retained number-icon drawing

`resumeSourceIconNumber` recovers the complete US revision0 local
`iconNumberDispGx80019120/284`. Inputs are a borrowed affine3x4 matrix,
signed32 number, unsigned mode, and borrowed RGBAword. The result is the
original accumulated integer width. This is not a text-formatting adapter.

## Original body

At entry, mode==0 selects spacing at8041f880 (20) and the halfword numeral
table80415084. Any nonzero mode selects spacing8041f884 (16) and table80415098.
The source reads spacing once, before decomposing the number. Its signed
multiply-high division sequences implement truncating division by100000,
10000,1000,100,10. Five stack words retain the low five signed decimal digits,
least-significant first. Nonzero quotient by100000 selects five iterations;
otherwise the highest nonzero retained digit selects the iteration count,
with zero still selecting one.

There is no absolute-value operation, sign test, minus icon, or leading sixth
digit. For example100000 draws five zero icons, and100010 draws `0,1,0,0,0`
in source call order. Negative remainders index before the selected numeral
table using wrapped32-bit address arithmetic; those are actual source halfword
reads, not positive digits or invented minus signs. Whether a resulting ID can
be rendered depends on the actual subsequent source directory/resource data.

The body captures the signed-conversion double bias802bf848/8 once before
the loop. For each index starting at0:

1. Read zero8041f87c once and pass it as both Y and Z. Convert negative loop
   index through the source biased-double subtraction/single store and multiply
   by retained spacing. Call PSMTXTrans8029e230 with these XYZ operands.
2. Call PSMTXConcat8029dd64 with the current borrowed input matrix and the
   produced translation. Capture the input only on entering that SDK call.
3. Read the current borrowed color word after the completed Concat. Copy it
   into the source private color local, then read the selected numeral halfword.
4. Invoke complete iconDispGxCol8001a4bc with flags16, that actual numeral ID,
   the produced matrix, and copied color. The implementation delegates to
   retained `resumeSourceIconMatrix`, which delegates to retained iconGX when
   no alternate retained whole-draw provider is supplied.
5. Convert the current integer width through the same captured bias, add
   spacing as a single operation, then execute the truncating integer update.
   Only after this does the next digit begin.

The final source integer width is exposed only after the entire call completes.
The native conversion domain requires a finite, signed32-representable result;
nonrepresentable fctiwz/FPSCR domains remain unavailable. This does not claim
Gekko/SDK paired-single, libm, rounding-mode, or GPU bit/pixel equivalence.

## Retention and binding

Base/color object addresses, scalar arguments, provider tables, and nested GX
service owner/table must remain stable through a held call. The borrowed objects
must outlive it. Providers must not recursively resume/reset this same cursor.
Borrowed values may change before the corresponding source capture
sites. Each acknowledged outer matrix operation and completed icon child survives
later waits. A held table read cannot recapture the preceding color. Output width
is untouched until whole completion. `reset` releases native metadata and does
not write source memory.

The existing SourceIconMatrix/GX resource readers remain their declared pure,
immutable resource providers. A false SDK or alternate whole-draw provider must
own any partially acknowledged effects and retain the same request until success;
it must not restart a partial GPU operation. The default retained GX child owns
its actual setter/quad prefix. Missing words, halfwords, camera, resources, math,
or draw admission hold honestly. No success-only draw provider is installed by
this implementation. Actual zero loaded-buffer pointers retain the original
iconDispGxCol early return.

The HP parent's retained `CameraViewMatrix` can be borrowed directly. Its number
callback receives RGBA by value, so the integration owner must store that copied
source call value in stable owned storage before passing it by reference across
waits. Do not retain a reference to the callback's temporary argument or construct
a temporary flattened matrix on each resume.

## Existing actual resources and integration boundary

The existing `harbor-tutorial-events.pmevt` contains all three required unchanged
original ranges: numeral tables80415084/40, signed bias802bf848/8, and
scalar constants8041f87c/12. They were compared directly against the read-only
ISO. No new import, changed pixels, or altered asset licensing is needed for the
actual tutorial VM path. Its actual word/half readers provide those constants;
compose the original icon resource owner for its native directory/TPL domains.

Standalone `SourceIconResources` has a bounded188-byte constant product whose
80415080/32 span omits the last12 bytes of the second numeral table and the
signed bias. That standalone provider alone is insufficient; do not invent table
entries or bias, or report successful completion when they are unavailable.
Normal product scheduling, stable HP number owner, actual source camera binding,
original GPU resource residency, and final GPU draw remain caller responsibilities.

Focused tests use the actual existing tutorial pack, original icon.bin/TPL,
explicit host camera/math and packet sinks. They cover both modes plus arbitrary
nonzero modes, decimal boundaries, five-digit overflow behavior, signed table
addressing, width, colors/transforms, late borrowed-value captures, all outer
SDK/draw waits, complete retained iconGX setter/quad waits, identity replacement,
and unavailable providers. Host sinks are not native GPU proof.

Source bodies/constants are pinned in ignored
`artifacts/icon-number-source-evidence.json`; the independent existing-resource
comparison is `artifacts/icon-number-existing-resource-proof.json`. No original
binary is committed. Related:
[queued enemy HP](QUEUED_ENEMY_HP_PRESENTATION_AND_RETAINED_BAR.md),
[HP source parent](../battle/ENEMY_HP_DRAW_AND_NATIVE_TATTLE_CHECK.md).
