# Original NPC room reset and teardown

Read-only static evidence from the original US G8ME01 revision0 DOL. This
answers the room-lifetime part of [EVT-01](../../requests/EVT-01.md), following
[registry construction](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md),
[retained native ownership](NATIVE_RETAINED_NPC_REGISTRY_OWNER.md), and
[history ownership](TAGGED_MAP_NPC_HISTORY_GATE_AND_DEFINITION_SOURCE.md).
No original runtime observation or completed native teardown is claimed.

## Actual room-change caller and ordering

`seq_mapChangeMain`80007c2c calls `_unload`80007ab4 at80007e78. Before
that request, the old module pointer is freshly obtained through GP+15c;
if nonnull its +38 function is called, the module is unlinked with
OSUnlink8029ab40, its current GP+15c allocation is conditionally freed when
unequal to GP+160, and fresh GP+15c is cleared. The unload arguments are
current GP+12c and `_next_area`8041e5e8; a third register is also prepared
with `_next_map`8041e5c8. `_unload` captures its first two arguments and
compares their first three bytes when both are nonnull. A changed prefix
sets a retained local used by its later setup-data release branch. It does
not gate the actor/pose/event cleanup below.

The relevant unconditional request order inside `_unload` is:

| Call site | Request |
|---|---|
|80007afc|marioReInit8005b1d8|
|80007b00|marioInitCamId801a9514|
|80007b04|mapUnLoad80028d78|
|80007b0c|npcReset8004bafc(0)|
|80007b10|extReset800d6b7c|
|80007b18|mobjReset800612f0(0)|
|80007b20|offscreenReset80073394(0)|
|80007b28|imgAutoRelease80071828(0)|
|80007b30|effAutoRelease8005ecd0(0)|
|80007b38|animPoseAutoRelease8003b028(0)|
|80007b3c|evtmgrReInit800377b0|
|80007b40|caseReInit8006ed28|
|80007b44|itemReInit800aeb20|
|80007b48|iconReInit8001b64c|
|80007b4c|windowReInit8007fb94|
|80007b50|winReInit8017eb70|
|80007b54|statusWinReInit8013dc38|
|80007b58|bgReInit8000a860|
|80007b5c|winMgrReInit8023f70c|

The remainder of `_unload` has sound/system/map-specific conditions and
heap/setup-data release operations. Implementing the table's NPC/pose/event
subset is not completion of that full function. In particular map/resource
unloading precedes NPC record clearing in the original sequence.

## npcReset: exact selected-bank effects

`npcReset`8004bafc/74 captures the signed input selector in r30. Its work
pointer starts at ordinary registry8041e8b4; **any nonzero** selector adds14hex
and selects alternate registry8041e8c8. It does not select using GP+14.

1. Load selected work+4 capacity, then selected work+c actor-array pointer.
2. Call memset800050b4(array,0,capacity*340hex). Source multiplication is
   the low32-bit PPC result; native bounded capacities need not admit
   overflowing/corrupt source allocations.
3. Store selected work+0 active count0. Capacity, array pointer and registry
   state words are retained.
4. For selector0 only, load fresh GP through80415fd0, load fresh GP+168,
   and store halfword0 at that F+0. This pointer is read **after memset**;
   it is not captured before the clear.
5. Store npcMainCount8041e8ac=0 for either selector.

There is no per-actor loop, pose release, event-ID test/delete, group-link
walk, history clear, reallocation, or new GP+168 publication in this body.
The source clear erases init ID+118, regular ID+11c and context/link/pose
fields as bytes without performing their individual teardown callbacks.
F itself survives: only F+0's halfword changes. In particular F+4c history
records and their masks survive, as do the other F bytes, auxiliary buffer,
other actor bank and pose pool. `npcInit`'s full F580 clear is a different
cold initialization operation and must not replace room reset.

The original capacities established by npcInit are36 ordinary and16
alternate. The reset callback nevertheless reads the live selected capacity;
it does not hardcode those constants or reread capacity after memset.

## animPoseAutoRelease: complete release scan

`animPoseAutoRelease`8003b028/1cc captures input mode in r28. Let W be the
animation work pointer freshly loaded through80415dc0. Let P be a pose,
M a16-byte model-cache record, and T a12-byte texture-cache record.

The scan starts index0/byte offset0. At **each loop condition**, reload W,
then W+14 capacity; signed compare index<capacity. If continuing, read
that W's +10 pose-array pointer and add the retained byte offset. Thus W,
capacity and pool pointer are not one immutable captured scan snapshot.
Advance index by1 and byte offset by170hex after every examined record.

For each P:

1. Read P+0 flags. Skip unless bit1 is set. Read P+c mode and skip unless
   exactly equal to captured input mode. Modes are exact values, not simply
   a zero/nonzero classification.
2. Read P+4. If nonzero, take the shared-pose branch: read P+8, subtract1
   modulo32 bits, store P+8, reread P+8 and signed-test. A nonnegative result
   retains the pose and skips all following resource releases. A negative
   result proceeds directly to model release; it does **not** inspect/free
   P's smart-buffer flags in this branch.
