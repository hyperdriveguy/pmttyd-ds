# Ending next slot/display owners and public sound registry

2026-10-14 US G8ME01 disc0 revision0. Read-only original ISO/DOL and native
source audit. This is the next bounded binding map after actual AudienceMain;
it does not assert that live battle display or audio backend work is complete.
Original inbox ending request remains its existing owner. No code/shared
documentation edits, build rerun, or commit are part of this delivery.

## Ctrl throw admission: exact source reads and requests

Complete CtrlProcess801a5ca8/3a8 retains B at801a5cb8 and A fromBaseGetPtr.
At801a5f00 it reads retained B19078. Mask20000 sets signed A137f4=10000
BEFORE testing retained Bef4 bit80. Clear bit80 returns to603c. Otherwise
fresh signed counter A137f0>=max A137f4 skips. Counter0,1,2 select bound1800,
6000,30000 respectively; unsupported values do not initialize a safe fallback
bound in this source branch. Fresh B19078&20000 overrides bound60.

irand(bound) nonzero skips. Zero initializes local signed index[i]=i for200
seats, then performs1000 swaps requesting irand200 twice each. It attempts
ItemOn801a5aac(index,0,1) in resulting order. Only return1 increments A137f0
(fresh reread/wrapping+1/store) and ends the scan; other returns try the next
seat through200. No actor-count/pouch8c/flag1 gate wraps this tail.

Actual Init publishes counter0,max0..3 from its separate original irand100.
Zero actors legitimately reject ItemOn as documented in
[live audience admission](AUDIENCE_LIVE_SOUND_WINDOW_THROW_AND_DISPLAY_ADMISSION.md),
including original type0 item table firstWORD121, seat4..15 predicate and
GetExist0. Preserve admitted random/shuffle/service requests even then.

## BreakSlot_Init establishes real controller/reel owners

BreakSlotGetPtr8020524c/10 returns current B+1616c. ReelGetPtr80205210/3c
obtains that pointer freshly and adds18hex+signedIndex*5c (no bounds guard).
Init8020507c/194 has no arguments, captures SlotGetPtr, copies the following
original arrays to local stack **before** clearing the owner:

| Source array | WORDs | Payload |
| --- | --- | --- |
| 802fe040 | 8 | 0,1,2,1,2,0,3,4 |
| 802fe060 | 8 | 3,1,0,1,4,2,0,2 |
| 802fe080 | 5 | 0,1,2,3,4 |

It memset(controller,0,134hex), stores controllerWORD4=0, then ReelGetPtr0
and assigns three reelWORD0 ordinals0..2 and reelWORD8=0 in ascending order.
For each reel separately: GetReelPtr(index), WORD18=count8/8/5, BattleAlloc
800f8934(count*4), returned pointer→WORD14, memcpy exact source array bytes
to that owner. There is no null-result/failure branch before memcpy here;
the allocator remains a real admitted provider. Use the map heap implementation
already recovered for BattleAlloc, not an unrelated pointer/count placeholder.

## Complete used BreakSlot_Main idle path

Main80203aa0/15dc is ordinary no-argument C service; it supplies no event
readiness return contract. Capture B first, then exact requests:

1. AudienceBaseGetPtr801a86a0; retain A (not used in idle branch).
2. BreakSlotGetPtr8020524c; retain controller C.
3. BattleGetMarioPtr800f87a8(captured B).
4. BattleGetPartyPtr800f8728(captured B). Both returned actors unused here.
5. C.word4 unsigned dispatch0→80203b10. GetReelPtr0; count3 reelWORD8==2.
   Actual initialized all0 means count0!=2; skip controller-state writes.
6. Common802043c4 calls GetReelPtr0 AGAIN, then processes each of3 reels.
   reelWORD8 unsigned0 dispatch→802043fc: reread reelWORD4, clear only bit1,
   store flags at80204408; then BYTE58=ff at8020440c. Ascending stride5c.

