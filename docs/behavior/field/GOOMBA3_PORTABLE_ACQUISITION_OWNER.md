# Goomba3 portable parcel acquisition owner

Implementation: `GoombaAcquisition` in `source/field/goomba_acquisition.cpp`.
Source basis: [original quest caller](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md),
[item95 sequence](GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md),
[entry registry](ITEM95_ENTRY_NAME_REGISTRY_AND_ICON_DRAW.md), and
[field service order](GOOMBA3_ITEMGET_FIELD_SERVICE_AND_EVENT_RESUME_ORDER.md).

This is a host-testable acquisition owner, compiled into the native program.
Normal Game/NPC admission and native presentation services are not yet bound.
It does not establish playable quest2 or change ordinary dialogue selection.

## Admission and owner boundaries

The supported actor is plaza Goomba3 (room0, key42ff5f1f), with known quest1420=2,
5406=false and 5405=false. Required presentation callbacks must be available
before printing. `begin` prints the original US `irai_02_00` through the real
FieldActorTalk/MessagePlayback, retaining actor attribution and control tokens.
It does not allocate an item, grant it or change quest switches while printing.

The event-manager visit waits for message completion, then enters kind95 as
`irai_item` at (0,-1000,0), mode16, applies flag2 and forces sequence2. The first
wrapper visit returns without granting. A separate item-owner visit runs the
finite original flags1003/status0 acquisition sequence. The real pouch receive
occurs after both source windows have opened. Presentation callbacks see the
published entry substate/status/position at each source call boundary.

The owner supplies actual pouch insertion and name-wide registry deletion;
injected receive/delete callbacks are not used. Deleted entries fail the second
active gate before icon synchronization. Only a later event-manager visit can
observe poll result2 and write5405. No rendering-frame delay is assumed.

First-name lookup remains the registry's original behavior. An older duplicate
entry can be forced instead of the newly allocated record; its initial XYZ is
retained through pre-effect callbacks. Cleanup deletes all matching entries.

## Remaining frontiers

An unavailable registry slot retains entry-pending without a story write.
Unsupported live flags/status, alternate item modes, alternate field globals,
complete allocator/replacement behavior and discard UI remain unimplemented.
Full key storage enters the existing discard-pending frontier without granting
or writing5405. Missing providers refuse initial admission.

Game binding must keep pouch, switches, talk, registry and callback contexts
alive throughout the acquisition and any borrowed-window close lifetime. It
must call event/item services in source order, defer requested Mario motion15
initialization to the following Mario owner visit, and bind native camera,
control, effect, windows, icon and sound consumers. Source audio providers and
original M_I_U/icon resources already exist independently.

## Verification

`tests/goomba_acquisition_tests.cpp` uses the original extracted US font and
actor catalog. It checks print-before-entry, actual pouch insertion once,
preserved HP/FP, source window/effect order and callback-visible entry state,
camera5 icon synchronization, blocked window acknowledgement, A/B completion,
name-wide deletion, delayed5405, duplicate initial position, full key storage
and unavailable presentation. Native build validation does not imply scene
rendering, hardware performance or native acquisition playability.

Validated 2026-10-14: isolated review tree
`41bc4a4549509ca81bb70314b4858a53cd6ada54`, committed as `3b71d849`.
`make test` passed225/225 C++ and141/141 Python tests; `make all
BUILD=build-goomba-acquisition-review TARGET=ttyd-goomba-acquisition-review`
passed. The shared working-tree run passed227/228 C++ tests, failing unrelated
`trio_attack_original` at its pose assertion; it did not reach Python tests.
Independent read-only review found no concrete mismatch in this finite path.
`python3 tools/check_doc_links.py --quiet` passed before this validation append.
No Azahar/Dolphin acquisition capture or physical3DS performance measurement.

Ignored artifact: `artifacts/ttyd-goomba-acquisition-reviewed.3dsx`, SHA256
`dbe22d4fd3ceaea1c6eb5341166129542909626f667aaf2f6ccd106c5db7318b`.
Logs: `artifacts/goomba-acquisition-review-test.log`,
`artifacts/goomba-acquisition-review-native.log`, and
`artifacts/goomba-acquisition-source-audit.txt`. Reuses original extracted
`romfs/font.pmfont` and actor/message catalogs; no new payload extraction,
texture conversion, soundtrack replacement or license change.
