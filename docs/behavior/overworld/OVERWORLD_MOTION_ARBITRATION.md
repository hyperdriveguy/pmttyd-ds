# Ordinary Mario motion dispatch and simultaneous jump arbitration

US G8ME01 disc0 revision0, recovered2026-10-12 from read-only ISO.
This closes the plain idle/walk/dash simultaneous-input slice left open in
[the movement contract](OVERWORLD_MARIO_MOVEMENT.md). Evidence is static binary
control flow, not a Dolphin gameplay/input trace. Reproduce20 containing bodies,
94 instruction anchors and15 data spans with
`python3 tools/verify_overworld_motion_arbitration.py`.

## Supported semantic domain

The finite ground closure below applies to ordinary Mario character0, current
motion0/1/2, force-move halfword+4e=0, scale1, no movement badge0x143 or Jabara
ability, no transformation/external movement, normal idle/walk/dash poses,
ordinary command flags, and no other buttons requesting items/Hammer/partner
abilities. Field/key/system eligibility and jump contact/talk admission are
separate owners; ordinary admitted A trigger is the domain used in the tables.
Caller stop/pause/pose/NPC gates must not be bypassed to enter this domain.

Retain actual input-trigger+24c and held+24a words separately; A=0x100.
Current input angle+198 and magnitude+194 have already been queried by
marioMain before the dispatch call. Raw signed8 axes+252/+253 supply squared
radius. Threshold uses the raw radius, not a rounded normalized analog float.
Magnitude is independently capped70 by marioGetStick. No radial .15 deadzone
is introduced by these field consumers; SDK producer clamping/deadzone and
native Circle Pad quantization remain separate adapter boundaries.

## Immediate dispatcher and changing the pending motion

`marioMotion`0x8009461c clears+200, reads current unsigned motion+2e, invokes its
main callback, then loops if command+0xc bit0 is set (0x80094658/664).
There is no fresh input sample in this loop. All callbacks in one dispatch call
see the same trigger word until an actual writer changes it.

An ordinary permitted `marioChgMot` request0x8009455c saves current into previous
+30, writes new+2e BEFORE invoking previous motion's post callback, then sets
pending bit0. It does not compare new motion with old to avoid same-state entry.
Motion-change lock MarioFlags0x10000000 prevents mutation. Previous16/17 special
cleanup is outside this plain domain.

Original callback table0x80310030 has12-byte rows(main,reserved,post):

| Motion | Main | Post |
| --- | --- | --- |
| 0 idle | mot_stay800a6c50 | mot_stay_post800a6b10 |
| 1 walk | mot_walk80087994 | Null |
| 2 dash | mot_dash8008763c | Null |
| 3 jump | mot_jump8009b480 | Null |
| 10 fall | mot_fall8009af1c | mot_fall_post8009aed4 |
| 11 fall variant | mot_fall8009af1c | Null |
| 12 upstairs | mot_upstairs8009b158 | Null |
| 13 land leaf | mot_land8009ade8 | Null |

Idle post cancels sound handles+27c/+280 (with motion19/20 exceptions); it does
not consume A trigger, alter input magnitude or prevent repeated jump admission.
Fall post cancels sound handle+284. Null walk/dash/jump post callbacks cannot
consume A or change the later threshold outcome.

## Jump query is a request, not an immediate launch

`marioChkJump`80092814 first reads trigger+24c and returns0 without A. It rejects
motion26, certain contact/node/NPC conditions on+1e0/+1e4/+1e8, and active
fbatNpcTalkCheck8004662c. On ordinary admission it saves current/previous motion,
sets current3, calls previous post if present, then sets pending bit0
(0x800929ec–80092a20). It returns1 after admission. With motion-change lock set
it can also return1 WITHOUT changing motion; return1 alone is not successful
jump-entry evidence. The caller must track actual state/entry mutation.

Grounded callers do not return immediately after this request. Subsequent
walk/dash/idle movement selection can overwrite pending3. Because neither A
trigger nor the eligible plain contact/talk inputs are consumed, a new ground
callback reached through the dispatcher can request3 again. Thus an overwritten
request does NOT imply a dropped jump. Actual jump initialization happens only
when the dispatcher finally invokes mot_jump with entry bit0.

