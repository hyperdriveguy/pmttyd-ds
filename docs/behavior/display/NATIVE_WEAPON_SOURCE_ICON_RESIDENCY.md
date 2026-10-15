# Native weapon source-icon residency

`Renderer::prepareSourceIconImage(image,pin)` exposes the existing genuine
single-image catalogue importer outside FrameBegin and source callbacks. This
is native resource admission, not an original source function, readiness write,
queue acknowledgment or substitute image. `enemyHpIconResidency(image)` remains
the ready-only query; its name does not grant an HP/source pass to weapon work.

## Original importer preservation

The exact `render_source_icons.cpp` importer from accepted6458 is factored into
`render_source_icon_preparation.cpp`. The unapplied integration journal pins
that baseline and replaces the old body with the shared call. Catalogue lookup,
empty/oldest unpinned choice, GPU synchronization, retirement-before-replacement,
file naming, exact one unrotated RGBA8 image, minimum8/power-of-two backing,
source dimensions plus two-pixel gutters, no mip/LOD bias, full backing subimage,
linear filtering, clamp, stamp and success-only generation remain unchanged.
A missing/malformed import holds and reports the existing failure text. No
native image/generation is manufactured. An invalid catalogue image is rejected
before marking pending. A new explicit image request clears an old legacy
pending slot only when the requested IDs actually match.

Legacy `prepareSourceIcons(Game)` preserves the actual Trio-active gate,
retained-pass observation, catalogue preparation even without a pending image,
and one pending-image service per invocation. With no current weapon pin, the
inactive path has the original behavior: all native source-icon residency and
legacy logical state retire. With genuine weapon ownership it retires only
unprotected images, preserving pending/building/sealed weapon cells. All legacy
bound/pending/material/retained-pass/viewport/scissor/transform state still
clears. Explicit shutdown/release is force-all, invalidating pin owners and all
actual cell generations before resource frees.

## Actual ordered ownership

The actual weapon context owns shared `BattleWeaponOrderedPublication` and
`BattleWeaponSourceIconPin` instances. Renderer stores only weak Pin links; Pin
stores only a weak publication link. A strong independent proof-holder lifetime
and pure long-current closure are mandatory. The closure proves the actual W
allocation/process-resource owner remains current after the short camera lease
finishes. A false result latches stale; the same pin cannot revive for a reused
allocation. The closure must not reenter Renderer, alter residency or retire
resources. Pin's own public reentry is rejected; retirement during its proof
latches invalid without destroying the executing closure.

Before a real missing image/import, mark that actual requested ID pending. A
successful import or request does not acknowledge source append. `acknowledge`
removes pending intent only if the actual publication's building/sealed packets
already retain the image. Both publication-retained IDs and still-pending IDs
protect every ordinary eviction path. Missing append/provider/residency keeps
the image pinned. `retains()` on the publication alone is insufficient: it does
not check actual long ownership and can still contain packets after its native
retire flag. The Pin requires its independent actual ownership proof throughout.

Retire the Pin at genuine source-context/allocation/resource retirement before
freeing that context. Renderer force-all release also retires registered pins.
Avoid a Context→Pin→Context cycle: use actual independent W/resource tokens and
weak context captures. Existing publication/frame binding resources must also
follow their separate lifetime/explicit-retire contract. No raw callback user
pointer is accepted by the new pin API. A missing/expired proof holder or actual
publication releases its cache protection without dereferencing old context.

## Scheduling and native proof boundary

Both new import and existing preframe import must run in the actual serialized
resource preparation phase. The source callback only requests/checks residency
and holds; it never invokes the importer. Public Citro3D exposes no in-frame
query, so this API states an honest caller precondition rather than reading or
resetting private SDK flags. Importer reentry is rejected. There is no GPU draw
or RNG operation in preparation. Actual normal weapon binding must supply its
own W generation/source resource proof and preserve pending pins until append;
no Game-active override, finishing/HP pass or synthetic callback is supplied.

## Validation

The focused test uses the real ordered-publication class with explicitly
labelled host resource fixtures. It proves pending protection before residency/
append, failed append retention, acknowledgment only after actual append,
building/sealed protection after the source camera ends, source ownership loss,
nonrevival, force retirement, expired Pin/publication/control blocks, missing
proofs and self-reentry/retirement. Strict host, ASan/UBSan and GNU ARM syntax
validate the new helper, native importer and exact patched legacy/header graph
against one consistent accepted6458 graph. The source journal is unapplied;
full root builds and actual import/raster probe remain integration work.

The earlier actual prepared Window/font/submenu GPU probe stays frozen and
unchanged. It does not establish this new icon import, native icon pixels,
normal Weapon entry, source queue, original PPC or physical-device parity.
