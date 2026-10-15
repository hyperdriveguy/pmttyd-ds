# Native common-menu file ownership

`BattleMenuFileCache` supplies real BE manager, 1024-entry pool, file holder and
original common-menu TPL backing to the tutorial VM. It executes the retained
source fileInit, file acquisition, type4 Set relocation and fileFree controllers.
See [source resource calls](../battle/NATIVE_CLASS_DRAW_CONTINUATIONS_AND_RESOURCE_OWNERS.md)
and [bank packaging](BATTLE_CLASS_COMMON_TPL_PACKAGING.md).

The DVD root string at8041f494 comes from the actual imported DOL data.
The file owner does not shadow that original data with an overlapping mapping.
Shared VM use is sequential simulation ownership, not a concurrent file service.

The native owner supports exactly `./battle/common/battle_common.tpl`, type4.
It admits the original bank size/header and validates the relocator's relative
accesses before exposure; the converter additionally pins the full original
SHA256. Unsupported content remains unavailable. No mounted archives is explicit
native resource policy; it does not establish the original archive population.
ROMFS reads and one fixed native smart-header allocation replace disc/heap I/O.
The fixed native tokens in 87000000..87400000 are actual backed objects, not
claims about original heap placement or complete file-system service coverage.

Acquisition uses real source state/count/name matching and list publication.
Multiple VMs observe the same descriptor and source reference count. End runs
source fileFree: exact count zero moves active state1 to state2, retaining the
holder and relocated payload. Reacquisition does not relocate a second time.
No destructor synthesizes an End call or decrements a reference. External RAM
registration is transactional, and the noncopyable owner retains stable backing
for every bound VM. Asset reload creates a new owner; existing VMs retain theirs.

`HarborTutorial` loads the exact ignored bank and binds the process-owned
catalog to each actual VM before script import. Its explicit
`initializeClassMenuResources` and `endClassMenuResources` calls execute the
whole source Init/End over that VM's actual current battle and source strings.
They are scheduler call boundaries, not a replacement BattleInit/BattleEnd.
Ordinary battle entry/teardown must still call them in recovered source order;
rendering and development menu input do not invent those calls. Source global
file ownership for other model/area resources remains unfinished.

Host tests exercise all 97 original image/palette descriptors, shared references,
state2 reuse, two simultaneous VMs, VM retirement, asset reload, registration
rollback and malformed-bank admission. The tests establish native ownership and
source controller composition, not normal scheduler integration, GPU parity or
physical 3DS performance.
