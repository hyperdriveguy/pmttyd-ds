# Smart texture object data patch and retained object identity

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis. `tools/verify_animation_texture_light.py` checks fourteen
complete local bodies,1189 instruction words,eleven binary32 constants,eight
original spans,two US lecture imports and41 direct DOL reference sites. These
are evidence counts,not full-game completion or execution coverage. No original
instructions or hardware images were executed for this recovery.

Complete smartTexObj8002f33c/0x3c and GXInitTexObjData802b76a8/0x10 establish
the wrapper's local data patch. Incoming r3 is the texture object;incoming r4
is a **pointer to a data-address holder**,not a boolean mode. Capture the original
object. If holder!=NULL,read word[holder] and call GXInitTexObjData(object,address).
Always return the same captured object. There is no allocator,copied object,
replacement identity,failure return or reference-count mutation in these bodies.

GXInitTexObjData reads word[object+0c],inserts rotated address bits with
rlwimi shift27,MB11,ME31,and writes only object+0c:

```text
word[object+0c] = (old & 0xffe00000) | ((address >> 5) & 0x001fffff)
```

Low five address bits are dropped;only original bits5..25 enter the field.
Upper eleven old field bits survive. No alignment check,address translation,
cache flush,null object check,holder dereference check or hardware ownership
rule is implemented locally. This field mutation does not by itself establish
GX texture memory coherency or lifetime.

The complete direct-reference census finds17 smartTexObj sites in seven DOL text
sections,including [override selection](ANIMATION_RENDER_TEXTURE_OVERRIDE_SELECTION.md)
and [extra-stage setup/rebind](ANIMATION_RENDER_LAST_STAGE_AND_REBIND.md).
Work+24/+28/+2c are passed as this raw holder argument there. Their producers and
valid lifetimes still require recovery. The extra-stage caller discards the
returned pointer,then loads the original captured object;these bodies prove
pointer identity remains equal,while the patch can still change object data.
Do not infer an allocation leak or treat the call as harmless solely because
its return is discarded. The helper has no original render-tick gate.
