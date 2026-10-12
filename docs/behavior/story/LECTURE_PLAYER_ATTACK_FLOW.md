# Lecture player weapon selection,Jump/Hammer input branches and mode handoffs

US G8ME01 disc0 revision0, ID1, recovered2026-10-12. Evidence B: gor professor
weapon/data records and original lecAttackEvent_Jump0x68e48,size0xb7c,197
commands;lecAttackEvent_Hammer0x699c4,size0xa2c,193 commands;lecture_end_event
0x6a3f0,size0xcc,13 commands. Selected DOL weapon-menu construction regions
close their actual default-weapon bindings. The large menu body is fingerprinted
but **not fully specified**. No original lecture execution or measured input
latency is claimed. REL addresses here are file offsets.

This extends [coordinator modes](LECTURE_COORDINATOR_MODES.md) with the missing
mode1→2/5→6 writers and work2 feedback. It does not substitute these lecture
scripts for [ordinary Jump](../battle/BATTLE_NORMAL_JUMP_EVENT_FLOW.md) or ordinary Hammer.
Professor P is the unit ID stored in BattleWork+0x424 and resolved by actor-6.
The current player attack owner remains-2;these are distinct selector contexts.

## Default weapon substitution and original power tables

Professor kind0x67fb8 has data table0x6824c:

| Key | Original binding |
| --- | --- |
| 0x44 | Lecture Jump weapon0x680cc |
| 0x45 | Lecture Hammer weapon0x6818c |
| 0x46 | Lecture end script0x6a3f0 |
| 0 | Terminator |

Weapon+0xb0 binds Jump script0x68e48 or Hammer script0x699c4. Their power
callbacks are the normal Mario-level readers,not a constant tutorial power:
Jump0x80189cb0 with six signed words(1,1,2,2,3,3);Hammer0x80189c38 with
(1,2,2,4,3,6). [Power callbacks](../battle/BATTLE_WEAPON_POWER_CALLBACKS.md) specifies
signed level/index/bonus and shared-result-bit dependencies. Reusing these tables
does not prove current gear levels,bonuses or final hit totals.

In _btlcmd_MakeSelectWeaponTable0x80123ec0,size0xd50,checked default construction
regions first create the normal entry and derive display fields,then optionally
replace its weapon pointer at `W+0x171c+0x80+index*0x1c`:

| Region / class | Replacement condition | Source |
| --- | --- | --- |
| 0x80124004..38,default Jump/class0 | W flags+0xef4 &0x09000000 nonzero,lecture ID+0x424 != -1 | BattleGetUnitPtr(W,lectureID),BtlUnit_GetData(unit,0x44) |
| 0x80124300..34,default Hammer/class1 | W flags+0xef4 &0x0a000000 nonzero,same ID guard | Same lookup,key0x45 |

The normal default entry is built only on its checked signed pouch level>=1
path. No local null-unit or null-data check occurs after the lecture ID guard.
The pointer replacement occurs after earlier name/icon-related work;do not
recompute the entire entry in a different order without closing those services.
Higher-level/badge/other weapon entries and complete class availability remain
outside these selected regions. The masks select these default entries;they do
not prove every Jump/Hammer option in the menu becomes a lecture weapon.

Coordinator mode0 sets0x01000000;mode4 clears it and sets0x02000000;mode8 clears
that and sets0x08000000. Thus the checked Jump replacement accepts01 or08;
Hammer accepts02 or08. **W+0x2740 is not the selection operand in these regions.**
Its proven reader instead suppresses status display,as documented in
[window arbitration](../battle/BATTLE_STATUS_WINDOW_ARBITRATION.md).

## Lecture Jump entry and teaching input branch

Jump begins JumpSetting(-2,20,raw0xf24a7a80,raw0xf24a7d4c),then
GetSelectEnemy(local3,local4). Target unit-1 jumps to label99 cleanup. Otherwise,
when P.work1==1,set P talk/stay T_1/S_1 and print **mac_0_b_t2_001**,mode2,
argument0,speaker-6. Declare attack,WaitGuardMove,pay current weapon cost,run
owner data event key7 as a child,then configure camera mode11/homing/zoom250.
Read current weapon action level,set AC difficulty,and SetupAC(-2,1,1,0).
Movement/takeoff/Mario-specific callbacks retain their original requests;see
[Mario motion](../battle/BATTLE_MARIO_JUMP_MOTION.md),[parameters](../battle/BATTLE_MARIO_JUMP_PARAMETER.md)
and [takeoff](../battle/BATTLE_JUMP_TAKEOFF_POSITION.md).

