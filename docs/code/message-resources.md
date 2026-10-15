# Message resource and native atlas ownership

Implementation: [resources](../../source/message/message_assets.cpp),
[resource API](../../include/message/message_assets.hpp),
[icons](../../source/message/message_icons.cpp),
[icon API](../../include/message/message_icons.hpp),
[font loader](../../source/message/message_playback.cpp).
Original lookup evidence: [resource contract](../behavior/message/MESSAGE_RESOURCES_AND_LOOKUP.md).

`MessageResource` retains alternating NUL-terminated key/text bytes and a sorted
index of byte offsets. It never sorts or rewrites the original bytes. Empty key
terminates the resource; empty text inside a pair, missing terminator or nonzero
trailing padding rejects the load. The 4 MiB file bound is a native guard, not a
recovered original allocation size. Parsing publishes the buffer and index only
after validation; failure preserves old data and borrowed pointers. Successful
replacement or destruction invalidates those pointers. This is transactional
publication, not a thread-synchronization guarantee.

Stable sorting retains duplicate keys so lookup can explicitly reject ambiguity.
Room lookup precedes global lookup; an ambiguous room key blocks global fallback.
Choosing another line would risk changing story attribution or conditions. A
missing/ambiguous result is null, not invented dialogue. Original IDs and all
control-token bytes remain part of the resource unchanged.

The font/icon packs are converted **little-endian native formats**, distinct from
original big-endian ISO resources. Loaders use `fread` of the documented layouts
on supported hosts/3DS, require exact EOF and publish only after full validation.

| Pack | Layout / admission |
| --- | --- |
| `PMFONT1` | Eight-byte magic, U32 count, 256 U16 code mappings, eight-byte glyphs |
| Font glyph | U16 atlas X/Y, U8 width/height and two padding bytes; count <=1024, nonempty cells <=24x24 within 512x256 atlas |
| `PMICON1` | Eight-byte magic, U32 count; per icon U32 ID/loop/frame count; frames are three U32 and four floats (28 bytes) |
| Icon frame | Original delay, width/height, finite normalized UVs with inverted V; 1..1023 unique IDs, 1..256 frames, dimensions <=512x128 |

Font loader requires every mapping to reference a resident glyph. Plain-font draw
helpers separately support manually supplied mapping `0xffff` as skip-without-
advance; that is not an accepted nonresident mapping in this loader. The 256-entry
map is the finite single-byte US domain, not arbitrary multibyte/Unicode support.

Icon views borrow a process catalog, invalidated by successful reload. `frame()`
requires nonempty frames, guaranteed by valid loaded icons. A looping icon uses
modulo total delay; a nonlooping icon holds its final frame. Zero-delay frames
consume no time, and all-zero total selects the first without modulo. Callers
supply icon tick coordinates; this lookup does not establish original wall-clock
or rendering cadence. Failed loads preserve the existing catalog.

No source art, dialogue, ISO bytes or conversion policies change in this
implementation guide. See [playback](message-playback.md) and
[font/window drawing](message-window-drawing.md) for the consumers.
