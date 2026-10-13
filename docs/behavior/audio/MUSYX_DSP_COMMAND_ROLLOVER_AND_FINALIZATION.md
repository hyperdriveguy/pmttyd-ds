# MusyX DSP command rollover and finalization

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salBuildCommandList initialization,
twelve rollover blocks and finalization REGIONS listed below. Whole builder
80288e00/25f8hex fingerprinted; unrelated command/voice/studio/budget regions
remain open. Addresses/offsets/sizes/masks hexadecimal; pending byte counts
explicitly decimal. Read [allocation](MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md),
[studio prepass](MUSYX_DEPOP_ACCUMULATION_FADE_AND_STUDIO_PREPASS.md) and
[sorted voice loop](MUSYX_DSP_VOICE_SORT_AND_COLLECTION.md).
Static publication/cache requests, not original command/DSP/audio execution.

## Global cells and complete initialization region

| Cell | Address | Width |
| --- | --- | --- |
| dspCmdLastLoad | 8041ef54 | Word |
| dspCmdLastBase | 8041ef58 | Word |
| dspCmdLastSize | 8041ef5c | Half |
| dspCmdCurBase | 8041ef60 | Word |
| dspCmdMaxPtr | 8041ef64 | Word |
| dspCmdPtr | 8041ef68 | Word |
| dspCmdFirstSize | 8041ef6c | Half |
| dspCmdList | 8041ef70 | Word |

Region80288e14..80288e38 exclusive: capture dspCmdList=P; LastLoad=0;
compare raw second argument to200 decimal for following budget work; retain raw
first argument X at stack.word08; dspCmdPtr=P; CurBase=P; MaxPtr=wrap32(P+180).
The comparison/budget branch continues outside the region. LastBase/LastSize/
FirstSize are NOT initialized here. No pointer/allocation/count/IRQ/reinit guard.
Allocation requests800hex bytes, while180hex is the local chunk stride; this
entry does not enforce total allocation extent on later chunk growth.

## All twelve complete local rollover blocks

Each row gives exclusive region end, command0d site and pending bytes k. Gates
outside these blocks remain separate; rows do not assert all blocks execute in
one invocation. Initial block additionally clears retained studio.word4c, as
recorded in the prepass contract.

| Complete region | Command0d site | k, decimal |
| --- | --- | --- |
| 80288f78..80289020 | 80288f98 | 6 |
| 8028906c..8028910c | 80289084 | 12 |
| 8028a868..8028a908 | 8028a880 | 6 |
| 8028a980..8028aa20 | 8028a998 | 2 |
| 8028aa88..8028ab28 | 8028aaa0 | 10 |
| 8028abb0..8028ac50 | 8028abc8 | 10 |
| 8028acc4..8028ad64 | 8028acdc | 10 |
| 8028adec..8028ae8c | 8028ae04 | 6 |
| 8028af94..8028b034 | 8028afac | 6 |
| 8028b09c..8028b13c | 8028b0b4 | 6 |
| 8028b1a8..8028b248 | 8028b1c0 | 10 |
| 8028b298..8028b338 | 8028b2b0 | 10 |

For each block capture current P and MaxPtr E. Compare UNSIGNED
wrap32(P+k)>wrap32(E-8). False (including exact equality) skips ALL rollover
writes/store and reaches the pending command outside the block. No widened
comparison, total-buffer bound or second post-rollover fit test. Address wrap
can change admission; arbitrary pointers are not made safe by this local check.

## Exact rollover link, deferred size and store protocol

On admitted rollover:

1. Store half0d at CAPTURED P. Freshly read MaxPtr and dspCmdPtr, store high16
   of that fresh MaxPtr at freshPtr+2. Reread both, store low16(MaxPtr) at
   freshPtr+4. The high/low target need not come from one captured value.
2. Freshly read dspCmdPtr, LastLoad and CurBase. Compute
   N=wrap32(wrap32(currentPtr+8-CurBase)+3)&0000fffc. Thus N is an aligned
   LOW16-sized word, not unbounded rounded32 length. Retain N across the child.
3. If captured LastLoad nonnull, store halfN at LastLoad+6, then read fresh
   LastBase and unsigned LastSize; request DCStoreRangeNoSync(LastBase,LastSize).
   Null LastLoad instead stores N into FirstSize, with no child request.
