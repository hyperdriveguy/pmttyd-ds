# Field party global countdown and queued action request

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
unk_8014140c at0x8014140c,size0x94;marioKeyOffChk at0x8005c2ec,size0x10;
partyDoWork at0x800bbddc,size0xa8. The first routine retains its unknown
original annotation;its local behavior is documented without assigning a
speculative original function name. No executed input/animation trace is claimed.

[partyMain](FIELD_PARTY_DRIVER_AND_INDEX_OWNERSHIP.md) calls0x8014140c once
when at least one pointer-table object is enabled,before rereading/driving
objects. Thus the countdown's local cadence is a partyMain call admission,
not a proven rendering-frame or unrestricted main-loop rate.

## Flag, input-off check and byte wrap

Capture marioGetPtr as M. If **word M+0x0c mask2** is clear,return0 without
reading the countdown byte or issuing the action request.

Otherwise call marioKeyOffChk. It rereads the global Mario pointer and returns
**signed byte Mario+0x39**;it does not resolve a pad mask or sample a button
history. Any nonzero result cancels locally:write byte0 at captured M+0x3b,
clear mask2 in captured M+0x0c and return0.

With result0,read the countdown as unsigned byte M+0x3b,subtract1,store the
low byte and sign-extend that low byte for the expiry test. If signed result
is **>0**,return0. Otherwise write byte0,clear mask2 and call partyDoWork.
This expiry arm returns partyDoWork's result,without replacing it with a fixed
success code. The caller partyMain ignores that result.

| Raw byte before call | Stored low byte after decrement | Signed test | Outcome |
| --- | --- | --- | --- |
| 0 |255|-1|Expire and clear0 |
| 1 |0|0|Expire and clear0 |
| 2 |1|1|Continue |
| 128 |127|127|Continue |
| 255 |254|-2|Expire and clear0 |

The table is static arithmetic evidence,not observed countdown duration. A
host signed-byte decrement,clamping negative input before decrement,or saturating
the byte at zero is not the same input-domain behavior.

## partyDoWork primary-slot admission

partyDoWork ignores its incoming argument. It gets the current global Mario
pointer and reads signed **Mario+0x245**,the primary slot index. Negative uses
null;otherwise read pointer table[index] at0x8041ead8,without an upper bound
check. Null returns0. If the object's unsigned mode byte+0x34 is neither
**0,1 nor2**,return0 without the following writes.

For an admitted object P,get the global Mario pointer again,write binary32
positive zero to **Mario+0x180**,then call
[partyChgRunMode(P,3)](FIELD_PARTY_MODE_INITIALIZATION.md). Return1 after
that call. There is no local input-history check,resource cost/HP/FP write,
position change or story flag write. The second Mario pointer read is not
substituted with the first pointer in this contract.

Mode3's subsequent [driver](FIELD_PARTY_DRIVER_DISPATCH.md) ORs0x100 into
P+0 and invokes kind record+0x0c with P,subject to the driver's admission/
early-shadow/G branches. That local request connection is now explicit;
it does not prove the action's actual effect,animation,timing or all eligibility
rules. Writers of M+0x0c mask2,M+0x3b and input-off byte+0x39 remain open.

## Verification and remaining work

tools/verify_field_party_drive.py pins these three complete local bodies and
the full driver body,58 instruction anchors,one zero constant,four data spans
and two field callback imports. Repeated metadata is checked separately from
original execution;no ISO/profile/save mutation or dialogue rewrite occurs.

Next:queued-action flag/countdown/input-off writers,kind-use callbacks,global
updater/partyMain caller cadence and original input/transition observations.
The local countdown and action request do not close all partner abilities,
field story events or the complete game.
