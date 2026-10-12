# Professor lecture field entry, choices and return requests

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: original
gor.rel event bytes and relocations, plus exact US message-resource bindings.
All addresses below are REL file offsets. This is a static request contract;
there is no original gameplay observation, measured timing or complete battle
transition/teardown claim.

| Original event | Offset | Size | Commands | Scope documented here |
| --- | --- | --- | --- | --- |
| kurihakase_wall_remove | 0x490f8 | 0x404 | 63 | Lecture handoff and preceding presentation requests |
| kurihakase_lecture | 0x494fc | 0x478 | 75 | Entire local request/choice/return graph |
| kurihakase_init | 0x410c8 | 0x218 | 37 | Professor field positions and story cases |
| gor_02_init_evt | 0x4d554 | 0x87c | 171 | Scoped lecture reload and Goombella NPC entry guards |

Whole event hashes pin containing bytes; they do not close every callback or
all unrelated map initialization. The NPC registration record has init pointer
at 0x45000 to 0x410c8 and talk pointer at 0x45008 to 0x412e0. The latter's
all-chapter conversation graph is outside this contract.

## Entry and story ownership

The story operand is raw **0xf5de0180**, the same operand used in the
[harbor opening](STORY_HARBOR_OPENING_BINDINGS.md). Preserve the encoded
operand until the persistent-variable namespace and save layout are implemented.
Two local ADDR32 references target the lecture: 0x494f0 and 0x4dba4. This
local pointer census is not a proof excluding computed or foreign callers.

The wall-removal event waits for door completion, disables input, waits 500 ms,
turns Mario to 270 and requests a camera transition. It takes party slot 1's
position, clears that party slot and positions the Professor NPC there. It
requests flag mask 0x40000020 on, NPC movement, then Professor message
mac_0_083. Paper/offscreen/image capture and animation requests remove the fence;
the brother requests SFX_OFF_FENCE_OPEN1, waits 1000 ms and requests
SFX_OFF_FENCE_FALL1. Those milliseconds are script literals, not observed audio
timing. Image animation completion is explicitly awaited before release; the
collision object A_saku_04 is toggled through evt_hitobj_onoff.

The tail positions the Professor at (-220, 305) through the movement service,
gets his position and requests two no-hit jumps with arguments (225, 30).
It prints mac_0_084, clears mask 0x40000020, waits 500 ms, releases the camera
with (500, 11), requests party slot 0 run and enables input. At **0x494e0** it
writes story **10**, then **0x494ec RUN_EVENT** starts the lecture asynchronously.
These are requests; successful original service execution has not been observed.

Map initialization separately checks story exactly **10** at 0x4db94, then
RUN_EVENTs the lecture at 0x4dba0. The Goombella field NPC is created when
story is **>=7 and <=10**, configured with c_pkr and her original tribe key,
hidden at (0, -1000, 0) and given rotation 90. Do not create that NPC
unconditionally in every chapter merely because this lesson references it.

## Crossing poll and introduction

The lecture first gets Mario position into LV0/LV1/LV2 and copies it into
LV3/LV4/LV5. Its unbounded DO loop gets the position again and calls
evt_sub_check_intersect with:

```
LV0, LV2, LV3, LV5,
0xf245ca80, 0xf24e6280, 0xf2475a80, 0xf24e6280, LV6
```

Under the recovered encoded-float literal route the four literals are
(-300, 250, -200, 250). LV6 exactly 1 breaks the loop. Otherwise the
script waits one frame, copies LV0 to LV3 and **LV1 to LV5**. The latter
instruction at **0x495ac** copies Y into the slot initially containing Z.
Preserve this original operand and order. Do not silently replace LV1 with
LV2. The [intersection and coordinate contract](../field/FIELD_SEGMENT_INTERSECTION.md)
now establishes closed-segment endpoint policy, rounded fused cross-products
and the query's encoded local-coordinate route. Its original observed effect,
Mario state writers and complete scheduler composition remain open.

After breaking, the script disables Mario input and stops party slot 0. It
sets shared operand **0xfaa2b580** to 1. A brother gets Mario position,
requests SFX_VOICE_MARIO_FIND1 there, requests a speech-bubble effect with
duration argument 60, turns Mario to 90, waits 800 ms and clears that same
operand to 0. Another brother turns party slot 0 to 90. The parent prints
mac_0_084_01, then opcode 11 waits on 0xfaa2b580. This variable is shared
across these requests; it is not established here as a persistent story flag.

Camera request arguments are (-245,100,639,-245,40,294,500,11). A brother
moves party slot 0 to (-290,280) with encoded argument 120 and turns it to
90. The parent moves Mario to (-270,305) with encoded argument 120, turns
him to 90 and prints mac_0_084_02. Movement callback interpretation and
brother lifetime/scheduler details remain service dependencies.

