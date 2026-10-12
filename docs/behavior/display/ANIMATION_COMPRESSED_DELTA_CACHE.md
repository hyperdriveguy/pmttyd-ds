# Animation cumulative delta caches and forward/reverse replay

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete animPoseMain,
local8003f0f4..80040084. Notation from
[entry/time](ANIMATION_MAIN_ENTRY_AND_TIME.md) and
[timeline sampling](ANIMATION_TIMELINE_SAMPLING.md). No execution.

## Five cached base buffers

Capture counts from loaded resource D at f0,f8,13c,140,12c before replay.
They are retained through later helper calls. The channels/requests are:

| Channel | Count | Base source pointer in D | Cache pointer in R | Byte length | Copy request |
| --- | --- | --- | --- | --- | --- |
| position triples | f0 | 154 | 48 | 12*count | memcpy_as4 at80005634 |
| auxiliary triples | f8 | 15c | 50 | 12*count | memcpy_as4 |
| byte states | 13c | 1a0 | 58 | count | memcpy at8000519c |
| node floats | 140 | 1a4 | 60 | 4*count | memcpy_as4 |
| texture records | 12c | 190 | 68 | 24*count | memcpy_as4 |

Position/node/visibility/texture names align with the existing local original
asset parser;auxiliary-vector semantic consumers remain a separate dependency.
The original code requests each copy when count!=0,including negative/raw
invalid counts;it does not validate byte lengths,allocation or distinct buffers.
Copies occur in the table order. Their full helper/alias semantics remain open.

## Cache-index dispatch

Let c=retained R+28,n=retained R+2c,a=retained R+30,old=CURRENT signed R+3c:

- old==c:skip base reset and cumulative replay. If ordered previous float R+40>a,
  write positive0 R+40;otherwise retain it until final publication.
- old==−1 OR signed old>c:copy all five bases. If CURRENT R+3c is−1 after copies,
  write0 there. Replay keys0..c inclusive,using unsigned pointer<end comparisons.
  This is a rebuild;do not subtract old deltas on backward motion/loop wrap.
- signed old<c with old!=−1:retain caches and replay old+1..c inclusive.

Rebuild iterates from K=word[T+28] to K+44*(c+1);forward iterates from
K+44*(old+1) to K+44*(c+1). All address multiplication/addition is low32-bit.
A valid nonnegative count/index domain gives those inclusive ranges;do not
extend that interpretation to pointer-wrap/malformed domains. Resetting−1 to0
before replay is a visible intermediate store,not a reason to skip key0.

## Five streams per44-byte key

For EACH replayed key,apply streams in position,auxiliary,byte-state,node,
texture order. Each stream resets its destination cursor to its current cache
base;cursor advances cumulatively within that key/stream. First byte of each
entry is an UNSIGNED relative index delta,not an absolute index.

| Channel | Key offset,count fields | T stream base | Entry size | Operation |
| --- | --- | --- | --- | --- |
| position | +4,+8 | +2c | 4 | cursor+=12*U8(entry0);XYZ fused increments from signed bytes1..3 |
| auxiliary | +c,+10 | +30 | 4 | same12-byte cursor andXYZ fused increments |
| byte state | +1c,+20 | +38 | 2 | cursor+=U8(entry0);byte +=U8(entry1),mod256 |
| node float | +24,+28 | +3c | 4 | cursor+=4*U8(entry0);single fused increment from signed byte1 |
| texture record | +14,+18 | +34 | 12 | cursor+=24*U8(entry0);byte0 +=U8(entry1),mod256;float+4 +=entryFloat4;float+8 +=entryFloat8 |

Stream start is base+entrySize*keyOffset;end=start+entrySize*keyCount.
Unsigned start<end gates iteration. Original loops unroll position/aux/texture
four entries and byte/node eight entries,with remainder loops;same entry order.
Bad counts/overflow can invalidate simple bounded-array interpretations.

Signed delta bytes use scalar `psq_l`,W1,I4 and GQR4=0x00060006 established
by Main. Replay float scale is EXACT binary32 1/16 at0x8041fbac.
For each float component use `fmadds(scale,signedByte,currentFloat)` followed by
store;do not split multiply/add. Texture float additions are separate fadds
WITHOUT the1/16 factor. Byte addition uses unsigned source bytes and truncating
stb;negative two's-complement deltas are equivalent modulo256,not booleans or
clamps. No local visibility==1 conversion is made by Main.

## Outputs and next dependencies

Caches represent fully replayed current-key state;output vectors and interpolated
node floats are separate buffers. Byte and texture streams have no fractional
interpolation in this function. Do not write fractional output back to cumulative
caches or rebuild from original bases on every advancing key without respecting
alias/intermediate store domains. Final cache index/fraction publication is
specified in [interpolation](ANIMATION_OUTPUT_INTERPOLATION.md).
`verify_animation_main.py` pins the complete local reset/forward/unrolled/remainder
graph. Loaded stream offsets/count/index authoring,buffer capacity/lifetime,
copy helpers,FPSCR and source-to-visible geometry still need closure.