Transform query80092624 also runs after jump request; without transform trigger
0x800 at+24c it returns0 without mutation. Other item/Hammer/partner queries are
ordered earlier and can suppress this plain path. They remain dependencies.

## Plain grounded callback rules

These rules preserve the movement-visible source order, not all presentation
and idle random-animation/audio requests.

Idle0:

- If heldA is clear, clear command flags0x00200000/0x00400000 first
  (0x800a6cac–cbc).
- On pending entry, consume bit0, clear air flags0x000f0000, clear jump tuple,
  reset signed counter+50 and phase/countdown+44/+48, capture position, then
  establish base+180=0 in ordinary mode (0x800a6cc0–e88).
- Ordinary action/pose/Jabara gates precede jump. Then request3 on admittedA
  at0x800a7120; transform follows at0x800a7124.
- If current magnitude!=0 and command0x00400000 is clear, request1 when
  raw radius²<=3025, otherwise2 with no movement badge (0x800a7128–71a4).
  This can replace pending3. Do not dispatch jump immediately at7120.

Walk1:

- On pending entry, consume bit0 and clear air flags0x000f0000. With force timer
  +4e==0, initialize base+180=walkBase*scale (ordinary1); reset phase/countdown
  according to its entry rules. These occur BEFORE jump query.
- After ordinary pre-gates, jump query80087c04 then transform80087c08 run.
- If raw radius²>3025 with no movement badge, request2 at80087c44 and return
  BEFORE writing travel heading.
- Otherwise write current+198 into travel+1a4 at80087c4c/450 and increment
  phase/count+48. This tail still runs after pending jump was requested.
- If current magnitude==0, request0 at80087cc0, replacing pending3 if present.

Dash2:

- Pending entry initializes base+180=dashBase*scale (ordinary2.25) with force
  timer+4e==0, before its jump query.
- Jump800878a8 and transform800878ac run; phase/count+48 increments.
- With raw radius²<=3025 (or movement badge present), request1 at8008792c,
  then return BEFORE travel write.
- Otherwise write current+198 to travel+1a4 at80087934/938. Ordinary nonzero
  strong input has no later idle request.

Zero magnitude and raw axes are not interchangeable in special restricted-axis
modes. The plain table below assumes ordinary signed axes where magnitude0
corresponds both axes0. `marioChgMotAuto`800a6bbc separately selects0 for exactly
zero raw axes,1 for nonzero squared radius<=3025,2 for>3025/no badge. It requests
entry through marioChgMot; it does not itself invoke the main callback or consume
A. Do not invent a direct call to Auto inside mot_stay/walk/dash.

## Derived source paths: same physical input sample

These are control-flow reconstructions, not observed original execution traces.
`request3→request2` means two requests within one callback, followed by dispatch
of the final requested state. The A-trigger word remains unchanged throughout.

| Starting motion | Input+A trigger | Callback/request sequence | Final launch base |
| --- | --- | --- | --- |
| Idle | Neutral | Idle requests3; Jump entry | 0 |
| Idle | Weak nonzero | Idle requests3→1; Walk entry requests3 and writes input heading; Jump entry | 1 |
| Idle | Strong | Idle requests3→2; Dash entry requests3 and writes input heading; Jump entry | 2.25 |
| Walk | Weak nonzero | Walk requests3 and writes input heading; Jump entry | 1 |
| Walk | Strong | Walk requests3→2; Dash entry requests3 and writes input heading; Jump entry | 2.25 |
| Walk | Neutral | Walk requests3→0; Idle entry resets base0 and requests3; Jump entry | 0 |
| Dash | Strong | Dash requests3 and writes input heading; Jump entry | 2.25 |
| Dash | Weak nonzero | Dash requests3→1; Walk entry requests3 and writes input heading; Jump entry | 1 |
| Dash | Neutral | Dash requests3→1; Walk entry requests3→0; Idle entry requests3; Jump entry | 0 |

