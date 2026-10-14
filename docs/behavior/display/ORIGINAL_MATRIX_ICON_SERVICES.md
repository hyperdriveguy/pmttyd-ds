# Original matrix-icon services

US G8ME01 rev0, read-only ISO reconstruction. Whole local bodies:

| Source | Length | SHA256 |
| --- | --- | --- |
| iconDispGxCol8001a4bc | 224 hex | 76ff3611a63baa74f3f6f8ff9490ffe72637b5649b125c839d0767624b3233dd |
| iconDispGx2 8001a6e0 | 238 hex | 81d66a9b10d3353c8dd6a9f1ffda9a90cadb7674fc599299f27707a27c1c3446 |

Implemented by `resumeSourceIconMatrix`. These are the actual linked services
requested by [audience item/AP callbacks](RETAINED_AUDIENCE_ITEM_AP_CALLBACKS.md).
`BattleAudienceAuxServices::sourceIcon` connects those callbacks to this retained
service. An explicitly supplied direct icon provider keeps ownership instead;
an unavailable direct provider is never bypassed by the alternative provider.

Both functions first call current camera8000f2e4 and retain the returned source
camera address. Only then read global8041e638 (icon.bin) and, if nonzero,
8041e63c (icon.tpl). Either actual zero returns without metadata or geometry.
A missing memory/camera service is unavailable, distinct from a source zero.
These globals must describe actual loaded resources when native binding arrives.

Flags and requested icon ID narrow independently to U16. Col copies supplied
RGBA. Ordinary matrix icon copies original DATA8041f864=ffffffff. This constant
has SHA256 ad95131bc0b799c0b1af477fb14fcf26a6a9f76079e48bf090acb7e8367bfd0e.
Read descriptor directory pointer8041e634. Icons73/103 locally select a U16
alias at80415040/8041504c +2*GP.word16c, through fresh GP pointer80415fd0.
The stack icon ID remains the original requested low16 ID. The original six
aliases are73,74,75,76,73,77 and103,104,105,106,103,107 respectively. These are
source table contents, not invented bounds on the source GP value.

Descriptor address is directory+U16(directory+2*(selectedLow16ID+1)). Addition
uses original32-bit wrap. Capture descriptor U16+2 width,+4 height,+8 texture,
+a delay and+6 frame count. Count0/1 retains the first texture and never reads
GP.1c. Counts>1 sum every U16 duration at descriptor+a+4*i. Read fresh GP
pointer then signed word1c; use signed remainder by the total. Iterate frames
with strict remainder<duration; otherwise subtract duration. Only selected
texture changes: the temporary record's delay remains the first frame's delay.
The descriptor's loop field is never read by either function. Consequently the
existing MessageIcon::frame loop/clamp policy is not a substitute for this path.
Negative signed clocks select the first interval even when its delay is zero.

Read the captured camera's twelve view-matrix words at+11c after selection.
Call PSMTXConcat8029dd64(view,suppliedMatrix), then
iconGX800197f8(result,temporaryIcon). No camera-facing rotation, billboard or
projection occurs in these wrappers. Camera and request remain captured while
linked services hold. Concat arguments are retained across its wait. On an
iconGX wait, selection, camera lookup, matrix reads and concat are not repeated.
The containing audience callback exposes the actual pending linked function and
resumes the remaining record scan only after the wrapper completes.

`SourceMatrixIcon` represents only fields these wrappers initialize: flags,
RGBA, ID, width, height, texture and delay. Uninitialized source stack fields
are not filled with guessed values. The nested [original iconGX service](ORIGINAL_ICON_GX_DRAW_SERVICE.md) now
consumes the initialized fields its source body reads, retaining matrix/texture,
material/fog and primitive calls across waits. Native resources and [flat material programs](NATIVE_ICON_MATERIAL_PROGRAMS.md)
are now bound; enabled effects, scene primitive/replay and frame ownership remain
open. See also
[icon flags](ITEM_SELECTOR_ICON_ANCHORS_AND_FLAGS.md).
No request here acknowledges a quad or awards an audience item.

Unavailable mapped words/halves, absent matrix/draw services and invalid animated
descriptor totals retain a wait. Zero/signed-overflow duration totals are outside
the supported authentic descriptor domain; Gekko divw exceptions are not guessed.
Memory availability is an adapter precondition, not a general per-load PPC
continuation. Numeric execution uses the supplied linked matrix provider; the
native scalar/FMA adapter does not establish Gekko paired-single/FPSCR parity.

Tests exercise both source functions, actual zero-buffer returns versus missing
providers, low16 IDs/flags, both GP aliases, single/zero-frame records, strict
boundaries, signed negative clocks, zero-delay and ten-frame sums, matrix camera
capture, retained selection and linked waits. A coupled actual audience callback
runs through the full matrix-icon wrapper to an explicitly held iconGX provider;
source records and metadata remain fixture memory. [Native directory/resource/registered-GP and published-camera reads](NATIVE_ORIGINAL_ICON_RESOURCES.md)
are now bound. Full outer Main GP ownership and native iconGX emission remain
open. These tests do not establish visible
items, native frame cadence, two-screen framing or playable audience throws.

Private inspection artifacts: `artifacts/audience-icon-color.dis`,
`artifacts/audience-icon-matrix.dis` and their ELF containers. Neither original
PPC text nor resource bytes are committed.
