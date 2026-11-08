# Battle publication identity, order and retries

The [class publication](../../source/display/battle_class_publication.cpp),
[HP presentation](../../source/display/battle_enemy_hp_publication.cpp) and
[weapon publication](../../source/display/battle_weapon_ordered_publication.cpp)
copy completed source work for presentation. Their native sequence numbers are
ordering metadata, not original guest clocks, simulation ticks or render frames.
A nonzero key alone does not establish a real source allocation's lifetime.

## Three distinct lifecycle contracts

| Owner | Completion boundary | Retry and replacement | Retirement |
| --- | --- | --- | --- |
| Class | Complete class backend and increasing sequence | Failed publication keeps old batch; render admission is consumed once | Clears binding, batch and admission flags |
| HP | Whole source HP pass, resource preflight, then completion recheck | Same active pass resumes; next building pass preserves prior complete publication | Defers clear during callbacks; discards building and published histories |
| Weapon | Complete source parent and every typed packet | Seal is immutable; next pass waits for successful prior submission | Permanently invalidates this one-shot owner; retains pins/storage until destruction |

Zero sequences/passes are unstarted. Rebinding the same class token/allocation
does not restart numbering; a new token distinguishes reuse of the guest address.
HP bind consults the actual allocation-current provider and creates a separate
native publication token. Weapon bind is one-shot and pins allocation, source
bank and native contexts. Each owner still relies on real resource owners to
keep borrowed GPU allocations alive.

Class copies only assets, optional surface lease and quads. Label/random/icon
callbacks can refer to a finished stack-local source pass and are deliberately
omitted. Without a surface lease, the caller must preserve the borrowed asset
owner. Publication itself is not native draw admission. Rendering reports the
exact key, and simulation can consume that notice only once; this does not
write an original menu-readiness byte or execute a source controller.

## Mixed HP order and actual residency

HP appends icons and bar primitives into one vector at successful source emission
points. Digits for one unit remain adjacent to its background/highlight/fill,
before the next unit's digits. Grouping all icons or all bars loses this order.
The stable HP sink retains its pass so a held old callback cannot append to a
later pass. Caller packets are copied before residency/view proof callbacks.

Icon admission requires source Camera8, finite geometry/raster state and supported
material/pipeline profiles. Native viewport/scissor coordinates are tilted
240x400. Source icon extents 1..1020 receive two texels per edge and round up to
a power-of-two backing from 8 through 1024; native format 0 is RGBA8. Inline
comments map admitted GX comparison, cull and blend tokens. These are native
support policies, not permission to fabricate missing source state.

Each icon retains the actual texture identity, generation and image/backing
metadata. Preflight queries residency again and compares every field. HP bars
instead retain their HP resource generation plus a projection-only raster view:
bar vertices already contain model/view, so reusing a preceding number icon's
combined transform applies those factors twice.

Callbacks cannot reenter mutation. Liveness loss or requested retirement defers
total clear until the guard unwinds. Ordinary resource unavailability holds
publication without replacing the old frame. A complete empty pass intentionally
publishes no geometry. Validation requires the actual published object; a copied
struct with identical fields does not prove source completion. Residency pins
cover both building packets and the previous published frame.

## Font reservation is an ordering barrier

Weapon publication has five typed leaf alternatives: font, submenu, icon,
window and class surface. The vector retains successful source emission order.
Capacity is reserved before source glyph RNG. A glyph reserves its ordered slot
before the original kernel's two random calls, then commits through a nonfailing
boundary. A pending reservation blocks sibling appends and sealing.

An unchanged reservation may retry, but float operands are compared by bits;
positive and negative zero are different requests. Commit cannot allocate,
call providers, fail, reenter or tolerate resource retirement. A violated atomic
contract aborts rather than dropping a glyph after its RNG effects. This is an
implementation contract, not a change to original RNG or action timing.

Submenu/class packets must retain the binding's actual common-bank owner,
including its shared ownership control block, pass and allocation generation.
A window has its own genuine global setup/palette/object lifetime, so it need
not impersonate that common-bank owner. Each typed validator checks its own
resource proof. Icons query actual import residency. Missing optional window/
class resources hold those leaf requests.

## Seal, validate and submit have different proof requirements

Append, seal and submit require the executing source pass as well as long
allocation/source-bank lifetime. Validate uses the long proof and typed resources,
so copied geometry may survive legitimate camera finish. Observed long lifetime
loss is sticky. A fresh pass cannot replace a pending glyph or unsubmitted batch.

Seal copies accepted entries and returns the same immutable batch on successful
retries. Both typed and opaque submission paths require exact object pointer
and shared ownership identity; copied structs and unrelated same-address aliases
are rejected. Preflight repeats before GPU work. Finite icon/submenu inputs also
receive checks on every homogeneous clip product and sum; no W division,
visibility culling or source RNG is added by those guards.

Exactly one native submission callback is bound. Nonfailing `draw` commits the
whole ordered batch synchronously. Alternative `tryDraw` may return false only
before every presentation effect. The caller then retries whole native preflight
and submission, without repeating source work or a GPU prefix. A successful
submission records once; duplicate calls still check currentness and resources
but do not draw again. The owner does not free native textures or establish
hardware timing, normal source scheduling or original raster parity.

See [ordered HP publication](../behavior/display/ORDERED_ENEMY_HP_PUBLICATION_AND_NATIVE_ICON_GENERATIONS.md),
[ordered weapon publication](../behavior/display/ORDERED_WEAPON_MENU_PUBLICATION.md)
and [GPU resource ownership](battle-gpu-resources.md).
