# MusyX macro messages, VID copying and key groups

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdSendMessage80282580/244,
mcmdGetVID802827c4/80 and mcmdSetKeyGroup80282844/dchex; four complete opcode
regions below. Addresses/offsets/masks hexadecimal; counts/opcodes decimal.
Static voice-message operations, not story dialogue or original audio traces.
Read [fetch/dispatch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md),
[VID mapping](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md),
[macro activation/key-off](MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md),
[key-off request core](MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md) and
[voice kill](MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md).

## Shared domains and exact registrations

V sender/current voice, C command pointer, W0/W1 its two words. Byte numbering
Bn(W) selects bits8*n..8*n+7. Variable ID j.low5 addresses V+ac+4*j for j<16,
otherwise global803f8fa4+4*(j-16). Variable values are FULL words here; no S16
narrowing or external-controller source. synthVoice8041ee90, stride458;
synthInfo803f8540.byte210 supplies live count. No active-system/IRQ/pointer/
slot/member/identity-generation/alias/concurrency guard added.

| Opcode | Complete region (end exclusive) | Action |
| --- | --- | --- |
| 42 | 80283398..3a8 | mcmdSendMessage(V,&shared cstep8041ef08) |
| 43 | 802833a8..418 | Inline receive to variable |
| 44 | 80283418..428 | mcmdGetVID(V,&shared cstep) |
| 89 | 80283754..764 | mcmdSetKeyGroup(V,&shared cstep) |

Shortened ends retain80283 prefix. The three stubs ignore child returns and
join with local stop marker unchanged (initial0). Receive also joins normally.
PC already advanced8 by fetch; child activation can alter target voice state.
Only these four regions are closed here, not the whole interpreter.

## Complete send: payload first, then route selection

Capture W1 first and payload=full variable word at j=(W1>>8).low5. Then read
W0. B1(W0)==0 selects macro/callback route; nonzero selects variable VID route.
Later target ID uses captured W1 even if child/shared command state changes.

Macro route: requested macro=M=U16(W0.high16). M!=65535 scans ascending voice
ordinals i using U8(i) both for count comparison and pool addressing; fresh
count before each iteration, fresh synthVoice each admission. Require
record.word34!=0 and record.half104==M. Read record.localNode f8 without null
check, read node.publicID8, request vidGetInternalId(publicID). Missingffffffff
skips enqueue; otherwise enqueue to low8(mappedID) in a FRESH voice pool.
No mapped record's own full identity/activity/macro/group recheck. Multiple
matching records can resolve to the same chain head and each enqueue separately;
no deduplication or self-exclusion. Captured payload/M persist across children.

M==65535: capture synthMessageCallback8041ee94; null returns. Nonnull reads
sender.localNode f8 and node.publicID8, requests the indirect callback(publicID,
payload) at802826e4. Callback return ignored. No queue/mapping/null-node guard
or callback ownership/reentrancy policy established. Bootstrap clearing this
pointer does not prove all later writers/providers.

Variable VID route: full variable word at j=W1.low5 supplies PUBLIC ID; request
vidGetInternalId. Missing returns; otherwise enqueue to low8(mappedID) in fresh
pool, without full-generation/activity/macro recheck. Payload and target indices
use different bytes of captured W1.

## Exact enqueue and conditional message-handler activation

Target T: count byte440, read index441, write index442, four word entries444.
Read count; count>=4 skips ALL queue/trap work. count<4:

1. Store count+1 at440.
2. Read RAW write-index byte442; store payload at T+444+index*4.
3. Reread index442, store (index+1).low2 at442.
4. Read T.byte68;0 skips activation. Otherwise read T.word58;0 also skips.
5. Read word64, store it as PC38. Reread word58, store as macro base34.
   Store word58=0, then request macMakeActive(T).

No write-index low2 mask BEFORE the entry address, no ring validation, overwritten
entry clear, message-source/public-ID storage, preserved prior PC/base or queue
rollback. Index normalization happens after the raw indexed store. The trap
is consumed before activation; child return ignored. Queue count rises before
payload publication. A full queue does not activate a pending handler.

## Complete receive: empty zero versus raw-index pop

Opcode43 reads count440 and initializes output0. count!=0 reads RAW read index441,
loads entry444+index*4, stores (index+1).low2 to441. Then reread count440,
decrement1/store low8. Entry contents remain unchanged; no count<=4/index<=3
guard, shared pop atomicity or sender identity validation. Initial count0 skips
index/entry/count writes and keeps output0.

Finally reread SHARED cstep.word0, destination j=(word>>8).low5; store full output
word to the current voice/local or global variable slot. Empty receive therefore
writes0, not an unwritten fallback/wait/stop. Message payload0 and empty output0
are indistinguishable through that variable alone. No handler activation here.

## Complete VID copy

mcmdGetVID captures W0. Destination j=(W0>>8).low5. B2(W0)==0 dereferences
V.localNode f8 and copies node.publicID8; nonzero copies raw V.word108 (child
reference). Store full word to local/global variable. No public/internal ID
conversion, valid-child lookup, node-null check, branch on sentinel, ID allocation
or separate success result. Both paths overwrite the selected variable.

## Complete key-group eviction request

mcmdSetKeyGroup retains V and FIRST stores V.byte106=0, before reading W0.
Requested group G=B1(W0), kill selector=(B2(W0)!=0). G0 returns with cleared
own group and no scan. G!=0 scans ascending full ordinal under fresh count,
reloads synthVoice each iteration, requires word34!=0, low flags118 bit2 clear
(high114 read but AND0 contributes no admission bits), byte106==G.

Admitted record: selector false requests macSetExternalKeyoff(record); true
requests voiceKill(RAW ordinal), not a masked ID. Child return ignored. After
scan store retained original V.byte106=G. Clearing own group before nonzero
scan normally excludes that same record; no pointer-based self-exclusion or
captured-pool guarantee. Macro IDs half104 and group byte106 are distinct.
No list cleanup/counter/priority mutation is performed locally; complete child
semantics and caller state determine those consequences.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_macro_messages_vid_keygroup_evidence.json) pins
three complete bodies/four regions/six linked identities and original dispatch
table:14 spans,183 anchors/11 direct calls/four opcode registrations.
Five enqueue/four receive/six key-group/five variable-index vectors are static
deductions, not original execution. Full fresh ELFs/listings stay `/tmp`; no
original bodies/binary assets committed. Hashes do not establish graph validity.

Remaining: macSetExternalKeyoff body, callback owner/provider, queue/trap/base/ID/group/variable/command/
count/pool writers, valid ring/slot/member/generation/null/duplicate delivery/
alias/IRQ/reentrancy/concurrency domains, other opcode paths and original
message/callback/activation/group/audio traces. EVT-01 remains OPEN.
