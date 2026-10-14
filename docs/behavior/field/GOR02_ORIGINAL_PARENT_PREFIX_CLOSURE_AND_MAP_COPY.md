# Original gor02 parent prefix, closure and map-copy owner

US G8ME01 disc0 revision0, static read-only recovery for
[EVT-01](../../requests/EVT-01.md). Complements
[outer-bank provenance](HOUSE05_OUTER_INIT_BANK_CLOSURE_AND_ROAD_LEAF_CENTERS.md),
[generic installer](GENERIC_DOOR_ENTRY_AND_GOR02_PRECEDING_CASE_INSTALLERS.md),
[Bero child closure](../overworld/BERO_RETAINED_INITIAL_POSITION_EVENT_CLOSURE.md)
and [map memory ownership](../memory/MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md).
Addresses/offsets/masks are hexadecimal unless a decimal value is explicit.
This is preparation for actual original parent execution, not proof that the
current native area has completed this event.

## Actual original parent and scheduler prerequisites

gor_02_init_evt@ gor_02.o is REL file4d554/87c, ending4ddd0 exclusive;
SHA256649d12889da97f2b7b29aa59f59f58671dfb916195160127f594df927e84109e.
The source _prolog registers it as gor_02's map event; seq_mapChangeMain
requests evtEntry(script,priority0,flags0) at800081d4 and retains its ID in
GP+164. The actual root constructor supplies zero16LW/three local-flag words,
not an inherited continuation bank. This has source provenance, independently
of whether later callbacks are already bound.

Use one retained area scheduler for this root, its attached children, independent
roots and NPC init/regular events. RUN_CHILD94 inherits and returns the actual
local bank; RUN_EVT92 is an independent root and does not copy its later locals
back. Appending source programs in a convenient native sequence cannot replace
these relationships. Context capacity is256 in the recovered original context
initializer; GP+4=60 is timing/refresh-rate state, not a60-slot capacity.
Current native guest allocator/addresses and standalone scheduling remain
explicit adaptations until source map/GP ownership is actually bound.

Required real input ownership includes: GP/global flags and incoming-entry
string, source saved story and switch banks, loaded original map data and REL
address catalog, current Mario/party owners, case pool, map heap, field clock
and timing/manager pause state. Bero callbacks additionally need INFOARR/NUM/
SW/EXEC/current-number lifetimes. Importing a script does not establish these
runtime values; unknown guards must hold rather than read fabricated zero.

Direct root operands include GSW0/story and GSW1420, plus GSF1203,1219,1221,
1248,5375,5525,5526,5529,5574,5575,5576,5577,5578 (decimal IDs). Some are
passed as raw destination/identity operands to services; this list is an
operand census, not a claim that each is read directly by the interpreter.
Children and callback internals require additional switch/GP inputs.

## First actual source requests before Bero

The root begins SWITCH34(GSW0) at4d554. Grouped OR43 story values are
18,64,120,174,221,271,347 decimal. On that group:

- 4d594 evt_snd_bgmon(0x200,BGM_EVT_FRANKURI1)
- 4d5a4 evt_snd_envoff(0x200)

Default case at4d5b4 tests story!=402 decimal, and only then requests:

- 4d5c4 evt_snd_bgmon(0x200,BGM_STG0_GOR1)
- 4d5d4 evt_snd_envon(0x110,ENV_STG0_GOR3)

END_SWITCH is4d5e8. SET50(LW0,original bero_entry_data) follows4d5ec;
evt_bero_get_info at4d5f8; attached evt_bero_info_run at4d600. Thus genuine
sound request services are normally the earliest missing frontier. Reusing
an already audible background track or no-op return2 does not execute these
source sound owner requests. A real request-service adapter may have explicit
backend limitations, but must retain the requested source identities/masks.

## Actual attached Bero child prerequisites

DOL evt_bero_info_run8030ec7c/3e4 starts into_info, set_now_number(LW1),
read_mario_pera and get_system_flag(LW0), in that order. System mask1 selects
save-load: EXEC writes plus two WAIT_FRAME1 visits, NPC battle-mode/start
services, attached bero_case_entry8030e608, then RETURN. The other arm first
checks current EXEC==1 and returns early when true. Otherwise it sets EXEC
mask1 and requests Mario key-off before further world setup.

It tests selected type1000 for background versus normal Mario/party setup,
tests base kind2 for pipe-specific previous-party/dokan preparation, and only
then requests attached initial_pos8030f060 at8030eefc. Later pipe/fade setup
has its own selected type2000 gate. The final8030f050 launches independent
evt_bero_mario_enter8030f2d8; it is not an attached completion before parent
returns. Complete source word inventory is
artifacts/house05-preceding-dol-events.json. The already recovered ROAD
initial_pos root is only one nested part of this child, so executing it alone
cannot establish that info_run or the map parent finished.

## Prefix after Bero returns, through preceding doors

At4d608 IF_GE(story,268 decimal) launches dearu_event independently at4d614;
ELSE SETs GSF1203=0 at4d620. At4d630 evt_npc_setup(npcEnt) owns genuine
registration/init/regular-event work. Its firstCall is significant: it creates
priority0 independent init events, assigns actor owner+170 and saves their
IDs at NPC+118; later polls wait for those IDs before starting regular roots.
NPC init events do not copy their local bank into the map parent. Source
provider boundaries include actual actor allocations, animation resources,
callback owners and live event IDs, not just finite preplaced catalog models.

When story==7 decimal,4d648..680 enters named 盗賊 with c_borodo_n,sets its
tribe,position(0,-1000,0),yaw90. Then unconditional requests are:

