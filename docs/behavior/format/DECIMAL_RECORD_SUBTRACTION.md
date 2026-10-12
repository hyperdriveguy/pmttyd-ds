# Numeric decimal-record subtraction, correction and normalization

US G8ME01 revision0,recovered2026-10-12. Evidence B: complete __minus_dec at
0x80266ddc,size0x4fc. See [neighbor/distance requests](DECIMAL_NEIGHBOR_REFINEMENT_AND_TIES.md),
[record equality](DECIMAL_RECORD_EQUALITY_AND_REFINEMENT_ENTRY.md) and
[internal numeric records](FORMAT_BINARY_DECIMAL_ENTRY.md). Static original
instructions only;no exact-arithmetic/execution equivalence claim. This helper
has no calls or floating arithmetic.

## Interface, copy order and zero-right shortcut (B)

r3=output O,r4=left A,r5=right B. Copy all42 bytes from A to O using interleaved
word loads/stores and final halfword. There is no count-limited copy,buffer check,
memmove call or overlap policy. O=A supports the literal self-copy before later
mutation. O=B overwrites the right operand before its first-digit test;partial
overlap follows the actual interleaved access order,not a protected snapshot.
No semantic scalar return is established;result is supplied by writes to O.

Then read B.first (byte+5). If numeric0,return the copied left record immediately,
without alignment,subtraction,correction or normalization. This is a first-byte
zero convention,not a proof that all right digits are zero. Input signs at+0 are
not compared or subtracted;O keeps the copied left sign. Exponents are signed16
at+2,counts unsigned bytes+4,digits numeric bytes starting+5. There is no digit
validation,negative-result detection,operand swap or local errno write.

## Alignment and output padding (B)

Let a=O.count,b=B.count,delta=O.exponent-B.exponent as a signed32 difference.
Compute M=max(a,b)+delta,cap only values above36 to36. No lower clamp appears.
While O.count<M,append numeric0 and increment its count byte. Existing count
above36 is not reduced:the cap applies to the alignment target,not validation
of the incoming record or guaranteed output capacity.

Let D=O+5,R=B+5,q=min(M,b+delta). Set output cursor p=D+q,right cursor
r=R+(q-delta),and save t=r as the first potentially omitted right byte.
These are word pointer calculations. Ordinary canonical left>=right magnitudes
have nonnegative delta;the code itself does not enforce that exponent/domain
condition. Invalid negative targets can point before digit storage. Count and
capacity constraints must come from caller evidence,not this local cap.

## Reverse subtraction and borrow (B)

While unsigned p>D and r>R,decrement both pointers by one and compare unsigned
byte O[p] with B[r]. If O[p]<B[r],search backward from p-1 until a nonzero output
byte is found. That search has **no digit-start or allocation bound**. From the
found byte to p,repeat:decrement current byte,add10 to the next byte,advance to
that next byte. Unrolled groups of8 implement the same sequential chain.
All stores retain low8 bits. Then store low8(O[p]-B[r]). Continue leftward.

No borrow is carried in an external boolean across digits;the output bytes are
modified in place before subtracting the current right byte. In valid numeric
borrow chains,intermediate zero digits become9 and the target gets10 added.
If no earlier nonzero digit exists,the backward search can leave the digit
array or fail to terminate;there is no saturation or zero/negative-result fallback.

The saved t does not move during reverse subtraction. On a complete retained
right prefix,p normally reaches D+delta. This remaining pointer matters in the
omitted-tail correction;it is not universally the least-significant output digit.

## Omitted right-tail decision and pointer distinction (B)

Compute k=t-R. If signed k>=B.count,skip correction. Otherwise read omitted
numeric byte B[t],initialize decrement flag0 and inspect it:

| First omitted byte | Decision |
| --- | --- |
| <5 | Set decrement flag1;leave p as produced by reverse subtraction |
| >5 | Keep flag0;skip correction |
| ==5 | Inspect all later right bytes;any nonzero skips correction. If all zero,reset p=D+(k+delta-1) and set decrement flag1 only if O[p] is odd. |

The exact-five branch alone repositions p to the final retained aligned digit.
The below-five branch uses the subtraction cursor,ordinarily D+delta. It does
not reset to D+q-1. This is a material local operation distinction;do not replace
it with a generic result-rounding routine merely because the flags resemble a
tie/parity test. A first omitted zero also takes the below-five route without a
check that the entire omitted tail is zero.

If decrement flag0,continue to normalization. If1 and O[p]>=1,decrement that
byte. If O[p]==0,search backward from p-1 for a nonzero byte,again without a
lower bound;propagate the same decrement/add10 chain to p,then decrement O[p].
No exponent update,count change,carry to a separately initialized leading digit
or error return occurs during this correction. The meaningful arithmetic
interpretation requires actual canonical operand/precision/caller evidence.

## Leading normalization and trailing trim (B)

Starting at D,scan forward until a nonzero byte is found,with **no count/end
check in that search**. If the first nonzero is at D,skip leading shifting.
Otherwise let shift=pointer-D and s=low8(shift). Set O.exponent to
low16(oldExponent-s). Compute end=D+O.count,and,if the found pointer is below
end,copy bytes from it to D forward through end. Then set O.count to
low8(oldCount-s). Vacated/inactive digit storage is not cleared.

If all active digits are zero,the forward search still reads inactive bytes and
can go outside the42-byte record. It may find a nonzero byte there or fail to
terminate. The helper does not locally construct canonical zero before this
scan;equal operands cannot be assumed safe just because their mathematical
difference is zero. Original runtime/caller coverage remains unexecuted.

Finally set end=D+O.count and scan backward over numeric0,up to the active count.
Set count=low8((last-tested-pointer-D)+1). In the ordinary nonzero case this
removes trailing zeros while preserving leading-digit exponent. No exponent
increase accompanies trailing trim because this record's exponent is leading-
digit order. The bounded trailing loop can leave count1 after reaching D;that
does not protect the earlier unbounded leading search.

## Caller composition and evidence

The converter requests X-A,B-X,X-D and A-X for neighboring candidate distances.
Those argument directions were recovered in [refinement](DECIMAL_NEIGHBOR_REFINEMENT_AND_TIES.md).
This helper now establishes their local copy/alignment/borrow/correction flow,
but does not by itself prove exact distances or globally correct tie rounding:
36-digit alignment,omitted-tail pointer behavior,generator precision,zero
normalization and pow/FPSCR still need composed execution comparisons.

A new whole-function fingerprint and24 independent anchors pin copy/zero gate,
alignment/padding,reverse cursors,borrow,tail/parity/pointer correction,leading
shift and trailing trim. No original routine is executed,linked or distributed;
ELF/disassembly/reports remain ignored. No asset conversion,message rewrite or
licensing change occurs.

Validate output records and source access order for zero-right,nonoverlapping/
in-place outputs,exponent gaps,multiple borrows,alignment truncation,below/exact/
above-five tails,zero results and inactive storage. Establish actual caller
operand/count invariants before claiming all edge cases occur in gameplay.
Next recover pow dependencies and execute complete scan/conversion boundary
comparisons. Continue drawing/string/resource/cadence and uncovered story/event/
field/battle/audio/save/content before complete-game requirements. Full goal
remains incomplete;no Switch numeric behavior is inferred.
