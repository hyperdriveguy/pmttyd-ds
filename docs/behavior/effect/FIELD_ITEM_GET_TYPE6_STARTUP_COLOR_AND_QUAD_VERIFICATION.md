# Item-get type6 startup color and quad verification

Read-only US G8ME01 disc0 revision0 DOL byte recovery, 2026-10-13.
Follow-up to [type6 bank/update/draw](FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md).
No implementation, runtime observation, or matched GX/PICA raster claim.

## Exact quad and model scale

At80211d00, `lfs f0,-812(r2)` reads80427114, binaryfloat64.
`fneg f1,f0`; `lfs f0,-840(r2)` reads804270f8,float.5;
`fmuls f30,f1,f0` yields -32. At80211d24, `lfs f31,-804(r2)`
reads8042711c,float32. Direct FIFO position writes at80211e04..e88 are
(-32,32,0),(32,32,0),(32,0,0),(-32,0,0), with corresponding UVs
(0,0),(1,0),(1,1),(0,1). Width64,height32; origin is lower center.

The source base transform still scales all axes by the single-precision
product payload10*payload14. For fresh type6, payload14 is retained .9
(up to source floating-point rounding). No extra factor32 belongs in the
modelscale: those dimensions are already the vertices. Pass0 right-concats
T(2,-2,0); pass1 has no additional local translation. Both retain the
separate camera-relative world displacement and yaw described in the parent.

## Positive-SDA2 color is startup BSS, not an initialized palette

`__init_registers`8000539c/90 establishes r2=80427440 with instructions
80005418/1c. Thus `lwz r0,0x2458(r2)` at80211d8c addresses80429898.
Original symbol metadata identifies this four-byte word as `.sbss2`,
`unk_80429898`, owned by `eff_itemget.o`; it has no loaded file offset.

The actual `__init_data`8000542c/c0 BSS table at800055b0 contains
(start,size) pairs (803c5360,4fbb0),(8041e580,ea4),(80429500,40c),
then (0,0). The third range covers80429898. Its loop at800054ac..cc
calls original zero fill800050b4 with r4=0 over each range. This is an
actual source producer for the startup value00000000. The draw replaces
only byte3 with trunc(128*payloadAlpha/255), preserving black RGB000000.
For payloadalpha255, pass0 is00000080. Pass1 separately loadsffffff00
at804270d8 and replaces alpha with the unscaled payload value.

A scan of every aligned loaded DOL text instruction found only one direct
r2+2458 memory access: the read at80211d8c. No loaded DOL word equals
the absolute target80429898, and no addi/ori/oris instruction constructs
its low-half9898 or uses r2+2458 to take its address. One unrelated addi
80242694/2458 constructs callback80242458 from a different
register; it is not a color write.

These facts support using startup-black as the source default, and reject
an invented shadow color. They do not establish absence of every indirect
write, REL relocation, injected debugger edit, or runtime mutation. A dynamic
Dolphin read/watchpoint would be the stronger runtime evidence. Preserve
that limitation when claiming current runtime or full raster parity.

## Reproduction and limits

Primary bytes read directly from /home/carson/games/roms/G8ME01.iso with
rb access; no source code executes. Existing ignored inspection output
artifacts/reference/goomba3-services/effectdisp.txt supplies instruction
locations; fresh ignored init-registers.elf, init-data.elf and
2458-candidate.elf were produced with tools/inspect_dol_function.py.
Source float/table bytes and all text scans were reread from the ISO DOL.
No original assets, ISO, extracted binary, or compiled packs enter history.
