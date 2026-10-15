# Trio class and weapon frame bindings

The factories and resume methods in [TrioAttack](../../source/battle/trio_attack.cpp)
connect actual VM resources to [class frames](battle-class-raster.md) and
[weapon frame publication](weapon-window-bridges.md). They create no source menu
activation, display registration, GPU import or ready-camera epoch. See
[menu providers](trio-menu-bindings.md) for allocation and message construction.

## Allocation, facade and camera lifetime

Both factories capture the current shared runtime, read actual global B at
`0x8041eb90`, require the native Battle address, and find the UI pointer W at
B+`0x1c78` in the runtime's allocation-token registry. A token's retained address
is a nonzero native generation identity; it is not an original allocation or
camera pointer. Retaining that token prevents the same host address from being
reused while the binding still refers to it.

The current predicate additionally checks that the public facade still has that
runtime, that W still has that exact token, and that actual global B/UI pointer
still match. Shared storage alone proves none of those current-state facts.
Several predicates capture `this`: keep the Trio facade alive at its original
address until all bindings retire. Keeping only the shared runtime alive does
not make a moved or destroyed facade safe to call.

Producer admission requires whole camera Main publication: no poisoned owner,
no waiting/active Main cursor, matching token generation and a nonzero pass equal
to the current publication count. Generation and pass are different native
identities, neither render FPS nor source time.

The synthetic camera-view reservations are `0x60010000` for class and
`0x60014000` for weapon. Each proposed range must be unmapped across the actual
VM/file views before use. These are native reserved addresses, not extracted
source RAM. The HP bridge has its own `0x60000000` reservation and callback.

## Original class labels and weapon resources

Class binding requires the immutable original class-message owner. The one
original cursor lookup can complete without writing its private optional output;
no-match is not repaired to cursor zero. Label lookup whitelists the seven mapped
original key addresses, then reads bytes through NUL into stable map-node strings.
Unknown addresses, unreadable bytes and scan wrap return null. The permanent
immutable per-VM message policy is required; cached `c_str()` pointers borrow
runtime storage. Original bytes/control tokens are preserved.

Class RNG uses the runtime source RNG and records the original request. Retrying
a later child must not repeat an acknowledged random draw. Both factories read
resource words from VM first, then the GP-address view; unavailable words stay
absent. A successful word read alone proves neither source callback admission
nor GPU residency.

Weapon binding additionally requires initialized/ready source menu files and the
exact common-TPL chain: B+`0xa0` descriptor, descriptor+`0xa0` holder cell, then
cell payload. All three captured pointers must remain equal on recheck, along
with the same file owner and initialization state. The variable named `palette`
here stores this TPL payload address; it is not GPU palette residency evidence.
The source-resource owner pins runtime storage while linked services separately
own their partial math/footer/raster work.

## Exact invoking queue records

Resume requires camera 8, userdata equal to Battle and nonzero camera publication.
The actual CameraDraw display cursor must currently be active and invoking the
matching callback: class `0x8012210c` or weapon `0x80121ce8`, with the same camera
and userdata. Registration, a matching pointer or an old publication count cannot
manufacture this record. The native frame owner then retains source/raster
progress and verifies its own queue/camera epoch through complete submission.

## Fresh entry and original poses

Fresh `begin` maps optional original class messages at native guest reservation
`0x85a00000` before initialization and runtime publication. Mapping/binding
failure prevents publication. Existing-runtime reuse returns earlier and cannot
adopt a replacement message bank. The HP allocation token is created only after
successful initialization; fresh defaults remain explicit native context.

Initial source pose type 28 selects actual MODEL/pose names for registry IDs
1/2 (Mario/partner), 3..5 (enemies), and 6 (Professor). Mode 1 is the battle pose
pool domain. Actual resources must match the source model and pose; missing data
fails rather than substituting another pose. Handles and native generations own
the entered instances. Audience script callbacks share the actual VM and public
sound/pitch owner. Missing HitN64 remains pending in the effect-taking route;
recording a sound/effect notice does not acknowledge that missing effect.
