# Weapon frame stages and concrete renderer lifetime

[The native frame](../../source/display/battle_weapon_native_frame.cpp) retains
one actual queued source invocation. [The renderer binding](../../source/display/render_battle_weapon_runtime.cpp)
connects source memory, TEX constructors, typed leaf packets and prepared native
submission. It preserves unavailable children as holds; it does not register a
queue callback, start battle Main or manufacture inherited GX state.

## Source completion is earlier than frame completion

Frame binding separately retains the UI allocation incarnation, source-resource
owner and their current predicates. Short source currentness additionally needs
the actual executing Camera8 callback 0x80121ce8, admitted Main/Draw epoch and
original source table's current callback. Long batch currentness deliberately
survives legitimate camera finish but still requires allocation/resource history.
Observed source or batch proof loss invalidates the binding.

The held pass fixes the exact queue-record address, not only callback/userdata
values. The camera engine also checks resetEpoch, preventing reset and identical
repopulation from reviving the old invocation. Native generation/pass metadata
is distinct from simulation ticks or render-frame count.

Begin, retained source draw, immutable seal, submit and camera finish have
separate latches. Empty seal or false submit preserves completed source work;
lookup, glyph RNG and source W writes do not restart. A true submit is recorded
before post-call guards, so subsequent lifetime loss cannot replay accepted GPU
work. Only a submitted, camera-finished current batch becomes public. A new pass
hides the preceding batch while building its replacement.

sourceComplete(pass) is available to the seal provider before finish. It is not
publication or GPU admission. Likewise pending() reports only the source cursor's
child frontier: zero can coexist with a held seal/submit/finish. Recursive bind/
resume is refused. Retirement invalidates immediately and defers destruction
until the active call unwinds, without executing guest End or free.

## Proof storage and mutable producer storage differ

The renderer's Proof pins actual source memory, startup state and font/class/
submenu leases. Its current check rejects recursive proof evaluation and latches
observed resource/allocation/inherited-state loss. Proof has a weak reference
back to the mutable WeaponMenuBinding. Packets can retain their proof storage
without retaining a strong producer back-reference through that field.

The frame's resource pin intentionally holds the producer itself. Explicit
release marks both producer and proof inactive, retires ordered state, then
retires the frame to release that pin safely. A pending attempt cannot be
reconstructed under the same allocation merely because readiness failed.
Releasing such an attempt records its allocation token as abandoned; preparation
requires a fresh incarnation before restarting source work.

A genuine new pass captures the actual camera once and creates leaf contexts
for that pass. Held retries reuse them. Class/submenu queries capture inherited
GX evidence at actual LoadTexture, independently of native atlas residency and
TEX construction. A completed TEX result is acknowledged only after the typed
TextureObject consumer accepts it; a downstream hold keeps the same result.

## Native capacities and original identities

The font leaf and ordered mixed arena each have a native limit of 4096 entries.
These are admission policies, not recovered source counts or measured original
memory use. Language 1 and the 512x256 font atlas are exact startup/font checks.
The explicitly reserved private TEX output is 32 bytes at 0x60018000, outside
the camera reservation and checked against actual mapped owners. It is neither
a guest stack alias nor an index-as-object handle.

Total font memory callbacks abort on an absent asserted-domain read/store rather
than return fabricated zero/success. Optional readiness queries use the actual
memory owner. The original cost helper reads pouch global 0x8041eb00 and actual
equipped badges; reached pow/double-conversion domains remain holds when their
numeric providers are absent. The free-cost arm can complete without those
unreached children.

## Text and child routing preserve actual source operands

The font-entrypoint legend names default start, color/palette/scale, width,
centered/matrix/scalar string and outline operations. These source addresses
route into the retained font owner. Class/submenu/window/font children keep
independent cursors and ownership proofs. Unbound icon children still hold;
recording a high-level request is not child completion.

Help can provide its retained private bytes to scalar text without a fabricated
guest SP address. A stable HelpText owner/offset survives holds, locks the live
producer before dereference and guards offset arithmetic in the 32-bit domain.
Halfword reads join independently known bytes in big-endian order. Unknown
bytes do not become an invented NUL terminator or rewritten dialogue. Real
message lookup remains the source provider's responsibility.

Window help uses an absent native surface owner when necessary so the actual
source-ready-zero no-draw branch does not depend on texture residency. Reached
window geometry still requires the genuine source object, native panel and
ordered append. No missing draw becomes an empty success.

Font ordered reserve occurs before the original glyph's two RNG calls; commit
cannot fail afterward or let sibling packets pass that ordering barrier.
The actual startup font object and native font lease must agree. This wiring
preserves existing source work; it does not establish complete original markup,
message setup or normal activation.

## Prepared submission is the final barrier

ready prepares and preflights one immutable native raster plan for the exact
sealed batch. tryDraw waits for weaponMenuSubmitReady before presentation; false
must precede every GPU effect and retains the same plan/batch. Opaque frame
adapters check exact object and shared ownership identity, rejecting unrelated
aliases. Source work and a submitted GPU prefix are not retry units.

The caller still owns actual frame/target lifetime, resource exclusion and real
queued source scheduling. This code does not prove normal battle completion,
original numeric/raster parity or hardware timing. See
[the weapon frame contract](../behavior/display/NATIVE_WEAPON_MENU_FRAME.md),
[the concrete source binding](../behavior/display/NATIVE_QUEUED_WEAPON_MENU_BINDING.md),
[ordered publication](battle-publications.md) and [camera tokens](battle-camera-numeric.md).
