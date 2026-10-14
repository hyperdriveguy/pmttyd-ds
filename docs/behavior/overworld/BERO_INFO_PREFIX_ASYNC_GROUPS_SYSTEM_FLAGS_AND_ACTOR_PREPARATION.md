# Bero info prefix: async groups, system flags and actor preparation

US G8ME01 disc0 revision0, read-only static recovery for
[EVT-01](../../requests/EVT-01.md). Complements
[actual parent closure](../field/GOR02_ORIGINAL_PARENT_PREFIX_CLOSURE_AND_MAP_COPY.md),
[selected-entry owner](../event/BERO_SELECTED_ENTRY_AND_INCOMING_LOCAL_BANK.md)
and [retained initial-position root](BERO_RETAINED_INITIAL_POSITION_EVENT_CLOSURE.md).
Addresses/offsets/masks are hexadecimal; counts, selectors, modes and state
numbers are decimal. These are actual source callbacks, not native no-op
admission shortcuts or an observed original execution trace.

## evt_bero_read_mario_pera is an asynchronous asset request

Callback8007a154/c0 has no event arguments. Despite its name, it does not read
Mario fields. It uses event+78 (event scratch work, not LW0) as retained state.
The native source-address mapping in source/event/event_entry_addresses.cpp
(read case0x78 atline94,write case0x78 atline116) maps this word specifically
to EventEntry::waitCount. Reuse that actual aliased owner; do not add a separate
callback work slot or confuse it with LW0 at+9c. WAIT opcodes use the same
source word at other times. On firstCall!=0 it writes state0 at8007a174; every invocation then reads the
current state and chooses exactly one branch:

| State | Ordered request and transition |
| --- | --- |
| 0 | animGroupBaseAsync80039d0c(name802c3060="p_dokan_x",0,0);nonzero result writes state1 |
| 1 | animGroupBaseAsync80039d0c(name802c306c="p_dokan_y",0,0);nonzero result writes state100 |
| Other | No group request |

The common tail reads actual event+78 and returns2 only when it equals100,
otherwise0 in the ordinary reachable0/1/100 state domain. A successful state0
request does not also request state1 in the same callback invocation. It writes
no LW bank, does not publish current Mario position and does not instantiate
Mario paper geometry. Both requests are unconditional in info_run, including
ordinary ROAD arrival; removing them because ROAD is not a pipe changes the
original event stream. Never replace a missing provider with ready=true.

animGroupBaseAsync80039d0c/134 itself requests file manager work for the group
and its texture association. Mode0 first calls fileSetCurrentArchiveType
80189d38(1). It makes fileAsyncf8018a0e8 requests of types5 and4, scans the
animation group's texture-name records, and returns normalized readiness from
the two original results. Its result treats original -1 statuses through its
explicit readiness branches; absence of extracted native metadata is not a
source ready result. An injected native request/readiness provider is a clear
boundary until actual original group/resource owner is bound. The complete
listing is artifacts/bero-prefix-animGroupBaseAsync.txt; this contract does not
claim the transitive file archive/async allocator service is fully implemented.

## Exact requested resources and readiness ownership

Every group callback invocation with mode0 requests file archive type1 before
both formatted requests. First call fileAsyncf(A=5,B=0,format8041fb34="%s/%s",
"a",actual group name). Retain its raw result R1. Then read animation work
pointer80415dc0,work+100 file handle,its+a0 payload holder,holder word payload.
Payload words0/4 are the relocated table pointer and byte length. Scan32-byte
name rows until length exhausted; first strcmp match uses the following32-byte
texture-group name,otherwise use the source empty string8041fb30.
Then unconditionally request fileAsyncf(A=4,B=0,format802c1068="%s/%s-",
"a",selected texture-group name), retaining result R2. A/B are the original
file API argument words, not proof of a native GPU resource category.

Both requests occur even when R1 is pending or -1. Only afterward calculate:
R1==-1→ready1;R1==0→ready0;otherwise R2==-1→ready1;otherwise ready=(R2!=0).
The original special -1 branch is therefore not an invented permanent pending
condition. See [actual fileAsync cache/publication](../file/FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md)
and [formatted forwarding](../file/FILE_FORMATTED_REQUEST_FORWARDING.md).
Cache-hit pending returns0; completed hit returns a file-entry address without
incrementing references. New DVD requests initially return0. The resource
ownership is the original file cache; this helper creates no animPose, stores
no animation resource reference and releases no handle. It is not a promise
that drawing/pose providers are initialized merely because both requests finish.

