# Original map loader light children and ambient storage

The original map continuation now routes reached lightEntry (8001bd98) and
lightReInit (8001c1d8) to an initialized FieldLightRegistryOwner supplied through
FieldMapHeapOwner::lights. Both owners must use the same EventAddressSpace and
remain stationary through held calls and borrowed renderer records. An absent
owner retains the existing SDK boundary; no default successful child is installed.
See [registry ownership](NATIVE_LIGHT_REGISTRY_OWNER.md) and
[map loading](NATIVE_SOURCE_MAP_LOADING_AND_DVD_WORKER.md).

The bridge forwards the actual source name, type, parent-stack XYZ and RGBA,
relocated row vector, attenuation, flags and f1/f2 bits. It does not rescale the
position, decode another map table or invoke loadMapLights. The original parent
already executes the complete loop. An engaged Entry index zero acknowledges
completion. A held raw call pins its call object and operands; another call or
changed operands cannot consume it. Original Entry owns its own retained effects.

The process light owner also maps exactly eight shared mutable ambient bytes at
8041e6d4..db, initially BSS zero. They are independent of the record banks and
imported immutable defaults. Original _mapLoad loads both defaults, then stores
paper_ambient at e6d8 followed by paperCraft_ambient at e6d4. Both are written
for secondary loads too. No adapter prewrites these values. Light reinitialization
clears only the selected bank; it does not clear ambient words or presets.
Future material consumers must read these actual shared words.

Source owner identity checks include the supplied light owner and exact VM.
Same-address replacement of any process light/ambient mapping invalidates that
ownership. Destruction unregisters only the owner's actual backing, preserving
foreign replacements. Keep map users and borrowed consumers frozen during unload;
source reaches light reset after hit reset and before environment/camera unload.

Static source evidence: original _mapLoad 8002951c..800296e4, retained in
artifacts/field-map-loader-evidence/_mapLoad.txt, and original registry evidence
in artifacts/field-light-owner-evidence. The existing compiled parents preserve
source order. Tests load the real unrelocated harbor files through one parent,
construct all 98 hit objects, create five original light records, publish ambient
and fog state and stop at camLoadRoad(4,"./m/gor_00/c"). G0 remains zero. Tests
also cover duplicate raw entries, held-name repair/caller isolation, group1 reset,
shared record pointer identity and ambient replacement. Native estimates used by
the collision fixture remain an explicit approximation, not original math parity.

This binds source children and process storage. Playable Game/EVT map epochs,
camera road ownership, texture/material/GX capture and active hit lifetimes remain
unfinished. No normal harbor GPU lighting, original pixels or hardware timing
claim follows from the fixture. Exact validation is recorded in
[progress](../../PROGRESS.md).
