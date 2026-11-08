# Harbor live BattleWork, unit and part addresses

## Address correction scope

`HarborTutorialPacked::battleAddress()` reads the actual US source global
`8041eb90`. `unitAddress(id)` resolves that BattleWork's 64-entry registry at
`B+20+4*id`, using the existing source registry helper and its ID bounds. A
registry ID is not an allocation address. Runtime actor lookup, installed-event
handles, physical/private pads, TimingA work, AP-report gate and the old
immediate class/weapon/command/audience adapters now use these live addresses.
The existing dynamic lethal-damage code already resolves unit and body-part
chains; it is preserved.

The constants `84000000`, `84100000` and `84200000` remain only as the legacy
native provisioning and `registerActor` allocation policy, or explicit
admission of those same fixed fixture allocations when no map token exists.
They are not source aliases for later ColdInit allocations. The four typed
native actor/damage slots and legacy Mario ID 1 admission are unchanged; this
patch does not infer a source ID from kind, reorder actors or authorize the
legacy harness as the original scheduler.

Actual source MapAlloc allocations expose their existing lifetime token via
`sourceAllocationLifetime(address)`. No generation is minted from pointer
values, polling, flags or copied actor contents. Same-address free/reallocation
has a different real token. Address-only correction does not create an
allocation, map an old alias or initialize unknown memory.

## Retained boundaries

The native TimingA manager captures the actual B and its allocation token with
its admitted physical sample. A held Main or display registration cannot
switch B or accept another allocation at the same address. The manager's
existing source cursor still retains its original successful work; a stale
invocation holds without resetting its queue, replaying its pad update or
writing the replacement's callback cells. The completed-sample/display
idempotence paths check the same identity before acknowledging a retry.

The old retained attack-selection adapter similarly pins the actual B,
registry-selected Mario unit, and their real allocation tokens. A held
selection cannot silently continue on another unit or another map allocation.
This does not add a message provider or complete any missing audience child.

The native status reader captures B and its real token when created. Its
current predicate checks the source global, token and actual contiguous work
range before entering its optional source readers. Replacement/free of B is
unavailable, including same-address reuse. A B proof alone does not establish
an arbitrary unit/pouch asynchronous lifetime: callers must still serialize
source retirement while the native status reader is held. Extending those
actual child receipts is a separate owner task.

## Projection ownership proposal; not activated here

A **separate, unapplied projection-address journal** makes `publish` and
`capture` resolve each admitted actor through the real
registry rather than the former `Units+id*1000` formula, and camera/declaration
fields through actual B. That candidate preserves field lists, `registered[4]` legacy admission,
write directions and call cadence are unchanged. Null live slots are skipped.
The independently integrable runtime/token journal leaves both methods
unchanged. Projection remains selective, not a whole-structure writeback.
The combined private fixture tests compile the explicit
`HARBOR_TEST_PROJECTION_ADDRESSES` switch; the default registered runtime test
does not activate that candidate.

There is a separate unresolved ownership problem: `HarborTutorialRuntime::call`
currently publishes scene values before invoking packed event callbacks.
Once original Main/Init/Turn owns the source state, this can overwrite source
turn/flags, actor HP/move counters/work, installed scripts, position and home.
Cold UnitEntry also does not imply legacy `registered[]` membership. Address
correction does not authorize those writes or fix that ownership gap.

The source-controlled policy should be explicit: use scene-to-source publish
only during the admitted legacy fixture path; after genuine source startup,
make source memory authoritative and capture presentation from actual source
registry identities. Actor visual-slot mapping must come from actual
constructor/registry evidence. Never set `registered[]`, fake source actors,
copy source menu state from overlay state, or normalize unknown fields to open
a menu. Root Main/Draw ownership must freeze simulation/input admission across
held display callbacks. No such activation or cadence change is included in
this patch.

## Typed damage/payment lifetime frontier

The current dynamic damage path reads typed snapshots in `readDamage(id)` and
later publishes selected fields through `writeDamage(id)`. It requires a
serialized interval with unchanged B/unit/body allocation identity. Its two
callers cannot express a new unavailable result safely:

- `WeaponCostServices::setFp` is void. Pay may already have effects and continues
  source count/AP accounting after the callback.
- `DamageApplicationServices::hitEvent` is void. Damage application may already
  have RNG, HP/accounting and reaction effects before this callback.

Changing only `writeDamage` to return false, discarding stores, or retrying the
outer synchronous call would acknowledge/replay a partial operation. A future
retained whole payment/damage owner must pin the original B/U/body and actual
allocation lifetimes and retain its child progress, or supply a genuine
serialized no-retirement interval. This patch preserves the existing invariant
and introduces no new production abort or detached output writes.

## Excluded owners and validation

Factory, Renderer, Game activation and source display binding are separate
pending batches. Their old direct drawing/presented helpers are not rewritten
here. A presented overlay still cannot certify original source readiness.

The test is an explicit legacy-parent relocation fixture. It runs the actual
VM, original assets and real nested source map allocator, copies that fixture's
already-known work to real allocations, and rewires its actual registry/part
links. It does not claim original ColdInit/UnitEntry execution. Tests verify
relocated B/U/P reads and writes, fresh registry replacement, original event
actor lookup, source physical work and retained TimingA/status rejection after
real free and same-address reuse. The reuse test retires the actual adjacent unit allocation as well, allowing
the original free path to coalesce enough space for the allocator's extra
header. It never forces an address or changes allocator metadata. The private build compiles every Packed consumer and the
actual Harbor runtime together with the changed header/storage, without
executing an old Game/Packed ABI. Parent full host/native gates remain required.