| File | Request owner |
| --- | --- |
| 4d694,4d6a8,4d6bc | map_playanim mizu,noname,ie_01_light, each(1,0) |
| 4d6d0 | hit_damage_return_set(hitDmgRetPoints) |
| 4d6dc | attached evt_gazigazi_entry803646e8/64 |
| 4d6e4,4d708 | original kururing-floor object constructors |
| 4d72c | original mobj_box01 item-box constructor |
| 4d754,4d778,4d79c | original three powerup-block constructors |
| 4d7c0,4d7e8,4d810 | original three item_entry requests |
| 4d838..4d86c | original door02,door03,roll03 SET/attached-entry sequence |
| 4d874..4d8e4 | story<23 locked Trouble door,else original generic door04 entry |
| 4d8e8 onward | actual house05 mutable copy/list/manual installer |

The gazigazi child has entry,position,pera,yaw,flag callbacks then RETURN/END;
it preserves inherited locals through those callbacks, while still requiring
those world effects. Objects/items/map animations likewise cannot be marked
complete merely because they have no local outputs.

## Later parent roots are part of completion scope

After house05, GSF1219 chooses lock-object setup versus copied house06 record,
list selection on GSF5375 and attached generic entry. Later conditions set the
story fence, temporary Goombella, professor lecture, and stage-specific
independent epigraphy roots. Unconditional independent requests include
source offscreen-door entry, gor_goki1_urouro,gor_goki2_urouro,gor_irai_init.
GSF1221 chooses cloud initialization plus an inline brother loop (op107..109)
versus generic rear-door entry and mask hierarchy hide/hit disable.
The tail registers two harbor/airport map services, launches front_wall_keshi,
tobiori_check and evt_sub_mail_event independently, attaches
evt_monosiri_follow, launches iri_19_init, and for story402 kills party then
attaches epilogue_npc_entry and launches epilogue_evt. RETURN/END occur only
at4ddc8/4ddcc. Do not equate the house05 installer frontier with parent death.

## Import closure census

A fresh read-only selected REL package audit of root
`gor_02.o::gor_02_init_evt` succeeds using current symbol/ADDR32 metadata:
970 original data objects,154 EVT objects,76924 packaged bytes. Metadata only
is retained at artifacts/gor02-parent-rel-closure-audit.json; no new pack was
written or executable PPC copied. This finite REL closure is larger than the
current door-only subset, but is not itself full runtime execution closure.

It has202 unique direct DOL import identities. Eleven are EVT objects:
generic door entry,door_open_se,door_close_se,npc_init_evt,generic door close,
generic door open,evt_door_open_roll,evt_door_open_quick,evt_bero_info_run,
evt_gazigazi_entry,evt_sub_mail_event. Their original words and their own
transitive DOL data/event dependencies need separate import. The remaining
191 direct identities are text callbacks in this audit. Local REL callback
code is excluded; each needs an admitted native service at its exact relocated
identity. Script operands that publish pointers are not automatically requests
to execute their target. Conversely callbacks such as npc_setup can create
roots not visible as RUN_EVT operands, requiring source owner dispatch.

Unknown callbacks must yield pending on the actual request. Do not fill full
switch banks with guesses to force a farther frontier. Initial parent execution
can use a real source root and halt at the first unbound sound/world callback;
this advances scheduling/import fidelity without claiming missing effects.

## Exact evt_door_data_copy and map allocator ordering

Callback800e8004/8c captures event and argument base, reads raw operand0 and
resolves source through evtGetValue80031260 at800e8028. It retains the returned
source address. Then fresh-loads primary map arena pointer8041e808 at800e8030,
requests _mapAlloc8002ff58(arena,72 decimal) at800e8038, retains new payload
in register5. There is no local null-result guard.

The copy loop is9 iterations, each with this precise alias-visible order:
read sourceword[2i],read sourceword[2i+1],write payloadword[2i],write
payloadword[2i+1]. It is not an eighteen-word capture before writes or a
per-word read/write interleave. After all18 words, freshly read raw operand1
from the captured argument base at800e8064 and call evtSetValue(event,raw,new
payload) at806c. An allocator/provider may mutate later source/destination
operands; captured source/base remain captured, while output operand is late.
Embedded original pointers are copied unchanged and not deep-cloned.

For requested72, _mapAlloc rounds payload to96,requires a free block of at
least128 (payload plus32-byte remainder-header allowance),and chooses the
smallest suitable free block, retaining the first equal-size candidate. It
writes chosen next to oldheader+128,payload size96,busy halfword1; constructs
new free header there with old successor,remaining size oldsize-128,busy0.
It zeroes96 payload bytes,flushes those96 bytes and returns oldheader+32.
No fit returns0,which the copy callback would still dereference. A safe native
arena admission must refuse before invoking this source invalid-memory domain;
it cannot publish a successful zero GW pointer. Exact general allocator
wrapping/size-zero behavior is separate from this72-byte valid domain.

The copy is owned by the map arena and remains live after parent/installer
death, referenced by GW11 and saved case LW0. Source event ReInit precedes
map-pool clear on unload. Share the allocator/record/case/GW lifetime; do not
allocate a private per-callback handle that dies at root completion or replay
the copy over later quest list mutations.

Fresh complete listings: artifacts/gor02-parent-door-copy.txt and
artifacts/gor02-parent-mapAlloc.txt. Original parent words/relocations:
artifacts/trouble-child-installer-events.json. Original ISO remains read-only;
no code/shared-doc/git/build changes were made for this recovery.