Animation initialization actually loads a/ag2tg.bin and retains its handle at
work+100; see [pool/auxiliary initialization](../display/ANIMATION_POOL_AND_TANGENT_INITIALIZATION.md).
Read-only ISO a/ag2tg.bin is41984 bytes,SHA256
d756d29d6e8eb7848bf2ff96b1c463223c140f0c33f7385ffac1ca40918125ba.
Original row byte39680 has group p_dokan_x and its following row p_slit;
byte39744 has group p_dokan_y and following row p_slit. Both therefore request
exactly the same texture archive a/p_slit-, not p_dokan_x- or p_dokan_y-.
Read-only FST/source file facts:

| Resource | ISO byte offset | Bytes | SHA256 |
| --- | --- | --- | --- |
| a/p_dokan_x | 922728396 | 41756 | 4cf08bf10e8d4e3353e3d1f1ab1b52d1979c89793be0bd926e7f39bfa7009373 |
| a/p_dokan_y | 922770152 | 18576 | aa93194c9a80f227e6129597b3dfa7adbec7de42c81fce85b83cf16a06b84247 |
| a/p_slit- | 924401432 | 22592 | 84ffe23a4bca900211eb373783cb5c363c1e5e9047f5a2d482b00ead6a6ecb3e |

An actionable bounded native provider should own actual source group and shared
texture bytes/readiness, retain the source two-request order on every poll and
the separate x-then-y EventEntry::waitCount phases. Resource decoding,cache
reference policy/native request latency and eventual pose/raster binding remain
explicit adaptations; no need to invent a Mario field read or render a pipe
just to service this unconditional prefetch request.

## Exact system flag callback address and operand order

The actual evt_sub_get_system_flag is80054f3c/34, not80055f3c. It captures
argument pointer from event+18 at80054f48, loads current GP pointer80415fd0
at80054f4c, reads raw destination operand0 at80054f50, then reads word0 from
the captured GP at80054f54 and calls evtSetValue80030cfc at80054f58. Return2.
It does not resolve its destination or read a named saved switch. A destination
provider mutation of the global GP pointer must not replace the captured GP.
The full original word, including every bit, is published to the actual raw
destination; info_run passes LW0 and subsequently tests mask1 for save-load.
Unknown native flags are not proven zero. Full-parent GP input must explicitly
supply word0 independently of refresh rate4/context14/timestamp38/3c.

## Mario key request immediately after ordinary EXEC admission

At8030eda4 info_run calls evt_mario_key_onoff800e7474(0), after source EXEC
mask1. The callback captures args, calls marioGetPtr8005e0c0 before reading
raw operand0, resolves with evtGetValue, and ifzero calls marioKeyOff8005c1f4
then partyKeyOff800b7688; nonzero calls marioKeyOn8005c1c0 then
partyKeyOn800b768c. Both party leaves are source empty return bodies. Return2.
Mario key leaves own original nesting/flag state; do not substitute a separate
one-key-off boolean domain. This source request has no LW output.

## Background mode owns more than one flag

info_run type1000 set branch requests evt_mario_bgmode_on800e7410; ordinary
branch requests evt_mario_bgmode_off800e73ec. These argument-free callbacks
call marioBgmodeOn8005c0a0 / marioBgmodeOff8005bfdc and return2. The helper
on sets Mario main+0 mask02000000; off clears it, then both freshly reload
Mario and read signed byte+3c before applying this shared table. Stores in
rows are ordered as shown; fields omitted by a row remain unchanged.

| Admission | Ordered stores |
| --- | --- |
| signed byte+3c==1 | +1b8=20,+1bc=47,+184=1,+188=2 |
| signed byte+3c==2 | +184=1,+188=2.25 |
| otherwise, main04000000 clear | +1b8=20,+1bc=37,+184=1,+188=2.25 |
| otherwise, main04000000 set | +1b8=10,+1bc=25.899999618530273,+184=1,+188=2.25 |

