# Shadow driver accessor leaves

US G8ME01 disc0 revision0; static contracts, no execution, cadence or rendering
claim. Closes the thirteen `shadowGet*` bodies in `shadowdrv.o` from the
[unaddressed register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md). None has a
definition in the pinned snapshot (rechecked with `tools/reference_source_gap.py`);
the register previously carried no row for any of them because their listing
names occur in no prose.

Three small-data words select the shadow work records (`r13 = 0x8041cf20`):

| SDA address | Displacement | Work record |
| --- | --- | --- |
| `0x804150c8` | `r13−32312` | depth-shadow record |
| `0x804150cc` | `r13−32316` | projected-shadow record |
| `0x804150d0` | `r13−32320` | character-shadow record |

Two further small-data addresses hold static texture objects:
`0x8041e738` (`r13+6168`, 16-level ramp) and `0x8041e758` (`r13+6200`, 8-level
ramp). Every body below is a leaf: no stack frame unless noted, no calls except
where stated, and no bounds/sentinel checks.

## Fixed-address ramp texture objects

| Body | Address | Size | Returns |
| --- | --- | --- | --- |
| `shadowGetRamp16TexObj` | `0x8002bc6c` | 8 | `0x8041e738` (`addi 3,13,6168`) |
| `shadowGetRamp8TexObj` | `0x8002bc74` | 8 | `0x8041e758` (`addi 3,13,6200`) |

## Matrix, projection and colour pointer getters

| Body | Address | Size | Returns |
| --- | --- | --- | --- |
| `shadowGetDepthShadowDepthMtx` | `0x8002bc0c` | 12 | depth record `+172` (`+0xAC`) |
| `shadowGetDepthShadowProjTexMtx` | `0x8002bc00` | 12 | depth record `+124` (`+0x7C`) |
| `shadowGetCharShadowProjTexMtx` | `0x8002bc94` | 12 | character record `+124` |
| `shadowGetProjShadowProjTexMtx` | `0x8002baf0` | 12 | projected record `+124` |
| `shadowGetCharShadowColor` | `0x8002bc88` | 12 | `word[character record + 256]` (a colour pointer) |
| `shadowGetProjShadowColor` | `0x8002bae4` | 12 | `word[projected record + 256]` |
| `shadowGetCharShadowProjection` | `0x8002bc7c` | 12 | `word[character record + 268]` (`+0x10C`) |

The three `ProjTexMtx` getters share the same `+124` offset, so the projected
texture matrix lives at the same relative position in all three records.

## Indexed configuration getters

`shadowGetDepthShadowConfig` (`0x8002bb50`, 13 lines) and
`shadowGetProjShadowConfig` (`0x8002bac8`, 13 lines) are identical except for
the SDA word they read (`r13−32312` versus `r13−32316`):

```
record = word[SDA];                  // depth or projected shadow record
index  = word[record+8];
return 0x80304a60 + 72*index;        // lis 0x8030 + addi 19040, mulli 72
```

So the configuration tables are 72-byte entries in a base table at
`0x80304a60`, selected by one word inside the shadow record. No bounds check
exists; an out-of-range index yields a pointer past the table. There is no
character-shadow counterpart body.

## Texture-object getters with a ready gate

`shadowGetDepthShadowTexObj` (`0x8002bc18`, 22 lines),
`shadowGetCharShadowTexObj` (`0x8002bca0`, 22 lines) and
`shadowGetProjShadowTexObj` (`0x8002bafc`, 22 lines) differ only in the SDA
word (`r13−32312`, `r13−32320`, `r13−32316`):

1. `flags = word[record+0]`; `clrlwi. 0,0,31` tests bit 0. If clear the body
   returns NULL and makes no call.
2. Otherwise `id = word[record+252]` (`+0xFC`) and it tail-calls
   `smartTexObj(record+220, id)` (`0x8002f33c`, `memory.o`, 60 bytes), returning
   that callee's pointer.

So the texture object is obtained lazily through the smart allocator, guarded by
bit 0 of the record's first word.

## shadowGetDepthShadowColor (0x8002bb6c, 94 bytes, 43 lines)

Packs a faded colour from the depth record's bytes into one word. Inputs are
`byte[+256]`, `byte[+257]`, `byte[+258]` (components) and `byte[+259]` (alpha).

For each component `c`, the body computes `(255 − c) * alpha / 255` using the
signed-division-by-255 idiom with the magic constant `0x80808081`
(`lis/addi` sequence producing `0x80808081`), `mulhw`, an add, `srawi …,7` and
the `srwi …,31` rounding correction. The three results are stored to stack
bytes 8, 9 and 10, byte 11 is explicitly zeroed, and the word at byte 8 is
returned — so the packed word's most significant byte is the faded value of
`byte[+256]`, then `byte[+257]`, `byte[+258]`, then `0`.

This is the only body of the thirteen that builds a value rather than returning
a pointer, and the only one with a stack frame.

## Not established

- Record layouts beyond the offsets above, and what the depth/projected/
  character records represent in rendering terms.
- The `72*index` table's meaning at `0x80304a60` and the valid index range.
- `smartTexObj`'s allocation policy, the callers of all thirteen bodies, and
  any on-screen effect. No rendering or original execution comparison is claimed.
