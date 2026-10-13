# Original item presentation metadata

US G8ME01 revision0; read-only static recovery2026-10-13. Supplements the
[item-table window](../message/ITEM_TABLE_SELECTION_WINDOW.md) for
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md). This is metadata/layout
recovery, not original execution, native draw parity or hardware performance.

## Table extent and consumer fields

Original symbol itemDataTable is803108a8, DOL-relative file offset0030d8a8,
size3520hex, from item_data.o. Its stride is28hex (40decimal) bytes:340
physical records, IDs0..339, exclusive end80313dc8. The next symbol at that
address is itemdrv_window_desc, sizea0. Do not extend indexing into that next
object or treat a U16 row identity as proof of table membership.

| Offset | Established presentation field |
| --- | --- |
| 00 | Debug item-name pointer; not the selector's displayed US name |
| 04 | Pointer to original name message key |
| 08 | Pointer to original description message key |
| 20hex | U16 icon ID, not a TPL image index |

Fresh select_disp8023bc74/df8 reads row identity with lhz at8023bd8c,
multiplies by40 at8023bd94, reads icon+20 at8023bf64, reads name+4 at
8023c02c and calls msgSearch80081b8c at8023c074. Description callback
select_disp3 at80239980/80 reads row+2 at802399cc, multiplies by40 at
802399d0, reads record+8 at802399d8 and calls msgSearch at802399dc.
These consumers have no local item-record count guard. Their original domain
is valid supplied rows; a native packed-file reader must separately validate
its own extent and explicitly distinguish unavailable metadata.

## Complete static census

A fresh bounded ISO census of every physical record establishes:

- IDs0..338 have nonnull name and description pointers to terminated keys.
  Every one of these keys occurs in msg/US/global.txt; none is missing from
  the US resources, and none additionally occurs in another US message bank.
- Record339 is entirely zero, including name/description pointers and icon.
  It belongs to the physical extent but supplies no presentation keys. This
  is observed data, not evidence that339 is a selectable item or named cancel
  row. Do not send its null keys to msgSearch or silently substitute item0.
- Item0 has name in_unknown_item and description msg_unknown_item, icon0.
  Item338 has name and description both in_unknown_item, icon96. Preserve
  this different description key rather than normalizing it to item0.
- All340 icon fields identify in-bounds descriptors with in-bounds frame
  arrays and TPL image references. There are305 distinct referenced icon IDs,
  spanning0..414. Items121,123,124,125 have multiple frames; a universal
  first-frame-only conversion would discard their original animation.

[Message resource loading](../message/MESSAGE_RESOURCES_AND_LOOKUP.md)
establishes msgInit loading global into slot1; msgSearch searches current
slot0 then slot1, preserving precedence and original missing-key fallback.
A global-bank census does not authorize changing that runtime lookup order.
It establishes that the retained global resource suffices for these original
keys when the ordinary current-room bank does not override them. Preserve US
text bytes and tokens; this contract copies keys, not displayed prose.

| ItemID | Name key pointer / key | Description pointer / key | Icon / descriptor / static TPL |
| --- | --- | --- | --- |
| 95 | 802c54c4 / in_irai_butsu | 802c54d4 / msg_irai_butsu | 307 /12f2 /312 |
| 107 | 802c56e8 / in_fighters_pants | 802c56fc / msg_fighters_pants | 308 /12fe /313 |

icon.bin contains8174bytes, first U16=572; original descriptor offsets are
U16 entries at2*(iconID+1), then descriptor loop/count and frame(image,delay)
metadata described in the window contract. icon.tpl contains613 images. The
complete item-field census checks both descriptor extent and every referenced
image against that TPL count; it does not infer image number from icon ID.

## Evidence and native recommendation

SHA256 of the exact itemDataTable3520-byte span:
cd9e52c1f0946f785301879d0e9560a5b87860abd648e9721973159291d0add9.
The original symbol CSV SHA256 is
6c3899e26338e098e50e688bb1a2afb9488180f6551f256418179a036f2fe1bd.
Fresh consumer spans: select_disp8023bc74/df8 SHA256
 d1f964b553eb903b16d2f162a5b2f9cb521a513d6ec9ed06235d6027b2335e89;
select_disp3 at80239980/80 SHA256
01d57c3482bb8c38c985968d8defc0c592bf8a9d2f4e2a46d2f23c735384c4f3.
Original msg/US/global.txt239847bytes SHA256
119158be9099d7d92ebabb9aa2e501a2908b83d9ebd9e60031c3ef96f7bb3280.
icon.bin SHA256
0be6b95a7e45e5d932641f24f8240a289a852889f1906d32be839046a680532b;
icon.tpl677792bytes SHA256
fb8e99398fd5ef3c36cc518077748405949c004fcc5c515cc8828a64340b74f9.

Inspection script, complete metadata-only census and consumer ELF/listings stay
/tmp/item-metadata-inspect.py, /tmp/item-metadata-evidence.json and
/tmp/item-metadata-select*. No original assets or dialogue prose committed.
Source ISO was opened rb only. Assets remain Nintendo/Intelligent Systems
resources without redistribution permission; local generated packs stay ignored.

Use a versioned, extent-validated native metadata pack retaining original IDs,
keys and icon IDs, with explicit availability for339/out-of-range values. Resolve
keys through the existing room/global original message service. Draw retained
original icon frames through a shared catalog; bind actual game animation clocks,
list/header/help drawing and native event/window ownership separately. This
metadata recovery does not close those UI/backend/runtime dependencies or
OW-INTERACT-01. No source code, shared index/PROGRESS/build/profile/save changes.
