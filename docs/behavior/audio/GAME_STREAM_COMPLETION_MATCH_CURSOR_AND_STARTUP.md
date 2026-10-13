# Game stream completion matching, cursor publication and startup

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete
_ssDVDReadAsync_activeChk800dfc98/418hex,262 instructions.
Addresses/offsets/masks hexadecimal; arithmetic/counts decimal. Static source
deductions, not original DVD/ARQ/IRQ/audio observations. Read
[interrupt requests and callback bridge](GAME_STREAM_INTERRUPT_REFILL_COPY_AND_LATCH.md),
[activation child](MUSYX_STREAM_ACTIVATION_COMPANIONS_AND_COMPOSITION.md) and
[pan routing](GAME_STREAM_PAN_SURROUND_CACHE_AND_REQUESTS.md).

## Shared counter and two independent scans

Inputs raw result R in r3 and callback record pointer P in r4. Retain both;
S803e0138 owns U16 counter20c/803e0344 and saved IRQ word210/803e0348.
Initial counter0 requests OSDisableInterrupts80299ad4 at800dfccc and stores
its returned state. Nonzero skips that request. Fresh counter increments with
low16 store. No overflow cap; initial65535 can wrap0. OS alias in the manifest
names that same entry address. No callback pointer/null/result-domain validation.

First scan loads pool word100 ONCE before the increment store and examines
three contiguous rows V, V+138, V+270hex. Nonzero U16 flags0 admits matching;
there is no mask8/2/1 gate. Compare wrap(V.word28+40hex) to P; a match selects
primary Q=V+28 and kind0. Then compare wrap(V.wordb0+40hex) to P independently;
a match selects secondary Q=V+b0 and kind1, overriding a same-row primary match.
No type2 gate protects secondary matching. The first row with nonzero selected
Q wins; no later matching row is visited. Wrapped Q/null/alias domains remain open.

For a matched row, signed R<0 exits to counter cleanup IMMEDIATELY, without
completion publications or the second scan. Nonnegative R is not a returned
byte-count operand; R0 and other nonnegative values all take the same completion
path. An unmatched negative result can still reach the second scan after the
first scan exhausts. These are local source branches, not an SDK error taxonomy.

## Requested-count publication and terminal flag difference

For selected Q, load word34 cursor C and word48 requested logical count N;
store wrap(C+N) at word34. Read U16 Q.half4c, reread word48 and store that count
at Q.word[3c+wrap((1-half4c)*4)]. No halfword bound or toggle is applied.
Store Q.half54=1 (row7c primary,104 secondary). This is distinct from startup
readiness at row7e/106.

Reread cursor word34 and end word60; unsigned cursor>=end selects terminal.
Terminal first loads word64 restart and stores it as cursor, THEN reads fresh
V.flags0. Mask200 SET clears pending mask1000 primary or2000 secondary.
Mask200 CLEAR instead ORs4000, retaining any pending mask. Nonterminal clears
the corresponding pending mask unconditionally using a fresh flag read.
No requested count/result equality, short-read check, extent clamp or event
call follows from these publications. Overflow can make the advanced cursor
less than end; source wrap vectors do not establish original valid file domains.

After completion, fresh V.half1a chooses shared-latch handling. S.half214/803e034c
exactly2 clears to0 only when signed primary busy half7c==1 and, for type2,
secondary busy half104==1 too. Every other type needs only primary busy==1.
Other shared/busy values skip the clear. This matches the interrupt consumer,
but callback timing/ownership and physical IO completion remain unobserved.
After this first successful matched row, continue to the separate startup scan.

## Second scan: readiness can precede this callback

Initialize index0/offset0 and scan at most three rows, reloading S.word100 for
EVERY row. Require flags0 nonzero and mask8 CLEAR. Rows with mask8 already set
are skipped regardless of matching. On admitted row U, compare each record
word28/b0+40hex to retained P; matching primary stores half7e=1, matching
secondary stores half106=1. Both writes can occur; no R check here.

