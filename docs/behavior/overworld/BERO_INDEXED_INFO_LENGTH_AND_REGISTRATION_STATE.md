# Bero indexed info, length and registration state

US G8ME01 disc0 revision0; read-only static source recovery2026-10-14 for
[EVT-01](../../requests/EVT-01.md). Extends
[selected-entry query](../event/BERO_SELECTED_ENTRY_AND_INCOMING_LOCAL_BANK.md),
[road ownership](OVERWORLD_ROGUEPORT_ROADS.md),
[length leaf](OVERWORLD_BERO_LENGTH_LEAF.md) and
[placement/contact](OVERWORLD_ROAD_POSITION_AND_CONTACT.md).
Addresses, offsets and masks are hexadecimal; indices, case types and lengths
are decimal unless explicitly prefixed0x. Source callbacks below ignore
firstCall and return2; native pointer/capacity refusal is separate policy.

## Indexed info: exact inputs and publication order

evt_bero_get_info_num80079e84/204 captures event arguments pointer+18,
reads raw operand0, calls evtGetValue80031260 once, shifts its resulting
32-bit word left2, then reads selected=BeroINFOARR803dc110[resolvedIndex].
It does not test signed negativity, BeroNUM, the16-slot bound or NULL entry.
The shift/address arithmetic wraps32 bits; large/negative inputs do not have
an original safe-error return. Native supported-index checks may hold rather
than access invalid memory, but must not be called an original bounds gate.

It directly writes owning event local words, not evtSetValue destinations:

| Order / local | Original value and store |
| --- | --- |
| 1 LW3 | selected+00 name pointer;80079ec0 |
| 2 LW4 | zero-extended U16(selected+04);80079ec8 |
| 3 LW5 | direction result;80079f88 |
| 4 LW6 | selected+0c address;80079f90 |
| 5 LW7 | word(selected+1c),or default_entevt80416060 ifNULL;80079fa4 |
| 6 LW8 | ffff0000;80079fb0 |
| 7 LW9 | BeroSW803dc0d0[index];80079fbc |
| 8 LW10 | word(selected+20),initial publication;80079fc4 |
| optional LW10 overwrite | when freshly reread selected+20==-1,apply kind defaults below |
| 9 LW11 | word(selected+24),or default_outevt80416074 ifNULL;8007a054 |
| 10 LW12 | ffff0000;8007a060 |
| 11 LW13 | word(selected+28),target map pointer;8007a068 |
| 12 LW14 | word(selected+2c),target entry-name pointer;8007a070 |

LW0/1/2/15 and local flags remain untouched. Source callbacks read entry fields
in the shown sequence; notably selected+20 is loaded twice, and default-kind
handling reloads U16(selected+04). A host API returning one immutable result
can represent the finite immutable catalog domain, but should not claim the
same intermediate publications for mutable/instrumented providers.

Direction uses the same original prefix table as into_info: direction!=20000
passes unchanged, including10000. AUTO20000 and base kind0/1 scans
n_,ne_,e_,se_,s_,sw_,w_,nw_; first prefix yields0..7, no match8. Other
base kinds yield10000. Only external string helpers strlen8026d6b4 and
strncmp8026d424 are called after the initial evtGetValue. There are no hidden
map/camera/Mario/party/case services in this local indexed query.

Only when selected+20==-1, base kind=U16(selected+04)&fff chooses:

| Kind | Default case type / store |
| --- | --- |
| 0xf00 | 0 /8007a010 |
| 0 | 10 /8007a01c |
| 1 | 9 /8007a028 |
| 2 | 4 /8007a034 |
| 3 | 7 /8007a040 |
| other | leave already published -1 |

Default entry/exit objects are actual20-byte event scripts, each
SET LW0=0,RETURN,END. Source addresses80416060 and80416074 have identical
bytes00020032fe363c80000000000000000200000001. They are not NULL success
tokens. Indexed info returns pointers without executing either script.

## Complete length query and signed conversion

evt_bero_get_info_length8007a76c/f4 captures args, resolves raw operand0
through evtGetValue, wraps index<<2 and reads BeroINFOARR[index] with the
same lack of source bounds/null guards. Read signed32 selected+18.

- If it differs from -1, convert that signed value to binary32, then call
  evtSetFloat800304bc(event,raw operand1,value) at8007a7d8.
- If it equals -1, reload U16(selected+04)&fff. Kinds0,3,0xf00 select
  float30 at80420518; all others selectfloat60 at80420520. Publish through
  evtSetFloat at8007a840 using a fresh read of raw operand1.

Destination is never resolved as an input value. No other local is implicitly
written; its identity is whatever actual operand1 denotes. No string, floor,
map, Mario or hidden callback is requested. Status2 is returned after the
publication. Invalid input/provider state is not an original pending domain;
native unavailable handling must precede unsupported accesses.

