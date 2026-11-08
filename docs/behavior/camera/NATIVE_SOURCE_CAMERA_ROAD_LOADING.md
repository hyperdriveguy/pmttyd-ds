# Native source camera-road loading

The compiled map graph includes complete original camLoadRoad8000f36c/e8,
camRoadSetup8000e104/284, camUnLoadRoad8000f310/5c, camSetTypePersp8000e8c0/1c,
camShiftInit800f3978/64 and mapGetDvdRoot8002b464/8. All source instruction
words are checked by tools/verify_field_map_source.py. This is static US
revision0 evidence, not an original runtime/pixel or hardware timing oracle.

## Real process and parent ownership

FieldCameraRoadOwner owns exactly road BSS803c93e8/210, shift BSS803e1b30/38
and a separate native private stack. Original initialized pointer words at
80414fa8 and804169c8 come from the editable source literal pack. Camera4's
actual existing table slot803c9608 and260-byte entry are borrowed with physical
backing identities. Native admission keeps that table slot/entry stationary.
The owner never creates, zeros or substitutes a camera entry. [Normal Game camera
bootstrap](NATIVE_SOURCE_CAMERA_BOOTSTRAP.md) now executes genuine camInit/
camEntryPersp in the same VM and supplies this actual borrow. Fixture-initialized
camera storage is still distinct from that normal process admission.

FieldMapHeapOwner::cameraRoad supplies the same-VM owner to the original parent.
Reached load/unload/perspective wrappers remain external child boundaries. The
owner resumes its own stationary compiled graph cursor using the parent's exact
child services, actual primary map heap and existing file worker. It never
recursively invokes the already-active map cursor. Both stacks, call objects,
providers, process allocations and borrowed camera survive all holds. A foreign
call or changed operands cannot consume the pending invocation. Identity checks
remain safe inside existing synchronous SDK raw RAM borrows.

## Preserved source effects

Load captures the existing camera, clears flag10 and follow88/8c/90 before Setup.
Setup clears all road work, writes its source tuning/status words and initializes
shift before archive/DVD acquisition. Shift Init preserves timestamps18/1c and
other unlisted bytes. Unavailable children retain these reached effects; genuine
Open0 executes the original failure branch after them. Native missing service
is never converted into a successful or failed acquisition receipt.

Archive lookup uses actual803dbff0 descriptors. The DVD fallback uses the shared
original manager/worker and source map allocator, publishes the actual container
pointer before the rounded read, and queues source Close. The unchanged harbor
c file is2048 bytes, already32-byte aligned. The native catalog now admits d/t/c;
other rooms still require their actual original transfers and owners.

Relocation tests only the first unsigned header pointer at container+e8. When
relative, all nine e8..108 words receive the base with32-bit wrap, including
absent-collection sentinel words. Exact version1.01 selects101; other original
versions select100. The wrapper preserves unrelated camera lens/view/event
history. Success selects mode1/flag4; real failure selects mode3/cleared flag4.
Both execute the original local resets. Load returns0 on both branches; Setup
returns1/0. Unload and perspective return the selected camera pointer. Unload
does not free the container or clear road/shift state; freeze borrowers before
the actual outer map-pool retirement.

## Inputs, validation and remaining work

Original assets are under assets/original/maps/harbor-camera-road with per-asset
licensing/provenance. New imports require an empty destination and preserve
existing edits. Normal builds compile these recipes after the ISO-backed base
pack; no runtime camera objects or generated pointers are imported. See the
[asset ledger](../../ASSETS.md), [camera lifetime](OVERWORLD_CAMERA_MAP_LIFETIME_AND_DUPLICATES.md)
and [map loading](../field/NATIVE_SOURCE_MAP_LOADING_AND_DVD_WORKER.md).

Host/source and isolated ARM validation details are recorded in
[progress](../../PROGRESS.md). A real source harbor parent is exercised with
explicit fixture-owned preinitialized camera storage. This is not normal Game
map activation. Normal Game camera CPU bootstrap and actual road borrowing are
now composed; camera solver/Main/GPU consumers, material/display-list and later
shadow services, Game/EVT map epochs and full prologue remain required.


The reached later map tail also consumes the original rodata prefix at
802bf958..fb97. It is retained in the same editable literal pack, adjacent to
existing loader literals without duplicating their editable bytes. No-fog color
80429520 instead belongs to mapdrv .sbss2. FieldMapSourceOwner owns that exact
four-byte cold process BSS and preserves it across rooms; original BSS-table
provenance is under artifacts/field-map-camera-road-evidence/no-fog-bss-word.md.
The harbor uses authored fog, so its default branch is separately covered by
storage/lifetime checks rather than asserted as an original harbor trace.
