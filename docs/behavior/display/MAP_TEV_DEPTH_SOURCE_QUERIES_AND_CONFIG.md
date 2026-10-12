# Depth shadow source queries, inverted color and configuration witness

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original CPU/GX execution or observed shadow images.
`tools/verify_map_tev_depth.py` checks1115 newly reviewed depth/source words,
eight identities(one containing TEV/seven complete source leaves),six original
spans,two US lecture imports,six direct DOL sites and65536 host byte products.
It also rechecks six contributing manifests covering all2927 TEV words. This
supports composed local REQUEST contracts,not SDK/GX/source/runtime equivalence.

Seven complete queried bodies supply both
[flag0x10000](MAP_TEV_DEPTH_FLAG10000_PROGRAMS.md) and
[flag0x40000](MAP_TEV_DEPTH_FLAG40000_PROGRAMS.md). Each work getter independently
loads dswp=word[804150e8],initial original pointer803c9c30. No work-null/valid
lifetime/index guard is introduced. dswp is distinct from pswp/cswp recovered in
projected/character-shadow paths;similar offsets do not establish shared state.

| Getter | Address/size | Complete local result |
| --- | --- | --- |
| Depth config | 8002bb50/0x1c | 80304a60+low32(72*word[dswp+8]) |
| Depth color | 8002bb6c/0x94 | Inverted and alpha-scaled RGB,output alpha0,as below |
| Projection matrix | 8002bc00/0x0c | dswp+7c ADDRESS |
| Depth matrix | 8002bc0c/0x0c | dswp+ac ADDRESS |
| Depth texture | 8002bc18/0x40 | word[dswp]&1?smartTexObj(dswp+dc,word[dswp+fc]):NULL |
| Ramp16 texture | 8002bc6c/0x08 | ADDRESS8041e738 |
| Ramp8 texture | 8002bc74/0x08 | ADDRESS8041e758 |

Ramp getters return embedded object addresses,not object pointers loaded from
those words;they do not initialize/check the objects or dereference an image.
Depth texture's second smart argument is a HOLDER pointer read at work+fc.
[Smart data patch](ANIMATION_SMART_TEXTURE_DATA_PATCH.md) can mutate embedded
object+0c,then returns the same object. Texture flag1 is the only getter admission.
Both depth consumers call it twice after flag admission;only first result is
checked,second pointer retained. Initial pointer/object addresses are metadata,
not captured shadow/ramp pixels or fully initialized GX resource state.

## Complete depth color result

Capture dswp once;read U8 R,G,B,A at work+100..103. Build local color:

```text
Rout = floor((255-R)*A/255)
Gout = floor((255-G)*A/255)
Bout = floor((255-B)*A/255)
Aout = 0
return raw big-endian RGBA word
```

The original integer sequence uses signed magic0x80808081,high multiply,add,
arithmetic shift7,sign correction and low8 store. Every bounded product is proven
against floor(p/255) across65536 byte pairs by the existing host checker.
This is host arithmetic proof,not original game execution. No source writes,
clamp or float conversion. Raw source color and inverted result must remain
separate;pswp/cswp color leaves return raw word+100 instead. Alpha0 is explicit
initialization,not preserved source alpha or a missing byte.

## Original configuration source

The getter forms a pointer into original data table80304a60 using72-byte stride;
it does not enforce the table's0x90-byte/two-record annotation. A negative or
large work+8 is not locally rejected. The consumers later read signed route
word at returned config+3c. The TWO supplied US initial records BOTH have route3:
first route at80304a9c,second at80304ae4. Entire144-byte source SHA256
b01f90b7b4e7213c09322f2c3319369f4f1c0a4b8b11dd2033163d94dc731c46.
Verifier checks table identity and both route words. This prioritizes route3 as
a concrete initial source witness;it does not prove actual executed scenes,
immutable table contents,valid runtime indices or unused other code routes.

Constructors/config updates/render capture/scene selection/ramp creation,
work/holder/matrix/color lifetimes and GX/SDK/cache/alias/numeric/original image
behavior remain open. Reusing original data is necessary but does not replace
those original service behaviors. Full-game requirements remain incomplete.
