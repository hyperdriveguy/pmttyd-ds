# Native US battle startup resource admission

This bounded provider admits actual original resource banks and GPU source
atlases for the [startup readiness sequence](BATTLE_INIT_SEQUENCE_DISPATCH_AND_RESOURCE_READINESS.md).
It does not complete paper capture, pose playback, effects presentation or a
battle. Readiness derives from successful CPU reads/validation and the native
texture-import callback; no timer or simulated successful acknowledgement is
introduced. Language1/context1 are explicitly the original US scope.

Original paper keys are802efbe4=`p_b_st`,802efbec=`p_jyabara`,
802efbf8=`p_kaiten_h`. The effNiceAsync US language row1 at80417a6c contains
`EFF_nice_us`, `EFF_good_us`, `EFF_great_us`, `EFF_wonderful_us`,
`EFF_excellent_us`, NULL, `EFF_lucky_us`, in that order. These point to
802f3d00/3d0c/3d18/3d28/3d3c/NULL/3d50. Native `paperReady(key,context)`
admits only the three original paper keys at context1. `niceReady(mode)` is
true only for mode1 when all six nonnull US effect resources are admitted.
The source sequence retains its own ordered short-circuits and repeated calls;
this provider does not write sequence fields or story progression.

`BattleStartupResources::load(root)` reads ignored PMBRDY01: nine fixed resource
records, context1/language1, full original model banks, source texture-owner
names and all25 original animation descriptors. It verifies fixed names/keys,
bounded fields, checksum, original model name/texture-owner header and source
animation loop/span/key descriptors. It then reads PMANIM4 for all23 currently
supported poses and validates their source descriptor correspondence and first
pose evaluation. Failed loads preserve the previously admitted resource owner.
CPU admission alone leaves every GPU readiness flag false.

`admitTexture(index,callback)` calls the platform importer with the actual
converted T3X path. Only callback success retains that resource's GPU flag.
Repeated requests reuse an existing admitted texture. The GPU owner must call
`releaseTexture` before freeing a texture, or clear the resource owner at
teardown. `resourceReady` permits exact key/context queries for the source nice
row scanner. This is native resource residency, not a reconstruction of the
original file-heap/cache/pose-pool implementation or its latency.

## Source capture ownership and dormant unsupported clips

All three paper banks use original material texturekind1. Their fallback TPLs
are retained for actual resource/texture admission; source live-field capture
must still replace the appropriate paper texture before playback. `p_jyabara`
has no own companion file: original header+44 names `p_slit`, so its actual
texture comes from `a/p_slit-`. The other two paper owners retain their own
companions. Six EFF banks use ordinary source texturekind0.

The surface exporter has an explicit `captured_textures=True` descriptor-import
option. Its default still rejects texturekind1. This scoped option retains
original wrapped UV, sparse geometry, RGBA and pass/cull and imports original
fallback art; it does not admit any live capture or infer kind1 rendering
semantics. Full original banks and the separate needsCapture marker retain that
ownership. The generic ordinary PMANIM4 pathway remains unchanged by default.

Original `p_jyabara` contains12 poses. `PM_J_3A` and `PM_J_1F` contain partial
node tangent-IN values outside the existing bounded decoder's initialized table.
Their original descriptor and complete serialized streams remain resident;
these two poses are absent from the converted evaluator, and requests to play
them remain unbound. They are not silently rewritten or presented as decoded.
The other10 accordion poses, four p_b_st poses, three p_kaiten_h poses and six
EFF A_1 poses retain their original supported sparse streams. Loading the raw
bank does not prove that every contained animation can execute natively.

Original atlas tiles resize to124pixels with source sampler gutters, then the
cropped atlas halves to512pixels wide. The retained UVscale accounts for unused
vertical rows. GPU sampler clamp pairs with existing portable source-wrap
subdivision, preserving the original pass/cull/material metadata. No generated
texts or replacement effects are used. Geometry, original model banks and
textures remain ignored and outside source history.

Tests verify nine actual source-bank/atlas paths,25/23 descriptor counts,
texture alias, explicit refusal before importer success, original key/context
and mode gates, all-six effect admission, release, missing/corrupt source input
and transactional preservation. The source verifier checks nine DOL strings and
the actual seven-slot US row with its NULL. Native GPU-import success and source
startup transition still require integration validation; host callbacks do not
establish hardware residency or source playback correctness.
