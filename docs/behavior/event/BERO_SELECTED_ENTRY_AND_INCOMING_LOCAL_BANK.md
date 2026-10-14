# Bero selected-entry service and incoming local bank

US G8ME01 disc0 revision0; read-only static recovery2026-10-14 for
[EVT-01](../../requests/EVT-01.md). Extends
[house05 incoming-local frontier](../field/HOUSE05_INCOMING_LOCALS_AND_INSTALLER_FRONTIER.md),
[ordinary road ownership](../overworld/OVERWORLD_ROGUEPORT_ROADS.md) and
[original position helper](../overworld/OVERWORLD_ROAD_POSITION_AND_CONTACT.md).
Addresses/offsets/bitmasks are hexadecimal; counts, indices, direction sentinel
values and coordinate literals are decimal. These are source contracts, not
observed original gameplay or a claim that the native road adapter has all
required source state.

## Original providers and record layout

evt_bero_get_into_info8007a214/2f8 has no script arguments and ignores
firstCall. It directly writes the owning event's local words at a0..bc,
rather than publishing through evtSetValue. It returns2 on both shown result
arms. Its dependencies are:

- GP pointer80415fd0, GP word0 flags and GP+11c incoming-entry C string.
- BeroINFOARR803dc110,16 pointer words, populated by actual get_info.
- BeroNUM8041ea10, actual registered-entry count.
- Original selected 60-byte entry, name string and embedded pointers.
- Fallback bero_null8030cd78/3c; default_entevt80416060/14.
- Prefix pointer table802c2c28/20 and the original strlen/strcmp/strncmp
  services8026d6b4/8026d464/8026d424.

An entry's relevant original fields are:

| Offset | Representation/use |
| --- | --- |
| 00 | name pointer |
| 04 | unsigned16 type/flags;low12 selects base kind |
| 06 | remaining16 bits in the original packed word,not read by into_info |
| 08 | signed32 direction or sentinel |
| 0c,10,14 | signed32 XYZ;0c==100000 requests original hit-object center in later position helper |
| 18 | signed32 movement length; -1 requests kind default |
| 1c | entry-event pointer;NULL means default_entevt |
| 20 | original case type; -1 requests kind default |
| 24 | exit-event pointer |
| 28,2c | destination map/name pointers |
| 30..38 | remaining original entry fields,not read by into_info |

get_info8007a50c/100 first clears all16 BeroINFOARR words, validates named
hit objects from the table in callerLW0, then loads pointers in source order
with stride3c, sets corresponding BeroSW803dc0d0 words to -1, publishes
BeroNUM and clears BeroEXEC8041ea14. This callback does not clear the event's
16 local words. The fixed16 pointer capacity is source storage; no safe
full-table refusal is established by its unchecked producer loop. Native
capacity/error handling must be described as native policy.

## Exact selection and fallback

Start index=0 and pointer cursor=BeroINFOARR. On every iteration load
selected=*cursor. If selected isNULL, select bero_null and index0, then
leave the search. If selected isnonnull, compare current GP+11c string with
the empty C string at8042051c. Its storage is annotated float0 but the caller
uses its leading NUL byte as a string. Empty incoming name likewise chooses
bero_null/index0. Otherwise compare selected.name to a fresh GP+11c pointer;
on equality retain selected and index, else advance cursor4/index1 and repeat.
The search does not read BeroNUM as a loop bound.

After the search, freshly read GP word0. If mask00000002 is set, freshly compare
GP+11c with empty again. If both conditions hold, take the special arm:
reload BeroINFOARR[0] as selected, force output index0 and use its name pointer
for LW3. This overrides the fallback record selected by the earlier empty-name
test. It has no shown NULL-array[0] guard. An implementation must require a
real first registered entry for this source domain, not silently substitute
bero_null again. The ordinary arm retains the searched/fallback record and
uses a freshly read GP+11c address for LW3, even when search fell back.

The special arm is at8007a2c0..8007a3d8; ordinary arm8007a3dc..8007a4e8.
Neither arm changes GP, selected records, BeroINFOARR, BeroNUM or BeroEXEC.
Source external string functions are ordinary pure string operations; native
service adapters should retain original read/publication order if exposing
mutable/instrumented providers.

