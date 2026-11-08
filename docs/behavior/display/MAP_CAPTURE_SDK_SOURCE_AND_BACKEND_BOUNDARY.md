# Original map capture SDK source and backend boundary

Read-only US G8ME01 revision0 DOL range reads and local LLVM PowerPC disassembly,
2026-11-08. This is static source evidence, not original runtime/GX hardware or
REA execution evidence. Callable boundaries were checked through closed direct
control flow to returns, all advertised reachable positions and adjacent entries.
No source bytes are fetched or decoded by the runtime. Fingerprints and initialized
data are pinned in `tools/map_capture_sdk_evidence.json`;
`tools/verify_map_capture_sdk.py` rechecks them against the read-only ISO.
See [the current map process](../field/NATIVE_SHARED_MAP_PROCESS.md) and
[portable material continuation](RETAINED_MAP_MATERIAL_SOURCE.md).

## Exact top-level complete boundaries

| Function | Address/size hex | SHA256 |
|---|---|---|
| GXBeginDisplayList |802b9e10/cc|5cc6292d6bb839fa8aa7c7b46984964ba0dc5498b3f60f7f1aeafca72aa3c4fa|
| GXEndDisplayList |802b9edc/c4|2bd7ebff96013a36445c2a07749c1271bcc118a42bc8ebb8a912bdb9cee4fb57|
| GXResetWriteGatherPipe |802b51b0/34|f6522d694a0205610c66c253bfb4b467489a5b65e03237fa16dd521433ee68e0|
| GXSetCullMode |802b5c8c/28|4c3e85c18378d7fc19693a84ff010cb1d921456d479ebcb5a60fd4b70796d1ad|

These four records were all byte/word/hash matched directly to the supplied
read-only ISO, not inferred from nearby prologues.

## Shared state and original identity

r2=80427440, r13=8041cf20. Initialized pointer804293d8 (r2+1f98) is
80413e40, the mutable **GXData** owner, length at least5b0 for this graph.
No GXData payload exists in initialized ISO sections. The display-list FIFO
record **D=80414678** and backup **D+24=8041469c**, length5b0, are BSS too.
Do not asset-import an invented zero GXData/FIFO snapshot.

| Actual mutable global | Purpose observed in these bodies |
|---|---|
|8041f390=r13+2470|PI MMIO pointer, GXInit stores cc003000|
|8041f394=r13+2474|CP MMIO pointer, GXInit stores cc000000|
|8041f3b8=r13+2498|current CPU FIFO descriptor pointer|
|8041f3bc=r13+249c|current GPU FIFO descriptor pointer|
|8041f3c4=r13+24a4|CPU/GPU linked byte|
|8041f3f0=r13+24d0|saved previous CPU FIFO descriptor captured by Begin|

GXInit802b2424/600 is already pinned by
`tools/verify_source_gx_inherited_state.py` (hash
e51f69665a91b2feab3bb5d0459653796abd7007d5e7445d472d8306046999cd).
At2458 it sets GXData+5a8 capture byte0; at2464/2468 sets save-context+5a9
and vertex-cache-related+5aa bytes1. It initializes MMIO globals at2480..2494,
creates/sets CPU/GPU FIFO via3490/3578/36a0, programs WPAR at24ec and enables
write gather through HID2 at24f4. Many later registers/texture callbacks and
__GXInitGX802b2a24/938 participate. The existing **semantic inherited history**
explicitly does not execute this parent or initialize actual raw GXData/FIFO.
A cold BSS clear alone cannot stand in for these source writes. Native capture
must own/journal actual SDK state or expose a clearly bounded adapted platform
bootstrap; never label a guessed raw snapshot actual executed GXInit.

Source tables used by dirty vertex-size helper41b8 are initialized original
bytes at r13+15f0=8041e510 (`00040102`),+15f4=8041e514 (`00080102`),
+15f8=8041e518 (`000c0102`). `tools/map_capture_sdk_evidence.json` pins their original spans and
pointer hash. No new asset import was made.

## Whole Begin protocol (void return)

Inputs r3=buffer B,r4=size L are captured. D is fixed80414678.

