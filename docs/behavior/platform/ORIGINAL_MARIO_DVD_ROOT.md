# Original Mario DVD-root register leaf

`getMarioStDvdRoot80007388/8` is exactly two instructions:
`addi r3,r2,0x8054; blr` (`38628054 4e800020`). The add immediate is signed16
`-32684`; addition wraps in the original 32-bit register. It performs no memory
load, writes no memory, calls no child and has no mutable state. Therefore the
portable `sourceMarioDvdRoot(actualSourceR2)` needs no retained cursor or service
callback. It returns the actual arithmetic result, including zero.

Original `_init_registers` instructions at `80005418/8` initialize r2 to
`80427440`. With that actual admitted ABI, the return is `8041f494`, whose
initialized original data is `2e00` (`.` and NUL). This is source evidence, not a
guessed native root pathname. `OriginalMarioDvdSourceR2` records that source ABI
base; it does not assert that a particular VM has initialized its data mappings.
Neither the helper nor its caller should substitute a host cwd, SD/RomFS path,
cache-ready flag, selected map or native atlas status.

The pointer identifies original process initialized data. Its returned value is
not a copied/allocated string or a new ownership token. The real process/VM must
own the actual initialized source bytes for consumers to read; missing bytes
remain unavailable to those consumers. The getter itself still completes when
such a consumer mapping is unavailable, because the source does not read it.
The helper does not register, initialize, overwrite or return a C++ string.
If source data is legitimately mutable, later consumers read its actual current
bytes rather than a cached hardcoded `.`.

## Concrete UI callsites and formatting frontier

The complete original font Setup calls the getter at `80075024` before its
selected `%s/f/%s.bfn` formatter (`802c2ba0`, callsite `80075048`), then again
at `8007507c` for **each** `%s/font/%s.bfn` probe (`802c2bac`, `80075094`). The
actual window Setup's zero-archive branch calls at `8007fc18`, then formats
`%s/f/msgWindow.tpl` (`802c3094`, `8007fc30`). All three format children target
original `sprintf8026a0b8/e0`.

`BattleMenuFileBinding::dvdRoot` is a cache facade whose result is gated by
`cache.ready()`. It is not this source body and should not be reused as the
actual getter in the font/window graphs. No shared facade was modified here.
Its formatter admits only `%s/battle/common/battle_common.tpl`; animation
startup adapters admit their own literal/name cases. Neither is a reusable
whole original sprintf implementation for these UI callers.

[Original formatting output/ABI](../format/FORMAT_OUTPUT_RUNTIME.md) documents
the actual `sprintf8026a0b8/e0` wrapper, `__pformatter8026a454/774`,
`__StringWrite8026a390/6c`, varargs and termination behavior. A successful
bounded native C-runtime adapter may separately admit these exact finite `%s`
formats and actual source strings, but it must state that finite port domain,
retain the real private `SourceFontPath`/`SourceWindowTexturePath` destination
identity and suffix bytes, and hold unknown memory/overflow/alias domains.
Passing `sprintf` requests or constructing a plausible path is not whole
original printf/varargs/writer completion. No new formatter is supplied by this
leaf, and no root/global/font/window ready flag is set.

## Verification and limits

`verify_source_mario_dvd_root.py` opens only the original ISO read-only and pins
the whole getter, actual r2 initializer, actual root bytes, three UI formats,
six caller BL targets and the whole sprintf wrapper hash. Tests compare wrapped
arithmetic across zero/wrap boundaries and load the actual root/format bytes
from initialized DOL spans. Strict host, ASan/UBSan and ARM syntax checks do not
establish normal process startup, filesystem mapping or whole Setup readiness.
Root owns combined build/test registration for this NEW-only batch. No original
asset, dialogue, native resource, input or shared source file was changed.
