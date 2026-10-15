# Original Stock EXP queued draw

US G8ME01 disc0 revision0, _btlStockExpDisp80100258/204. Original complete-body
SHA c051f6009f34587c7a681d9631c6b91c651bf9f62c273a9666a14136f09af9b3;
read-only recovery, constants and full listings in
artifacts/stock-exp-display-evidence/. Static source facts are distinct from
native tests or original execution. Parent request [EVT-01](../../requests/EVT-01.md).

The callback ignores incoming camera/userdata. Capture GLOBAL B from8041eb90,
then signed Bf04 once. Count<=0 returns without camera, position or icon calls.
It does not reread registration's Bef8 mask2, ending, menu or fade flags.
Positive count is bounded to100 locally without storing back. Retain quotient
count/10 and remainder count%10 before any child. Emit remainder small icons
first, then quotient big icons. Count100 is ten big stars and no small stars;
it is not a numeric100 glyph or100 individual star icons.

Every icon separately reads source float X8041eb98 then Y8041eb9c. Template Z
is original-100 in both loops. With S a separate single-rounded addition and
N(a,b,c)=-singleFMA(a,b,-c), small index i uses:

```
b=S(S(308+X)-3.5)
position=(N(13,float(i),b), S(Y-220), -100)
scale=original float word3eb33333 (.35 approximate)
```

Big index j uses:

```
b=S(S(308+X)-10)
position=(S(4+N(20,float(j),b)), S(16+S(Y-220)), -100)
scale=.5
```

All calls are iconDispGx8001a918(position,flags16,ID404,scale), supplying alpha255
to the [complete positional icon wrapper](../display/ORIGINAL_POSITION_ICON_SERVICES.md).
Source fnmsubs negates the fused rounded result, including its signed zero.
Do not reassociate the big offsets or replace this with a numeric font.
Each child completes before index increment. Holds retain that icon's captured
position/scalar inputs; later icons reread X/Y and may observe child mutations.
The callback changes no Work count, global position or resource flag.

Source x$1184/y$1185 are startup-cleared SBSS floats. The native process owner
stores their eight big-endian bytes in the shared CameraManager object as a
storage policy, separately from camera records. cameraInit/room/battle/checkpoint
owners preserve those bytes. Successful staged checkpoint replacement copies the
retained bytes into its successor camera owner only after admission; failures
leave the current owner untouched. Each battle VM between such replacements maps
the SAME external RAM backing,
so original-address byte/half/word writes stay coherent across facade lifetimes.
It is not a per-battle zero reset or a callback literal0. Caller retains backing
storage until facade destruction/unregistration. External registrations reject
null/zero/wrapped/overlapping ranges, including normal RAM and semantic views;
source VM bounds behavior is not inferred from this native admission policy.
Read/write aliases and complete-range write validation use the existing facade.

A direct RA13 load/store census over all seven loaded DOL text sections found
four coordinate loads, all inside this callback, and no direct stores. This
bounded census does not exclude indirect, REL, event or DMA writers; the owner
remains mutable. No fabricated setter or writer-completeness claim is supplied.

Native bridge validates actual queued80100258/camera/userdata, then reads the
real global B/count through EventNativeRuntime memory. The callback's original
icon path uses current published camera, actual original icon metadata/texture,
flat flags16 state and retained source GX quad submission. Scene publication
and physical replay follow the existing retained-frame boundary; packet capture
alone is not proof that a held whole camera pass rendered pixels.

Host tests cover signed gate/no-position calls, positive clamp, ones-before-tens,
live coordinates, held icon without rereads, retained count/B and negative-zero
fnmsubs. [Progress](../../PROGRESS.md) records exact native validation/limits.
Original framebuffer/FPSCR parity, all indirect writers, complete battle teardown
and full prologue/story/field restoration remain separate unfinished work.