## Direction derivation and exact ordered outputs

Read selected.direction. If it is not20000, retain it exactly, including10000
or any other signed word. For direction20000, inspect selected.type&fff:
kind0 or1 scans eight prefix strings, in order
n_,ne_,e_,se_,s_,sw_,w_,nw_. Each iteration calls strlen(prefix), then
strncmp(prefix,selected.name,length). First equality yields index0..7;
exhausting all eight yields8, not10000. Other kinds yield10000. Preserve the
source's redundant comparison of20000 with10000 if translating its literal
branch structure; the resulting semantics above are sufficient for these
ordinary immutable inputs. Prefix strings live at804204c0/c4/c8/cc/d0/d4/d8/dc.

Outputs occur in this order, each as a raw32-bit event-local word:

| Local / event offset | Value |
| --- | --- |
| LW1 / a0 | selected index,or0 in fallback/special domain |
| LW2 / a4 | freshly loaded BeroNUM |
| LW3 / a8 | special:selected.name;ordinary:fresh GP+11c address |
| LW4 / ac | zero-extended U16(selected+04) |
| LW5 / b0 | direction result above |
| LW6 / b4 | selected+0c address,not copied XYZ or native position |
| LW7 / b8 | selected+1c pointer,or default_entevt80416060 whenNULL |
| LW8 / bc | ffff0000,not ffffffff and not zero |

LW0 and LW9..15 are untouched by this local callback. Default_entevt is an
original event script (SET LW0=0,RETURN,END), not an empty string or actor
identifier. Native guest addresses must identify actual retained record/
string/script objects; matching numeric direction alone does not supply these
pointer-valued outputs. No Mario/partner position, camera or floor query is
needed to implement this particular callback faithfully.

bero_null's60 original bytes are:

```
00000000 0f000000 00004e20 000186a0 00000000
00000000 ffffffff 00000000 ffffffff 00000000
00000000 00000000 00010001 00000000 00000000
```

Thus ordinary fallback publishes typef00,direction10000,XYZ pointer to
bero_null+0c,default entry script and ffff0000 sentinel. The ordinary LW3
still points into GP's incoming-name storage rather than fallback.name=NULL.

## Actual gor_02 table and native road information loss

gor_02 bero_entry_data is REL file3ee24/b4: two records followed by a complete
zero sentinel. Existing native room ID2 is this area.

| Source row | Name/type | Direction/XYZ/length | Entry/exit scripts | Destination |
| --- | --- | --- | --- | --- |
| 3ee24 | w_bero,type0 | 20000;(100000,0,0);-1 | entrygor_02_event_00,file3ed64;exitNULL | gor_01/e_bero |
| 3ee60 | dokan,type2 | 9;(100000,0,0);-1 | entryNULL;exitfile3edac | tik_01/dokan_2 |
| 3ee9c | NULL sentinel | zero | zero | zero |

The road entry resolves direction6 by w_ prefix; pipe retains9. Actual
BeroNUM after this table's get_info is2, with source indices0/1. Source name
pointer offsets are184a4(w_bero),184bc(dokan),destination184ac/184b4 and
184c4/184cc. These actual pointer bindings can be exported with the existing
original REL section-address policy, independently of converted room IDs.

tools/room_convert.py exits() only accepts cardinal n_/s_/e_/w_bero names,
targetgor_*,arrival*_bero and signed kind==0. It returns a reduced subset;
it does not preserve all60 bytes, selected index/count or script pointers.
tools/build_assets.py further writes ROADv1 records containing room IDs,
converted centers/spawn positions, source/arrival node IDs and cardinal codes.
Source length/type/event/name-pointer fields are absent. Source sentinel
XYZ and actual runtime hitObjGetPos ownership are replaced with converted
marker-derived native positions. This is a useful ordinary-route adapter,
not a reconstructible BeroINFOARR/GP provider.

Game::finishWarp retains native reciprocal source/arrival node identities
and scripted arrival coordinates, but does not publish original GP+11c storage
or GP.flags bit2. restore() explicitly makes unsaved BeroEXEC history unknown.
FieldBeroExec::afterInfo supplies known0 only after actual get_info providers;
native room restoration alone is not that producer. Full source entry data,
actual incoming-name/system flags, pointer-backed storage and the selected
entry/count must be retained before admitting this callback as original.

