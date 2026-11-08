# Original Logo startup and file lifetime

Whole original US revision0 seq_logoInit800874b0/0x18c and
seq_logoExit8008746c/0x44 are implemented by distinct retained cursors. Body
SHA256 values are respectively
f48865f05382fae7aba7ffe8d636f0d27ffefee61b7232a76c06317989903860 and
54b7691885ff90c84e987575559e093c599c4c125b11f62e9cc6358bb2da3500.

## Initialization

Init reads wp80416108, clears32 bytes at its current work pointer, refreshes wp
and stores work+4=-1. It obtains actual DVD root, calls whole
fileAlloc8018a61c(type4,format802c3b68,root), refreshes wp and stores its actual
return into work+0. The exact format is `%s/mariost.tpl`. It then executes all
original reset/DTV/progressive/button/VI branches. Repeated OSGetResetCode calls
are independent live children. Progressive mode selection can require a second
retrace. The reset path separately refreshes GP for flag8 and flag4 publication;
the ordinary video path returns without those writes. Every reached accepted
child/read/store is retained across native holds. Distinct parent cursor identity
prevents Init completion from acknowledging Exit.

## Exit

Exit invokes whole fontmgrTexSetup, windowTexSetup, fadeTexSetup, iconTexSetup,
envTexSetup, effTexSetup (including N64) and marioPoseInit in source order. Only
then does it freshly read wp, its actual file handle and fileFree8018a260(handle).
A held Mario Entry cannot release the Logo file early. Texture job admission
and independent DVD completions retain their respective original meanings;
this parent itself inserts no synthetic source readiness/frame wait.

## Native ownership and current frontier

`NativeLogoStartup` cold-imports five exact original ranges: initialized wp
80416108/4, BSS work8041ea80/20, padded format802c3b68/10, and original60-byte
VI mode inputs8030c230/3c and8030c26c/3c. It borrows the actual UI/animation VM,
root/GP/SDK and file cache. Existing mappings reject new admission. Equal-byte
backing replacement or a replaced work owner invalidates use. Native fileAlloc
publishes the real shared filename before the existing cache acquisition. A
held acquisition never repeats completed formatting; a later platform hold
retains the actual handle and reference. Exit releases only that acquired
nonzero handle through whole fileFree. Teardown supplies no implicit source
release/cancellation receipt.

The original raw mariost.tpl is now staged in the same immutable prologue cache
catalog before its first bind/acquisition. This adds2128288 original bytes plus
the cache's existing padded payload storage policy; there is no per-frame load.
Its26 descriptors are relocated by actual source unpack. Normal asset recipes
compile Logo memory and copy raw TPL; imports refuse nonempty destinations.
Per-span/source/asset offsets, hashes and original retained rights are tracked
under assets/original/display/logo-startup/. No redistribution license is granted.

A retained `NativeLogoChildren` provider must acknowledge every reached whole
platform or Exit child. With no provider, real Init work/file acquisition runs
then holds at OSGetResetCode; default reset/video/Mario success is never supplied.
A single provider/lifetime is retained across holds. Successful external children also require post-call currentness,
including the terminal retrace. Child providers retain their accepted prefix
across that admission hold. The borrowed DVD-root backing has an independent
identity check and is never reset or remapped. Native fixtures use explicit
TEST-only platform/Exit receipts to verify actual cache reference ownership and
release barriers; they do not establish original VI/GPU/field/Mario completion.
The source fixtures cover both reset families, DTV/progressive/button branches,
retrace multiplicity, live GP/work changes and held acquisition/Exit order.

Normal Game does not yet bind this Logo owner or execute whole Logo Init/Main/
Exit. Whole raw MarioInit, source sequence activation, actual platform provider
and texture/Mario child composition remain required. See
[raw Mario pose startup](../field/SOURCE_MARIO_POSE_STARTUP.md),
[platform query leaves](../platform/SOURCE_LOGO_PLATFORM_QUERY_LEAVES.md),
[the platform request](../../requests/EVT-01.md) and
[Crump integration requirements](../story/CRUMP_OPENING_INTEGRATION_REQUIREMENTS.md).
Fresh source/review evidence lives under artifacts/mario-logo-startup/ and
artifacts/fade-startup/. This adds no original pixel or physical performance claim.
