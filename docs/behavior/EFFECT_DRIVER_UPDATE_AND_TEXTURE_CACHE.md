# Complete effect-driver update traversal and texture-cache refresh

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete effMain8005ea7c/170 and getMarioStDvdRoot80007388/8. Verify:
`python3 tools/verify_effect_driver.py`:14 complete/linked hashes,186 instruction
anchors,eight source spans,four direct DOL sites and the original US texture
fingerprint. File/heap/outer caller identities are linked,not newly complete
platform/resource engines. Read
[pool initialization/context release](EFFECT_POOL_INITIALIZATION_AND_CONTEXT_RELEASE.md),
[Majinai update](MAJINAI_EFFECT_UPDATE_STATE_MACHINE.md),
[outer update order](GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md) and
[asset provenance](../ASSETS.md).
Static original behavior;no measured frame/driver/resource execution trace.

## Capture one record bank,query current context for EACH record

Read effect-manager pointer80415ee0,then SIGNED count M0 and record-array
pointer M4. Retain count and array for the entire first loop. Index starts0,
record stride28 hex=40 bytes;count<=0 skips updates and still executes the
texture tail. Manager/count/array are not recaptured after callbacks.

For EACH record E,in this exact order:

1. Freshly read GP pointer80415fd0 and GP.word14.
2. If GP14==0,require WORD E.context4==0;otherwise require E.context4==1.
   Every other raw context value skips. Nonzero GP14 is normalized by choosing
   comparison1,but E.context is tested for EXACT0/1,not nonzero equivalence.
3. Only after context admission,read E.flags0 and require numeric active bit01.
4. Read callback E10;null skips,nonnull is called indirectly with E in r3.
   Ignore callback result;no firstCall/elapsed-time argument is supplied.
5. Increment index and pointer by40;test against captured signed count.

No GP18 pause/system-level mask,key/time/retrace/lifetime/credit/speed query,
callback result interpretation,sort or redispatch loop. The local driver supplies
ONE callback invocation per admitted record visit. Other owners may suppress or
schedule the driver;that is distinct from adding an event-tick credit policy
inside this loop. E.context stamped by effEntry is a source role,not a proof it
matches current GP across scene mutation.

Later flags/context/callback fields are read LIVE as the ascending scan reaches
them. An effect allocated into a not-yet-visited slot can run during this SAME
driver invocation;an earlier slot waits for another invocation. A callback that
replaces its own slot is not re-run immediately. GP14 changes between callbacks
can change which later contexts qualify. Array replacement/free during callbacks
has no local retained-pointer repair;native storage lifetime/domain policy must
be explicit. No precomputed active-effect list is used.

This differs from NameToPtr's entire flags!=0 predicate and effEntry's exact
flags0 free predicate. Raw flags02 can remain occupied/name-visible while driver
skips it for missing active01. Auto-release also tests active01. Do not unify
these three conditions into a single host “exists” boolean.

## Texture tail: compare AFTER every effect callback

After scan,read CURRENT manager M0 and CURRENT GP G0. Compare raw WORD
M0.cachedSelector14 with G0.selector16c. Equality returns without cache/resource
query. Otherwise read old file handle M0.cache10;nonnull calls fileFree(handle),
then RELOAD current manager M1 and store M1.cache10=0. Old null handle skips
that free and explicit zero store. Last selector is not cleared here.

Then perform asynchronous source request A:

1. RELOAD current GP G1,read selector R1=G1.word16c.
2. Read name N1=WORD(prefixTable80415ee4+wrap32(R1<<2)).
3. Call getMarioStDvdRoot();retain returned root P1.
4. Request fileAsyncf(4,0,format802c1f20,P1,N1,N1),with CR6 cleared for varargs.
5. Return exactly0 exits WITHOUT a synchronous allocation or selector update.
   Any nonzero result proceeds to request B.

Request B repeats the source reads independently:

1. RELOAD current GP G2 and selector R2;read N2 from the same raw table indexing.
2. Call getMarioStDvdRoot AGAIN for root P2.
3. Request fileAllocf(4,format802c1f20,P2,N2,N2),again clearing CR6.
4. RELOAD manager M2;store returned handle to M2.cache10,including0.
5. RELOAD GP G3,then RELOAD manager M3;store G3.word16c to M3.cachedSelector14.

