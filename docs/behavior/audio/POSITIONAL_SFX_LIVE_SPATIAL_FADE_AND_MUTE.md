# Positional SFX live spatial controls, fade and mute order

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete scoped psndSFXMain regions
800d9f50..800da010 and800da398..800da9d0 exclusive. Together with
[completion/loop/tail](POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md)
and [startup](POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md), these close the
local whole-body request graph. Child services/runtime remain separate.
Read [calc3D](GAME_CALC3D_SPATIAL_RESULT_AND_ANGLE_TABLE.md).
Offsets/addresses hexadecimal; explicit decimal arithmetic literals labeled.

## Captured context and backend selection

G=psnd803df6e8. Slots P have stride28hex,40 decimal slots at803df988;
row R=80316b28+14hex*(P.key&1fff). Occupied second-pass entries retain that row
pointer from BEFORE pending startup and enter live region only if startup was
already complete or just succeeded. Delay/start failure bypasses live processing.
Slot key/engine ID and other fields are freshly read at the indicated sites;
row pointer is not recalculated after each request.

Backend is selected from R.word4: bit80000000→SS, otherwise40000000→song,
otherwise effect. Both bits set choose SS. Linked controls below are requests,
not proof that a backend accepts a volume/pan, remains live or emits audio.

## Global fade factor and percentage producer before the slot pass

Initialize retained fade factor F=0 at entry. After completion sweep, if G.byte24
is2, read signed half28, decrement/store low16. If signed new half<=0, store
state byte24=0. EVEN AFTER that state clear, reload signed half28 and half26;
F=divw(wrap32(remaining*127),denominator). No remaining clamp, denominator check,
saturation or rollback. Typical remaining1→0 yields F0; remaining0→-1 preserves
negative quotient in this invocation. Later invocations outside state2 start F0.
Signed half wrap and divide-by-zero domain must not be repaired as source behavior.

Set percentage target T=100 decimal; G.half56 bit20 set AND bit40 clear makes
T120. Request seqGetSeq8002e210. If return3, skip percentage movement. Otherwise
read global pointer(r13-6f50).word1c bit1; if clear also skip. On admission,
read G.byte59 signed; if below T increment/store low8. RELOAD byte59 signed;
if now above T decrement/store low8. No integer range clamp/normalization;
invalid signed initial bytes can wrap on increment. The second compare is fresh,
not an else branch. This global percentage moves before all slot updates.
Sequence/global-bit sources and physical update cadence remain open.

## Ordinary volume arithmetic and spatial controls

Let U=P.byte8, A=P.byte9, H=P.unsignedHalf10. The emitted signed multiply/shift
sequence computes floor(U*A/127) over all byte inputs. Ordinary volume
V=clamp(floor(U*A/127)-H,0,127); clamp occurs before low-byte argument publication.
No floating ratio or rounding-to-nearest is introduced.

If fresh P.flagsHalf6 bit2 SET, compute V, then request
calc3D(P.key,&P.position1c,U8(V),P.unsignedHalf12), saving returned word to stack.
Read R.word4 AFTER calc3D and select backend. Packed result bytes0/1/2 are
spatial volume/pan/surround. Requests, in exact order:

| Backend | Spatial requests |
| --- | --- |
| SS | SetPanCh(P.engine18,byte1); SetVolCh(P.engine18,scaled byte0); SetSrndPanCh(P.engine18,byte2) |
| Song | SetVolCh(P.engine18,scaled byte0) ONLY |
| Effect | SetPan(P.engine18,byte1); SetVolume(P.engine18,scaled byte0); SetSrndPan(P.engine18,byte2) |

For each scaled volume, read G.byte59 SIGNED and stack byte0, multiply, divide
by100 with truncation toward zero, then clamp signed word0..127. SS/effect read
this percentage AFTER the pan request; song reads it directly. Every backend
request reloads P.engine18. Pan/surround come from the retained stack result,
not fresh shared PAN memory. Song does not request either spatial pan control.

If bit2 CLEAR: read R.word4 to select backend. SS/effect first request stored
P.byte0a pan, then reread ordinary-volume inputs and request V. Song requests
V only. No G.byte59 percentage scaling or surround request in this path.
No local stale-volume cache or unchanged-value suppression: controls repeat
on each admitted slot invocation.

## First mute gate after ordinary/spatial updates

Fresh P.flagsHalf6 &0a30 NONZERO skips this gate. Otherwise read G.half56;
bit80 SET issues volume0 to the selected backend using fresh R.word4 and P.engine18.
No flag store/stop/key clear or return: proceed to fade regardless. This is a
later volume overwrite; preceding pan/surround requests still occurred.
The mask is literal exemption policy; character/story meanings require writers.

## Fade volume and zero-result stop/clear

At800da6a4, fresh P.flagsHalf6 bit40 admits fade; otherwise skip to final gate.
Read R.word4 to select backend. Request GetVolCh/GetVolume with fresh P.engine18.
Use returned LOW BYTE B, multiply by retained raw F with word wrap and apply the
emitted signed division127 sequence. Retain un-narrowed quotient Q; request
backend SetVol with U8(Q). NO clamp0..127 here. Test Q itself for exact0 AFTER
setter returns; a quotient whose low8 is0 but Q!=0 does not admit stop.
Outside global fade state2, retained F remains0, so a bit40 slot still requests
zero and follows this zero-result path. No assumption that flag implies state2.

On Q0, retained loop ordinal i==-1 skips removal; normal second-pass ordinals
are0..39. Otherwise select slot X=803df988+28hex*U8(i), read fresh X.key.
Key-1 skips. Fresh X.flags bit1 set clears X.key then X.engine18 to-1 without
backend stop. Otherwise read X.engine18, derive descriptor row from FRESH X.key
low13. If engine!=-1, select fresh descriptor.word4 and request SSStopCh/
SongStopCh/EfxStop(engine). Then store X.key=-1, X.engine18=-1 in order.
This reread path is repeated for all three fade backend branches; it does not
stop via the earlier retained R or captured input handle. Stop return ignored.
Other fields/generation/preferences/position remain untouched.

## Final mute gate and loop continuation

At800da978 read G.half56 bit100; if set, read fresh P.flagsHalf6 bit400.
Both set requests volume0 using fresh retained-row word4 selection and fresh
P.engine18. No current-key or engine-1 guard: this may run AFTER fade cleared
the slot. No return/stop/flag mutation; continue ordinal increment and next slot.
The final occupied-count/preference stores follow all slots as already documented.
No reordering mute/fade/stop to eliminate apparently redundant backend calls.

## Verification and whole-local closure boundary

[Manifest](../../../tools/game_sfx_live_updates_evidence.json) pins two newly
complete regions,15 linked child identities and whole psndSFXMain fingerprint:
18 spans/446 anchors/35 resolved direct calls. All65536 unsigned-byte product
cases verify division127; all65536 unsigned×signed-byte cases verify division100.
Six fade decrement/wrap vectors are derived only. Fresh full ELF/listing stays
`/tmp/inbox-sfxmain.elf`; no original function executes/body/asset exports.

Earlier complete startup and sweep/loop/tail contracts plus this delivery now
cover the local full psndSFXMain body. Remaining: SS/song/effect child semantics,
G/slot/descriptor source writers and flag/fade/denominator/alias/concurrency
invariants, live stop/audio/lifetime observations. Whole-local graph is answered;
full backend/native/source/runtime equivalence and EVT-01 remain OPEN.
