# Weapon source memory and retained texture constructors

[Source memory](../../source/display/battle_weapon_source_memory.cpp) routes
reads/stores through actual startup and destination VM owners.
[Source textures](../../source/display/battle_weapon_source_textures.cpp) executes
the retained TEX/GX constructor into explicitly reserved private storage. Neither
native atlas readiness nor a copied descriptor replaces those source owners.

## Ownership has first refusal

owns(address,size) means any intersection with an owner, not that every byte is
readable. Startup allocations, including unknown tails and partial overlaps,
have first refusal. Any destination VM intersection comes next, even if a semantic
reader cannot provide that value. Only fully unmapped requests may use immutable
literal fallback, and the whole read must fit one admitted span. Unknown bytes
cannot fall through to a more convenient owner.

The three original literal spans are 0x802c2b30/144 bytes, 0x804203e4/96 bytes
and SDK min-filter table 0x8041e558/8 bytes. Each has its own FNV-1a64 fingerprint:
offset basis 14695981039346656037, prime 1099511628211, wrapping modulo 2^64.
Literal reads assemble big-endian values; the spans are not writable RAM.
Stores admit widths 1/2/4 only and target actual mutable font or VM storage.
Font writes preflight all bytes before a multi-byte update.

Creation binds the supplied genuine GP-language alias after startup/font/literal
preflight. Global 0x80415fd0 selects the destination GP; its +0x16c lane shares
the actual completed source process's language owner. It is not a copied private
language value. The same installed alias may be reused; another source binding
cannot replace it. Alias binding is persistent destination state; a later create
failure does not roll back the installed alias. GP/font/startup/language identity
is rechecked during work;
observed loss is sticky. Reentry returns unavailable. Total actor callbacks abort
on missing asserted-domain storage, while optional reads represent real holds.

## One private output, with initially unknown bytes

The texture service admits a nonzero 32-byte output span outside the thirteen
0x400-byte camera slots and all actual source owners. Wider arithmetic rejects
wrap across 2^32. The private span is an adapter policy, never an inferred guest
stack address or texture-index handle. Unknown private bytes become known only
through actual constructor clear/stores. A retained byte cursor preserves clear
progress, and partial private intersections cannot fall through to source memory.

Long currentness proves allocation/common-bank ownership and continued range
exclusion. Short sourceCurrent additionally proves the executing callback.
The genuine palette comes from B+0x163f8 FileEntry, +0xa0 holder and its pointer
cell. Indices 0..3 cover class I4 and submenu CMPR. No native atlas creates a
missing palette, source header or SDK object.

## Capture original constructor operands at their read sites

Thirteen metadata fields are observed at the original TEX read PCs, including
LOD/filter/bias reloads after GXInit. observed_ == 8191 (0x1fff) means every
required field was read. This mask is native capture bookkeeping, not source
readiness. Rereading headers afterward would lose the original operand history.
The extra CLUT-pointer check is native resource admission: each eight-byte TPL
descriptor has its palette pointer at +4, and these surfaces require it zero.

Double reads join high/low big-endian words with wrap protection. The unsigned
byte subtraction provider admits only binary64 `(2^52 + U8) - 2^52`, bias
0x4330000000000000. The masks reject every other domain; returning exact float
U8 bits here does not establish generic PPC/FPSCR conversion behavior.

A held constructor fixes palette/index. Once complete, its same known words and
metadata remain available until actual downstream acknowledgement; clear, stores
and numeric calls do not rerun. The witness checks output address, callbacks,
shared ownership identity, all known words and exact metadata. Bias equality is
bitwise, preserving signed-zero distinctions. After downstream acceptance,
acknowledge permits another original TEX call to reuse the private cell. Class
LoadTexture snapshots independently preserve earlier constructor results.

These services do not supply a VM execution receipt, source UI readiness, normal
menu activation or GPU presentation. The caller must keep source owners stable
through synchronous work. See [the concrete binding contract](../behavior/display/NATIVE_QUEUED_WEAPON_MENU_BINDING.md),
[surface snapshots](battle-surface-packets.md) and
[weapon frame ownership](weapon-frame-ownership.md).
