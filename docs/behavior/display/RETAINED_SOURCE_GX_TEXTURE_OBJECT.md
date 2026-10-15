# Retained original GX texture-object construction

The new `source_gx_texture_object` controller implements the complete US revision-0
`GXInitTexObj` (`802b72b0/24c`) and `GXInitTexObjLOD` (`802b7544/164`) CPU-side
bodies. It writes actual supplied guest memory; it neither imports a native
texture nor submits a draw. [Font resource setup](NATIVE_SOURCE_FONT_RESOURCE_OWNERSHIP.md)
remains the original caller. Original game/SDK rights remain with their owners;
no extracted source asset, ISO, texture or compiled pack is added here.

## Actual source dependencies

`tools/verify_source_gx_texture_object.py` opens the supplied ISO read-only and
pins both whole bodies, the original linked memset wrapper, initialized r2/r13,
and the complete format-jump/min-filter/scalar spans. Source r2 is `80427440`,
r13 is `8041cf20`. The routines read initialized original data at `80429440..67`,
`803c4e84..bf` and `8041e558 + minFilter`. These are live reads through actual
providers, including a full unsigned minFilter index and ordinary wrapped guest
address arithmetic. No runtime index clamp or hardcoded substitute table exists.
An unsupported original indirect branch target holds at that captured target.

Init's sole linked child is actual `memset800050b4(object,0,32)`. `clear` must
own that complete invocation and real memory effects before returning an engaged
result; zero is an ordinary completed result. Existing `eventFillMemory` can
provide the original fill when its entire destination/store domain is already
admitted. A provider that can wait within its stores must retain its own child;
it cannot restart committed fill stores. Neither this controller nor its
constructor synthesizes an initialized object or BSS history.

## Retention and alias ordering

The controller copies source argument bits, services and a strong lifetime at
entry. It retains a separate boundary for every supplied source memory load,
store and `fctiwz`. Missing data or children hold at the exact instruction/child;
substituted argument bits or services are rejected without advancing. A failed
current predicate latches stale, and reentry/reset during invocation is rejected.
Completed calls consume their cursor. The caller owns source scheduling and must
serialize invocations that share an object; this is not a process-wide lock.

Source bit insertion uses PPC rotation/masks. Width/height truncate to low16,
format stores its full word while the dispatcher uses low4, image insertion
preserves the original address rotation, tile counts wrap/mask to15 bits, and
mipmap/edge tests use low8. Every source read-modify-store reload is retained.
Init reads word4 before the mip conversion; LOD reads word0 before bias
conversion. In contrast, LOD writes minLOD into word4 before maxLOD conversion,
and reloads that word only after conversion succeeds. A foreign mutation across
a held conversion therefore has the original distinct effect in each case.

The actual numeric provider owns `fctiwz`, including its supported exceptional
operand domain. Binary32 products and the original unsigned-bias conversion
retain source rounding points. Source comparison branches preserve unordered
NaNs through to the genuine conversion provider. No invented NaN result,
integer saturation, new texture-size clamp or FPSCR behavior is supplied. Host,
ARM syntax and sanitizer checks do not establish Gekko FPSCR/hardware parity.

## Binding and genuine readiness

Use `resumeSourceGxTextureObject(function,input,services,cursor)`. Init integers
are object/image/width/height/format/wrapS/wrapT/mipmap. LOD integers are
object/minFilter/magFilter/biasClamp/edgeLOD/anisotropy; its three raw binary32
words are minLOD/maxLOD/bias. The typed source Setup requests already have exactly
these operand sequences. Keep stable per-child input/services/cursors through a
hold, and expose completion to Setup only after this controller returns true.

`word`/`byte` must claim the real shared process font memory first, including
unavailable owned bytes, then actual initialized source storage. `store` receives
actual size1/2/4 source stores, including the halfword at object+28. Held stores
must retain an identical invocation without repeating committed effects. Memory
mapping, CPU alignment/access validity, allocation, ownership and serialization
are supplied by actual owners, not fabricated tokens.

The original US GLY1 gives 512x256 I4/clamp/no-mip. Whole Init plus linear/linear,
zero min/max/bias LOD produces word0=`0x190`, word4=`0`, word8=`0x3fdff`,
word12=`(actualImage>>5)&1fffff`, word20=`0`, half28=`0x0800`, byte30=`1`,
byte31=`2`. This observation
is a CPU descriptor witness, not a replacement descriptor producer or native
readiness predicate. Initial clear and every original update must actually run.

The actual font static texture object is literal `8041e97c`. Native
`BattleFontGxState.staticTextureCurrent` still needs the same original BFN heap,
actual mapped object and original Setup completion, matching imported font
pixels, resident native texture generation and a live allocation/frame owner.
A nonzero global font pointer, object bytes, this cursor's completion, vector of
requests, fixture camera or imported atlas alone cannot prove that predicate.
No normal renderer, Trio, camera, queued callback or texture import is activated.

## Validation

Focused tests exercise actual original US BFN and initialized DOL spans read
from the read-only ISO, all16 low-format cases, full format/high argument bits,
all memory/child boundaries held, exact trace equivalence after resumption,
full-width filter-address wrap, alias mutations around conversion boundaries,
missing children, unsupported indirect target, NaN pending, reentry, changed
service/argument rejection and sticky stale lifetime. Actual original file
identity and all source bodies/data are independently pinned by the verifier.
Strict host, ASan/UBSan and GNU ARM syntax checks are reproducible with the
ignored validation driver recorded in the scoped artifact manifest. Root owns
combined `make test`/`make all` after integrating these new files.
