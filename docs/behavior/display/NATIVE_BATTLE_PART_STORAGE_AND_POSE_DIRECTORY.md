# Native battle part storage and original pose directory

2026-10-14. Native projection of the read-only US definitions; see
[original construction and release](BATTLE_PART_POSE_ENTRY_RELEASE_AND_OFFSET_SERVICES.md).

Mario's original kind `80372afc` has three parts from `80372a18`, with
`4c`-byte source definitions. Goombella's kind `80393bec` has one part from
`80393ba0`. The former native `500`-byte part spacing within each `1000`-byte
unit partition put Mario's third part at Goombella's primary address.

The native address projection now retains Mario's primary `84201000` and
Goombella's primary `84202000`. Mario's additional parts occupy the unused
unit0 partition at `84200000` and `84200500`. Live next pointers retain source
part order; source-definition and unit-owner pointers remain distinct. These
are native projected addresses, not original allocator results. Existing
linked-part gauge/color initialization reaches all three parts.
The native address-space owner retains the whole mapped parts region. This
projection does not reproduce the original contiguous BattleAlloc block or its
BattleFree operation; source unit teardown still needs its own real provider.

`tools/battle_pose_directory.py` retains original model/texture header names
and every descriptor name in original order. The private `PMPOSE01` sidecar
contains eight magic bytes, two zero-padded ASCII fields of 64 bytes, a LE32 count,
and count zero-padded 64-byte names. It carries names/index identity, not source
pointers, executable code, descriptor geometry or a complete source file ABI.
Malformed extents and unterminated/empty names fail conversion.

Mario's source directory contains 85 descriptors and begins `M_Z_1`; Goombella's
contains 32 and begins `PKR_Z_1`. The converter now includes both genuine default
poses in the existing converted battle packs. Full source descriptor order is
independent of the converted subset. Mario's additional parts request
`KRN_H_1`, which is absent from `b_mario`: original name selection falls back to
descriptor 0, `M_Z_1`. A known source pose absent from the converted subset must
remain an unavailable evaluation provider; it must not be treated as an unknown
source name and silently replaced with descriptor 0.

This change does not yet register real battle pose handles or bind the original
animation owner. The existing display boundary at `animPoseSetLocalTimeRate`
remains pending. Next work is retained pose/resource/texture ownership,
per-part entry, exact selection/restart clocks and release counts. Renderer
animation playback remains the separate native presentation adapter.
