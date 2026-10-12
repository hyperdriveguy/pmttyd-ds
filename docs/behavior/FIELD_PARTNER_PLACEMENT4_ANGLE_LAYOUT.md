# Placement4 angle tables,cursors and adjacent stack words

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
[placement4](FIELD_PARTNER_PLACEMENT4.md). Initializer0x800c5a3c..0x800c5d34
(end exclusive) copies59 original words from0x802cc430..0x802cc51c into five
arrays. Original symbol labels span multiple arrays;their declared types are
not evidence of these stack boundaries. The verifier reconstructs every word's
source address through lwz,stw and register-copy instructions.

| Selected array | Frame start | Source start | Copied signed words |
| --- | --- | --- | --- |
| Normal,no captured buddy | +f4 | 802cc430 | 180,150,-150,120,-120,90,-90,60,-60,30,-30 |
| Normal,captured buddy,current flag8 clear | +c8 | 802cc45c | 150,-150,120,-120,90,-90,60,-60,30,-30,180 |
| Normal,captured buddy,current flag8 set | +9c | 802cc488 | -150,150,-120,120,-90,90,-60,60,-30,30,-180 |
| Extended,other map | +68 | 802cc4b4 | 180,180,150,-150,120,-120,90,-90,60,-60,30,-30,0 |
| Extended,usu_00 | +34 | 802cc4e8 | 45,60,15,75,0,90,-15,105,-30,120,-45,135,-60 |

## Live selection and conversion

For index0,use current P+54 directly,without looking up an offset or requesting
toMovedir2. For later indices,reread P+8 mask0x00200000:

- If set,compare current GP+12c map string with original `usu_00` at0x802cc5b8
  on EACH attempt. Select extended usu_00 on equality,otherwise extended other.
  Base argument is current P+54.
- If clear and captured buddy Q is null,select normal no-buddy.
- If clear and Q nonnull,reread P+0 mask8;select the respective normal buddy
  flag-clear or flag-set array. All normal bases are the CAPTURED opposite
  heading revise360(R(180+entry P+54)).

The word is converted as a signed integer through the original double bias
at0x802cc5b0,then passed as f2 to
[toMovedir2](FIELD_PARTNER_PLACEMENT3_MATH.md),with selected base in f1.
That helper consults global Mario/camera state and can reverse the offset;
it is not simple base+offset. Loop limit was captured earlier and is not
recomputed if the table-selection flag changes. Do not discard the captured-
buddy/current-flag-clear branch without proving its source domain.

## The extra read is part of the compiled behavior

Every rejected attempt increments ALL five table pointers,including index0.
Thus offset element0 is never used for an angle request. With unchanged normal
selector,the12 attempts are direct facing,index1..10 copied offsets,and index11
one word beyond that array. Unchanged extended selector makes14 attempts:
direct facing,index1..12 copied offsets,and index13 one adjacent word.

| Selected table | Final offset source under unchanged loop selector |
| --- | --- |
| Normal,no buddy | Frame+120: query header;not initialized by angle copy |
| Normal,buddy/flag clear | Frame+f4: first normal no-buddy word,180 |
| Normal,buddy/flag set | Frame+c8: first normal buddy/flag-clear word,150 |
| Extended,other map | Frame+9c: first normal buddy/flag-set word,-150 |
| Extended,usu_00 | Frame+68: first extended other-map word,180 |

Frame+120 is also the base of the reused horizontal/downward ray query.
The placement4 local body writes query origin,direction and length,but does
not locally initialize that header word. Its value after earlier backend calls,
or inherited stack contents if not written there,is a dependency. No constant
final angle can be asserted for normal no-buddy. The verifier checks that this
slot is absent from the initializer's writes;it does not certify all backend
paths or the original stack's state. If selector flags change,reads can cross
other boundaries according to the captured index and live table selection.

A reimplementation must either preserve these adjacent/query-word semantics
or prove the exceptional reads cannot affect any shipped reachable state.
Clamping,index wrapping,inserting an extra zero angle,or shifting every cursor
only after offset-bearing attempts changes the recovered local behavior.
Original usage traces and full query-header/backend/source domains remain open.