Fresh original bytes at802c3058 are4330000080000000, the exact double
2^52+2^31 used after xoris8000. This corrects the older length leaf's statement
that this constant is2^52 exactly. The subtract produces the signed integer,
then source fsubs rounds the result to single precision; do not reinterpret
the integer word as IEEE float or use unsigned conversion. evtSetFloat then
performs the original event-float encoding, distinct from raw IEEE storage.

## get_info: lookup validation, diagnostic and state writes

evt_bero_get_info8007a50c/100 directly consumes callerLW0 as a table pointer.
It clears all16 BeroINFOARR words at8007a534..8007a570. It then scans60-byte
entries. For each candidate, it reads name, calls hitNameToPtr80014b58,
then rereads name at8007a580. NULL name terminates the scan; the sentinel
therefore also calls hitNameToPtr(NULL), which returnsNULL without an error.
For nonnull name with NULL lookup result it calls
mapErrorEntry8002b46c(1,name) at8007a598, then continues scanning.
This is not an abort, exception, synthetic HitObj constructor or attribute
mutation. Even missing names do not locally prevent later table publication.

mapErrorEntry8002b46c/6c appends a record in buffer803c97b0 with stride24,
stores error code at+20, strncpy(name,31) into the record, sets error-present
word8041e670=1 and advances index8041e674 using its signed remainder32
sequence. There is no separate explicit byte31 NUL write. Source normally
uses the initialized bounded diagnostic ring. A native valid-domain loader
may refuse missing membership rather than fabricate the diagnostic ABI;
that is stricter native admission, not an original early return.

