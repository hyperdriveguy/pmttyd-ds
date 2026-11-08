# Native original shadow process initialization

The fixed native graph includes complete shadowInit8002dcec/4f8,
shadowGetCharShadowProjection8002bc7c/c and GXGetTexBufferSize802b708c/15c.
All original words and full callable boundaries are verified against the
read-only US revision0 DOL. This is static original evidence and a native
continuation implementation, not an original execution observation.

SourceShadowProcess owns actual cold process BSS: primary803c9e38/11c,
projection803c9d34/104, depth803c9c30/104 and the two original32-byte ramp
texture objects8041e738/40. It borrows initialized cswp/pswp/dswp pointer
words804150e0/e4/e8. An aligned32KiB private stack with32-byte guard is a
native frame policy, not an original measured stack size. VM, backend and its
real allocations/services must outlive the owner and retained calls. Original
initialized constants and SDK tables are imported separately; no initialized
shadow BSS or image is imported.

Init executes its complete source sequence: primary defaults, real heap0
allocation/clear of64 records, full texture sizing, smart image allocation,
depth defaults, original tiled I8 and IA8 CPU loops, two GX texture constructors,
two LOD constructors and cache flushes, then final projection defaults. These
are distinct owners; the C++ ShadowWork layout is not raw-aliased. The wrapper
publishes ready only after the whole original initializer returns. Early gate1
at primary10c does not permit a projection query while later children hold.
No child result, constructor effect, allocation, cache operation or rendering
completion is synthesized by the owner.

Backend current is a pure identity/lifetime check; it must not yield or mutate
mappings, including inside SDK raw borrowers. Other backend callbacks own
complete actual child effects and remain current through
holds; false preserves the original child and its partial effects. The graph
retains same request object/registers/stack/CTR across child and budget yields.
The CPU ramps use original byte/half stores and tiled addresses, not converted
native gradient images. Source sizing includes its format dispatch, low-width
inputs and mip branches; it does not return fixed shadow-specific sizes.
Initialization and query reentry are refused. A changed raw query object,
function or input bits cannot consume a retained query. Queries read live gate
storage, including an actual later zero. A completed Init is not replayed.

Mapping identities and all three source owner pointers are checked. Replacing
owned backing invalidates admission; old-owner teardown preserves replacement
mappings. Process state survives map room changes; room unload does not destroy
or reinitialize it. FieldMapHeapOwner may borrow this exact process. Its reached
projection child is routed only through the ready owner. Without that owner,
the original map boundary remains explicitly held. [Normal Game startup](NATIVE_SHARED_SHADOW_BOOTSTRAP.md) now installs this owner in the existing
animation VM; map publication still requires explicit camera/map composition.

Native generic heap0, shared smartInit/Alloc/AutoFree, GX constructor adapters
and CPU VM cache services are composed for normal cold activation. Actual
collection/GPU/cache lifetimes remain separate. First smartAlloc triggers actual smartAutoFree(3);
NativeFileCache's simplified smart header is not that arena. Later shadowMain,
collision preparation, cylinder/GX drawing, projection passes and renderer
residency remain separate work. See [source request ownership](BATTLE_SHADOW_REQUEST_OWNER_AND_DEFERRED_PREPARATION.md),
[shadow source domain](ORIGINAL_ORDINARY_TEV_SDK_AND_SHADOW_SOURCE_DOMAIN.md)
and [the documentation inbox](../../requests/EVT-01.md).

The [native shared smart arena](../memory/NATIVE_SOURCE_SMART_ARENA.md) now
supplies the real source header in the integrated shadow fixture. This replaces
its former smart allocation boundary token. Same-VM owner borrowing is available;
normal Game activation is composed, while genuine file/GPU/cache lifetimes remain required.
