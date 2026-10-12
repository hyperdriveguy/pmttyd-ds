# WeaponSelectMenuMain: navigation, cancellation, audience, and End

Completes [confirmation/admission](WEAPON_MENU_MAIN_CONFIRMATION_AND_LIGHT_REQUESTS.md)
for static US G8ME01 Main80121d0c/0x35c. Retained B/T/U/M,privateSP08,and
resultR initially0 have the meanings there. [Verifier](../../../tools/verify_weapon_menu_main.py)
also checks complete WeaponSelectMenuEnd80121cb0/0x38. Preserve request and
partial-store order;no original execution or input-button mapping is claimed.

## Forward boundary uses Trigger;interior uses Repeat

When no confirmation is requested,load cursor from SP08. Read current count
WORD(C+8) BEFORE index WORD(C),compute wrap32(count-1),compare signed.
If index<count-1,request BattlePadCheckRepeat(0x00020000). Nonzero reloads
cursor/index after that request,stores wrap32(index+1),requests cursor sound,
then exits to common display with R0. It omits reverse,cancel,and audience.
Zero continues to the reverse test.

If index>=count-1,instead request BattlePadCheckTrigger(0x00020000).
Nonzero reloads cursor,stores index0,requests cursor sound,and exits as above.
Zero continues. Do not cache the original index for the later store or use
Repeat at the wrap boundary. The mask is0x20000,not the class/target menu's
0x80000 forward mask.

## Reverse boundary uses Trigger;interior uses Repeat

Reload cursor/index after the unsuccessful forward path. Signed index>0
requests Repeat(0x00010000);nonzero reloads cursor/index,stores index-1,
requests sound,and exits. Signed index<=0 instead requests
Trigger(0x00010000);nonzero reloads cursor,count,stores wrap32(count-1),
requests sound,and exits. Zero continues to cancellation. Negative/zero counts
are not repaired;negative indices use the boundary branch.

All four sound sites pass source address802efe98,
`SFX_BTL_CURSOR_MOVE2`,to psndSFXOn AFTER the index store. A successful
forward movement prevents reverse handling in this Main. This differs from
Input state11's independent forward/reverse target gates. Boundary-versus-
interior classification uses pre-request reads;the actual arithmetic uses
fresh post-request cursor/index/count reads.

## Cancel policy for retained type2 and actor mode

Only after no direction moved,request Trigger(0x0200). Nonzero examines
retained T. If T==2,read U8(U+0x2f3). Nonzero enters the special cancellation
route;otherwise read U8(U+0x2f4),whose nonzero also enters it. Both zero uses
generic cancel;other T skips these reads and uses generic cancel.

Special route reads S8(retainedU+0x20):

- Zero:request sound2521,WeaponSelectMenuEnd(B),then store state15.
- Nonzero:request WeaponSelectMenuEnd(B),then store state12;NO sound here.

Generic route requests sound2521,WeaponSelectMenuEnd(B),then stores state1.
All cancel variants set R1 and reach common display. No direct type/cursor
reset,help request,or target sampling call is added. The actor pointer is the
entry lookup;its status bytes are read only when the route reaches them.

## Audience branch remains result0

If cancel trigger is zero,request BattleCommandAttackAudienceCheck() with no
battle argument. Zero reaches common display. Nonzero requests
WeaponSelectMenuEnd(B),then stores BYTE(B+0x1c74)=1,and reaches common
display without setting R1. This service may itself write menu/type/state;
its full writer/event contract is separate. Do not return1 merely because
pending state or UI lifetime changed. [Input's pending re-entry](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md)
can subsequently drive progress despite this Main returning0.

## Complete End leaf and live flag masking

WeaponSelectMenuEnd(B) retains B,requests BattleMenuDisp_WeaponSelect_End(B)
at80121cc4,then reads current WORD(B+0x1720) (M+4),clears ONLY bit0x10
using mask0xffffffef,and stores it back. Read flags AFTER UI End;preserve all
other live bits,including0x01000000 set by setup. No explicit state/type/index/
reason reset or scalar success assignment occurs in this End body. Its rawr3
is the UI End service return. Actual UI End8011600c is a linked larger body,
not the separate four-byte Main stub.

## Port requirements and unproved domains

Implement interior Repeat versus boundary Trigger,typed wrapping arithmetic,
post-request rereads,forward priority,confirmation-before-cancel policy,
retained-type/actor short circuits,sound omission,explicit result ownership,
and UI-before-live-bit-clear End. Reuse original sound assets/keys.
Full table production,remaining item/operation/party/protect/confusion/gift/
audience writers,query callback/event reachability,and original Game execution
remain open. Main's fresh unsupported-type stack output does not gain a fallback
from Input's separate initialized cursor slot.