Constants are original binary32 words from mario.o sdata2:80420004=2,
80420008=1,80420078=10,80420088=20,804200b8=2.25,
804200bc=47,804200c0=37,804200c4=25.899999618530273.
On then calls camFollowYOff801a8eb4; off calls camFollowYOn801a8ef4.
These obtain camera4, read U16camera+0, set/clear10, write U16; then get Mario
freshly and set/clear display+4 mask00020000. The main background flag and
camera/display follow flags are distinct owners. No reset of camera event
view/target timers, road work or clock occurs in these helpers.
Argument-free evt_mario_cam_y_off800e7314 independently calls camFollowYOff
then returns2 in the background branch.

## Raw previous-party preparation for the ordinary nonpipe branch

info_run calls evt_mario_set_prev_party800e3058/c8 with literal0 then1.
Capture args, get Mario once, then read raw selector operand0; it is not an
evtGetValue integer. It indexes the actor's signed bytes245+selector and
247+selector. In the used selector0/1 domain:

1. On firstCall!=0, read signed current-index byte245+selector. If>=0,
   return2 immediately. Ifnegative, read signed saved-kind byte247+selector;
   ifzero return2 immediately.
2. For nonzero saved kind, request partyEntry800bea68(kind) when selector0;
   request partyEntry2800bddd0(kind) otherwise. If returned index<0 return2.
   Otherwise store the low byte at245+selector.
3. On firstCall==0, or after a successful allocation/store, freshly read the
   signed byte245+selector and call partyGetPtr800bf1b4. Both null and present
   results return2 without additional writes in this body.

This callback always returns2; it does not use async pending as a replacement
for actual partyEntry. Missing original actor allocation/art/setup service
must be refused at native admission rather than silently inventing a negative
result. A known saved kind0 or actual already-present index is a source
completed branch; unknown native fields are not proof of those conditions.
These Mario slot bytes are not current-index global8041ead0.

## Pipe/background branches remain separate providers

For kind2, info_run instead calls evt_mario_set_prev_party_dokan800e3514/dc
with raw0 then1. Its first-call current/saved-kind checks and partyEntry choice
are analogous. Successful new allocation stores index, looks up actual party,
requests partyChgRunMode800bcb2c(actor,0), then ORs actor main04000000 and
02000000 in separate read/write pairs and copies zero XYZ template802e3e9c
to actor+70/+74/+78. It returns2; on subsequent calls it skips that creation
branch. Actual helpers/art/registry remain required.

Selected pipe direction requests evt_mario_dokan_prepare800e7e04. It gets
Mario, ORs main100 BEFORE resolving operand0. Mode4 copies actual template
802e3cd8 to Mario+c8/cc/d0, sets display1000, then operates on actual previous
party owners through fieldMario party-index getters, partyGetPtr and
partyStop800b7530 plus actor flags/templates. Mode5 also requests partyStop
for both selected actors, with no source null guard in that shown branch.
Other modes preserve only the initial Mario main100 write in this body.
The complete listing artifacts/outer-bank-evt_mario_dokan_prepare.txt pins
these branches; full model/template/party-stop closure is not claimed here.
Background Mario kill-party requests are additional actual owner work.

The ordinary ROAD binding can advance only after its genuine async asset,
system flags, EXEC, key, background-off and previous-party services complete,
then its original attached initial_pos. Save-load adds NPC mode/start and case
entry, while background/pipe paths require their own providers. None of these
callbacks' unchanged LW banks establishes completed world side effects.

## Evidence

Original complete listings: artifacts/outer-bank-evt_bero_read_mario_pera.txt,
outer-bank-evt_sub_get_system_flag.txt,outer-bank-evt_mario_key_onoff.txt,
outer-bank-evt_mario_set_prev_party.txt,
outer-bank-evt_mario_set_prev_party_dokan.txt,
outer-bank-evt_mario_cam_y_off.txt,outer-bank-evt_mario_dokan_prepare.txt;
fresh helper listings artifacts/bero-prefix-marioBgmodeOff.txt,
bero-prefix-marioBgmodeOn.txt,bero-prefix-camFollowYOff.txt,
bero-prefix-camFollowYOn.txt,bero-prefix-animGroupBaseAsync.txt.
Names/constants/addresses match artifacts/reference/us_symbols.csv. No source
code/shared documentation/git/build changes or original-runtime claim.