1. GXGetCPUFifo802b3be0 returns current CPU pointer P; retain P.
2. Fresh GXData+5ac dirty word: nonzero calls complete __GXSetDirtyState802b59ec.
3. Fresh GXData+5a9 save-context byte: nonzero copies all5b0 bytes from current
   GXData into D+24 through actual memcpy8000519c.
4. Store D+0=B,D+4=B+L-4,D+8=L,D+1c=0,D+14=B,D+18=B. Original does not locally
   clear D+c/+10/+20 or validate size/alignment/overflow/nested capture.
5. Fresh GXData pointer, store byte+5a8=1.
6. GXSaveCPUFifo802b3818(P): GXFlush of the OLD FIFO first, then save hardware
   PI/CP fields into that actual P descriptor.
7. Store retained P in8041f3f0. GXSetCPUFifo802b3578(D) switches source PI/CP
   state and current pointer. Call GXResetWriteGatherPipe802b51b0.
8. Return with original void ABI; do not invent Boolean source success.

The map parent then independently calls Reset a second time at8002a364. Both
are actual source effects. A retained native service may hold before either
reached boundary but must not collapse/replay prior copies/switches.

## Whole End protocol and genuine returned count

1. GXFlush802b5154 while capture FIFO still current.
2. Read actual PI pointer global; capture bit26 of PI word+14 as overflow.
   This happens BEFORE saving descriptor/returning the old FIFO.
3. __GXSaveCPUFifo802b384c(D), then GXSetCPUFifo(WORD8041f3f0).
4. Fresh GXData pointer/save byte+5a9. If nonzero: OSDisableInterrupts;
   capture current GXData+8; memcpy current GXData from D+24,5b0 bytes;
   reread GXData pointer; restore captured word+8; OSRestoreInterrupts.
   This deliberately preserves the changed CP control word across context
   restoration. Do not restore every field blindly or preserve dirty+5ac
   instead of the specified word+8.
5. Fresh GXData pointer, byte+5a8=0. Return D+1c when captured overflow0,
   else0. There is no local extra alignment/size-failure protocol.

D layout used here: base+0,end+4,size+8,watermarks+c/+10,read pointer+14,
write pointer+18,read/write distance+1c. For nonlinked capture save, PI+0c/+10
supply base/end, PI+14 supplies write pointer after bit26 cleared; all receive
cached-region conversion by add80000000. Count=write-read; if signed negative,
add D.size. Linked branch instead reads CP halfword register pairs for read and
distance. These are **hardware-owned observations** in the original, not a
number supplied by mapSetMaterial. A native FIFO adapter must update equivalent
state from actual accepted serialized writes and explicit overrun/wrap policy;
claim source-hardware equality only with the corresponding validated semantics.

## FIFO packing, dirty state and write gather

GXResetWriteGatherPipe loops PPCMfwpar802950a8 until lowbit0, then
PPCMtwpar802950b4(0c008000). PPCMfwpar itself executes sync before reading
SPR921; PPCMtwpar writes SPR921. A CPU barrier alone is not this operation's
buffer-reset/ownership effect. Native adapter can replace platform mechanics
but must model pending write-gather ownership or retain the boundary.

GXFlush checks fresh GXData+5ac; if nonzero executes __GXSetDirtyState. It
then performs **eight actual32-bit zero writes** to FIFO MMIO cc008000,
then PPCSync80295004. Width/order/endian of source writes matter. Original
write-gather/PI updates determine the returned count. This static listing does
not prove physical32-byte gather burst/overflow/padding behavior; do not derive
or bake a length formula or high-level request count from these instructions.
Exact hardware gather behavior is a remaining original/platform evidence gate.

GXSetCullMode does NOT write a FIFO command: permutes input cull bits, inserts
into GXData+204 bits14..15, ORs dirty bit4 into GXData+5ac. End's GXFlush
causes __GXSetGenMode802b5d04 to emit **U8 61 then BE U32 GXData+204**, then
GXData half+2=0. This deferred emission is decisive evidence against summing
one immediate BP packet per high-level call or setting an arbitrary list size.

