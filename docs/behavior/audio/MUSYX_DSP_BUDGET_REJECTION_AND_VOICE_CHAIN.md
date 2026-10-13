# MusyX DSP budget, rejection cascade and voice chain

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salBuildCommandList REGIONS
80288e38..80288ebc,8028a654..8028a868 and8028a908..8028a980, ends exclusive;
initial unsigned comparison80288e20 separately anchored. Whole builder
80288e00/25f8 fingerprinted; remaining sample/voice startup regions stay open.
Addresses/offsets/masks hexadecimal; shifts/counts/scalar constants decimal.
Read [command initialization/rollover](MUSYX_DSP_COMMAND_ROLLOVER_AND_FINALIZATION.md),
[sorted voice loop](MUSYX_DSP_VOICE_SORT_AND_COLLECTION.md),
[envelope/update prefix](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md),
[depop](MUSYX_DEPOP_ACCUMULATION_FADE_AND_STUDIO_PREPASS.md) and
[studio commands](MUSYX_DSP_STUDIO_COMMAND_PAYLOADS_AND_FADE_BINDINGS.md).
Static cost/admission/request order, not measured CPU/DSP cycles or audio capacity.

## Exact initial accumulator and outer-loop seed

The earlier initialization compares raw unsigned second argument D against200
at80288e20; region entry uses that retained CR condition. If D<200, accumulator
B (r23)=10430. Otherwise read raw system word C at800000f8 and compute:

`K = unsignedHighMultiply(C,51eb851fhex) logical>>7`
`J = unsignedHighMultiply(K,d1b71759hex) logical>>12`
`B = wrap32(wrap32((D-200)*J)+10430)`

Then freshly read dspHRTFOn8041ef78; any nonzero adds45000 with wrap32.
Fresh dspCompressorOn8041ef74 nonzero adds3000 with wrap32. No scalar/input
unit conversion, maximum delay or measured original C asserted. D>=200 can
wrap the multiply/add; do not substitute a widened or saturating cost.

Clear stack.half10=0, retain studio base80407c68 at stack.word28, set outer
studio byte stack+24=0, branch to the existing fresh count test8028af84.
Only first mask half is cleared here; the voice setup region separately clears
four further halves. No allocation/state/table/list/FPSCR/IRQ check.
The studio/input command regions add their source costs later; do not reinterpret
B as an independent measured available-cycle budget.

## Complete voice cost tables and accumulation

V=r29, PB=r19, S=r16 retained by preceding code. This region begins AFTER
envelope advancement and the per-voice command-buffer DCFlushRange request.
Capture F=unsigned PB.half0c and PB.halfba. Original mixer table at80303f50,
16 unsigned halves, is:

`0,760,760,1470,760,1520,1520,2230,0,1265,1265,2470,1265,2530,2530,3735`

In order add with wrap32:1278, then table[F&f], then table[(F>>4)&1f], then
 table[(F>>9)&1f]. Source implements these byte offsets with rotate/masks,
not a safe lookup abstraction. The second/third index can reach31 despite the
16-entry declared table; no local upper clamp or error return. Out-of-table
addresses remain source behavior requiring reachable-flag/address evidence.
If captured PB.halfba==1, additionally add553; other nonzero values skip it.

Read unsigned mode=PB.halfa6. For mode0/1, row=mode; all other values use row2.
Read unsigned type=PB.half08. Add original dspSRCCycles803be440 at halfword
index3*row+type. Nine halves/three rows:

`2990,2990,1115; 3300,3300,1115; 3700,3700,1115`

There is no type<=2 guard. These are local cost-table roles; do not infer an
actual codec/SRC meaning, sound frequency or performance measurement from names.

Read raw C at800000f8 and compute unsigned limit K as above. Before comparing,
read PB.half44/46/48/4a/4c freshly, in order, add each value<<2 to B with wrap32.
These are the preceding envelope loop's command-pair counters. Reject ONLY if
unsigned B>K; equality and lower values reach admitted chain handling. No
rollback of preceding envelope/host/counter/cache mutations on rejection.

## Current voice and remaining sorted-array rejection