StartAC(1) at0x69064. Set local10=0;label10 is the repeated jump/hit entry.
The script uses MarioJumpPosition requests with count36,mode3 on its later-hit
route and mode0 on its other route. These are callback arguments,not measured
36-frame input windows. In the teaching branch at0x6914c:

- Read P.work1. If exact1 and local10<=0,run/join a named P rotation0→180,
  interpolation mode5,duration20,21 iterations with one-frame waits.
- In mode1,set TimingA flag mask1,read success frame/window into local0,integer
  DIV by2,WAIT_FRAME(local0).
- Only when local10<=0,set P T_1/S_1 and print **mac_0_b_t2_002** as P. Wait1,
  then label11 polls PadCheckTrigger(channel0,mask0x100,local0). Zero waits1
  and jumps back;nonzero proceeds. WAIT_MSEC150,evt_msg_continue.
- Outside mode1,request the same timing-mask1/success-frame/DIV2/frame-wait
  sequence without that first-hit message/poll block.
- Clear timing mask1,then ResultAC at0x6930c.

The timing flag callback resolves its on/off operand but reads its mask **raw**,
ORing/clearing it in W+0x1f50. Mask1 is not mask2,the TimingA count-window
selector. Success-frame callback writes the existing window helper's result to
its raw destination and returns2. Neither callback samples input or stops the
physical/battle pad producer by itself.

Crucially,[TimingA result routing](../battle/BATTLE_TIMING_A_COMMAND.md) has an already
recovered consumer of battle flag0x01000000:after its option/token and actor
automatic routes,it selects classification3,result2,count+1 without ordinary
history checks. The lecture coordinator supplies that flag during its initial
Jump stage. The script's pedagogical A-trigger poll therefore must not be
replaced with a claim that the later ResultAC always judges that press against
an ordinary timing window. Other earlier result routes still have precedence.
Mode9 practice uses08 rather than01 on the checked phase path;other writers and
runtime traces remain necessary before inferring every live flag value.

## Jump feedback,damage and repeated-hit ownership

After ResultAC,only when local10==0,write P.work2=0. If P.work1==9,get output
parameter **0**,then map3→work2=1,1→2,2→3. Other outputs retain that just-written0.
Later repeated hits do not overwrite this first-hit feedback through that block.

GetResultAC(local6). The success-bit mask2 controls original command damage:

| Path | Damage request / continuation |
| --- | --- |
| Success bit clear | CommandCheckDamage(-2,target,part,0x100,local5);first hit declares audience failure-1;StopAC/aftereffect/recovery then label95 |
| Success bit set and shared result count<=1 | CommandCheckDamage flags0x20000;success-effect/prize/audience requests;WAIT_FRAME2;local10++;GOTO10 |
| Success bit set and count>1 | CommandCheckDamage flags0x20100;WAIT_FRAME2;fall into label90 |

The count read is shared GetResultCountAC,not local10. Setup/start are not rerun
on label10. StopAC at label90 follows final/terminal result handling,then audience
AP recovery/report and weapon aftereffect requests. The earlier failure path has
its own StopAC. Preserve original flags and declaration sites rather than inventing
an extra damage multiplier from a successful input. Move/home/rotation callbacks
and child lifetime determine actual elapsed time.

At label99(0x6995c),ResetFaceDirection(-2),run default SuitoruBadgeEffect as a
child,read P.work1. Exact1 writes **P.work1=2** at0x69998. Then StartWaitEvent(-2),
delete/end. The write is not conditioned on the success result in this cleanup
block;the target-1 route also reaches it. Do not implement1→2 solely as a
successful-hit transition.

## Lecture Hammer setup and held-left teaching branch

Hammer also gets selected target and sends unit-1 directly to label99. It
otherwise declares attack/pays current weapon cost/data-child work and reads
P.work1. Exact5 prints **mac_0_b_t2_008** as P,then label1 polls
PadCheckNow(channel0,0x40000). While zero,WAIT_FRAME1 and repeat;the shown
entry branch requires held-left rather than an A/B trigger.

The named setup event records its ID in local15. Read weapon action level/set
difficulty;AcSetParamAll(4,25,0,0,0,raw sentinel0xf1194d80 repeated3). If mode5,
AcSetFlag(2),else AcSetFlag(0). SetupAC(-2,3,1,0),perform its motion/camera/pose
requests,StartAC(1). In mode5,label5 polls AcGetOutputParam(0);zero waits1 and
repeats. Other modes omit that poll. This is an explicit ID3/period25 binding;
it does not use the ordinary Hammer core's other-weapon period40 branch.