This preserves controller/reel pointers, data arrays, ordinals and all other
fields. State0 with exactly two reelstate2 has a separate original branch:
read reel1WORD10, reel0WORD10; if unequal set C4=1,C8=0. It must not be
substituted with the fresh three-zero result after later reel-state producers.
Nonzero controller/reel states remain full original active providers.

## Registered slot display is not a no-op even without reel vertices

BreakSlot_Disp80203a68/38 always calls dispEntry80010bc4 with camera8,class0,
callback BattleBreakSlotDispReel802036d4, parameter0, float priority490
(raw constant804268ac=43f50000). It does not inspect controller/reel state.
The actual callback802036d4/394 obtains ReelPtr0 and first walks3 reels;
flagWORD4 bit1 clear skips its initial icon pass. It gets ReelPtr0 again and
tests reel0 state:1..4 selects scissor(0,82,608,32), state11 skips that scissor,
otherwise requests GXSetScissor802ba644(0,176,608,128), including state0.
It then gets ReelPtr0 again and skips its second3-reel draw pass when flags1
are clear. Finally it ALWAYS restores scissor(0,0,U16 GP170,U16 GP172).

Original idle pass therefore has no reel vertices but real callback/viewport
state requests. A declared 3DS scissor mapping can implement the used logical
rectangle with the actual native framebuffer dimensions and restore state;
it is not original608-pixel output equivalence. Active icons/reel geometry
remain their source render services, not synthetic native reels.

## btlDispMain: first surviving-unit provider and complete footer order

btlDispMain801024f0/b94 captures B at8010252c. Iterate64 registry indices;
each BattleGetUnitPtr800f88cc uses **fresh global B**, whereas several later
calls/footer use retained captured B. Null unit skips. Trio ending keeps
Mario/Goombella/Frankly; removal of3 enemies does not prove empty display.

For an ordinary surviving unit, the initial special kind223 carrying/following
branch is not admitted by actual kinds. The unconditional used prefix:

- Capture old U.BYTE312, OffUnitFlag801277ac(U,2) (UWORD1c).
- If old alpha differs BYTE311, OnUnitFlag801277bc(U,2); if target higher,
  add12 and clamp no-higher-than target; if target lower subtract2 and clamp
  no-lower-than target. Store LOW8 resulting alpha→BYTE312.
- **BtlUnit_HpGaugeMain80125d88(U)** at8010275c is the first unconditional
  transitive provider. Native animation/render projection does not close it.
- FLOAT U114=1 (literal8042224c=3f800000).
- BtlUnit_GetStatus80127f34(U,11,&signedByteDuration,&signedByteStrength),
  then status10 with the same temporary outputs. Status11 positive duration
  and negative strength scales U114 by max(.5,single(.5*strength+1)); status10
  positive duration/positive strength scales by min(2,single(1+strength)).
- BattleStatusEffectMain801966f0(U), then BattleStatusIconMain80253ca8(U).
- Walk linked parts U14→P0. Actual pose handle P1c0=-1 skips a part; otherwise
  original status pose selection, animation requests/update, offsets, queue,
  shadow, Vivian-main, and pose-sound providers execute as their flags admit.

The original complete static direct-call inventory (site,target,name) is saved
in ignored ending-next-btlDispMain-calls.json. It supplies69 direct-call sites;
indirect draw consumers are separate. The original body remains the authority
for each status/part branch. A native pass that executes only the footer must
retain the unit prefix/part graph as pending, not mark wholeDispMain complete.

After all64 units the exact unconditional footer at80103030 is:

1. BattleStockExpDisp8010045c().
2. BattleStageDisp80139084().
3. BattleCommandDisplay80122870(captured B).
4. BattleAudience_Disp801a6c18().
5. BattleBreakSlot_Disp80203a68().
6. Reread captured Bef4; if10000set, clear10000/store; reread again, if20000set,
   clear20000/store. This happens AFTER all five services, not at entry.