__GXSetDirtyState captures the dirty word ONCE and dispatches bits in order:
1→7ec8,2→887c (complete single blr!),4→5d04,8→40fc,10hex→4a34;
then if either8/10hex→41b8; finally fresh GXData pointer dirty word=0.
7ec8 can call7e28 (source texture-coordinate scale/SU state);40fc emits CP/XF
vertex-description packets;4a34 emits selected CP VAT packets and clears
GXData+5ab;41b8 recalculates the source vertex byte-size halfword+6 without
FIFO writes. Exact listings/calls/hashes are in the fingerprint manifest and original disassembly. Parent material
setters and shared previous GX state determine which dirty bits appear. No
fixture-wide dirty0 assumption is valid.

Each source stB/stH/stW/stFS to cc008000 must append its actual width's ordered
bytes to the selected capture transport, not write an EventAddressSpace scalar
cell at that address. CP packets are byte08,byte register,U32 value; BP packets
are byte61,U32 packed register; XF packets are byte10,U32 count/address header,
then U32/rawF32 words, according to the reached SDK bodies. Native GPU commands
are a separate translation/storage domain. Native IR size cannot replace the
original source return count used by map memcpy, allocation and object slots.

## Complete local dependency boundaries and hashes

Names for address-only helpers are descriptive labels, not recovered symbols.
All sizes hex; direct callees were checked in the original disassembly. These bodies do
not recursively close hardware/services simply because their CPU CFG is closed.

| Function/label | Address/size | SHA256 |
|---|---|---|
|GXGetCPUFifo|802b3be0/8|e3bf3b44adf40b81076c0c6698613ea7d1f2c60ef399a76b8dfe2ad0c1901984|
|GXSetCPUFifo|802b3578/128|1d7565a951f0bb16691ac34be2673ff03bbd9caf82cfe6ee1fb8e050268e8de4|
|GXSaveCPUFifo|802b3818/34|9d25c01065a2d5116315745f98b75e1137d44c51011732c8f00bf211c2ee0a54|
|__GXSaveCPUFifo|802b384c/c8|0b39b85700efcd5938a872b9fb2fb65cfa2d3bfba143eeee09fe3549e12d71dd|
|GXFlush|802b5154/5c|292f0fed2ad804761e2c1909aa6cea251caf0c1d8e4498c3891c106b31008676|
|__GXSetDirtyState|802b59ec/80|cc2ad762c117ecda6a049a11f512507114c8994e54616145e7de118941cb9e30|
|__GXSetGenMode|802b5d04/24|003b670e48d94a967abe434a8cbc5352d1d2d68d8aaf08ebf49413e4adf66763|
|PPCSync|80295004/8|1560a9543d41d35b61c288453bb0972cfd1359499e5fcc7ef1e2c52391fa12cb|
|PPCMfwpar|802950a8/c|5882ed333d61193341c80c4ba985212a8ee2d26c8837820d429867c27c160a14|
|PPCMtwpar|802950b4/8|67fa43b7cdebc70d41db82010c290cea0385b707d272548f1545438cc79ea89e|
|OSDisableInterrupts|80299ad4/14|91253610f8caa4988f3df8595ccfc6e5eafaf0b6df1b302668555ab7d528b1bc|
|OSRestoreInterrupts|80299afc/24|54db610a580003e2db23e460425f4662f4f4e7a4da3ae6f89a632df9e52b427a|
|FifoHelper3a4c|802b3a4c/34|06a3aaab54bf92156019ce37c48eacebb9a43d6c6cee22bdc0b1640ce9452c52|
|FifoHelper3a80|802b3a80/30|549de3ccd8189fa4e909488f426f4e1997e93842330b74ed4848ea4c1e190f37|
|FifoHelper3ab0|802b3ab0/30|d822f65a8d9accfad6901dc242f65a3b298316286c269a76b8f32ad8dfede8b5|
|DirtyBit1Child7e28|802b7e28/a0|496b80581e47b8b070799bc6296252521af509d98b5a71b54f8d3f541980df56|
|DirtyBit1|802b7ec8/17c|481563c3b0eea2238615d3d16749958fdc2c93b667ff251fea2b2fb2660f7106|
|DirtyBit2|802b887c/4|f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e|
|DirtyBit8|802b40fc/bc|6cc3e2a9277f5977b03f9a44f0389081cd45b40e412d1bf2b583a9ddbd143550|
|DirtyBit16|802b4a34/88|75d181dbb6da1df6a2e83fa2cd30d286f619a9411e28d9eb767006a777294f6d|
|DirtyBits8or16|802b41b8/124|498728136b2c88928be767d202b35d5d38314cbbb6454cf34efde6a1d5a6e8d8|

