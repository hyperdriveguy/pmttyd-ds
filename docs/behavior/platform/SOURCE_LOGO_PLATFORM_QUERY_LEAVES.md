# Logo platform query leaves

Fresh read-only US G8ME01 DOL evidence. These three complete local bodies are recovered; their native hardware/platform ownership is not supplied by recovery. Larger VI/SRAM source dependencies remain in the separate platform request. No native MMIO/input/interrupt implementation is implied.

## OSGetResetCode8029b544/0x38

No arguments are read. Atb54c read word80412d10; compare exact0. If nonzero, independently read word80412d14 atb558, return that raw word OR80000000. If flag0, read original volatile processor-interface MMIO wordcc003024 atb56c, mask low3 bits withfffffff8 then logical-right-shift3 and return. There is no OS interrupt call, retry, wait, null check or local write. Do not snapshot flag/value together: second word is reached only after the flag branch. A repeated parent call performs fresh reads. Word80412d14 is not read on hardware path; MMIO is not read on saved-reset path.

An admitted portable service can represent reads as optional raw32 values and retain the chosen branch/value when a native read is unavailable. Missing MMIO/reset state is a held provider, not reset code0. Original body itself has no such optional-return status. Native newgame/reset policy must be explicitly named and owned; original cold zero bytes alone do not establish current real hardware reset history. The fresh original reset-global bytes/provenance are recorded separately; do not cold-import them into a live SDK OS owner merely to force the saved path.

## keyGetButton80005fd0/0x14

Incoming r3 is channel rawword. Read current GP pointer at80415fd0. Compute offset=(r3<<2) modulo32 and address=(GP+offset+1328) modulo32. Read rawword there and return. The used Logo caller passes0, reading GP+1328. No range clamp, channel validation, physical resampling, trigger conversion, masking, initialization or consumption occurs. GP+1328 is the held-button source; keyGetButtonTrg's separate trigger slot must not replace it. The pointer read and actual button word read are distinct reached reads and must retain chronology across a native hold. Mapping known GP without the genuine current input producer does not establish valid button0. Existing port physical input may bind a deliberate same-process adapter; never synthesize B/zero from a message confirmation to steer the original progressive-mode branch.

## U_VIGetDTVStatus802a5088/0x3c

No incoming argument is read. Call OSDisableInterrupts80299ad4 at802a5098 and retain returned r3 token. Read hardware unsignedhalfcc00206e at50a0, retain low2 bits into nonvolatile r31. Call OSRestoreInterrupts80299afc at50a8 using the retained disable token (r3 unchanged by the MMIO/bit extraction). Return retained r31&1. Thus it captures MMIO status BEFORE restore and does not reread it afterward. Result is exactly0/1; the read includes two bits but only lowbit escapes. No local polling loop or VIFlush/retrace request.

If a native interrupt-disable child completes and the MMIO read or restore becomes unavailable, retain the original token and reached status, and keep scope ownership explicit. Retrying must not repeat disable, substitute token0, or reread status after successful capture. A genuine original lock/hardware lifecycle or explicitly adapted 3DS platform provider must own that scope; a no-op receipt cannot prove it. No source VI register may be invented from TPL presence or C3D_FrameBegin.

The complete direct child bodies OSDisableInterrupts/14 and OSRestoreInterrupts/24 remain with the platform interrupt contract/frontier; source identities are known. Their MSR semantics and the admitted native synchronization policy are not implemented by this leaf description. Hardware MMIOcc00206e andcc003024 are live hardware inputs, not initialized ISO assets or RAM spans to import.

## Source fingerprints

| Body | Address/size | SHA-256 |
| --- | --- | --- |
| OSGetResetCode | 0x8029b544/38 | `0879e3328694ef68f60cbf382d3d709f2f381cb641d5342aedc2067e3651b37e` |
| keyGetButton | 0x80005fd0/14 | `5287c1aecc6288e7ced4896b25abe8d79d1c504600cec2edd20a640bc5cf6a9c` |
| U_VIGetDTVStatus | 0x802a5088/3c | `aea8f58cb3e3d7f2a9f8d41d454edfefe12cf676117fc312e68409f984a41bc2` |

Exact source offsets are in `platform-leaves-evidence.json`; corrected complete listings are in `../fade-startup/`. The standalone `platform-request.md` should now describe these local leaf bodies as recovered and retain their genuine hardware/interrupt/native binding frontier. Full progressive SRAM, VIConfigure/Flush/retrace bodies still need transitive source contracts.


See [Logo startup](../display/SOURCE_LOGO_STARTUP.md) and
[the remaining platform request](../../requests/EVT-01.md).
