# toMovedirSimple: global camera-relative quantizer without facing override

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
toMovedirSimple0x800961f8,size0xcc. The
[slit collision helper](../partner/FIELD_PARTNER_SLIT_COLLISION.md) calls it with270,180,
then0. Input D is a degree float in f1,retained before marioGetPtr. Capture the
returned GLOBAL pointer G and read current camera-relative float C=G+19c.
No supplied partner/owner pointer is used and no P/G field is locally written.

Compute with separate single-result subtract/add:

```
X = round(round(270-D)+C)
bias = +0.5 if ordered X>=0, otherwise -0.5
J = fctiwz(fmadds(1000,X,bias))
Q = round(signed_word_to_float(J)/1000)
```

The original fmadds is single fused. Convert its integer result through the
signed-word/double bias0x802c3ec0,using the original fsubs rounding before fdivs.
Do not replace this with host round(),a double expression,two separately rounded
operations or an epsilon sign test. Unordered X uses the negative-bias route.
Exact constants reside at0x80420a60,+68,+88,+8c,+90,+94.

Repeatedly add360 with fadds while Q is ordered<0. Then repeatedly subtract360
while Q is ordered>=360. Return the resulting float. These are ordered loops,
not a total modulo function for infinite/NaN/overflow/out-of-range input domains.
There is no final requantization after wraps.

Unlike [toMovedir](../partner/FIELD_PARTNER_FALL_FACING.md),this leaf does not inspect the
global paper/facing flag0x00100000,does not read G+1ac and does not substitute
cardinal headings. Unlike [toMovedir2](../partner/FIELD_PARTNER_PLACEMENT3_MATH.md),it
accepts no offset and applies no offset-sign reversal. Substituting either
related helper is not justified by their similar camera conversion.

Slit probes capture width-derived offsets from the270 request,then make fresh
180 and0 requests. They therefore preserve three global-pointer/camera reads;
no local proof makes every captured pointer/offset identical. Exact numeric/FP/
global/source domains and original query angles remain open after the static
body/constant/bias checks pass.