## Next implementation and bounded documentation request

Do not hold the parent's raw mapSetMaterial work for more local SDK contracts:
its portable CPU caller is now implemented and can retain exact unbound SDK/GPU
children. For capture, the smallest coherent ownership slice is **one stationary
SourceGxCaptureProcess** rooted in the same actual UI/animation VM as
NativeMapProcess, owning actual GXData, FIFO context+backup and these globals,
plus an explicit platform FIFO/gather transport. It must reject foreign/replaced
physical mappings and preserve partial Begin/copy/switch/Flush/End progress.
It should expose source Begin/End/Reset as retained original void/word ABI,
record concrete source FIFO bytes, and associate captured spans with separately
translated native material programs. Keep real mapLoad at Begin until those
ownership obligations are available; a token receipt that merely lets the raw
material parent run would lose original cache/FIFO effects.

Implement full closed Begin/End/Reset and CPU FIFO/save/dirty graph above as
fixed native continuations or equally exact retained source services. Route OS
critical sections/PPC sync/WPAR/PI/CP access through explicit 3DS/native platform
adapters; do not execute Gekko MMIO as ARM pointers. IRQ adapters must preserve
the source captured previous-enabled state, including nested disable/restore;
no source result is implied by C3D_FrameSync. The renderer pre-load fence already
owns native queue completion and stays separate from this source transport.

Cold raw GXData/FIFO state requires an actual initialization contribution with
provenance and complete ownership. The whole existing original GXInit and
__GXInitGX hashes are available, but transitive register/OS/alarm/texture-region/
callback/SDK setup is not closed by this handoff. Never remap or initialize over
an existing UI owner or synthesize a presumed zero saved FIFO pointer. A bounded
native platform bootstrap can initialize precisely declared adapted facilities,
while executing real source cache-init writes; its receipts must identify the
omitted GXInit services and cannot claim whole original initialization.

For native map acceptance, all reached material GX setters—including texture
object loads, channel/color, TEV and deferred state—must write through this SAME
SDK cache/transport. Existing PICA shader setters, NativeShadowProgram and
SourceGxInheritedState are not that transport. Validate serialized output and
native material translation separately. Expand support through every material
admitted by both original harbor passes; an unsupported mode/flag/format holds
the complete parent instead of removing the effect.

Tests should check actual temp writes and current FIFO switch, optional save0/1,
dirty entry flush before backup, old-FIFO Flush before switch, End's first
capture Flush, captured overflow before save, preserved GXData+8 across restore,
actual returned descriptor distance, deferred cull bytes, all source word/byte
packing, holds after copies/writes without replay, foreign replacement/recovery,
overrun/wrap behavior, abandoned capture with no premature publication, and
pass1/pass2 actual allocations/copies/slots. Original runtime captured command
bytes/counts and 3DS native image comparison remain required; host receipts alone
cannot close hardware equivalence.

The remaining precise source/platform research request is **original Gekko
write-gather to PI FIFO behavior for Begin/Flush/End**: SPR921 busy/reset effects,
commit granularity, residual bytes after eight-zero Flush, PI write-pointer
update/bit26 overflow timing, end/wrap behavior and source call ordering under
interrupts. Inspect original runtime or authoritative hardware/provider evidence;
record exact requests/results. This evidence is not supplied by the software
SDK disassembly. Until available, implement a declared conservative bounded
native transport and retain any unsupported domain rather than invent capture
lengths. It may produce real native work without claiming unverified byte parity.
The request for **local GXBegin/GXEnd/GXReset boundaries** is now answered by the
verified complete bodies above and should not be reopened as undocumented.

Current EVT-01 remains open for full capture/resource/GXData initialization,
source Load publication, native material/geometry consumers and original
runtime/hardware acceptance. No source G0, room readiness or prologue completion
was produced by this documentation task.