3. If P+4 is zero, test bit40 in the flags originally captured by step1.
   If set, read P+15c and call smartFree8002f878. Then read fresh P+0 and
   test bit80; if set read P+164 and call smartFree. Do not reuse the old
   flags for the second test across the first provider call.
4. Model release: load fresh W, read P+10 model index, then W+0 model-array
   pointer; M=pointer+16*index. Read M+4 refs, subtract1, store M+4,
   reread M+4, signed-test. If positive, skip texture/file release.
5. For refs<=0, store M+0 flags0. Read M+c texture index, load fresh W,
   then W+8 texture-array pointer; T=pointer+12*index. Read T+4 refs,
   subtract1, store, reread, signed-test. If <=0, read T+8 file pointer,
   call fileFree8018a260, then store T+0 flags0. Regardless of whether
   texture refs stayed positive, read M+8 file pointer and call fileFree.
6. Store P+0 flags0 last. Other source pose fields are not memset here.

Steps4–6 occur in duplicated source blocks for shared/ordinary branches;
AutoRelease does not call public animPoseRelease8003b2d0. Its ordinary/shared
resource order matches that leaf, but its active/mode scan and final refresh
are additional behavior. Public Release takes a handle directly and has no
active/mode admission or final refresh.

After the scan, the condition's most recently loaded W remains in r3. If
captured input mode==1, store W+10c=0. Other input modes preserve W+10c.
Then **always call animPoseRefresh80041c84**, even when no pose matched.
Mode0 room cleanup therefore preserves W+10c and invokes refresh.

`fileFree`8018a260/44 is a retained file reference transition, not necessarily
an immediate allocator free. Null pointer or null file+a0 returns. Otherwise
it reads byte file+0; only status1 decrements signed halfword file+2, stores
it, rereads it, and when exactly0 stores byte status2. Other values/states
return without that publication.

## Refresh tail and native representation boundary

`animPoseRefresh`80041c84/25c first loads fresh W and tests W+104. If null,
request __memAlloc800300f0(heap0,size180000hex), then freshly reload W and
publish returned pointer to W+104. Load fresh W, copy W+104 into W+108,
then scan with fresh W/capacity at each condition and fresh pose base per row.
It processes any nonzero P+0 flags (not just active bit1), subject to W+10c:
nonzero W+10c excludes P.mode0; zero W+10c excludes P.mode1; other modes
remain eligible.

For each eligible pose, repeatedly reload W/P.model index/model-file chain
W+0→M+8→file+a0→word0 MODEL between each allocation. Call animAllocBuffer
80042ac8 and store these result fields in order:

| MODEL size source | P destination |
|---|---|
| MODEL+f0 *12 |+48|
| fresh MODEL+f0 *12 |+4c|
| MODEL+f8 *12 |+50|
| fresh MODEL+f8 *12 |+54|
| MODEL+13c bytes |+58|
| reread P+58 (no allocation) |+5c|
| MODEL+140 *4 |+60|
| fresh MODEL+140 *4 |+64|
| MODEL+12c *24 |+68|
| reread P+68 (no allocation) |+6c|

It finally stores P+3c=-1 twice. This is real scratch ownership work on
surviving poses, not an empty completion callback. A native pool may own
CPU shape/cache storage differently, but must name that adaptation and
preserve surviving state/resource ownership rather than assert the original
raw arena/guest-pointer ABI has run. Exact animAllocBuffer exhaustion and
all upstream smart allocation services remain separate providers.

## Event and case cleanup is context reinitialization

At80007b3c, after AutoRelease, `evtmgrReInit`800377b0/6c loads current GP
once and selects context803d8d30, plusa0hex when captured GP+14 is nonzero.
Read GP+38 then GP+3c; store the low word to context+9c, then high word
context+98. Read captured selected context+0 capacity and context+90 entries;
memset entries for capacity*1b0 bytes. Store runtime allocation count8041e860=0
and manager-active8041e858=0. Finally call evt_msg_init800d2db0, whose entire
body is blr. Context work/flags and scheduling count are not freshly cleared
by this function.

This erases all selected-context entries, including independent roots,
attached children and brothers. It does **not** call evtDelete, recursively
copy child locals to parents, run retirement hooks, or inspect each NPC's
init/regular event IDs. The source uses byte clear; native facade cleanup
must reconcile its own cursors/diagnostics without inventing source delete
callback effects. See [NPC setup](NPC_SETUP_ORDERED_INSTALLATION_AND_RETAINED_PROVIDER_FRONTIER.md):
its init/regular events are independent roots with actor context+170, not
children owned by a source npcReset loop.

`caseReInit`8006ed28/38 then captures initial case-array pointer, walks160-byte
records, clears only halfword flag bit1, and reloads the case-work pointer and
capacity each condition. It does not erase events. The actual native
FieldCasePool already represents this clear-bit operation separately.

## Individual deletion and battle lifetimes remain distinct

