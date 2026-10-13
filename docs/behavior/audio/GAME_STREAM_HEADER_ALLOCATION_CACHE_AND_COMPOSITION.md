# Game stream header allocation, cache setup and whole callback composition

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Scoped SoundSSPlayChEx_main800de6c0..800deab4
exclusive,253 instructions. Combined with
[matching/format/reset and exit ABI](GAME_STREAM_HEADER_MATCH_FIELDS_AND_FORMAT_PREFIX.md)
and [initial reads](GAME_STREAM_HEADER_INITIAL_READS_AND_PENDING_PUBLICATION.md),
this closes the whole local627-instruction callback800de104/9cchex. Addresses/
offsets/masks hexadecimal; counts/arithmetic decimal. Static deductions, not
original setup/audio observations. Read
[MusyX allocation ABI/length](MUSYX_STREAM_ALLOCATION_AND_LENGTH_PUBLICATION.md),
[pan routing](GAME_STREAM_PAN_SURROUND_CACHE_AND_REQUESTS.md) and
[cache callbacks](GAME_STREAM_CACHE_CALLBACK_TRANSFER_AND_INITIAL_READS.md).

## Length request and ordered zeroing before allocation

Retain OLD row V/GPR31, S/GPR29 and channel index I/GPR30 from earlier regions.
Request sndStreamAllocLength8027b7d0(3584,flags0) at800de6c8. Load V.word50
AFTER child, retain returned L, zero exactly L bytes using forward eight-byte
store groups then remainder bytes; L0 skips stores. No null/extent guard.
The recovered length child gives7168 bytes for these fixed inputs, regardless
of later ADPCM allocation flag. This overwrites the header buffer after its
fields/coefficient copies were consumed; it does not prove its actual capacity.

Read type half1a FRESH after primary zeroing; exactly2 loads V.wordd8 and zeroes
the SAME retained L bytes there. Every other type skips secondary zeroing.
These stores are not atomic or overlap-protected; buffer aliases can affect
later type/flag/field reads. No local cache service between zeroing and allocation.
Valid pool/buffer/header/coefficient/provider/lifetime domains remain required.

## Four allocation sites; raw results published without failure check

Fresh type half1a selects retained primary pan64 exactly for type1, otherwise0.
Then fresh V.flags0 mask100 selects ADPCM allocation branch. Each calls
sndStreamAllocEx8027b330. Label raw register arguments A..H=r3..r10 and stack
I/J low bytes at callerSP+b/f, K/L/M/N words at callerSP+10/14/18/1c, matching
the linked child contract. Do not confuse channel index I here with ABI label I.

Common A..H: (255, fresh stream buffer,3584,fresh U16 frequency18,0,pan,0,0).
I/J bytes0; L callback_sscallback800deda0. Primary M=channel index; secondary
M=channel index OR100hex. Callback/user tag does not prove index bounds or
checked generation. Captured unmatched index3 from prefix can propagate here.

| Branch/stream | Call | Buffer | Pan | K flags | N coefficients | Raw result store |
| --- | --- | --- | --- | --- | --- | --- |
| 100 clear/primary | 800de7f8 | V.word50 | retained64 or0 | 10000 | 0 | word4c |
| 100 clear/secondary | 800de858 | V.wordd8 | 127 | 10000 | 0 | wordd4 |
| 100 set/primary | 800de8bc | V.word50 | retained64 or0 | 10001 | V+90 | word4c |
| 100 set/secondary | 800de924 | V.wordd8 | 127 | 10001 | V+118 | wordd4 |

After PRIMARY allocation/raw store, read type half1a FRESH; exactly2 issues
secondary with fresh buffer/frequency. Other types skip it. No comparison of
primary/secondary result toffffffff, status/sentinel guard, rollback/free,
common frequency snapshot or primary-success dependency. Child failure results
are still stored and later consumed. Linked child interprets K10000 as deferred
activation; valid callback/user/buffer/ADPCM/DSP providers remain separate.

100-clear branch then freshly reads OLD flags and OR8/store EVEN if allocation
failed. 100-set branch has no corresponding OR8 write; existing8 is preserved.
There is no direct sndStreamActivate call here. Earlier initial IO can have
been requested before any allocation; later completion owns startup activation.
The source order must not be replaced with a success-conditioned transaction.

