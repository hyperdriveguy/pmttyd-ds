# Unit badge/count import and concrete setup callers

US G8ME01 revision 0; recovered 2026-10-12 by documentation agent ID 2.
Evidence B: complete _EquipItem0x800f7ca4/0x678 and
BtlUnit_EquipItem0x800f831c/0xc8, scoped caller regions of
Btl_UnitSetup0x800f8aac/0x2f8, and original Mario setup/kind records.
Item IDs are **decimal**;field offsets/masks/addresses are hexadecimal.
Friendly equipment names are unnecessary for these numeric runtime bindings.
No original execution or inventory equip-menu policy is asserted.

## Wrapper inputs, reset and scan

BtlUnit_EquipItem(U,F,item) retains unit,flags and raw signed32 item argument,
then calls pouchGetPtr even when U is null. Null U returns without mutation.
For a valid U, execute in order:

1. If F&1,zero exactly40 bytes at U+0x2e0..U+0x307 inclusive. Adjacent held-item
   word308 is not included. Status bytes,move counters and other flags are not
   part of this reset.
2. If signed kind word U+8 ==220,return. The reset above still occurred when
   requested,but no item scan or resource revision follows.
3. If F&6 is nonzero,ignore the supplied item argument,clear mask1 from the
   flags passed to _EquipItem and visit **all200** signed halfword pouch slots
   at pouch+0x38a+2*i,in ascending order i=0..199. Pass each value unchanged to
   _EquipItem(U,F_without1,value),including zero/negative/unrecognized values.
   There is no early zero terminator or item-count query.
4. Otherwise call _EquipItem(U,F,item) once with the original raw item word.
5. Call [ReviseHpFp(U)](UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md) once after the
   scan/single request,even if nothing matched. This wrapper does not consume,
   remove or equip entries in the pouch;it imports their effect counts into U.

Count reset and resource revision are ordered separately. Accumulating calls
without F&1 retain existing counts. Scan imports at most200 values per wrapper
call,but repeated/direct calls can overflow byte counters;no saturation exists.

## _EquipItem dispatch and arithmetic

Item0 immediately returns. The common IDs in the first table below are checked
before mode masks and match regardless of F. Every ordinary match increments
the selected **unsigned** unit byte by1 and stores its low8 bits;255 becomes0.
The matched route returns immediately. There is no HP/status/belong check,
RNG,inventory access or unit-kind filtering in this leaf.

For all other IDs,capture `partnerMask=F&4`. If it is zero,search the Mario
table in listed order. If no return,capture `marioMask=F&2`;if zero,search the
partner table. Thus F2 skips partner pairs,F4 skips Mario pairs,F6 skips both,
and F0 can recognize both ordinary tables. The wrapper's kind gate is220 only;
the leaf does not infer a mode from actor kind. Do not replace the flags with
a boolean derived from the actor's name.

| Common item ID | Incremented byte |
| --- | --- |
| 305 | 2fb |
| 324 | 305 |
| 325 | 306 |

| Mario table ID (requires F&4==0) | Incremented byte |
| --- | --- |
| 284 | 302 |
| 283 | 301 |
| 299 / 301 / 303 | 2e0 / 2e1 / 2e2 |
| 276 | 2ed |
| 260 / 262 / 264 / 266 / 268 / 270 | 2e4 / 2e5 / 2e6 / 2e7 / 2e8 / 2e9 |
| 288 / 272 / 274 | 2ea / 2eb / 2ec |
| 292 / 294 / 291 | 2ee / 2ef / 2f0 |
| 278 / 280 | 2f1 / 2f2 |
| 285 / 287 / 306 | 2f6 / 2f7 / 2f8 |
| 295 / 297 | 2f9 / 2fa |
| 335 / 256 / 337 | 2fe / 2fc / 2fd |
| 281 / 258 / 326 | 2f5 / 303 / 304 |

| Partner table ID (requires F&2==0) | Incremented byte |
| --- | --- |
| 300 / 302 / 304 | 2e0 / 2e1 / 2e2 |
| 277 / 265 / 261 / 269 / 275 | 2ed / 2e6 / 2e4 / 2e8 / 2ec |
| 336 / 271 / 307 | 2fe / 2e9 / 2f8 |
| 296 / 298 | 2f9 / 2fa |
| 257 / 338 / 293 / 279 | 2fc / 2fd / 2ee / 2f1 |
| 273 / 267 / 263 | 2eb / 2e7 / 2e5 |
| 282 / 286 / 259 / 330 | 2f5 / 2f6 / 303 / 304 |

The table is exhaustive for the two ordinary sequences. In particular,partner
table has no separate2f2/2f7 entries;do not invent a partner FP-maximum count
mapping by filling apparent gaps. The [calculator](../battle/BATTLE_DAMAGE_CALCULATION.md),
[defense](../input/BATTLE_INPUT_AND_DEFENCE.md) and [resource revision](UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md)
read these imported bytes under their own documented masks and conditions.

## Special mode-gated tail

After both ordinary tables fail,when marioMask!=0:

- Item254 increments2f3 modulo256. Reread the resulting unsigned byte;if it is
  >=2 and byte2f4 is0,increment2f4 to1. If the new2f3 is<2 or2f4 is already
  nonzero,leave2f4 unchanged. The route returns immediately.
- Item329 increments2f4 modulo256 and returns.
- Item289 increments2ff modulo256 and returns.
- Item290 increments300 modulo256 and returns.

After these fail,when partnerMask!=0,item255 performs the same2f3/2f4 rule
as item254 and returns. Finally,if either captured mask is nonzero,item331
increments307 modulo256;otherwise/unrecognized,return with no write.

The2f3 threshold follows narrowing:old255+1 becomes0,so it does not enable2f4.
Old2f4=255 is nonzero and is not normalized by the threshold rule. Direct
item329 can wrap2f4 to0. These are local arithmetic cases;valid gameplay
reachability of extreme counts remains unverified.

The [Mario power callbacks](../battle/BATTLE_WEAPON_POWER_CALLBACKS.md) add the imported
unsigned2ff/300 bytes to Jump/Hammer power respectively. This pins concrete
item289/290 writers,not the player's initial equipment or per-hit AC state.
Other badge effects/counter meanings require their concrete readers;byte2f3/
2f4 alone does not establish item-command availability or inventory consumption.

## Actual initial setup binding

Btl_UnitSetup receives BattleWork. Its first two entry requests use original
setup records entryunit_system0x80416a80 and entryunit_mario0x80416a50,in that
order. Immediately after each entry,it calls BtlUnit_EquipItem(returnedUnit,3,0).
System kind220 is excluded after reset;Mario uses F3=clear1+Mario scan2.
The retained Mario pointer is then passed to SetParamFromPouch again at
0x800f8b38,after badge import/resource revision. Do not assume the earlier
SetParamFromPouch inside unit entry already had the final imported count bytes.

Mario setup word0 binds unitdata_Mario0x80372afc. That original kind has
unsigned part count3 and part pointer0x80372a18,closing the missing concrete
caller/count link in [part defense construction](UNIT_PART_DEFENSE_AND_LIFETIME.md).
The actual initial setup therefore constructs its three annotated part records,
including their shared signed zero defense tables and distinct attributes.

If signed selected-partner word at word(BattleWork+0x2738)+4 is nonzero,the
caller selects `entryunit_party0x80363790+0x30*(word-224)` and enters it.
After positioning requests,it calls EquipItem(partner,5,0),then
SetParamFromPouch(partner) at0x800f8bb4. F5=clear1+partner scan4. The selection
word has no local range check;valid selected kinds are a producer obligation.

For each original enemy setup record,the caller enters the unit,stores its
index byte0xe,and initializes held item308. It reads word at setup-group+0x5c+
4*S8(index). If nonzero,it stores that held item and ORs unit flag1c mask
0x40000000,without calling EquipItem in this branch. Otherwise it stores word
setup-group+0x1c+4*index and calls EquipItem(enemy,1,thatWord):reset plus single
raw-item request. Then it copies group+0x3c+4*index to unit30c and sets current
HP/FP from revised maxima108/10e. Preserve the two item ownership branches;
do not assume every held item is imported as a badge or that current HP always
retains the earlier kind store. Group producers and later init overrides remain
separate dependencies.

These are scoped setup regions,not complete Btl_UnitSetup closure. Side/work
initializers,position helpers,final battle-wide badge-flag scan and main lifecycle
remain open. Full-function fingerprinting does not eliminate those gaps.

## Scoped implementation requirements and closure limits

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| EQUIP-01 | Preserve reset range,kind220 gate,200-slot signed scan and final revision | Null still calls pouch;kind220 can clear;F3 scans after clear;F0 dispatches one raw item;zeros do not terminate scan |
| EQUIP-02 | Import exact numeric table/mode mappings as unsigned counts | Common IDs work in every mode;260/261 route by F2/F4;F0 recognizes both;255 increments wrap to0 |
| EQUIP-03 | Preserve threshold/special tail and power-byte writers | Second254/255 copy ensures2f4 only when zero;329 wraps;289/290 write2ff/300 only with mask2;331 requires mask2 or4 |
| EQUIP-04 | Bind concrete setup before resource/status reimport | Mario F3/partner F5 precede later pouch import;enemy saved/non-saved held-item branches remain distinct;Mario count3 reaches part constructor |

No helper writes the pouch inventory,changes BP affordability or implements an
equip UI. Original item data,inventory selection/validation/new-game equipment,
all later badge import callers,partner switch/reload owners,AC/global policy and
executed damage/guard/resource comparisons still need closure. Parent requests
remain open. Reuse original item/message data when presenting names;do not invent
story dialogue or equipment descriptions from these numerical bindings.

tools/verify_behavior_contracts.py pins32 new instructions,whole import/caller
fingerprints and concrete Mario setup/kind/part pointers;inspection outputs and
original data stay under /tmp or ignored artifacts. Read-only ISO access does
not grant asset redistribution rights or assert measured tutorial behavior.