Parent sets local14=0. In mode5,launch a named monitor event saved in local14.
It has an original five-iteration loop,with these nested tests/requests:

1. Read held-left. On zero,CHECK_EVENT(local15,local0).
2. If that check is nonzero,STOP_EVENT(local15),AcSetParam(2,1),P T_1/S_1,
   print **mac_0_b_t2_009** as P.
3. Label6 polls held-left until nonzero,waiting1 per zero result.
4. WAIT_MSEC200,evt_msg_continue,AcSetParam(2,0),START_EVENT(local15).
5. WAIT_FRAME1 and the original loop/set operations;then end the named event.

An empty CHECK_EVENT==0 branch does not stop/restart an invalid/completed event.
Do not turn this bounded monitor into an invented perpetual watcher or merge
its event-stop state with the command's separate parameter2 suppression source.
[Hammer main](../battle/BATTLE_HAMMER_STICK_KEEP_LEFT.md) provides the mode-bit1 retry and
suppression rules;flag2 also differs from the ordinary mode0 release behavior.

Parent joins setup local15. If monitor local14!=0,delete that event via opcode95.
In mode5,set P T_1/S_1,print **mac_0_b_t2_010** as P. Label10 polls held-left:
nonzero waits1/repeats;zero proceeds to ResultAC,WAIT_MSEC200,evt_msg_continue.
Other modes directly ResultAC. Thus the teaching script preserves hold/rehold/
release phases,explicit stop/resume and command parameter changes;the native
harness's ordinary command does not establish this interaction.

## Hammer feedback,damage and mode5→6

After its ResultAC block,write P.work2=0. If mode9,get output parameter **2**,
map3→work2=1,1→2,2→3;other output leaves0. This differs from lecture Jump's
parameter0 and first-hit-only block. Play original hammer swing/pose/wait7/hit
sound requests,then GetResultAC. Success bit clear requests CommandCheckDamage
flags0x100 and audience failure-1,GOTO90;success requests flags0x20100 and its
prize/success/audience effects. Label90 stops AC and performs camera/recovery
requests;label95 handles returning home.

Label99(0x6a390) resets face direction,reads P.work1 and on exact5 writes
**P.work1=6** at0x6a3c4. StartWaitEvent(-2),delete/end. Again,the mode writer
is at shared cleanup rather than conditional on a successful result;target-1
also reaches that block. It must not be replaced by a success-only progression
latch or by writing the attack owner's work slot.

## Lecture end script and restoration boundary

Professor data key0x46 binds lecture_end_event0x6a3f0. When dispatched,it requests:
Mario pose M_S_1;camera mode3/moveto(0,58,256,0,41,0,30,11);WAIT_MSEC500;
StatusWindowOnOff(0);professor T_1/S_1;print **mac_0_b_t2_024**,mode2,argument0,
speaker owner-2;camera mode0;WAIT_MSEC500;set battle flag0x20;delete/end.

There is no local clear of W+0x2740,no removal of Mario HP-skip attribute
0x40000000,no work-mode reset and no resource export/reward calculation in this
13-command stream. Full battle-end key dispatch,unit teardown,field return and
restoration must be recovered before claiming complete lecture cleanup. Setting
flag0x20 is a request,not proof of a measured successful exit or result code.

## Evidence and implementation boundaries

Six original US message sites bind keys001/002/008/009/010/024 in gor_02;
player-script speakers are selector-6 (P),end-script speaker is owner-2 (P).
Reuse exact text/control tokens and preserve their guarded order. The verifier
pins three whole event/count spans,two weapon records/professor table,selected
menu regions,mode/feedback/AC parameter writes and message payload identity.
Movement/sound/timing callbacks are requests with separately recovered contracts;
this document does not calculate a total attack duration or hardware input latency.

Implementable checks:default record replacement uses masks09/0a and professor
ID guard;lecture Jump setup is ID1,lecture Hammer ID3 with literal4/25;first-hit
Jump feedback differs from Hammer parameter2;cleanup writes1→2/5→6 even on a
valid target-1 cleanup path;status suppression is a different field/service;
end script performs only its listed requests. Original branch/input/counter/field
traces and complete menu/end controllers remain open before full lecture/game
or complete-game requirements closure.
