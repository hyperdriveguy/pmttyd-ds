# Partner direction conversion used by stationary floor footprints

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
partyToMovedir0x800962c4,size0x138. [Floor query](FIELD_PARTNER_FLOOR_QUERY.md)
calls it only when P+104 compares equal to0,passing P+10c in f1. Other callers
and heading/state producers remain separate. This helper returns a float and
does not directly write P or owner state.

Capture M=P+160 at entry without a null guard. If P+0&0x1000 is CLEAR,
preserve the incoming heading f1 as base H;do not read P+10c for classification.
If that mask is SET,read P+10c instead and choose H in this order:

* Exact equality with280 or460 → H=0.
* Otherwise exact equality with260 or440 → H=180.
* Otherwise repeatedly add360 while ordered value<0,then repeatedly subtract360
  while ordered value>=360. From that wrapped value,set H=0 if ordered>=180;
  otherwise H=180. The special equalities are tested BEFORE wrapping.

Then compute A=R(R(270-H)+M+19c). The owner camera-heading offset is read
after classification. This is not the global Mario getter or the floor slope
record. Quantize A to thousandths using original fused1000*A plus ordered-
A>=0?+.5:-.5,fctiwz,signed-word conversion/division by1000. Finally repeatedly
add360 for ordered result<0 and subtract360 for ordered result>=360. Return
the remaining float. No additional wrapping or integer-degree rounding occurs.

The helper's original loops must not be presented as a total mathematical
modulo function for all IEEE values. Very large finite values can fail to change
under a single-result ±360;infinities may not terminate;NaN takes unordered
branches and then depends on target fctiwz behavior. Intended domains and CPU
state must establish valid inputs. The floor direct-heading branch bypasses
this helper's classification,owner offset,quantization and wrapping entirely.

Constants are exact original binary32 values;T conversion bias double is at
0x802c3ec0. tools/verify_field_partner_ground.py pins this complete local body,
exact cases,loop predicates,owner read,quantizer and actual floor call. Finite
deduced cases should distinguish P+10c=280 vs wrapped equivalent -80,the
incoming f1 when the mask is clear,and owner-offset application before the
final wrap. No original trajectory or bit-exact host math equivalence is claimed.