## Exact choice branches and original US text

The lesson choice is the value returned in LV0; preserve the numeric branch
tests rather than inferring English yes/no labels from their IDs.

| Step | Original request | Branch |
| --- | --- | --- |
| First selection | mac_0_084_02_yesno | LV0 exactly 1 goes to battle label 50 |
| Otherwise append | mac_0_084_03 | Followed by second selection |
| Second selection | mac_0_084_03_yesno | LV0 exactly 0 goes to battle label 50 |
| Otherwise append | mac_0_084_04 | Go to common label 100, skipping battle |
| Battle label 50 | Append mac_0_084_05 | Perform NPC/party conversion and launch |

All identifiers bind uniquely to **msg/US/gor_02.txt**. The verifier pins each
payload hash and its original key, service and speaker binding; the original
payload and control tokens must be reused without rewriting or translation.
mac_0_083, mac_0_084, mac_0_084_01, mac_0_084_02 and mac_0_084_07 print
with the Professor actor key at 0x18a3c. mac_0_084_06 prints with Goombella's
actor key at 0x19fc4. Appends 03/04/05 carry no new explicit actor operand.
The original Japanese actor-key bytes are identifiers, not English dialogue
or a license to substitute a guessed English actor string.

## Battle launch and common return

At label 50 the event gets party slot 0's position, sets Goombella NPC mask
0x8280, gets the party position again and positions the NPC there. It waits
one frame, clears mask 0x80, calls evt_mario_kill_party(0), requests a no-hit
NPC jump with that position and (200,20), clears mask 0x8000 and prints
Goombella's mac_0_084_06. Preserve the repeated position read and call order.
Mask values here specify requested operations, not fully recovered flag names.

At **0x498b0** evt_npc_set_battle_info(Professor, **3**) selects the
[lecture setup](CHRISTINE_HELPER_ROLES.md), then evt_npc_battle_start and
evt_npc_wait_battle_end are called in order. This is the actual used field
binding to the [coordinator](LECTURE_COORDINATOR_MODES.md) and
[player attacks](LECTURE_PLAYER_ATTACK_FLOW.md); it differs from Crump's
setup index 2. The [field wait contract](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md)
accepts field sequence 2 and any nonzero battle result. This event contains
no additional victory-result test after the wait.

On return it clears Goombella NPC mask 0x8000, requests
evt_mario_set_prev_party_pos(0,LV0,LV1,LV2), hides that NPC at (0,-1000,0)
and prints Professor mac_0_084_07. The
[previous-position callback](../partner/FIELD_PARTY_EVENT_CALLBACKS.md) can request party
creation on its first call when the current slot is negative and its saved
kind byte is nonzero. The event needs no separate creation opcode for that
route. Constructor resources and saved-kind lifetimes remain open;do not
invent an HP/resource reset,reward or explicit lecture-mode clearing here.

Both the skip path and battle-return path converge at **label 100**.
At **0x49938** they write story **11**, release the camera with (500,11),
request party slot 0 run, enable Mario input and delete the event. Advancing
to 11 is therefore not victory-only and does not require accepting the lesson.

## Professor field positions

Init sets mask 0x40000000 on the current NPC named me and switches the
story operand. CASE_EQUAL bodies do not fall through into following cases.

| Story state | Requested position |
| --- | --- |
| 8 | (-135,0,-240) |
| 9 | (0,-1000,0) |
| 10 | (-220,0,305) |
| 11, raw flag 0xf8406037 equals 0 | (-220,0,305) |
| 11, that flag otherwise | (0,-1000,0) |
| Following range 11..17 | (0,-1000,0); exact 11 has already matched |
| 376..402 | (0,-1000,0) |
| 18,64,120,174,221,271,347 | (-140,0,-225) |
| Default | (-140,0,-250) |

The flag's writers, badge lesson, other chapter talks and save serialization
remain open. This init does not by itself prove a map transition or dialogue
will occur in any supplied native save state.

## Reproduction and limits

Run tools/verify_lecture_field_flow.py against the read-only ISO. Its pinned
metadata manifest checks four whole event spans (**346 commands**), **332**
scoped ADDR32 bindings, **22** operand anchors, **four** entry/NPC links and
**11** exact US message sites. Reports live under ignored
artifacts/reference/behavior; original executable bytes and dialogue are not
included in source history.

Next: complete variable namespace composition and Mario coordinate writers;
field party constructor/destructor/previous-kind/run-mode lifetimes; complete battle-end and
lecture-mode/window/HP immunity teardown; full story flag writers and original
observations for lesson accepted, declined, interrupted and map reentry.
These dependencies prevent a claim of complete equivalent lecture execution.
