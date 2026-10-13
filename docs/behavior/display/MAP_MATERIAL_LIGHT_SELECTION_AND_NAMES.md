# Map light selection, named slots, nearest append and ambient query

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original instructions/GX execution or observed images.
`tools/verify_map_material_source.py` checks six complete local bodies,806
reviewed words,six binary32 constants,six original spans,two US lecture imports
and five direct DOL sites. Loader/source/GX/numeric/lifetime/runtime remain open.

Complete mapSetLight8002493c/0x1cc takes object O r3,output pointer array r4.
Capture O/output;count starts0. ALWAYS PSMTXMultVec(O+1c,O+10,&local worldOrigin)
before selection. Read control=word[word[O+8]+58];capture U8[control+2]&8 and
initial word[control+4] selection mask. No pointer/transform/slot validity guard.

If control byte flag8 SET and mask0:lightGetNearObj(worldOrigin,output,8,0),
return that count. If flag8 SET and mask!=0:scan named bits below,then if count<8
append NearObj(worldOrigin,output+count,8-count,0);add returned count.
If flag8 CLEAR and mask!=0:scan named bits only. If flag8 CLEAR and mask0:return0,
without clearing output. This is not a cached-light query or per-tick update.

## Named-mask scan

Scan i=0..31 in ascending order. Reread control through O+8 and word+58 each
iteration,then current word[control+4] & (1<<i). Set bit:
resolve CURRENT map group G=word[8041e660]+0x2f4*word[8041e664];signed index
S=I16[O+de];name-table pointer=word[G+0x178*S+98]. Load slot pointer
word[table+4+4*i],then its word+0 string pointer. Call lightNameToPtr8001bcf8.
IMMEDIATELY read U16[returned pointer] without NULL check. If ambient mask0x8000
set,skip;otherwise publish pointer,count++,advance output,then stop if count>=8.
No extra character0x80 exclusion or deduplication. Missing name can dereference
NULL locally;do not invent a nearest fallback for that miss. Table/slot/string
indices and named-light lifetime are original source invariants still open.

When nearest append admitted,it can repeat a light already published by the
named scan;no exclusion list is passed. NearObj flag argument0 means any admitted
nonambient active light,not character-only. Its previously recovered
[source-order <=8 versus distance-sort >8 and postwrite/tail fill](MAP_CHARACTER_LIGHT_QUERIES.md)
semantics apply. Named-only output tail is not zeroed locally.

## Complete name and ambient leaves

lightNameToPtr8001bcf8/0xa0 retains requested string;mapGetActiveGroup()==1 selects
bank8041e658,otherwise8041e650. Base=array pointer bank+4;stride96 bytes.
Reread signed count bank+0 every loop test. Only record U16 flag1 active admits
strcmp(requested string,record+2 embedded name)8026d464. First equality→record
pointer;otherwise continue,exhaustion→NULL. No null string/duplicate/type guard.
The map caller—not this leaf—excludes ambient afterward.

lightGetAmbient8001ba60/0x70 calls the same selector and scans a captured signed
count/array. Count<=0→NULL. First record with flags1 SET,0x80 CLEAR,0x8000 SET
returns pointer;otherwiseNULL. No distance/name/type filter. This differs from
lightGetCharaAmbient,which requires0x80 SET. Neither source leaf initializes the
bank or supplies source/color lifetime guarantees. Original active group/light
creation/reset/names/content/reachability and strcmp/vector numeric backend
remain separate from these complete local request/source contracts.