Then read fresh type half1a. Exactly2 requires both signed readiness halves7e
and106 NONZERO; every other type requires only half7e nonzero. These values
need not equal1. The row can pass through preexisting readiness even when
neither record matches P. Initial mask2 does not block this scan.

First ready row: clear half74,half fc,half7c,half104 in that order. Request
sndStreamActivate8027cab4(primary word4c) at800dff14. After primary child,
reread OLD U.type half1a; exactly2 loads fresh secondary wordd4 and requests
activation at800dff28. Ignore both results. No rollback, flag recheck, ID sentinel
or primary-success condition. No readiness halves7e/106 clear here.

## Inline mix recapture precedes old-row flag8 publication

After activation(s), reload S.word100, read OLD U.half6/low8 Pn, form NEW row
W=pool+wrap(index*138). New flags0 exactly0 skips pan cache/mix. Otherwise
store Pn as NEW half6, reread NEW flags0 and require mask8 for mix.

This gate occurs BEFORE the callback's final OLD-row OR8. In a stable same-row
case admitted by mask8 clear, and absent a child's flag mutation, the mix gate
therefore skips all mix requests. A changed pool or child can produce a different
source path; do not move flag8 publication earlier to force a mix.

When admitted, fresh NEW type exactly1 requests
sndStreamMixParameter8027b814 at800dff88 with
(newPrimaryID, fctiwz NEW float10 low word, Pn, low8 NEW half8, AuxA0).
Every non-one type follows the linked pan contract's divisor63 split:
P<64 retains primary gain W.float10 and scales secondary byP/63; P>=64 retains
secondary and scales primary by(127-P)/63. Multiply and divide round separately
to single precision; signed fctiwz/spill yields raw volume words. No127 clamp.

Dual requests occur at800e0024 and800e0048 with Pan0 and127 respectively,
AuxA0, fresh respective IDs and fresh low8 surround before each call. Secondary
gain remains in FPR31 across the first child. No type/volume/pool/flag recheck
between requests. Valid FP/ABI domains and invalid conversions remain separate.

Whether new-row mix gates passed or failed, reread OLD U.flags0, OR8/store.
Then EXIT the second scan, so later ready rows are not started by this callback.
The old row's flag8 is published even if activation returned failure or the
new-row mix was skipped. This does not prove successful playback. The retained
S register used for the scan is overwritten by the NEW row only after selecting
a ready row; there is no subsequent scan iteration on that path.

## Counter exit, evidence and remaining work

Read counter FRESH. Zero skips decrement and restore; nonzero decrements with
low16 store, resulting0 requests OSRestoreInterrupts80299afc at800e0090 using
fresh saved S.word210. Stable initial65535 wraps0 and performs no local restore.
Children own their independent IRQ nesting; no stack-retained IRQ token or
counter invariant is enforced. No independent success/failure assignment.
Epilogue restores paired/single FPR31, GPR27..31/LR and80 decimal stack bytes.

[Manifest](../../../tools/game_stream_completion_callback_evidence.json) pins
seven spans, all262 body-word anchors and seven direct calls. Eight completion,
four match/seven result/eight readiness/five mix-gate/five stable counter vectors
checked; linked pan evidence reverified. Models are mathematical deductions,
not original execution. Generic verifier checks pins/targets, not provider/
runtime claims. Fresh ELF/listing/generator stay `/tmp`; no bodies/assets committed.

Remaining: SoundSSMain/play/cache-next/cache-ARAM callbacks and deferred DVD
manager consumer; actual pool/record/result/cursor/count/end/restart/index/flags/
busy/readiness/type/ID/volume/pan/surround/latch/counter/IRQ-state producers and
valid value/extent/alias/lifetime/IRQ/concurrency/ABI/FP domains; transitive SDK/
ARAM/AI/DSP and original error/short-read/startup/callback/IRQ/audio traces.
EVT-01/native/full-game binding remain OPEN.
