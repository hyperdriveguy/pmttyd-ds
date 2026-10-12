# Animation timeline sampling and unguarded predecessor domains

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete animPoseMain
body;this file covers8003ef7c..8003f0f4. Symbols and loaded record/resource
notation come from [entry/time](ANIMATION_MAIN_ENTRY_AND_TIME.md). No execution.

## Two independent scans

After selecting Z and storing R+24,scan the continuous key list K=word[T+28],
count Nk=signed word[T+8],stride44. Start i0;if Nk>0,for each entry break ONLY
if ordered float[key+0]>CURRENT R+24;otherwise increment i and pointer until
count exhausted. Equality admits a key;unordered comparisons also fail to
break. No binary-search/sortedness/finite-time validation is local.

Publish R+28=i−1,then reread and publish R+2c=(current R+28)+1.
Separately scan discrete state list V=word[T+40],count Nv=signed word[T+20],
stride8,using the same ordered timestamp>selected-time break. Publish R+34=j−1,
then reread and publish R+38=(current R+34)+1. Counts<=0 skip their loops but
still produce predecessor−1 and successor0. A time before the first key does
the same. The subsequent code does not sanitize these predecessor indices.

If captured loop selector C==0,clamp EACH successor separately:if signed
R+2c>=Nk,replace R+2c with CURRENT R+28;similarly R+38>=Nv replaces it with
CURRENT R+34. This is not a predecessor clamp or a lower-bound guard. C!=0
skips these clamps at this point.

## Discrete publication and continuous fraction

Always load word at V+8*current R+34+4 into R+80. R+38 does not supply an
interpolated discrete state. No check for−1 precedes this read. It can access
before V in unsupported/malformed time/count domains.

Set cur=current R+28;next=current R+2c;kc=float[K+44*cur]. Compute
numerator N=round32(current R+24−kc). Interval denominator:

- C!=0 and signed next>=Nk:write next0 to R+2c and use
  round32(float[CURRENT word[T+24]+8]−kc). The next pointer wraps to first key,
  while denominator uses the authored END time,not key0−kc.
- C!=0 and next<Nk:round32(float[K+44*next]−kc).
- C==0:round32(float[K+44*next]−kc),including next==cur after clamp.

If denominator compares equal0 via `fcmpu`,store positive0 R+30. Otherwise
store single N/denominator R+30. There is no epsilon,clamp to0..1 or shortest
interval substitution. NaN denominator takes division. Reload cur,next and
fraction into retained registers for
[cache replay](ANIMATION_COMPRESSED_DELTA_CACHE.md) and
[output interpolation](ANIMATION_OUTPUT_INTERPOLATION.md).

## Concrete original asset example

Read-only original a/a_mario is215124 bytes,SHA256
61b585d6b2f1a36551d3b779551a12122f228bf1354ebc5bb44651c58f118293.
Its59-animation name table selects M_Z_1 at index0,serialized T offset0xa5fc.
Control triple is C0,S0,E0. Its single44-byte continuous key has time0 and
channel counts(position23,auxiliary-vector0,texture0,byte-state0,node2).
The zero-span branch therefore selects time0 independent of incoming elapsed
frame;in this stable valid one-key domain cur0,next0,denominator0,fraction0.
This establishes an original-data example for the literal zero-span and replay
paths. It does not prove a particular original scene/state selected that name
or replace live loaded-resource/pointer validation. Native idle bake provenance
is recorded in ASSETS;native output is not an original runtime oracle.

`verify_animation_main.py` independently rereads/hash-checks the ISO resource,
its name/index/offset/counts/control/key metadata. It does not call the original
Main,decode its loaded addresses or claim full animated geometry parity.
Review additional source-derived domains:equal-time keys all admitted;empty
counts/pre-first time retain predecessor−1;nonloop end makes next==cur;loop end
wraps next0 but retains authored end interval;duplicate-time denominator0.
Full resource/source authoring invariants and original time traces remain open.
