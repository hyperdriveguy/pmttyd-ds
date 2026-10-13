# Native original item presentation catalog

Implements the recovered [metadata fields](ITEM_PRESENTATION_METADATA.md) for
the [explicit item-table window](../message/ITEM_TABLE_SELECTION_WINDOW.md).
This is original resource admission and a reusable native lookup service;
complete item-window drawing and Goomba quest execution remain unbound.

`tools/item_presentation_assets.py` opens the US revision0 ISO read-only,
maps original DOL sections and verifies the complete itemDataTable SHA256.
It exports all340 physical records in original ID order, retaining name+4 and
description+8 pointer identities, their original terminated ASCII keys and
iconU16+20hex. It does not copy the debug-name field, weapon pointers or other
item logic into this presentation pack. Record339 retains null pointers/keys;
it is not a generated fallback or cancel item. IDs0..338 use unchanged original
keys through the existing room-first/global-second message lookup.

## Pack and ownership

Ignored `romfs/item-presentations.bin` uses little-endian native pack version
PMITEM01. Header is8-byte magic followed by WORD340, WORD803108a8 and WORD40.
Each record contains WORD name pointer, WORD description pointer, U16 icon,
U16 name byte length, U16 description byte length and U16 reserved0, followed
by each nonnull key including its terminator. A zero source pointer has length0;
a nonnull empty string has length1. They remain distinct. No alignment padding
is inserted between records. Original source pointers are identity metadata,
never host or ARM dereference addresses.

`ItemPresentationCatalog` rejects wrong headers/extents/reserved fields,
null/byte mismatches, embedded NULs, non-ASCII keys, truncation and trailing
bytes. Its256-byte key and200000-byte file bounds are native pack validation
policies, not measured original allocator rules. Failure preserves the resident
catalog and returned pointers; successful replacement invalidates old pointers.
Out-of-range lookup returns null. Record339 is in range but its `name()` and
`description()` return null; a consumer must not pass those to a message service.

The renderer loads the shared catalog once at startup. Direct ID lookup is
constant time with no file access, string allocation or linear table search
during rendering. The present pack is16578bytes. Metadata availability does
not mean every item icon has been uploaded; future area/event packs must admit
the original referenced icon frames before drawing them.

## Original selector art

The shared message/selection icon atlas additionally admits original IDs307,
308,445,446,447 through icon.bin descriptor lookup, retaining loop/frame/delay
metadata. Their TPL images are312,313,465,466,467 respectively. The first two
are40x40, arrows48x48 and stripe24x24. Native selector scale0.5 for the quest
items belongs to the future draw caller; this converter does not resize art.
All pixels fit the existing512x512 RGBA8 atlas with2px transparent padding,
so adding these resources does not increase resident GPU texture dimensions.
Tex3DS conversion and shared citro3d texture ownership remain the existing
backend. These additions do not replace the original System border geometry.

## Validation and remaining binding

Host tests cover explicit null versus empty-key identity, every original key's
US message lookup, both exact quest metadata records, malformed pack rejection
with resident pointer preservation, original icon dimensions/frame metadata,
and pixel equality between selected decoded originals and atlas regions.
Source exporter verifies the full original table hash and exact extent.

The list/header/description callbacks, scissor, font geometry/help pagination,
status/key ownership, complete sound macros, event continuation and full pouch/
switch persistence remain future bindings. No menu is newly enabled on screen
here. No matched original-runtime pixels, emulator capture or New2DS hardware
performance is claimed. These native adaptations do not close OW-INTERACT-01.