Equality squared radius3025 belongs to Weak/walk. Under the fixed plain
admission domain each ground start reaches a stable ground or Jump in at most
four callback invocations. This is a derived domain bound, NOT a clamp enforced
by original marioMotion; exceptional callbacks/services/flags can change it.
A native semantic reduction can classify this current ground input and establish
base/travel BEFORE jump begin, discard any ground displacement, then execute
jump-policy displacement. It must preserve entry counters and special owner
boundaries instead of inventing a blanket jump-first priority.

For standing+.5 horizontal Circle Pad mapped to signed original magnitude35,
base1 plus ordinary current-input drift35*.00375 gives1.13125 original units,
or.0113125 native scale0.01. Drift-only.0013125 is not this admitted ordinary
idle→walk→jump path. A simultaneous current-heading turn writes the new heading
before jump snapshot; using preceding-tick travel gives the wrong launch direction.
After launch, neutral air input still retains this established travel.

## Jump/fall entry and launch counters

Jump entry consumes bit0, sets phase0/countdown2, clears signed counters+50/+52,
sets jump0x00010000 and clears fall0x00020000. It retains base/travel and snapshots
base into independent+190. Same invocation countdown2→1 and counter0→1 occur.
There is no ground walk/dash query in mot_jump. First vertical integration is
next manager call at phase10/counter2; released-A shortening requires counter>=4.
See the [movement contract](OVERWORLD_MARIO_MOVEMENT.md) for pinned tuple gates.

Fall10/11 entry consumes bit0, clears jump flag and sets fall flag, clears two
command flags and phase+44, handles fall sound/pose, but does NOT reset retained
base/travel or signed+50. If counter==0 it captures the return position. The
ordinary callback increments signed+50 once at8009b074–07c. Jump→fall transition
inside physics can set pending10 late in the manager call; this does not invoke
fall main again immediately because the earlier marioMotion loop has finished.
The next manager call processes fall entry. Contacts, tuple/fall-at-height policy,
Jabara flags and ceiling-motion requests remain their own documented owners.

## Ordinary late landing and the separate land13 leaf

`marioLandOn`80092c94 resets signed counters+50/+52 and ordinarily clears the
jump tuple unless the landing node's attribute0x200 requests retention. Its
ordinary landing route excludes special surface attributes, char2, ride/external
modes, Jabara and motion21. It reads current magnitude+194 and command0x00400000.
With magnitude>0 and movement-command flag clear, selects pending WALK1 at
8009365c/668/694 (no Jabara). Neutral ordinary landing selects pending IDLE0 at
800937dc/7e8/814. It does not universally select land13 or impose a landing wait.

These late landing requests preserve base/travel until next dispatch entry.
`marioMain` already called marioMotion before the movement/fall/landing path;
there is no second dispatch or input sample in the normal tail. The next manager
call queries CURRENT input before entering idle/walk. Strong input can change
pending walk→dash; a NEW admitted A trigger can then launch through the same
bounded paths. An A trigger seen only in the prior airborne/landing call is not
locally buffered or replayed. HeldA affects vertical shortening, not new-launch
admission. This distinction matters for presses just before ground contact.

The table also contains mot_land13, but its exposed leaf is separate: pending
entry clears air flags, counters/phase/base; phase0 requests idle0 immediately
(8009ae84), and dispatcher repetition can then reach ordinary idle/walk/dash.
Its phase1/10 countdown tails belong to existing-state domains; pending entry
sets phase0. Do not insert state13 into all ordinary landings merely because this
leaf exists or its name suggests a general landing animation.

## Boundaries and next evidence

Full SDK deadzone/clamp, force timer/background/Paper/badge/Jabara, stale or
special poses, B/item/Hammer/transform/partner activity, node/NPC/talk admission,
previous16/17 cleanup and root-lock paths remain excluded dependencies. No
partner movement ownership was changed or researched by this slice.

The native plain reduction should check neutral/nonzero/threshold equality,
idle and both crossing directions, current-heading turns, repeated heldA without
new trigger, airborne presses before landing, late pending idle/walk and fall
entry retaining travel. Runtime source comparison remains requested; no original
input trace, arithmetic exceptional-domain equivalence or full field fidelity
claim follows from the finite local closure.
