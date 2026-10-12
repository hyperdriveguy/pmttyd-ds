# Field party initialization, driver eligibility and control-index census

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
partyInit at0x800bd6ac,size0x2c;partyMain at0x800bc574,size0xec;
marioBgmodeChk at0x8005c164,size0x10. The mode helper bodies are documented
[separately](FIELD_PARTY_MODE_INITIALIZATION.md). This is a local update-order
and static writer census,not complete scheduler/party mode behavior.

## Initialization and enabled-object count

partyInit requests memset(**0x8041ead8**,0,**8**) and returns. It clears the
two pointer-table entries only. It does not call object destruction or pose
release,clear Mario slot/saved-kind bytes,or explicitly initialize the adjacent
control-index/global-record words. Calling it over live objects would therefore
not reproduce the normal [destruction path](FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md).
Its intended initialization cadence requires caller evidence.

partyMain first calls marioGetPtr,whose return value is unused in this local
body. It counts table entries0 and1 that are nonnull and have **word P+0
mask0x1** set. If count==0,it returns without the following global or per-object
update requests.

If count>0,call **0x8014140c** once (annotation unk_8014140c),then iterate
table0 followed by table1. Each iteration rereads its table pointer after
that global call and after the preceding object's work. Null skips the object;
nonnull with word+0 mask0x1 clear also skips. A callback that changes table1
can therefore affect the same partyMain invocation's second update.

For each eligible object,call marioBgmodeChk. If its returned integer is
exactly1,skip driveParty;all other return values call **driveParty(P)** at
0x800bbe84. marioBgmodeChk returns **(Mario+0 >>25)&1** from the global
Mario pointer,with no null guard. This branch is therefore a concrete Mario
flag gate,not a generic hypothesis about pause or battle modes.

The global call at0x8014140c still occurs when count>0 even if every per-object
drive is then suppressed by the flag. The counted set and driven set are not
necessarily identical because the body rereads pointers/flags and the Mario
gate at different times. It does not capture a fixed list of object snapshots,
count rendered frames or locally manipulate simulation credit/EVT speed.
[Global updater](FIELD_PARTY_GLOBAL_COUNTDOWN_AND_ACTION.md) and
[driveParty local dispatch](FIELD_PARTY_DRIVER_DISPATCH.md) are now specified;
their called ability/mode/effect/collision/presentation engines remain open.

## Control-index word and bounds of the census

[Run/stop callbacks](FIELD_PARTY_EVENT_CALLBACKS.md) with raw operand0 load
an index from **0x8041ead0** (r13+0x1bb0). Symbol metadata labels the four-byte
word partyCtrlNo in .sbss. The original DOL header places it within BSS
0x803c5360..0x8042990c (size0x645ac). It has no initialized file bytes;
do not read an unrelated DOL file offset as its live value.

The verifier scans all seven DOL text slots,at original four-byte instruction
alignment,for D-form integer byte,halfword and word stores (including update forms)
based **directly on r13** whose spans overlap this four-byte word. It finds
**zero** such stores. This is narrower than proving no writes. It excludes
pointer aliases,indexed/floating stores,REL code,bulk BSS clearing and external changes.
It also does not prove constructor/destructor callbacks cannot change the word.

partyInit's table memset,partyMain's traversal and the recovered
[position constructors](FIELD_PARTY_POSITION_CONSTRUCTORS.md) do not locally
assign this index. The recovered destruction bodies clear table/owner slots
but do not locally reset it either. BSS placement is consistent with startup
zeroing;the startup clear and all later writers have not been fully specified
here. Do not label the word permanently0 or assume it always agrees with
Mario+0x245 merely because the direct-store census is empty.

## Implementation and verification limits

Preserve table order,enabled-bit checks,global-call cadence and per-object
Mario gate. Do not implement field stop/run by disabling table entries or
skipping partyMain wholesale;the recovered
[mode transition path](FIELD_PARTY_MODE_INITIALIZATION.md) changes the mode
and local flags without clearing enabled mask0x1 on the ordinary0/1 paths.
Full driveParty/EVT scheduling remains necessary to establish movement.

tools/verify_field_party_modes.py checks the complete local bodies,field import
bindings,original BSS containment and the scoped census. The metadata report
records scanned section address/size and zero matching stores;it contains no
original executable bytes. Gekko inspection artifacts remain ignored.

Next:partyInit/partyMain caller cadence and driver-called mode/ability/services;alias/REL/current-index and owner-slot writer census;startup BSS clear,
field/battle resource/party teardown and original update/transition traces.
These remaining dependencies prevent full party-system or game completion.
