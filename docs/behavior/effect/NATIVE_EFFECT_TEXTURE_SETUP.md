# Original primary and N64 effect texture setup

`SourceEffectTextureSetupCursor` executes whole US revision0 effTexSetup
8005ee0c/0x114, its mandatory effTexSetupN64 child801d7910/0x110 and the separate
completion callbacks8005efa8/0x48 and801d7a58/0x48. It reuses the existing whole
UnpackTexPalette and source texture primitive/path ABI. Sharing that ABI creates
no window state or resource ownership. Source memory and provider identities
remain fixed through holds; accepted reads, stores and child calls do not replay.

## Original source order

Both installers separately reload their state pointer before clearing palette
and readiness, call arcOpen with the original name/zero operands, publish its
return and reload the actual palette for the branch. The archive route unpacks
then reloads the state pointer for readiness. The DVD route reads actual GP+16c:
zero chooses JP, every nonzero value chooses US. This differs from the effect
Main language-library table. It obtains the actual DVD root, formats the source
private path, opens type2, gets length, allocates wrapping(length+31)&~31 on
heap0, publishes the palette, writes the handle at its+6c and reloads the palette
before ReadAsync. Setup acknowledgment means complete job admission only.

Primary always continues into the whole N64 installer after either route. Their
states and queued callbacks are independent. A callback first loads its state
pointer, captures the handle from secondArgument+2c, then reads the palette from
the previously captured state pointer. After unpack it closes the captured handle,
reloads the current state pointer and writes readiness1. The first callback
argument is ignored. No callback is inferred from setup returning successfully.

## Actual native owner

`NativeEffectProcess::beginTextures` lazily attaches `NativeEffectTextures` over
the actual UI/field-animation VM and source SDK heap. Five exact new initialized
literal ranges are imported; existing manager, N64 state, root, primary prefix
strings and shared archive descriptors are borrowed and checked by actual
backing identity. No fresh private VM, duplicate manager/BSS or source reset is
introduced. Successful mappings and heap allocations remain process history.

The finite native backend supports original US direct-DVD paths and actual
inactive-four-descriptor arcOpen exhaustion. Active archive descriptors hold for
an archive provider; JP selects its real path and holds rather than substituting
US. Source native entry tokens begin at60050000 with152-byte entries; these are
backend addresses, not recovered original heap placements. Actual VM-owned
entry bytes and real heap0 palette allocations are retained and alias-checked.

The finite two-string formatter preserves the actual mapped format/root/prefix
inputs and typed private destination. Its128-byte including-NUL capacity is
native admission policy, not a claim about original unbounded sprintf safety.
ROMFS opens/read/close and byte budgets are explicit native IO policy. Actual
ReadAsync stores, separate flag reloads, progress/remaining/destination updates,
callback userdata and whole source unpack/close/readiness are preserved.

`pumpTextures(byteBudget)` delivers at most that many actual file bytes across
both jobs and invokes their independent callbacks only after complete verified
transfers. A callback may complete on a zero-budget pump once bytes are delivered.
Delivery uses a synchronous raw-memory borrow with word/byte writes; no pointers
escape, child runs or yields occur inside it. `texturesReady` requires both native
jobs completed and both source ready words. `busy` tracks real queued jobs and
held setup independently: forged source readiness cannot authorize a Game
restore while a DVD job is pending. Callers retain the owner through all jobs.

## Assets and activation boundary

Primary `e/us/effect.tpl` is266784 bytes with118 images. N64
`e/us/effect_n64.tpl` is235776 bytes with151 images. These unchanged original
banks are distinct from the later106880-byte `effect_us.tpl` language library.
`tools/import_effect_primary_assets.py` opens the ISO read-only, verifies source
bodies/data/TPL hashes and refuses nonempty destinations. Normal manifest recipes
produce `effect-primary/memory.pmevt`, `effect.tpl` and `effect_n64.tpl` under
`effect-primary/`. Per-asset source paths/offsets and retained original rights
are in `assets/original/effects/primary/provenance.json`; no redistribution
license is granted. Existing editable asset imports are never overwritten.

The sole original direct primary setup caller is seq_logoExit8008748c, after
the original environment setup8019df84 and before marioPoseInit. This API does not claim that complete logo
Exit: its preceding environment owner and remaining sequence children must be
bound before original normal sequence activation. Normal Game owns the effect
process but does not yet call these texture methods automatically. Source Main
callbacks, actual GX/3DS texture residency/draw, audio and retirement still need
production consumers. Texture bytes/readiness do not establish rendered Crump
reactions or a complete field/battle/result/field-return story chain.

See [the effect process](NATIVE_EFFECT_PROCESS.md),
[original item texture contract](FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md)
and [Crump requirements](../story/CRUMP_OPENING_INTEGRATION_REQUIREMENTS.md).
These source identities are documented; remaining integration is implementation
work rather than a duplicate documentation-inbox request.

The native transfer implementation is now shared with the original environment
startup as `NativeSourceTextureStartup`; `NativeEffectTextures` is a compatible
alias selecting the unchanged default effect profile. Separate source cursors,
two effect jobs and mandatory N64 setup are retained. See
[shared ownership](../platform/NATIVE_SOURCE_TEXTURE_STARTUP.md). Original normal
sequence activation still needs the preceding fade process and complete parent.