## Fade-in setup and inline mix using a recaptured row

Both branches load OLD floatc target, store it as OLD float14 step, then fresh
OLD flags OR10/store. No timing normalization, step clamp or float10 update.
The next SoundSSMain rise uses this step under its ordered mode processing.

Compute wrap(channelIndex*138), RELOAD S.word100, read OLD half6/low8 P and
form NEW row U=pool+offset. New flags0 exactly0 skips pan cache/mix. Otherwise
store P as NEW half6, reread NEW flags and require mask8. No mask2/null/index/
generation check. OLD target/step writes do not copy volume to a different NEW row.

Fresh NEW type exactly1 requests sndStreamMixParameter8027b814 at800de99c with
(newPrimaryID,signed fctiwz NEW float10 low word,P,low8 NEW half8,AuxA0).
Every non-one type follows linked pan formula: capture NEW float10 W; P<64
retains primary W, secondary=single(single(P*W)/63); otherwise retain secondary W,
primary=single(single((127-P)*W)/63). Literal8042190c is63.0 bits427c0000;
conversion biases802e3cc0/c8. No127 clamp; FP invalid conversion domains open.

Dual calls800dea38/800dea5c use Pan0/127, AuxA0, fresh respective IDs and low8
surround before each. Gains computed before primary remain retained; no type/
volume/flags/pool/status recheck between requests. Child ABI/IRQ/mix/companion
behavior is separate. After mix/skips, subsequent cache gate reads OLD V.

## Fresh mask400 starts file caching after allocation/mix

Fresh OLD flags mask400 clear exits. Set: load primary record28 and request
DVDMgrGetLength80144dcc at800dea70. AFTER child read OLD cursor5c as C, use
returned raw length L. N32768 when wrap(C+32768)<L STRICTLY, else N=wrap(L-C).
No C<=L/extent/nonzero guard. A=wrap(N+31)&ffffffe0.

Read primary record28 and cache buffer54 FRESH; request DVDMgrReadAsync80144de4
(record,buffer,A,C,_ssDVDReadAsync_cache_next800df7d4) at800deaac. Offset raw C,
without header/plane addition. AFTER request store retained N at OLD word70.
No returned-count/status gate, progress advance, cache flag write or callback
invocation here. The next callback advances cursor using70, flushes/transfers
fixed32768 and publishes progress; actual SDK/DMA/short-read domains remain open.

The shared epilogue restores GPR25..31/LR, paired/single FPR31 and112 decimal
stack bytes. No independent success/failure assignment or game counter/OS IRQ
request appears anywhere in this whole callback; children own nested services.

## Whole-body composition, evidence and remaining work

| Manifest | Region / words |
| --- | --- |
| [Prefix](../../../tools/game_stream_header_match_format_prefix_evidence.json) | 800de104..800de3cc:178;800deab4..800dead0 ABI:7 |
| [Initial reads](../../../tools/game_stream_header_initial_reads_evidence.json) | 800de3cc..800de6c0:189 |
| [Tail/composition](../../../tools/game_stream_header_finish_composition_evidence.json) | 800de6c0..800deab4:253 |

These disjoint spans cover ALL627 instructions with no gaps, verified against
the read-only ISO. Tail manifest pins eleven spans/253 new anchors/ten calls,
linked callbacks/constants and whole-body hash. Earlier manifests reverified.
Six zero/five allocation/five secondary-type/six cache/four mix-gate vectors,
allocation ABI/call/binding sites and627-word union checked; allocation/pan child
pins reverified. Generic verifier checks pins/targets, not semantic/provider/
runtime claims. Mathematical deductions separate from original execution.
Fresh full ELF/listing/scoped generator stay `/tmp`; no bodies/assets committed.

Local whole callback is answered; original execution/transitive/native fidelity
is not. Remaining: _sscallback, deferred DVD manager/open/read consumers and
actual setup/flags/pool/channel/header/type/frequency/buffer/coefficient/ID/
callback/target/step/pan/surround/cache providers and valid value/extent/alias/
lifetime/IRQ/concurrency/ABI/FP domains; transitive SDK/ARAM/AI/DSP and original
allocation-failure/initial-read/cache/short-read/callback/startup/audio traces.
EVT-01/native/full-game remain OPEN.
