# Bero key leaves, fresh actor owners and next ROAD frontier

US G8ME01 disc0 revision0, read-only static recovery for
[EVT-01](../../requests/EVT-01.md). Complements
[Bero prefix services](../overworld/BERO_INFO_PREFIX_ASYNC_GROUPS_SYSTEM_FLAGS_AND_ACTOR_PREPARATION.md)
and [full parent scope](GOR02_ORIGINAL_PARENT_PREFIX_CLOSURE_AND_MAP_COPY.md).
Addresses/offsets/masks are hexadecimal; selectors, counters and story values
are decimal. Synthetic story402/w_bero is a source-branch fixture, not an
original gameplay observation or proof of a saved state.

## Pointer/getter and byte-counter identities

Mario current-pointer cell mp is8041e900 (r13+19e0). marioGetPtr8005e0c0/8
loads that cell and returns it; there is no lookup by actor name, room or event.
KeyOff/KeyOn independently load the same cell directly. Neither calls the
getter or retains an actor pointer from evt_mario_key_onoff's earlier lookup.
Mario main word+0 and U8 counter+39 are required live actor fields. Counter
loads use U8 arithmetic with low-byte writes; return/test uses signed8.

marioKeyOff8005c1f4/28:

1. c1f4 load mp once into actor pointer r4.
2. c1f8 read actor main+0;OR mask8;c200 write main+0.
3. c204 read U8 counter+39;add1;c20c store low byte+39.
4. c210 reload U8 counter+39;sign-extend;return that signed value.

marioKeyOn8005c1c0/34:

1. c1c0 load mp once into actor pointer r4.
2. c1c4 read U8 counter+39;subtract1;c1cc store low byte+39.
3. c1d0 reload U8 counter+39;sign-extend and compare exactzero.
4. Only zero: c1dc read current main+0;clear mask8;c1e4 write main+0.
5. c1e8 reload U8 counter+39;sign-extend;return that signed value.

There is no counter saturation, lower-bound guard or unconditional main8 clear.
For example byte0 KeyOn wraps to255, returns-1 and retains main8 as it was;
byte127 KeyOff becomes128, returns-128 and sets main8. Native supported-domain
admission may restrict corrupt/unbalanced calls, but cannot claim those source
wrap semantics were clamped. Do not collapse the counter into a boolean or
substitute the separate one-key-off flag domain.

marioKeyOffChk8005c2ec/10 loads mp afresh,reads U8+39,sign-extends and returns.
It does not test main8. Therefore its exact return may differ from both a
boolean lock status and the independently mutable main8 bit.

## Event wrapper captures and fresh leaf owner

evt_mario_key_onoff800e7474/64 captures event/argument pointer, calls
marioGetPtr at800e7490 before reading raw operand0 at7494, then resolves the
operand via evtGetValue80031260 at749c. That initial getter result is unused.
Ifzero: call marioKeyOff at74a8,then partyKeyOff800b7688 at74ac.
Otherwise: marioKeyOn at74b4,then partyKeyOn800b768c at74b8. Return2 regardless
of the signed byte returned by the Mario leaf. Both party key leaves are
single blr instructions, so their calls are genuine source empty operations.

An instrumented resolver may change mp. The Mario leaf must then use the new
pointer; it must not mutate the wrapper's unused initial actor. Conversely
once the leaf captures mp,all its flag/counter accesses use that same actor
even if a provider changes the pointer cell during a write. Preserve the final
counter reread rather than returning an arithmetic temporary. The counter and
mainflags must share the actual source actor owner across Bero,door,message
and other key callers; a separate Bero-only counter invents nesting behavior.

## Exact ordinary branch after key-off for the synthetic fixture

Assume actual known GPflags do not select save-load, actual registration has
cleared EXEC,selected incoming name is w_bero, and original gor02 entry data
is loaded. Story402 only affects outer parent guards; it does not turn
w_bero into a pipe. Original w_bero has U16type0,normalkind0,resolveddirection6.
After info_run's EXECget overwrites LW0 with actual0 and EXEC mask1 is set,
its key-off request is8030eda4. The subsequent selected branches are:

| Source command | Actual request/control |
| --- | --- |
| 8030edb0..edc8 | LW0=LW4;AND1000;nonzero background arm is skipped for type0 |
| 8030ee10 | evt_mario_bgmode_off800e73ec |
| 8030ee18..ee30 | LW0=LW4;ANDfff;kind2 pipe previous-party arm skipped |
| 8030ee58 | evt_mario_set_prev_party800e3058(raw0) |
| 8030ee64 | evt_mario_set_prev_party800e3058(raw1) |
| 8030ee78..eef8 | kind2 dokan_prepare arm skipped |
| 8030eefc | attached evt_bero_initial_pos8030f060 |
| 8030f060 | child into_info8007a214,refreshing actual selected locals |
| 8030f11c | child get_info_length8007a76c(LW1,LW10) |
| 8030f12c | child normal helper80079b74(1,LW10) |
| 8030f13c onward | READF then actor setters and camera reset |

After implementing real key,bgmode_off and previous-party providers, the next
currently absent built-in field-area service is length8007a76c (as inspected
in this task),then the full normal/recovery helper. Their known standalone
implementations do not become bound to a different shared scheduler merely
because those C++ mechanics already exist. Supply the actual same event/memory
bank and world owners;do not invoke a second standalone initial_pos root in
place of the already retained attached child.

Required background-off actor state: main+0,signedbyte+3c,conditional fields
+1b8/+1bc and+184/+188. It clears main02000000,then freshly reloads mp before
those branch reads/writes;its trailing camFollowYOn gets camera4 and clears
U16flag10,then freshly gets Mario and clears display+4 mask20000. These are
distinct fresh-owner boundaries,not one captured actor for the whole chain.

Required previous-party state: the wrapper gets Mario once before reading raw
selector,then uses signedbytes245+selector and247+selector. On firstCall an
already nonnegative current index or zero savedkind completes immediately.
Otherwise actual partyEntry/partyEntry2 allocation/setup is required;negative
returned index is a real completed branch only when produced by that service.
The helper captures its actor through those calls and stores the returned index
into the captured actor's slotbyte,then gets the actual party pointer. This
state is separate from global current-party index8041ead0. See the complete
prefix contract for first/poll branches and pipe/background boundaries.

## Evidence and limits

Fresh complete listings artifacts/bero-key-marioKeyOff.txt,
bero-key-marioKeyOn.txt,bero-key-marioGetPtr.txt,
bero-key-marioKeyOffChk.txt. Event wrapper and prepare listings:
artifacts/outer-bank-evt_mario_key_onoff.txt,
outer-bank-evt_mario_set_prev_party.txt,
bero-prefix-marioBgmodeOff.txt,bero-prefix-camFollowYOn.txt.
Original info_run bytes are inventoried in
artifacts/house05-preceding-dol-events.json;original type/direction data and
relocated table identity are independently retained in romfs/bero-info.json
and the read-only original REL. Current field-area built-ins were inspected
without editing or running builds. No native completion/raster/runtime timing
claim and no code/shared-doc/git changes.