On rejection, read stack.word20 (new-setup flag). Only if it is0 AND retained
r17 (last envelope result)==0, request HandleDepopVoice(S,V). No separate
V.active==2 test on this current-voice path. Then always request
salDeactivateVoice(V), followed by salSynthSendMessage(V,1). Callback return
ignored; message observes post-deactivation state. Full callback semantics are
separate from numeric message identity.

Decrement remaining count r28 once; rebuild end pointer from fixed array
80408248+wrap32(remaining<<2). While SIGNED remaining>0:

1. Read pointer at end-4, inspect its bytef0. Exact2 requests
   HandleDepopVoice(S,that pointer); other values skip depop.
2. RELOAD pointer from end-4; request salDeactivateVoice.
3. RELOAD pointer AGAIN from end-4; request salSynthSendMessage(pointer,1).
4. end-=4, remaining-=1; test again.

The three pointer reads can select different records under callback/alias array
mutation. There is no rescan/re-sort/member/identity/count/index validation or
array entry removal. This path covers current studio's unprocessed sorted tail,
not already admitted earlier array entries. B is not reduced or retested here.

## Later active studios: captured successor before children

Read outer studio byte, add1 and truncate to byte as starting ordinal. For each
ordinal whose low8<fresh salMaxStudioNum8041efb9, select studio S2 at
80407c68+bc*low8(ordinal). Only S2.byte50==1 admits traversal; other values skip.
Capture S2.word48 as root once. For each V2 until null:

- Read bytef0 and capture V2.word0c NEXT BEFORE child calls.
- Exact active byte2 requests HandleDepopVoice(S2,V2); otherwise skip depop.
- Always request salDeactivateVoice(V2), then salSynthSendMessage(V2,1).
- Follow captured NEXT regardless of callback/link mutation.

After that studio increment ordinal, recheck fresh max byte. No second-list root4c
processing, new-root rescan, callback result gate or resource-free request in this
rejection sweep. Studio bounds/list/alias/cycle/lifetime domains remain open.
After all later studios, branch directly to8028a974, the produced-command flag
check below; do not enter normal current-voice chain insertion.

## Admitted command02/parameter-block chain and command03 gate

When B<=K, test retained previous PB pointer r18. Null branches to the separately
recovered six-byte rollover check then enters8028a908. Append command halves
`02,high16(PB),low16(PB)`, with fresh command-pointer loads for the split stores.
Set stack.word1c=1 between opcode and address-half stores; advance dspCmdPtr by6.
No pointer/type/alignment/cache-child status validation.

Nonnull previous PB enters8028a940: write current PB's high16 to previous.half0,
set stack.word1c=1, write current low16 to previous.half2, then request
DCStoreRangeNoSync(previousPB,c4). No command02 is emitted on this path.
Both paths retain r18=current PB. Shared reverse-loop tail subtracts4 from end
and1 from remaining; SIGNED remaining>0 admits next sorted record.

At8028a974, read stack.word1c. Zero skips command03/its rollover directly to
8028aa34. Nonzero admits the separately recovered two-byte rollover and
[command03 publication](MUSYX_DSP_STUDIO_COMMAND_PAYLOADS_AND_FADE_BINDINGS.md).
The final retained PB link is cleared/stored there even when the command flag
was zero. The region does not establish actual DSP traversal or hardware playback
of the published parameter-block chain.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_dsp_budget_and_voice_chain_evidence.json) pins three
complete regions/five linked fingerprints/two numeric tables: ten spans,
197 instruction anchors/ten calls, including initial comparison binding.
Seven initial-budget/eight mixer-index/six SRC-index/six rejection/three chain
vectors are deductions, not original execution/performance/domain proofs.
System-word fixtures are synthetic; no original clock/C measurement inferred.
Out-of-table vectors record indices only. Full original fresh builder ELF/listing
stays `/tmp`; no bodies/binary assets committed.

Remaining: startup/sample/voice-format regions, actual D/system word/flags/
mode/type/count/new-setup/result/table/array/list/PB/callback writers and valid
index/overflow/member/cycle/alias/lifetime/IRQ/concurrency domains; cache/DSP
consumers and original cost/rejection/chain/voice/audio traces. Three local
regions answered; whole builder/full native binding and EVT-01 remain OPEN.