No pointer/selector identity check between these captures. Source attempted
asynchronously can differ from synchronous source and stored tag under a mutating
provider. Capture boundaries are observable;do not choose one language/file path
once before callbacks or collapse the two root/name queries into a private cache.

A nonzero async result followed by allocation0 STILL advances the selector tag.
A later invocation with matching selector skips retry despite cache0. Async0
keeps the prior selector tag,so mismatch can retry later. Replaced/null cache
ownership is not repaired locally. No allocator success branch,TPL parsing,
texture-object installation,model lookup or rendering occurs in this tail;
those file/library consumers remain required backends.

## Exact source format,seven prefixes and US file evidence

getMarioStDvdRoot returns constant pointer8041f494 to original string `.`.
It does not query OS cwd,SD paths or current map. Format802c1f20 is exactly
`%s/e/%s/effect_%s.tpl`,including terminator. Prefix table is seven initialized
address WORDS (28 bytes),not a validated index range:

| Raw selector | Pointer | Original prefix |
| --- | --- | --- |
| 0 | 80420180 | jp |
| 1 | 80420184 | us |
| 2 | 80420188 | ge |
| 3 | 8042018c | fr |
| 4 | 80420190 | sp |
| 5 | 80420194 | it |
| 6 | 80420198 | du |

The code supplies no bounds/null/negative-selector fallback. These are source
prefix IDs;do not interpret GP16c as a battle stage or field gameplay region.
Other selector writers and whether other language files exist in a given ISO
are separate requirements. Table existence alone does not prove every file ships.

For stable selector1,the requested path is `./e/us/effect_us.tpl`;read-only FST
inspection confirms `e/us/effect_us.tpl` in the supplied US ISO. Size106880 bytes,
SHA25693ef5d667ded662d6abf13133082bef138f9c88132365ff8b11f8abf2129dd14.
The verifier locates it through CURRENT FST rather than assuming a fixed ISO
file offset,and checks size/hash. No extraction/texture conversion/runtime lookup
was performed. Native reuse needs the original TPL and actual indexing/palette/
filter/library contract;replacement artwork is not established by this cache loop.

## Source update order and implementation acceptance

The sole direct DOL driver caller is marioStMain at8006f9a4,request32 in the
complete local chain. makeKey,battleMain,camMain,mapMain and evtmgrMain precede
it;mapDisp/Mario/mobj/NPC/window/world calls also precede it. Thus effects created
by event callbacks can be eligible later in the SAME marioStMain invocation.
Animation main is an earlier separate request;actual queued display execution,
pose Main inside draw,pause/outer scheduling/retrace and measured cadence remain
external contracts. No “rendered frame equals one effect tick” claim.

Check captured count/array versus live slot fields/per-record GP,both exact
context branches,active01 versus any-nonzero predicates,null callbacks,ignored
results,same-pass later allocation,negative count still running texture tail,
cache comparison after callbacks,free/manager reload,each async/sync/tag capture,
async0 retry versus alloc0 accepted tag,all seven path prefixes and raw index
arithmetic. Driver/resource/platform/scene/numeric and original traces remain
EVT-01/JUMP-01/DMG-02 work despite full local loop closure.


[Complete formatted forwarding](FILE_FORMATTED_REQUEST_FORWARDING.md) and
[file-handle release](FILE_HANDLE_RELEASE_AND_EFFECT_CACHE_OWNERSHIP.md) now
close called wrapper/free leaves. fileFree decrements signed reference count/
marks state2;it does not deallocate payload. Shared filename/varargs and actual
async/load/collection/TPL/backend/source/runtime requirements remain explicit.

[Complete async request](FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md) confirms
first DVD-open failure returns-1. This is nonzero under the driver's gate,so
it still calls fileAllocf and can advance its selector tag after a null result.
[Completion](FILE_ASYNC_COMPLETION_AND_DVD_REQUEST_STATE.md) distinguishes
pending clear,relocation/deferred close and user callback;none is a texture-ready
boolean supplied by this driver. Full backend/source/runtime remains separate.
