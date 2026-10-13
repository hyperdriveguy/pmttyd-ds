# MusyX stream zero-position wrapped refill

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete streamHandle REGION
8027aaec..8027acb0, end exclusive,113 instructions. Addresses/offsets/masks
hexadecimal; arithmetic constants decimal. Static source deductions, not original
refill/audio execution. Read [position/forward refill](MUSYX_STREAM_POSITION_AND_FORWARD_REFILL.md)
and [flush bridge](MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md).

## Entry and producer request

The preceding position region enters here only when captured cursor C exceeds
normalized position P, unsigned. This region tests P: nonzero goes to remaining
wrapped-refill8027acb0. Zero processes captured format0/1;2..255 skip to common
footer8027af10. Because equality already skipped earlier, this zero route has
captured C>0. Format1 includes hardware positions0..13 normalized to0.
No new hardware-position query or format recapture here.

For format0 read logical length word18=L, source word14, callback word10 and
user word50, then invoke at8027ab34:

`callback(source+wrap32(2*C),wrap32(L-C),0,0,user)`.

Format1 similarly captures L/source/callback/user and computes O=B(C), with
B(x)=8*floor(unsigned32(x)/14), retaining O across child. Invoke at8027ac04:

`callback(source+O,wrap32(L-C),0,0,user)`.

The source additions wrap32. C/format were captured before these reads; logical
length is captured now. No callback-null/signature/source/extent/count/encoding/
IRQ/lifetime check or returned-count cap. R0 goes straight to footer without
reading state. Nonzero return R freshly reads state+c; only exactly2 admits
cursor/flush work. Other state values skip local publication; earlier callback
mutations are not rolled back.

## Fresh modulo and flag bypass

After admitted callback read current cursor word20=F then logical length word18=N.
Compute A=wrap32(F+R); unsigned division/multiply/subtract forms new cursor
K=A%N when divisor valid. N0 is unguarded; invalid-division domains remain open.
No reuse of pre-callback C/L for this modulo. Read flags8; mask00020000 set
bypasses upload but still publishes the retained new cursor.

## Four exact flush branches

Flag clear selects the following offset/size; source14 and selector byte0e are
freshly read for each request. Arithmetic wraps32.

| Format/new cursor | Offset | Size |
| --- | --- | --- |
| 0,K0 | 2*F | Fresh byte length word1c minus offset |
| 0,K nonzero | 2*F | 2*(K-F) |
| 1,K0 | Retained O=B(C) | Fresh byte length word1c minus O |
| 1,K nonzero | Retained O=B(C) | B(wrap32(K+13))-O |

Each branch requests hwFlushStream(source,offset,size,selector,0,0), at8027ab94,
8027abb8,8027ac64 or8027aca4 respectively. The two format0 zero-cursor branches
in this and the earlier forward region are different: THIS uses byte length1c;
forward uses2*(logical length18-F). Their equality requires a source invariant
not enforced here. Likewise format1 retains pre-callback O even if callback
changed cursor/length. K+13 wraps before its block mapping.

No size clamp, negative-wrap rejection, alignment check, implicit second upload,
callback/completion wait or upload-status test. After request, or immediately on
flag bypass, store retained K at cursor20; then branch common footer8027af10.
No state recheck after upload, fresh modulo or new-cursor suppression. A child's
cursor write can be overwritten by this final publication. No state/format reset.

## Evidence and remaining work

[Manifest](../../../tools/musyx_stream_zero_position_refill_evidence.json) pins
three spans (region/whole handler/flush child),113 whole-region instruction anchors,
four direct calls/two recorded indirect callback sites. Six dispatch/five callback/
nine refill vectors checked, including different logical/byte lengths, callback
cursor mutation, retained block offset, flag bypass and invalid divisor unresolved.
Block formula follows the previously checked /14 mapping; this delivery does not
claim measured encoding/callback behavior. Generic verifier checks hashes/anchors/
direct targets, not callback ABI or semantic deductions. Fresh full handler ELF/
listing/generator remain `/tmp`; no bodies/binary assets committed.

Remaining: nonzero-position wrapped-refill/footer/ABI, callback/position/format/
cursor/logical-versus-byte-length/flags/source/selector/voice/count providers and
valid divisor/value/extent/alignment/encoding/alias/lifetime/IRQ/concurrency
domains, transitive ARAM/DSP consumers and original refill/audio/native binding.
EVT-01 remains OPEN.
