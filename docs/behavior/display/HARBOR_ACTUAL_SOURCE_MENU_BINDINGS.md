# Harbor actual source class and weapon bindings

`HarborSourceMenuFactory` binds the existing retained native class/weapon Frames
to the actual Harbor VM. The existing `TrioClassMenuFrameBinding` and
`TrioWeaponMenuFrameBinding` records contain generic source services; reusing
these value types does not create a Trio runtime or a second battle.

The factory captures actual source-global B, actual VM and original common-menu
file owner, the current UI allocation-lifetime accessor and the actual shared
[Harbor display pass](HARBOR_SOURCE_DISPLAY_PASS.md). Runtime owns Factory and
Display; factory State weakly references Runtime and Display. Returned bindings
retain State, source file storage and independent UI tokens. They do not retain
a public Harbor facade or dereference one in a callback.

## Source admission and lifetimes

Binding reads B+1c78 for the real UI W allocation, not a fixed native work address.
Each native Frame generation identifies that W token. Its producer predicate
separately checks the captured Display generation and successful Main pass.
Those generation domains need not have equal numeric values. `resumeClass` and
`resumeWeapon` lock actual Runtime for the complete native Frame call, and accept
only the exact currently invoking Display record/references, camera8, captured B
and original callback8012210c /80121ce8 respectively. Registration alone,
`viewPublished` or an inactive default cursor cannot produce admission.

Both routes require original B+163f8 → FileEntry+a0 → relocated TPL payload, the
same actual NativeFileCache/VM aliases, owned payload bytes, FileEntry state1,
type4 and nonzero references. These are native admission reads, not extra
instructions attributed to the original drawing function. In particular source
menu End leaves B+163f8 unchanged. The real released FileEntry state/references
reject that stale chain. UI allocation and common payload proofs remain distinct;
this bounded immutable staged cache has no invented generic eviction/ABA lease.
Actual W retirement and source-process retirement remain mandatory. The unrelated
old B+a0 field is not the common bank producer/consumer slot.

Optional reads never fall through unread owned VM memory to GP. Class label
strings are copied only from actually known NUL-terminated VM bytes, with a native
16384-byte admission bound. No terminator or label is supplied on exhaustion;
subsequent byte changes reject a cached immutable label. All US dialogue/control
bytes and IDs remain unchanged.

Weapon help uses the existing complete private-VM `BattleVmMessageLookup`, one
retained caller cursor per native binding and one immutable per-Runtime original
room/global MessageAssets snapshot. Existing actual class-label pointers are
verified before alias reuse. Cache base85b00000/capacity4MiB and key bound4096 are
explicit native address/admission policies, not original msgdrv placement.
Unknown/ambiguous keys and unbound original language fallback hold. No empty
message or seven-label-only fallback is invented.

The optional weapon source-memory owner reuses the genuine NativeUiStartup
font/window/icon resources and actual four-lane source GP-language alias. Its raw
VM pointer is protected by a strong actual Runtime lifetime; the factory caches
that memory weakly to avoid Runtime→Factory→Memory→Runtime cycles. Currentness
still requires the public/source registration described below, actual B and the
real Display receipt. Native resource import does not produce source readiness.

## Facade copies, replacement and destruction

The exact Harbor integration journal adds a facade registration slot containing
only a weak actual Runtime identity. A fresh accepted begin creates a fresh slot.
The factory captures the slot weakly; retaining the VM through a source-memory
or packet owner alone cannot preserve a retired facade's source admission.

The existing default copy semantics share the same actual Runtime and registration
slot. A new begin/replacement gives only that facade a new slot; an existing copy
may continue owning the old actual Runtime. Moves transfer that ownership;
assignment releases the destination's previous slot. Once no facade copy retains
a slot, old factory providers reject it before accessing source state, even when
an external owner still pins the old VM. This states the menu binding's ownership
policy; it does not claim arbitrary copied Harbor event/scene scheduling is safe.
The existing active-battle begin guard is preserved: a rejected begin does not
replace a registration or invalidate its current providers.

## Validation and native frontier

Focused tests use real EventNativeRuntime, NativeFileCache acquisition/relocation/
release, original common TPL bytes and unchanged US messages. They cover W-token
replacement, exact source bank chain, released-pointer rejection, real invoking
queue/producer identities, distinct W/Display generations, unknown data and weak
Runtime retirement. An additional narrowly friended test TU compiles the exact
journaled Harbor.cpp and executes real beginFresh; externally retained old Runtime
plus facade destruction/replacement/copy/move exercises the actual registration.
Its camera receipt is explicitly a TEST fixture, not normal source initialization.
No Game from an older archive ABI is instantiated.

Renderer source selection/actual original class, weapon and target presentation
still require concrete consumer wiring, outside-frame genuine resource admission,
the real field initializer's completed8000e95c(4,2) receipt and all original queue
registrations. These factories neither activate source Main nor call native
`classMenuPresented`/`weaponMenuPresented`, and they introduce no command touch
hitboxes. Original physical directional/A/B/X paths remain source-owned.
