# Flying callers' two speed writers are not interchangeable

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
unk_800cbf84(size0x38) and unk_800cbfbc(size0xe4). Flying
[behind](FIELD_PARTNER_BEHIND_FLY.md) and
[homing](FIELD_PARTNER_HOMING_FLY_ENTRY_AND_SPEED.md) request both with different
semantics. Names remain navigational annotations;fields and operations below
are binary deductions. R denotes original single-result rounding;F single fused
multiply-add. The verifier pins both complete bodies and exact constants.

## Immediate writer0x800cbf84

Inputs P and float A. Capture owner M=P+160,load M+228,and compute
T=R(A*M+228). Choose factor0.75 if P+0 mask0x80 set,else1.0. Multiply again
with a single-result operation,store P+108,then LOAD that field again and store
it into P+104. No signed+28/motion gate or acceleration smoothing applies.
Even A==0 performs the owner load and multiplications;it is not an unconditional
literal-zero assignment in all exceptional numeric domains.

## Smoothed writer0x800cbfbc

If input A compares equal to0 using fcmpu,store positive0 to P+108 and RETURN.
It does not reset P+104 on this route. Unordered A does not take equality.
Otherwise store A to P+108 FIRST. If signed halfword P+28 is nonzero,return;
if unsigned motion P+36 is2,3 or6,return. Target108 has already changed on
these gated returns while current speed104 is retained.

For admitted calls,capture M=P+160 and current speed S=P+104. If M+0
mask0x8000 is set,target T is M+180 directly. Otherwise compute
T=R(R(P+108*M+228)*factor),factor0.75 if P+0 mask0x80 set,else1.0.
For ordered T>0:

```
U = R(R(0.5+T)-S)
N = F(0x3d4ccccd,U,S)
if ordered N>=T: N=T
P+104=N
```

Otherwise,including unordered T:

```
U = R(-1-S)
N = F(0x3dcccccd,U,S)
if ordered N<=0: N=positive0
P+104=N
```

The coefficients are binary32 approximately0.05 and0.1. Preserve fused
operation order and ordered CR/equality clamps;do not substitute double
arithmetic,epsilon clamps or an elapsed-render-time factor. Negative target is
not clamped before selecting its branch. These requests match the corresponding
speed arithmetic in the ordinary callback but have different input/early writes.

Homing may request the smoothed writer several times in ONE invocation while
consuming history;each admitted request updates current speed again. Behind's
completion path instead requests the immediate writer with0,after motion reset.
Collapsing calls into one final target assignment changes the local behavior.
FP exception/denormal/source/alias/cadence domains and original measured speeds
remain open;green host harness tests do not establish original numeric parity.
