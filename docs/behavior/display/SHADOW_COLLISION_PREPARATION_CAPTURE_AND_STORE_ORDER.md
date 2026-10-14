# Shadow collision preparation: captures and store order

US G8ME01 disc0 revision0, read-only static recovery 2026-10-14.
Complete shadowCharShadowDisp_Collision8002d16c/214 and
PSMTXTrans8029e230/34. Extends
[display setup](SHADOW_DISPLAY_SETUP_AND_EMPTY_RECORD_CONTINUATIONS.md) and
[ray/filter/ground shape](../overworld/OVERWORLD_MARIO_SHADOW_REQUEST_AND_GROUND_POLYGON.md).
Ignored inspection listings: artifacts/reference/display-class-check/.
This is source behavior, not a Dolphin observation or a certified native ray.

## Entry and record admission

1. Read retained shadow-owner pointer; capture owner.word108 as primary pool.
2. Call camGetCurPtr8000f2e4 at8002d194. Return value is unused. Do not skip
   the query or recapture pool if that query suspends/mutates the owner.
3. Capture source zero float8041f9d4; initialize signed loop index0.
4. Each loop test reads the shadow-owner pointer AGAIN and reads its word104
   count; signed comparison index<count. The captured primary pool is not
   changed if the owner/pool pointer changes during a provider call.
5. Read record float10 radius and establish record.matrix pointer+2c; skip
   only radius==0 (unordered/NaN does not compare equal). Read FLAGS HALF0
   once into captured integer; skip if captured mask2 set. Read type BYTE2;
   type3 selects bypass. Otherwise test captured flags mask1: clear bypasses,
   set queries the ray. A provider wait cannot replay these gate reads.

Records advance by92 bytes; increment index and captured pointer only after
skip/miss or complete matrix/flag publication. Native bounded-pool admission
must remain explicit when a fresh source count/pointer is unavailable.

## Ray query captures and provider lifetime

At8002d1d4–22c the source constructs a stack HitQuery before calling
hitCheckVecFilter800158a4 at8002d230, with filter8002d380.
Capture source X word4, Y word8, then Z wordc before the call. Preserve exact
float bit patterns; origin.Y is a separate single-precision Y+1 operation.
Copy direction words from802bfd10 (0,-1,0) and length float150 from8041fa00.
Caller source stack query+0c is origin, +18 direction, +24 returned point,
+30 returned normal, +3c length; caller stack begins query at+8.
The ray owns query outputs and its own transitive continuation.

An unavailable ray provider is distinct from completed return0/miss. A held
call retains its captured origin/direction/length/filter and record token.
Changes to live position/type/flags during the hold do not restart admission.
On completed miss, advance immediately without writing this helper's matrix
or prepared2, even if the provider changed live flags. On hit, use returned
point/normal without normalization, quantization, winding flip or floor reuse.
The independent tagged-object filter is in the linked ray contract.

## Hit: fresh dimensions and exact matrix publication

Let Q be returned point, N returned normal, s freshly read radius10 at8002d244,
h freshly read height14 at8002d24c. Dimensions are read AFTER the ray returns.
They are not the radius that admitted the call. Source reads point.X/normal.X
before s and computes their sum before reading h. No transitive services occur
in this arithmetic/store sequence, so this suffix can execute atomically in a
portable owner once hit output is available.

Each arithmetic operation marked f32 below corresponds to an individual
fadds/fmuls/fsubs store-width result. Do not replace operations with fused
multiply-add, distribute products, or use double intermediate arithmetic.
Negation is the source fneg. Matrix offset is relative to record+2c:

| Store order/address | Matrix byte offset | Value/operation order |
| --- | --- | --- |
| 1 /8002d254 | 0c | f32(Qx+Nx) |
| 2 /8002d264 | 1c | f32(Qy+Ny) |
| 3 /8002d274 | 2c | f32(Qz+Nz) |
| 4 /8002d280 | 10 | f32(Nx*s) |
| 5 /8002d28c | 14 | f32(Ny*h) |
| 6 /8002d298 | 18 | f32(Nz*s) |
| 7 /8002d29c | 20 | source +0 |
| 8 /8002d2a8 | 24 | f32(Nz*s) |
| 9 /8002d2b8 | 28 | f32((-Ny)*s) |
| 10 /8002d2d8 | 00 | b=f32(Nz*Nz); a=f32((-Ny)*Ny); c=f32(a-b); f32(s*c) |
| 11 /8002d2ec | 04 | a=f32(Nx*Ny); f32(s*a) |
| 12 /8002d300 | 08 | a=f32(Nx*Nz); f32(s*a) |

In particular source m00 is not a freely reassociated
`-s*(Ny*Ny+Nz*Nz)` calculation, and m01/m02 multiply the two normal components
before radius. Source m10/m12/m21 multiply normal by radius directly.
Every element of the3x4 matrix is written on a hit.

## Bypass: captured translation, then fresh dimension overrides

Capture record.Y first at8002d310, X at314, compute f32(1.5+Y) at318,
then capture Z at31c. Call PSMTXTrans(record.matrix,capturedX,capturedYplus,
capturedZ)8029e230 at8002d320. The provider's destination/captured arguments
survive a hold. The source matrix initializer itself is a service-free leaf
whose float stores occur in this order:

| Order | Matrix byte offsets | Values |
| --- | --- | --- |
| 1 | 0c | captured X |
| 2 | 1c | captured Y+1.5 |
| 3 | 04,08 | paired +0,+0 |
| 4 | 20,24 | paired +0,+0 |
| 5 | 10 | +0 |
| 6 | 14 | 1 |
| 7 | 18 | +0 |
| 8 | 28 | 1 |
| 9 | 2c | captured Z |
| 10 | 00 | 1 |

After its return, freshly read radius10 at8002d324, store it to matrix28
FIRST at328, then matrix00 at32c. Freshly read height14 at330, store matrix14
at334. Provider mutations of dimensions must be visible in these overrides.
Source does not replace translation with a postcall live position.

## Commit and continuation

Both completed hit and bypass join8002d338: freshly read FLAGS HALF0,
OR2 and store low16 at8002d340. Preserve any provider/owner changes to other
flag bits. Only then increment loop index/pointer and perform fresh signed
count test. Already prepared records skip; failed rays never set prepared2.
No source texture, GPU, camera-view matrix or request-pool reset is performed
by this helper. Its completed return only closes preparation; the caller's
camera2/class5 image/projection services remain separate.

## Byte identities

ISO opened rb only; DOL header mapping and US symbols cross-checked.
| Complete body | SHA256 |
| --- | --- |
| shadowCharShadowDisp_Collision8002d16c/214 | 66a1090b8706be85e643ce99cff7a9bf75429f0d0c90c31ddc09e6088d20ef0d |
| PSMTXTrans8029e230/34 | 617da755381d59f14f4d26dc4f016ce25b3005cbafdab53e7b89556303a695f9 |

PSMTXTrans's symbol span is34 bytes (not38: the next function starts8029e264).
Float constants and pinned direction were read from their original DOL data
spans. No new runtime or hardware claim.