StockExpDisp scalar logic: signed Bf00>Bf04 admits wrapping increment/store
Bf08; freshly read signed counter>=4 increments Bf04 (no local counter reset).
Bef8&2 then gates camera8,class1 callback80100258,parameter0,priority901
(80422238=44614000). StageDisp first BattleGetUnitPtr(current B,B420), ignores
the returned pointer in the wrapper, then registers camera4,class2,param0
callbacks80138da8 priority5000 and801389a0 priority6000 (80422b34/38).
Audience four callbacks/window gate remain in the linked live contract.
CommandDisplay and every queued consumer retain their original own gates.

## Original public SFX bootstrap versus native NDSP channels

[Source provisioning](../audio/POSITIONAL_SFX_SLOT_PROVISIONING.md),
[stop](../audio/BATTLE_POSITIONAL_SFX_OFF.md), and
[accessors](../audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md) already close
the40-record public pssfx bank803df988 with28hex stride. Bootstrap psndInit
sets each keyWORD0=-1,engineWORD18=-1,flagsHALF6=0; it does not reset generation
HALF4. This initialization must be the global sound owner's real lifetime,
not repeated at AudienceInit or battle exit. Do not derive key/generation
from native DSP voice number or audience record flags.

Original On800d9934 captures eight arguments, resolves original raw/string
key, content/global/story gates, selects the last free slot or first admitted
reclaim, writes key/defaults/pending flags and operands. Finally increment
global GHALF50 (G=803df6e8), store LOW16→P4, reload U16 G50 and return
(generation<<8)|slotIndex. This public handle is NOT backend engineWORD18;
pending Main owns actual voice creation and its result. Generationwrap0 is
possible and encoded consistently. Unknown/unassigned request failure retains
the separately declared native failure adapter; no fabricated source key.

Off800d95f8 rejects raw-1, otherwise selects LOW8slot with **no generation
validation**. Free key-1 returns unchanged. Pending flags1 clears key then
engine-1 without backend stop. Started slot chooses SS/song/effect stop by
original descriptor bits80000000/40000000 before clearing key/engine. It never
changes P4 or global generation. Chk800d95a0 alone compares encoded generation
with U16P4 before key liveness. Handle0 is invalid only with an actual mismatch
or free key; it cannot be rejected solely because numeric0.

Read-only native inspection of source/runtime/audio.cpp and classAudio in
include/runtime/platform.hpp confirms current NDSP services own8 fixed cue clips on
channels2..9; music uses1,0,18. Audio::play cues and fieldNPC requests do not
return encoded original handles and contain no40-record generation/key owner.
Native waveform/channel availability therefore cannot answer sourceChk by
itself. Implement a retained typed original public registry independently;
an admitted started request may map its engine owner to a real NDSP voice.
Do not infer that this projection already establishes MusyX macro/resource/
playback cadence, which remains the actual backend dependency.

## Evidence and validation

New full ignored ELF/disassemblies and direct-call inventories use
artifacts/recovered-temp-evidence/ending-next-*. Source body hashes are the
ones pinned by the existing top-bank4 contract: Main80203aa0/15dc
3d1402e7ea435de74ede23e837a7f2252544e3f3d58179ba8a1c3365c4dec01b;
SlotInit8020507c/194
0dc91abdce67abf87e0c2808852f078071579933c644d3e3723edd7cd505848f;
btlDispMain801024f0/b94
6281c04f5a9784643665fd82f9f0bbc37f053f52fb2cdf672076049a4c17802f.

Source SFX verification passed:
`python3 tools/verify_overworld_roads.py --manifest tools/positional_sfx_corrections_evidence.json`:
14 complete/scoped DOL spans,85 anchors, no runtime/playback equivalence claim.
Scoped documentation diff check passed. Native tests/build remain parent-owned.
