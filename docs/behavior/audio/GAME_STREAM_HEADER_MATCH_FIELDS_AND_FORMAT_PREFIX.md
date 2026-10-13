# Game stream header matching, fields and format prefix

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Scoped SoundSSPlayChEx_main800de104..800de3cc
exclusive (178 instructions), plus exit ABI800deab4..800dead0 (7).
Whole callback800de104/9cchex remains in progress; this contract does not answer
allocation/initial IO/cache continuation. Addresses/offsets/masks hexadecimal;
counts/arithmetic decimal. Static deductions, not original header/playback traces.
Read [public header request](GAME_STREAM_PLAY_WRAPPERS_REPLACE_SLOT_AND_HEADER_READ.md)
and [cache/completion consumers](GAME_STREAM_COMPLETION_MATCH_CURSOR_AND_STARTUP.md).

## Negative result exits; unmatched result still processes third row

Inputs raw result R/r3 and record callback pointer P/r4. Signed R<0 returns
before loading sound state/pool/header. No flag/record/slot cleanup on this path.
Other values, including0, are not validated against requested256 bytes and do
not serve as a header length operand.

Capture S803e0138 and pool S.word100 ONCE. Unrolled checks for channel0,1,2
require each row U16 flags0 nonzero, then compare wrap(row.word28+40hex)==P.
Only primary record matches are considered; first admitted match wins.
No mask8/2/1/type gate, null pointer, generation or callback-record extent check.

If NONE matches, retain THIRD row anyway and set channel index3. Execution
falls into the same header-processing label800de1a0; no unmatched return exists.
This also happens when all three flags are0. Consequently header loads/writes
can affect an inactive/unmatched third row, and later setup receives index3.
Do not insert a native unmatched guard into an original source contract.
Valid callback/provider/lifetime domains may exclude this case, but that requires
separate evidence. Row V stays captured; no pool recapture in this prefix.

## Retained header pointer and ordered field mapping

Load H=V.word50 once; load unsigned format byte H[0] and retain it. No magic,
version/count/byte1/alignment/bounds/header-pointer check precedes access.
Ordered mapping uses big-endian loads; word H4 narrows to a halfword, not a
validated enum. Header sources are not snapshotted before the row stores, so
aliasing can affect later reads.

| Header offset/read | Row offset/store | Consumer description |
| --- | --- | --- |
| 2 / U16 | 18 / U16 | cached frequency |
| 4 / word32 | 1a / low16 | type |
| 8 / word32 | 1c / word32 | common length |
| c / word32 | 20 / word32 | common restart |
| 10 / word32 | 88 / word32 | primary end |
| 14 / word32 | 110 / word32 | secondary end |
| 18 / word32 | 8c / word32 | primary restart |
| 1c / word32 | 114 / word32 | secondary restart |
| 20 / U16 | 76 / U16 | primary parameter |
| 22 / U16 | fe / U16 | secondary parameter |
| 24 / U16 | 78 / U16 | primary parameter |
| 26 / U16 | 100 / U16 | secondary parameter |

Descriptions reflect recovered consumers, not a complete authored file schema.
Type word10002hex stores2 and admits later type2 checks. No signed field
interpretation beyond the explicit signed-half loads below is established here.

Read fresh V.word20. Exactlyffffffff stores0 at20 WITHOUT clearing existing
flags200. Every other value, including0, reads fresh flags, OR200/store.
The source implements the sentinel through wrap(word20+10000hex)==ffffhex;
it is exactlyffffffff, not a signed-negative range. Prior200 can survive the
sentinel path. No cross-field end/restart/range validation.

## Format0,1,2 branches; other bytes preserve prior format state

Format0: fresh word1c copies to88, then reread1c copies to110; fresh word20
copies to8c, then reread20 copies to114. Set header half4=40hex (64 decimal).
Does not clear prior flags80/100. Original H10..1c values were already stored,
then overwritten by these copies, even for non-two types.

Format1: fresh flags OR80/store. Signed half76 loads/stores into half7a,
signed half fe into half102; low16 payload remains identical. Set half4=40hex.
Does not clear100 or replace end/restart fields. No coefficient-table copy.

Format2: fresh flags OR100/store. Copy32 bytes H+5c..7b to V+90..af using four
iterations of eight ordered byte loads/stores. Fresh type half1a exactly2 then
copies32 bytes H+bc..db to V+118..137 with the same ordering. Every other type
skips secondary coefficients. These are forward byte copies, not atomic
snapshots or overlap-protected memcpy. Header pointer H is retained; primary
copy/aliasing can affect the later type/source reads. Set half4=100hex (256).
Does not clear80, convert coefficients or validate type/count/format combination.

Formats3..255 skip these branch writes and fall into reset with half4/format
flags/coefficients preserved. Format byte is unsigned0..255; no rejection or
fallback-format assignment occurs. Common field/sentinel writes already happened.
The names ADPCM/PCM and full file semantics require their consumer evidence;
this prefix pins the actual discriminants and raw transfers.

## Common reset and continuation boundary

All nonnegative-result paths reaching header processing perform these ordered
zero writes after format branches:

1. half74; word6c,5c,60; half7c,7e,80,82,84;
2. half fc; wordf4,e4,e8; half104,106,108,10a,10c.

This is18 stores, not a whole-row reset. Existing flags/type/step/volume/IDs/
buffers/header/end/restart remain unless changed above. Next instruction800de3cc
reads fresh flags and begins storage-path/initial-read processing; that and
allocation/cache continuation remain open here. The prefix contains no direct
child call, shared counter/IRQ request, activation or success assignment.

Entry creates112 decimal stack bytes, saves LR/GPR25..31 and paired/single
FPR31. Negative exit uses the shared seven-instruction epilogue restoring those
values/frame. Valid stack/ABI/paired-single domains remain separate; the callback
has no independent status assignment on this scoped path.

## Evidence and remaining work

[Manifest](../../../tools/game_stream_header_match_format_prefix_evidence.json)
pins two spans/all185 words, zero direct calls. Seven match/five sentinel/five
format/five type vectors, twelve field mappings/two coefficient copies/eighteen
reset stores checked. Generic verifier checks pins, not prose/provider/runtime
claims. Mathematical deductions separate from original execution. Fresh full
callback ELF/listing and scoped generator stay `/tmp`; no bodies/assets committed.

Remaining: whole callback allocation/initial-read/cache continuation, exact
actual header producers/schema/resource bindings and valid result/match/index/
format/type/frequency/end/restart/coefficient/pointer/alias/extent/lifetime/IRQ/
concurrency/ABI domains, deferred manager and original malformed/unmatched/
header/cache/playback/audio traces. EVT-01/native/full-game remain OPEN.
