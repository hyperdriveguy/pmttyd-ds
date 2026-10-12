# Badge recovery presentation: local float encoding and recovery effect route

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Static scoped local-work evtSetFloat/evtGetFloat/evtGetValue paths and MULF/ADD
regions in evtmgrCmd; scoped evt_eff prefix/recovery/common-return route with
complete effGetSet8005e8a0/88 lookup. Containing-body hashes do not establish
all namespaces/effects or actual rendering. Read
[tail graph](BATTLE_ATTACK_BADGE_RECOVERY_TAIL.md) and
[service wrappers](BATTLE_BADGE_RECOVERY_SERVICES.md).
Verify: `python3 tools/verify_badge_recovery_tail.py`.

## Quantized local-work floats and integer ADD

Event local operand Li is0xfe363c80+i; stored word lies at entry+9c+4*i.
For the ordinary valid scalar domain used here, evtSetFloat(Li,x) stores
wrap32(-230000000 + fctiwz(single(x*1024.0))). It does not store IEEE float
bits. Constants1024.0/1/1024 are44800000/3a800000 at8041fa18/8041fa1c.
The old stored word is read before storage and used to form the setter's return
value;the consumers here discard that return. Local encoded values are decoded
by signed word addition230000000, single-rounded biased signed-to-float conversion,
then single multiplication by1/1024. evtGetFloat returns that decoded float;
evtGetValue applies fctiwz to it. Ordinary integer stored values instead use
signed integer-to-single conversion for evtGetFloat and remain integer values
for evtGetValue. These are scoped valid-local routes,not validation/clamping of
encoded namespace extremes,overflow,NaN or FPSCR behavior.

GetStatusMg therefore quantizes scale at its store into L6. GetHeight stores
integer height in L7. Opcode60 at8003200c captures raw destination, resolves
RHS L6 through evtGetFloat FIRST,then destination L7 through evtGetFloat;
fmuls rounds their product once to single and evtSetFloat quantizes it into L7.
It returns dispatcher2. Then opcode53 at80031dfc captures raw destination,
resolves RHS L7 through evtGetValue FIRST,then destination L1 through evtGetValue,
adds words and evtSetValue stores integer L1. Thus final Y uses integer part-world
Y plus truncated DECODED quantized height product. It is not an ADD_FLOAT and
does not preserve fractional scaled height in the effect position.

For a normal example, height10/scale1.5 produces scaled height15 and adds15.
Height3/scale0.5 produces encoded1.5,then integer ADD contributes1;negative
height-3 contributes-1. These are derived arithmetic examples,not original
observations. For arbitrary scale retain both1024 quantizations,intermediate
single rounding and fctiwz;do not just truncate a host double height*scale once.

## Exact evt_eff operands and resolution ownership

Both brothers pass15 event operands:
`(0,0x802ee900,type,L0,L1,L2,L5,0,0,0,0,0,0,0,0)`.
Type0 is HP branch,1 is FP branch; the event supplies those concrete values.
The common evt_eff prefix resolves operand0 through evtGetValue THREE times,
retaining the FIRST result as optional name pointer. It similarly resolves
operand1 three times,retaining its FIRST result as effect-type name. Then
resolve operands2..14 once each as integers IN ORDER. Subsequently resolve
operands2..14 once each through evtGetFloat IN ORDER,including unused values.
The retained recovery arguments are integer operand2 (type), float operands3..5
(XYZ), and integer operand6 (amount). Service callback side effects/order are
not equivalent to an optimized subset of these reads.

Call effGetSet with the retained operand1 string pointer. That complete lookup
scans eight-byte records at803097a8 in order until signed16 tag-1, comparing
record.word+4 string against the supplied name via strcmp. First exact match
returns record pointer;no match returns null. This evt_eff caller immediately
reads the result's signed16 tag WITHOUT a null check. Valid effect name is an
invariant; there is no generic missing-name fallback here.

Both the event's string802ee900 and catalog string802c1ce0 are exact bytes
"recovery\0". Catalog row803097f8 has tag10 and dispatch table word
8030c0a4+4*10 targets8006dc68. That route requests
`effRecoveryEntry(type, X, Y, Z, amount)` at8014335c, retaining its effect-pointer
return. There are no other argument writes in this route and no local resource
mutation. The renderer's complete initialization,lifetime,motion/sound and draw
behavior remain separate dependencies.

Common tail at8006e2ec skips naming if returned effect pointer is null OR
retained optional name pointer is0. Both actual callers supply0,so no strcmp/
effSetName request follows creation on their valid route. The wrapper returns2
regardless of creation result;it does not publish the returned effect pointer
to event locals,wait for animation or gate later recovery on allocation success.
The event's later120 wait/recovery is independent of presentation completion.

Acceptance cases: scale/store/product quantization, fractional/negative height
integer addition; RHS-first read order; complete repeated operand resolution;
first exact effect catalog match; type0/1 and retained amount; null effect return
still allowing the event's later recovery;optional name0 bypass. Full variable
namespace/numerical extremes,effect renderer/assets/audio,display lifecycle and
original execution traces remain open. These request bindings must not be
replaced with generated tones/art or fabricated story text.


Complete [integer namespace](EVENT_INTEGER_OPERAND_NAMESPACES.md),
[float namespace](EVENT_FLOAT_OPERAND_ROUTING.md) and
[saved storage](EVENT_SAVED_SWITCH_STORAGE.md) contracts now extend the scoped
local-coercion descriptions above. Full renderer/audio and numerical/source/
persistence/original execution dependencies remain open.