## Downstream local-bank closure and remaining frontier

evt_bero_info_run8030ec7c/3e4 has different source domains. Its first
into_info publishes LW1..8 above, followed by system/Bero flag queries into
LW0. Attached child returns copy their whole local bank back; independent
RUN_EVT/INLINE entries have their own copies and do not supply parent copy-back.

Ordinary domain RUN_CHILDs evt_bero_initial_pos8030f060/278 at8030eefc.
That child invokes into_info again, writes LW0=LW4&fff, and selects:

- Kindf00/3: bero_get_position_normal(0,encoded0), then READF BeroSX/Y/Z
  intoLW10/11/12 and publish Mario/party0/party1 positions.
- Kind0/1: evt_bero_get_info_length(LW1,LW10),
  bero_get_position_normal(1,LW10), then the same three READF/publications.
- Kind2: length query then bero_get_position_pipe_pure(1,LW10), followed
  by the same READF/publications.

All branches then evt_cam_road_reset. READF performs evtSetFloat semantics;
the event locals are encoded event floats, not interchangeable raw IEEE words.
For the inspected event words, LW9,LW13,LW14 are untouched by this child.
The normal-position helper80079b74 does not directly write event locals; it
consumes LW3/5/6 and arguments and writes Bero globals. Its actual center,
floor-ray,SDK trig/fused arithmetic and Mario motion requests remain required
for exact LW10..12. The existing position contract establishes those providers;
native spawn coordinates alone do not establish the same encoded outputs.

Save-load/system-bit1 domain instead RUN_CHILDs bero_case_entry8030e608/190
at8030ed64. It READs BeroNUM intoLW2,setsLW1=0 and loops all registered
entries. evt_bero_get_info_num80079e84/204 reads resolved index and publishes:

| Locals | Actual source outputs |
| --- | --- |
| LW3..8 | entry.name,U16type,direction,entry+0c,entry/default script,ffff0000 |
| LW9 | BeroSW[index] |
| LW10 | entry+20 case type,or defaults kindf00→0,kind0→10,kind1→9,kind2→4,kind3→7 |
| LW11 | entry+24 exit script,or default_outevt80416074 |
| LW12 | ffff0000 |
| LW13 | entry+28 destination-map pointer |
| LW14 | entry+2c destination-name pointer |

The case-default notation means kindf00→case0,kind0→10,kind1→9,kind2→4,
kind3→7; unsupported kinds preserve the initial -1. Registration/attribute
requests, actual case index outputLW15 and switch saving follow in original
pool order. The loop incrementsLW1 until it equalsLW2. Therefore this path
**does write through LW14**; a claim that both Bero domains preserveLW13/14
would be incorrect. Actual case IDs and their initial pool state remain
mandatory inputs, not fabricated zeros. The loop's independent switch-on
inline does not copy local changes back.

The inspected later door02/03/roll children before house05 overwriteLW1..3
andLW15 but do not erase these prior high locals. This recovery closes the
local into_info callback semantics and identifies both downstream producer
paths. It does not establish every original root's entire incoming16-word
bank: actual Bero flags/entry, transitive callback effects, position providers,
case allocation state and other gor_02 init callbacks must still be bound or
verified before claiming exact house05 case snapshots.

## Evidence

Ignored listings: artifacts/house05-bero-into-info.txt,
house05-bero-get-info.txt,house05-evt_bero_get_info_num.txt,
house05-evt_bero_get_info_length.txt,house05-bero_get_position_normal.txt.
Original DOL event inventories: house05-preceding-dol-events.json,
house05-bero-subchildren.json. Into-info helper is complete8007a214..8007a50b;
no partial function lifting is claimed. Original REL SHA256
d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
evt_bero_initial_pos SHA256
214cdff31534df948655149d8ba53441124be40ce6b980d353dee0b48952c519;
bero_case_entry ae7a9047e05ef618ad6e43a8702b4343d272ad16328466caa34575d51a6f786d.
No code/shared documentation/assets/build changes were made for this contract.