4. AFTER that branch/child, reload MaxPtr, dspCmdPtr and CurBase. Store in order:
   LastLoad=freshPtr; LastSize=low16(retained N); LastBase=freshCurBase;
   dspCmdPtr=freshMaxPtr; CurBase=freshMaxPtr;
   MaxPtr=wrap32(freshMaxPtr+180).

Command0d's target address is the next chunk. Its size half at+6 is NOT filled
in this block: a later rollover/finalization supplies the NEXT chunk's size.
The current block fills the PREVIOUS link's size field with this chunk's N,
then stores the preceding chunk using its captured/published LastSize. On first
rollover, FirstSize records the initial chunk's N. Current chunk's link/header
is stored later, not by an unconditional immediate current-chunk store here.
No initial zero placeholder at new link+6 is written by these blocks.

Use source publication order and fresh reads rather than a captured structure
transaction. A child or unsupported alias/shared writer can change the later
MaxPtr/current/base values; N still reflects the pre-child captured reads.
All adds/subtracts wrap32 and sizes store low16. No allocation extension/free,
cache-child success check, DMA/DSP send or rollback in these blocks.

## Complete final command publication and cache tail

Region8028b338..8028b3e0 exclusive follows the final rollover check (k10).
Append halfwords in order:

| Offset from command start | Value/source |
| --- | --- |
| 00 | 0e |
| 02 | High16(fresh dspSurround8041ef80) |
| 04 | Low16(fresh dspSurround8041ef80) |
| 06 | High16(captured stack.word08 X) |
| 08 | Low16(fresh stack.word08 X) |
| 0a | 0f |

Command pointers are freshly loaded for each store. X high is captured before
writing command0e; X low is reread later. No fixed-channel/output-buffer type
inference is added to raw X; its full callers remain separate.

After the five0e halfwords, capture currentPtr, add0a and publish dspCmdPtr;
compute another+2 and publish dspCmdPtr AGAIN, THEN store half0f through the
retained pointer at oldPtr+0a. Pointer publication precedes this final command
store. Under stable disjoint globals/memory total cursor advances12 bytes.

Capture LastLoad, CurBase and dspCmdPtr. Compute
N=wrap32(wrap32(currentPtr-CurBase)+3)&0000fffc, without rollover's additional8.
If LastLoad nonnull, write halfN to LastLoad+6, read fresh LastBase/LastSize and
request DCStoreRangeNoSync for that previous chunk. Otherwise set FirstSize=N without
that request. Then reread CurBase/currentPtr and ALWAYS request
DCStoreRangeNoSync(CurBase,wrap32(currentPtr-CurBase)), using RAW unrounded current
length. It is not the retained aligned N. The ordinary ABI epilogue follows;
no independent scalar status is assigned and child returns are ignored.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_dsp_command_rollover_and_finalization_evidence.json)
pins fourteen complete regions and linked whole-builder/cache child:16 spans,
533 instruction anchors/14 direct calls. Whole-body aligned instruction census
found the twelve exact li0,0d rollover sites listed; all region words are pinned.
Twelve threshold/equality/wrap, seven aligned-low16 size and three final-command
layout vectors are deductions, not original execution/cache/concurrency tests.
Full fresh builder ELF/listing remains `/tmp`; no bodies/binary assets committed.

Remaining: pending command payload/admission/budget/studio/voice regions,
actual pointer/cell/X/size/allocation/cache/DSP consumers and valid total extent/
wrap/alias/lifetime/IRQ/concurrency domains; original chunk/link/store/command/
DSP/audio traces. Fourteen local regions answered; whole builder/full native
binding and EVT-01 remain OPEN.

## 2026-10-13 — Studio command follow-up

[Studio payloads/fades](MUSYX_DSP_STUDIO_COMMAND_PAYLOADS_AND_FADE_BINDINGS.md) closes ten input/aux/mode/surround/
compressor/chain/fade/loop regions around the rollover blocks. Remaining voice/
budget/admission/domain/cache/DSP/original audio execution stays open.

## 2026-10-13 — Cache-service source-name correction

[Cache range/source correction](GEKKO_CACHE_RANGE_SERVICES_AND_AUDIO_CALL_CORRECTIONS.md)
corrects earlier service-name attribution against US symbols/direct targets.
The body above now names the emitted original service; pointer/length/order
evidence stays unchanged. Hardware/handler/runtime cache effects remain open.