`npcDelete`8004b708/120 detaches each nonnull associated+330..33c by clearing
its owner+32c and the original slot; handles previous+324/next+328 splicing;
clears query bit2; reads signed pose+104 and calls Release if >=0; stores
pose=-1; clears active1; freshly loads GP+14 to choose ordinary/alternate
registry; reads/decrements that registry's active count. No init/regular
event deletion occurs in this body either.

`npcDeleteGroup`8004b420/44 chooses optional +32c owner, follows +324 until
head, then invokes _npcDeleteGroup8004b464/2a4. The latter includes recursive
+328 traversal, associated actor deletion and inline link/pose/count effects.
Neither complete body calls an event deletion service. Do not substitute
this graph operation for map reset's whole selected-array memset.

Battle entry/exit have different ordering: battle_init calls
npcReleaseFiledNpc8004ce58 at80073924. That helper conditionally snapshots
ordinary posed/query-ready actors into the retained auxiliary buffer and
releases their handles; it does not clear the ordinary actor registry.
`battle_exit` calls npcReset(1) at80073750, AutoRelease(1) at80073778,
evtmgrReInit at8007377c, npcRecoveryFiledNpc8004cd38 at80073780, then stores
fresh GP+14=0 at80073790. The event reset therefore selects the current
battle context before the explicit GP mode switch. F/history are preserved.
Do not apply ordinary mode0 room teardown at that boundary.

## Smallest honest native composition

Existing retained FieldNpcRegistryOwner/Binding provide stable selected
actor banks and F; add an admitted reset primitive over those **same** bytes,
not a cold owner replacement. EventNativeRuntime::reinitializeContext and
FieldCasePool's existing reinit operation model their respective source
clears. Native side cursors and published event handles need explicit
invalidation when selected context entries are cleared; normal per-event
retirement observers are not source calls made by ReInit.

BattlePosePool::release already owns ordinary/shared model/texture refcounts
and rejects unsupported smart-buffer flags. A future admitted auto-release
scan can compose that retained owner for supported records, using the
source active1 and exact mode filters and preserving mode1/mode2 records
when mode0 is requested. It needs complete up-front admission or retained
progress if any downstream operation can hold, so a retry never decrements
references twice. The existing native CPU shape/cache ownership is not
raw animPoseRefresh arena allocation. FieldPosePool's immutable installed
resource catalog may remain retained independently from released live pose,
model and texture references; claiming original fileFree/global cache state
would require the actual shared file manager owner.

Keep the binding/backing owner and actor address identities alive across
reset until selected VM/case cleanup has completed; do not permit an event
pass between zeroing actors and resetting that event context. Finish all
source lifecycle gates before registering the next map. This subset cannot
acknowledge mario/map/other resource teardown, module unloading, arbitrary
map transitions or battle return as complete. Original shared pose global
and current native separate field/battle pools are also distinct ownership
domains until their composition is explicitly implemented.

## Reproducible evidence

Fresh disassembly is under ignored artifacts/npc-room-teardown/, produced
by tools/inspect_dol_function.py from artifacts/reference/us_symbols.csv
and the read-only ISO. Caller evidence also exists in
artifacts/bero-case-unload.txt and artifacts/outer-bank-seq_mapChangeMain.txt.
These are original static instruction/data facts, not emulator captures.

| Body | Address / hex size | SHA-256 of original body |
|---|---|---|
| npcReset |8004bafc/74|21ca3d26d749c6737d10a90de8dfc514381602dd43ac0bc59788b00d9ed3eac9|
| animPoseAutoRelease |8003b028/1cc|51275c269573abc1c5c526330c39094e442a1e2230fa0f9e7e1e0dad14ac8709|
| animPoseRelease |8003b2d0/194|32d91c22e37a382d32a667e138d66e29f3d42fb2287b963bdc390154457863b6|
| animPoseRefresh |80041c84/25c|7a396663eaab9fb8e4b2f2e6610e0dfbc3ed0c81aafbcfaec813b1e35f9464b3|
| evtmgrReInit |800377b0/6c|5a0e1a5af9008548501e81de999f0f6d12a44f4d9642941a46127a4205e29feb|
| _unload |80007ab4/178|e217052388ebb30ee45513043dab0e0a1008cddcb6ab92c8c3b78f117a575099|
| seq_mapChangeMain |80007c2c/af8|517ca2e7b145ff0a315ba6a3c11312e083faced219eeed728b21c54dbadd66eb|
| npcDelete |8004b708/120|fb3c48f992092e0d564390f8b6a1f09b6ae29556ec76425d0579a2c270af9852|
| npcDeleteGroup |8004b420/44|b866e9ca1ad538fc72959db910a21dd097b2966a3c5425a3b890d2045c2d3ae5|
| _npcDeleteGroup |8004b464/2a4|ff6da3b55d571299c17bbdfbcbab05e3756e87eb3eb114e13350f7ea8e4faea8|

Only this new contract is authored for this task; source/build/shared-doc
changes and validation remain with the owning implementation work.
