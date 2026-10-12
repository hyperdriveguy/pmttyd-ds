# Local facing conversion used by fall recovery

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
toMovedir0x800963fc,size0x168,and revise3600x80096564,size0x34. Actual
[fall recovery](FIELD_PARTNER_FALL_AIR.md) calls both after its below-world
exceptional gate. These do not write partner state;the caller stores the result
to P+54. Full producers and CPU/numeric domains remain open.

## toMovedir: quantized global camera-relative base

Input direction D arrives in f1. Capture it,then obtain global Mario G. If
G+0&0x00100000 is CLEAR,preserve D. Otherwise read G+1ac and choose replacement
D:exact280/460→0;exact260/440→180;otherwise repeatedly wrap that field into
[0,360),then ordered wrapped>=180→0,else180. The exact cases precede wrapping.
This is the base-conversion portion of
[toMovedir2](FIELD_PARTNER_PLACEMENT3_MATH.md),with no offset argument or
post-base signed-offset addition.

Read C=G+19c. Compute B=R(R(270-D)+C),then quantize to thousandths with
ordered B>=0?+.5:-.5,fused single1000*B+bias,fctiwz,signed-word conversion
through double bias0x802c3ec0 and single divide by1000. Repeatedly add360
while ordered result<0,subtract360 while ordered>=360,then return that float.
There is one global-pointer request;the caller's captured P+160 owner does
not replace it or supply C. No coordinate/history/status query occurs here.

## revise360 and the actual composition

revise360 receives f1 and uses original float0/360 only. Repeatedly add360
for ordered value<0,then repeatedly subtract360 for ordered value>=360.
Return the result with no thousandth quantization,pointer lookup or state store.
For ordinary finite values this wraps into[0,360);infinite/NaN/too-large values
are not sanitized. Single-result loop progress/int-conversion/FPSCR domains
must remain explicit rather than substituting a total mathematical modulo.

Fall first loads its CAPTURED owner M+1b0,requests toMovedir,adds original180
with a single-result add,requests revise360 and stores P+54. Its root-directed
angle P+100 is computed separately before these requests. Thus facing+54 and
travel direction+100 need not be the same value or share one pointer/camera
read. Placement4 afterwards may change root XYZ independently.

tools/verify_field_partner_fall.py pins both complete leaves,global-facing/
camera tests,quantizer/wrap predicates,constant spans and actual fall composition.
Finite deduced cases should distinguish exact flagged values from wrapped
equivalents,input D preservation with flag clear,global C vs owner C,base
quantization before+180 and revise360's wrap without re-quantization. These
are static fixtures,not original facing traces or bit-identical host math proof.