hitNameToPtr obtains current map work before its NULL-name test, visits maps
and their HitObj slots in source order, skips runtime U16 flags+0 mask80,
then strcmp(source node's name). It does not reject runtime mask1 or test
the HitObj attribute word for this lookup. A disabled collision-query flag1
must not automatically make an otherwise registered name absent. Authored
source-node identity alone is insufficient if actual runtime80 is unknown.

Second get_info pass reloads callerLW0, visits each nonnull-name record and
publishes its pointer into INFOARR. It writes **only used BeroSW slots** to
-1 at8007a5d4; unused SW slots retain prior values. It then stores BeroNUM
at8041ea10 and BeroEXEC0 at8041ea14. It does not reset BeroID, clear the
event local bank or register cases. The later ROAD attribute OR00800000
belongs to bero_case_entry8030e690, after get_info, not this callback.

## Runtime HitObj80 construction evidence

hitEntry80018658/9c allocates storage for (map hit count+128) e4-byte records,
rounds allocation to32, and clears it at800186c4. It calls
hitEntrySub800186f4/278, which recursively invokes _hitEnt8001896c/48c.
_hitEnt selects an unused record, then explicitly li0=0 at800189c4 and
sth0,HitObj+0 at800189cc before publishing sourceNode+8. Its complete body
has no later store to U16+0; its other halfword stores target+aa/+a8.
hitEntrySub only calls recursive construction/_hitEnt, not a flag setter.
Thus actually constructed ordinary map HitObjs start with mask80 clear.

This supports an explicit retained native80 lookup owner initialized alongside
validated named collision membership. Existing native hitEnabled(1) state
must remain distinct. Apply actual later source80 writers when admitted;
do not infer their absence in full-game or indirect callbacks from this
constructor. Area staging/rejection must transactionally restore both owners.
This evidence is not permission to claim a complete original HitObj ABI,
transformed runtime hierarchy or every map initialization side effect.

## BeroSW, BeroID and switch callbacks are separate owners

Both16-word arrays are in startup-cleared BSS:
BeroID803dc090 and BeroSW803dc0d0. Startup zero is not the per-area -1
producer; actual get_info performs the latter only on used SW slots.
evt_bero_case_id_save8007928c/60 resolves index then caseID and writes
**BeroID[index]**, at800792cc,return2. It does not write BeroSW.
The corresponding case_id_load80079230 reads that separate owner.

evt_bero_switch_on800794b0/104 and switch_off800793ac/104 resolve operand0.
Signed values0..15 mean a direct index. Other values are treated as a source
name pointer and searched by strcmp against INFOARR entries in source order.
The shown fallback dereferences an INFOARR record before testing its name;
valid named input/registered storage is a source caller invariant. It is not
a safe general missing-name service. The actual stack-address-dependent
index initialization/increment tests are always nonzero on the original stack;
a native implementation should not reinterpret host stack addresses as guest
data. Do not clamp an invalid integer input to a numeric slot silently.

Then publish in this order:

| Callback | LW0 before store | SW write | LW1 after store |
| --- | --- | --- | --- |
| on | oldSW==1 ?1:0 | SW[index]=1 at80079590 | resolved/searched index at80079594 |
| off | oldSW!=1 ?1:0 | SW[index]=0 at8007948c | resolved/searched index at80079490 |

These functions call no case constructor/deletion/event callback. LW0 means
the shown already-on/already-not-on test, not a universal changed flag.
Existing road prose describing these local callbacks as recreating/deleting
cases is overbroad; those consumers must be recovered separately. In the
inspected evt_bero text, SW writers are get_info used-slot -1 and these on/off
stores. This is a scoped local-function census, not a full-game indirect or
arbitrary event-memory writer closure. Preserve unknown/prior unused slots
rather than pretending each area rebuild sets the whole array to zero/-1.

## Correct native ordinary-road length integration

Each current original ROAD record has base kind0 and length-1, so this exact
length callback returns30. A meaningful binding reads the original selected
record through the new source Bero catalog and publishes the callback's
encoded float result to its actual destination; it need not pretend get_info,
case registration or original arrival placement has already completed.

Length and the fixed geometry radius30 have different roles in
bero_get_position_normal80079b74/310. Let C be the source center and u(H)
the source direction unit vector. Nonzero distance first builds
P=C-30*u(H), raycasts downward at P, then computes final tuple
F=P+(30+distance)*u(H). Mode1 rotates heading180 degrees; mode0 does not.
For ROAD initial position, original script requests mode1 with the queried
length30. Thus, ignoring the independently resolved Y, P=C-30*u(Hrotated)
and F=C+30*u(Hrotated). The initial P radius remains30 even for a nondefault
length. Do not replace all occurrences of30 with queried length, or treat
the query as a complete fixed-duration travel service. Distance0 skips the
direction/ray branch entirely. Original fused arithmetic/trig, hitObj center,
floor provider and BeroSX/EX tuple ownership remain in the placement contract.

## Current number: separate retained global and literal caller

evt_bero_set_now_number8007a63c/30 captures args pointer event+18, reads raw
operand0 at8007a64c, resolves it through evtGetValue80031260 at8007a650,
then stores that exact resolved32-bit word into BeroNOWNUM8041ea0c at8007a654.
It returns2, ignores firstCall and has no bounds check, normalization, local
publication, case operation or additional global write. Negative/sentinel
values are not clamped by this source body.

evt_bero_get_now_number8007a60c/30 captures args pointer at8007a618, reads
BeroNOWNUM at8007a61c, THEN reads raw operand0 destination at8007a620 and
calls evtSetValue(event,raw destination,captured current word) at8007a624.
The destination is not resolved as input. It returns2 and ignores firstCall.
Preserve the global-before-destination-read ordering for mutable providers.

BeroNOWNUM is a separate4-byte .sbss object, source bootstrap zero. Complete
get_info8007a50c/100 does not write it: resetting INFOARR/usedSW/NUM/EXEC is
not resetting the current index. In the inspected evt_bero text, the current
word has the get read and set write above; full-game indirect/raw-memory
writers are not excluded by this local census. Retain this global across
area initialization until an actual writer/bootstrap occurs. An external
checkpoint restore without source history does not establish its value.

The literal original evt_bero_info_run prefix is into_info at8030ec7c,
then evt_bero_set_now_number(LW1) at8030ec84, then
evt_bero_read_mario_pera8030ec90. A native caller with an actual admitted
selected-entry publication may execute query→set-current as that bounded
prefix. This establishes only the current word through the actual resolved
LW1; it does not invent other globals or imply completion of subsequent
Mario paper/Bero child/arrival/case services. Separate native unknown/ready
tracking is appropriate when the source state provider is unavailable.

## Evidence and remaining boundaries

Ignored source listings: artifacts/house05-evt_bero_get_info_num.txt,
house05-evt_bero_get_info_length.txt,house05-bero-get-info.txt,
bero-followup-evt_bero_case_id_save.txt,bero-followup-evt_bero_switch_on.txt,
bero-followup-evt_bero_switch_off.txt,bero-followup-hitNameToPtr.txt,
bero-followup-mapErrorEntry.txt,bero-followup-hitEntry.txt,
bero-followup-hitEntrySub.txt,bero-followup-_hitEnt.txt and
bero-followup-text.txt; current-number listings
bero-followup-evt_bero_get_now_number.txt and
bero-followup-evt_bero_set_now_number.txt. Original REL SHA256
d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
Native valid-domain readiness, actual case pool/IDs, remaining SW consumers,
indirect writers and original runtime/raster comparison remain separate.
No native code/shared documentation/index/build changes were made here.
