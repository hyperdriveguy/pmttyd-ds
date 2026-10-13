# Ordinary Hammer used binding for the underground trio

US G8ME01 revision0, documentation-only original-source recovery2026-10-13.
Rechecked original387-command NormalHammerCore8035d6d4/14d4, SHA256
a1db1fb43cdc5b0891c083bffb4506211972f85d4444879c77ff94b5b4007aef,
weapon80353de8, wrapper80416c40, and usedlevel/result leaves. Ignored inventory
/tmp/trio-hammer-events.json retains everycommand/rawoperand. No original
runtime impact tick, audio fidelity, native implementation or completion claim.
See [trio current source/adapter boundaries](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md),
[ordinary bindings](BATTLE_ORDINARY_ATTACK_BINDINGS.md),
[Hammer branches](BATTLE_HAMMER_WEAPON_BRANCHES.md),
[ID3](BATTLE_HAMMER_STICK_KEEP_LEFT.md), and
[inbox AC-02](../../requests/AC-02.md).

## Concrete ownership and smallest incremental implementation

Original weaponW80353de8+b0 points wrapper80416c40: SETLW14=0,
RUN_CHILD(core8035d6d4), RETURN, END. Install/launch this originalstream through
existing source selected-weapon/actor/event ownership ratherthan translating
Hammer intoa native timedimpact orusingoneoflecture/badge/firstattack graphs.
The current data-only DOL pack alreadycontains these originaldata records;
actual runtime callbackcoverage stillneeds implementation. No original PPC
execution isrequired tolaunch the retained eventscript viaexisting nativeVM.

A first reviewable increment binds source wrapper/core through ID3 setup and
approach/charge, keeping downstream unboundleaf as explicitpending. The next
increment binds original-result-driven impact and reaction/recovery. It must
preserve source ResultAC pendingpoll and all originalchild/brother/ACmanager
lifetimes. Existing portable HammerCommand canserve ID3 mechanics butisnot
itselfthe387-command attack. Original menu choice/filter/branch remainsowned
by source commanddata; no autonomousweapon substitution isintroduced.

## Fresh ordinary command path through approach

Assume admitted next-roundMario command, sourceBattleflagBef4mask2clear,
selectedW80353de8,targetbodypart1, actorlevel1, originalnot-special wrapperLW14=0.
The source repeatedlyreads mask2; retain those reads as livegates.

- Core8035d7b8 obtains selectedweaponLW12, GetSelectEnemy targetLW3/partLW4;
  no-target exitslabel99. AttackDeclare,WaitGuardMove,PayWeaponCost,
  RunDataEventChild(actor,7), childMajinaiPowerUpCheck80363388; cameramode11,
  homingunitparts and sourcecamera requests follow. All alreadyusedbyoriginal
  Jump are reusecandidates; a missingcallback isnot a successstub.
- Setup8035d8a0..994 reads unsignedW6e actionlevel3, appliesdifficulty,
  AcSetParamAll(4,25,0,0,0,rawsentinel,rawsentinel,rawsentinel), rawAcSetFlag0,
  SetupAC(−2,ID3,RAW1,resolved0). Preserve sourceSetupreset/registration,
  parameterpreservation and constructor/main callback lifecycle.
- Ordinary approach8035dc6c..dd14 requests type42(M_R_1), targethitXYZ;
  overwrite destinationYwithactorcurrentY; FaceDirectionAdd(target,X,35);
  actorspeed2; destinationZ+=5; calculatefacing mode1/change; MovePosition
  N0,sound−1,arg6zero. FreshSpiked targetdirection−1 thereforeX=targethitX−35.
  No fixed native approach duration isauthorized bythis sourcegraph.
- Calculatefacingmode16/change; PreCheckDamage8010abac at8035dd6c usingF100
  intoLW6. Sourceprecheck5 stopsAC, callsCheckDamageF100 thenRETURN; other
  nonordinaryprecheckresults preserve their later StartAvoid/cleanup branches.

[Target contracts](UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md) give default
Hammer A01001260/C21002000: excludes airbornePara; front-candidate occlusion
remains originalsampler. AftergroundGoomba removed, survivingfrontSpikedpart1
istheadmittedordinaryHammer target. Weapon counterresistanceW78=7fb hasmask2,
so source[early spike counter](BATTLE_COUNTERS.md) C1 cannotadmit withthisW;
use realprecheck/counter providers, nothardcodedSpikedimmunity. A Jumpchoice
stilluses itsoriginalcounter/warning graph. Preserve datakey25 sourceFrankly
mac_0_092_06/07 firstwarning and sourcework0oncegate; donotinjectwarning into
Hammer orautochange aselectedJump. No rewritten/invented storyline isprovided.

## ID3 main/input/result/cleanup ownership

Original ID3registration pointers are main8012a17c,result8012a174,
display80129fcc,delete80129fc0. Reuse source/battle_hammer.cpp `HammerCommand`,
`HammerContext`, `HammerSoundSink`, backedbyoriginalpacked fields in
[BATTLE_HAMMER_STICK_KEEP_LEFT](BATTLE_HAMMER_STICK_KEEP_LEFT.md).

| Service | Needed native binding | Source boundary |
| --- | --- | --- |
| SharedSetup/Start/Result/Stop | Existing ActionCommandView/battle_action_services plus exact ID3 registration and packedstate | SourceSetupAC actor/rawoption semantics; reuse existingSetupownership |
| Main | Import/export HammerCommand fields and actorflag/autobyte frompackedowner | Generalheldleft40000 atB1034; separate privateguardhistory |
| Result | FullwordB1cb8 returnedby8012a174, actualResultAC pendingbit0poll | No fabricated completion atchosenanimationframe |
| Delete | Setsstate1005only | Sourcefinish60 countdown; no TimingA-style immediate decrement |
| Display/sound | Originalgauge/cue resource serviceorretainedrequest | Originalgauge exactdraw remains AC-02; originalcues/presentation separatelyowned |

Required packed fields: stateB1c9c,resultB1cb8,countB1cb4; setupB1c94/specialdelay
B1c98, modeB1cc4, paramsN/B1cc8,T/B1ccc; suppressionB1cd0/percalllatchB1cd4;
finalsoundB1cd8; enteredB1ce8,stepcountB1cec,classificationB1cf0; sharedforced
S16B1d18; scratchB1f20..1f4b,countdownB1f4c,stageB1f50,finishB1f54,
12handlesB1f58..1f84. Bindsharedforcedcounter ratherthan asecondcounter copied
fromguard; preserve automaticactorbyte307 andactorflags gates. Fresh0 provides
ordinarymanualpath, notproofthese fields canneverchange.

Use simulation AC-managerinvocations with onegeneralpad heldsnapshot fromthe
actual native producer. Source holdsLeft ratherthanA/Battackbutton ortrigger
history. ResultAC poll derives fromactualmain result; holding4charge stages
N4/T25 and releasing in sourcewindow selects success. No-input timeout follows
sourcecountdown120/failure route; releasedearly fails. Exactvideo/hardwarecadence
isretainedboundary. Sound keys are original SFX_AC_PI1/SFX_AC_PONE1 with
[resource identities](../audio/HAMMER_CHARGE_CUE_RESOURCE_BINDINGS.md); native
soundhandles mustremainmeaningfulfororiginalonce-perstage tests. Missingaudio
is explicit retainedpresentation, notgeneratedtone/fakevoice success.

StopAC's ID3deletewrites1005; main1005setsfinish60/state1006withoutdecrement,
subsequent1006decrements→1007, **later1007clearsallfourcallbacks** andreturns1.
Export those registrationwrites; manager-main-return0 alone willnotclear them.
Pendingstate1004otherwiseawaits externalstatewriter, asoriginal.

## Original windup, input-dependent impact and source poses

_get_mario_hammer_lv80115648/4c hasoneRAWintoutputtoken: pouchGetPtr,
readS8pouch99, evtSetValue(output,level),return2. No actorinput/fallback.
Ordinarylevel1 selects exact bankb_mario poses:
M_H_1A(802ef24c),M_H_1B(802ef254),M_H_1C(802ef274).
Higherlevels2/3 andTsuranuki pointer use separateauthored poses; retain explicit
branch/keybank ratherthanfallbacksidleorplaceholderhammerart.

At8035df48 setchargeM_H_1A. Originalbrother handles onlypointer-specific badge
star/soundbranches; these arenotordinaryW80353de8 requests. Parent
ftomsec(30)→WAIT_MSEC at8035e0b4/e0c4, M_H_1B at8035e0cc,
StartAC(0) at8035e0e0, **ResultAC** at8035e0ec. Whenactualresultnotpending,
source requests level1 swingkey, setsM_H_1C at8035e150, WAIT_FRAME7.
AfterimpactSFX request, precheck!=1 executes original avoidance labels before
ordinarydamage. Otherwise GetResultAC8035e264 tests successbit2:

- Failure: CheckDamage(actor,target,part,W,F100,LW5) at8035e27c;
  AudienceDeclareACResult(W,−1); label90cleanup.
- Success: CheckDamageF20100 at8035e2b8; sourcecameramove/zoom,
  GetResultPrizeLv(target,0),GetHitPos/ACSuccessEffect,AudienceDeclareACResult.

This exact ResultAC→animation→WAIT7 sequence suppliesimpactadmission. It isnot
acomplete measurableglobaltick: charge/release, VMcredits, motion/pose/camera
callbacks andhardware sampling remainliveowners. Never scheduleimpact simply
atN4*T25+7 afterattacklaunch orbasedonlyonrendererposeframe.

## Damage/reaction and reusable original mechanics

Use `ordinaryHammerWeapon()`/`hammerWeaponPower()` and WeaponPowerServices from
source/battle_weapon_power.cpp; HPcallbacksource80189c38 usescapturedACB1cb8
bit1, S8pouch99level, unsignedMarioU300bonus and**HammerW80353de8** sixwords
1/2,2/4,3/6. Current Jump-onlypowerWordreader mustnotbe reusedunchanged;
ordinaryHammer defaults andsourceW74=30887/W78=7fb/element0/pattern0 mustbe
passedthroughsharedcalculate/apply/precheck providers. Level1bonus0 gives
H1failure/H2success beforeoriginaldefense/status/directapplication. Preserve
P99 originalfreshlevel1 adapteror realprogress producer; level0 isnotclamped
byoriginalcallback andreads beforepowertable. FP/APcost0 stillthroughsourcecost.

Shareddamage services/lifecycle from[trio contract](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md)
canreuse preservedHP/accounting/part/damageevent/deathbarriers. Spiked ordinary
pose39→TGK_D_1 (RELfilecae0),wait28→TGK_S_1; originaldamage/root/table mustbe
used, notgroundKURposehardcode. ForfreshHP2, failedH1leaves1/code10117;
successH2lethalflags30117→30717 throughoriginaldirectapplication. Ifprior
superguardalreadyleftHP1, failedH1canalsolethal throughsource10717. Source
lowbyte23 remainsnormalreaction, withlethal/secondflags preserved. No
HP==0 fabricatedvictory bypasses installedsource death/registry removal.

Afterdamage sourceStopAC→GetDamage/cappedshake→WeaponAftereffect;
ACRStart/GetResult stylishbranch (no-try usedpath) then audienceAP/report,
camera,rotationreset,walkpose/speed5/homeMovePosition,ResetFacing,
Sutorubadgechild,StartWait. Existing Jump/Headbonk stylish/barrier services
are reusecandidates; optionalsuccessful stylishgraphs/voice/effects require
actualsourceproviders orretainpending. DefaultwrapperLW14=0 ispreserved.

## Bounded next dependencies and verification obligations

Minimal newruntime work is packed ID3roundtrip/registration/heldinput/cleanup,
leveloutputhelper, selectedweapon-specificHammerpower, originalposeimports.
Reuse actualVM/move/cost/declare/precheck/damage/child/stylish contracts and
retain unboundcamera/audio/audience services. Fulldisplay/audio andmodifier/
forced/status/firstattack/badgeboundariesremain openinAC-02/AC-03/DMG-02.

Reviewable checks should exercise originalwrapper/core through currentmenu
selection; no-input andearlyrelease failure; actualstage/window success;
frontSpiked/airbornePara targetfilters; nonlethal/lethal originalhit/death
barriers; sharedforced-counter lifetime; Stop60cleanup; subsequentGoombella
handoff. Observe nativevisual poses/gauge/input prompts withoriginalassets.
These are implementation obligations, notnewclaims oforiginaltrace parity.

## Ready packed ID3 import/export and event protocol detail

Rechecked main8012a17c/534, result8012a174/8, delete80129fc0/c and existing
sharedSetup/manager/Start/Result callbacks. Allnumeric offsetsbelowhexrelativeB.
The semanticHammerCommand fields map exactly:

| HammerCommand member | Packed owner/type |
| --- | --- |
| state | 1c9c signedword |
| count | 1cb4 signedword |
| result | 1cb8 unsignedword |
| mode | 1cc4 unsignedword |
| chargeSteps,period | 1cc8,1ccc signedwords |
| suppression,suppressionLatch | 1cd0,1cd4 signedwords |
| finalSound | 1cd8 word0/1 |
| enteredCharge | 1ce8 word0/1 |
| stepCount,classification | 1cec,1cf0 signedwords |
| specialDelay | 1c98 signedword |
| forcedCounter | 1d18 signedhalf, **shareddefenseowner** |
| scratch[0..10] | 11rawwords1f20..1f48 |
| countdown,stage,finishCountdown | 1f4c,1f50,1f54 signedwords |
| handles[0..11] | 12signedwords1f58..1f84 |

HammerContext is generalheldwordB1034 (currentchannel0), setupOptionsB1c94,
actorFlags=word(actorU+**27c**, notU1c), automatic=byte(actorU+307)!=0,
difficulty=U8B1cbd. Maincapturesautomatic andsuppressionlatchoncebeforeits
same-callredispatch; portabletickimplementsit. Scratchstoresincludeexactraw
float−300 bitsc3960000 atscratch5/B1f34, +0atscratch6/B1f38,20atscratch8/B1f40.
Do notreinterpret scratchasmeteruserdataoroverwritepreservedotherfields.

Setup writesactor/option,state0,count0,result1,clearregistrationmain/display/
result/delete inthatorder, specialdelayfinalarg, cc0=0, thenloadactualID3record
[3,8012a17c,8012a174,80129fcc,80129fc0]. Displayregistration iscopiedonlywhen
U27c&10clear; main/result/deletecopiednormally. Sharedparameter/mode/difficulty/
pad/scratch resets notlistedbySetupare **not** invented. ID3initialmain0runs
onlywhenACmanagercallsit, initializeswork andsetsstate99; Setupdoesnotcall
Hammer.tick orconsumeaninputsampleitself.

Managerfirstupdatesprivatepadchannel0(asexistingnativeguardowner), thenif
mainpointer1ca0nonnull invokesselectedmain(B). ID3normalreturn1preservesmain;
externalbodywritesremainvisible. Result callback8012a174 isliterallyloadB1cb8
thenreturn; itdoesnotcallHammer.tick. EventResultAC invokesresultprovider,
discardsregisterreturn, rereadsstoredpendingbit0; pending→0,otherwisehelp0/2.
Thus repeatedeventResultACpolls mustnotadvancecharge/mainorresampleheldinput.
MaingetsitsordinaryACmanagersamplingcadence, notadditionalVMpollticks.

StartAC(0) storesstate100 only, nohelp2dueon0; laterMain100setsC120/state1000
andfallthroughprocessescurrentheldleft. StopACselectsdeletepointer1cac;
ID3deleteONLYstoresstate1005 (retainsotherwork/result/forced/history). Main1005
setsfinish60/state1006withoutdecrement; subsequent1006decrements andon<=0
sets1007, thenlater1007clearsmain1ca0,display1ca8,result1ca4,delete1cac inthat
order, withreturn1. Preserve explicitcallbackwritesinimport/export: portable
livebooleans falsecanrequestclear0; **true mustnotreinstallregistrations** or
undooriginaldisplay-gate0. Keepactualregisteredpointersunlesssourceclears.
FreshroundtripshouldnotbootstrapextraID3statefromnativeTimingAfields; fields
withsameoffsets havecommand-specificmeaning undercurrentregistration.

Source-main specialsetupoption/actorflags10 route returns0 andmayincrement
countwhennegativeRNGtestsuccess (unreachableordinaryiranddomain); portable
manager muststillhonor mainreturn0ifthatadmittedrouteeverruns. Freshflags0/
manualordinarypathdoesnotrequestRNGinthemanualcharge/releaseengine. No new
successstub/physicalinputprovider/voice/gaugecompletion isclaimedhere.

## Manager return, Spiked counter admission and recovery resource census

Rechecked BattleActionCommandManager800fa5b0/7c: privateWorkUpdatefirst;
ifinitialmainpointer0 skipsremainingbranchincludingdisplay. Otherwisecallsmain;
**mainreturn0clearsONLYB1ca0** at800fa5f4, thenre-readsdisplayB1ca8 andqueues
itifnonnull(camera8/layer1/depth900). Itdoesnotclearresult/delete/display merely
becausemainreturned0. Hammer1007 separatelyclearsfour registrationsitself,
withreturn1. Preservethese two distinctcleanupowners.

Original Spiked partdefinitionfile30d1c word44 iscounter**1**. DefaultHammer
W78counterresistanceword=**7fb**, containingmask2. OriginalearlyhazardC1row
requiresSpikeShieldbyte0 ANDresistancelacks2; thelatterisfalseforHammer.
FreshCotherhazards/statusaccumulations0thereforedo notadmitcounter. This is
sourcebitproof, notanativeclassificationbasedonweaponmenu label. HammerW74
30887 contains20000, so originalorchestrationalsoexcludesdefender-superguard
pathfromplayerHammer impact irrespectiveofpreviousstoredguardresult. Preserve
actualWcounter/specialfields andtargetpart ratherthanusingJump-onlydefaults.

No-try recovery usedposes afterM_H_1C: sourceM_S_1 pointer80422338 at8035e47c,
WAIT_FRAME10; label95 thenoriginalAP/report,camera,SetRotate0,
M_W_1 pointer80422320 at8035eb04, speed5, typedpose40(M_W_1),GetHomePos,
MovePositionN0/sound−1,arg6zero; ResetFaceDirection, childSuitoruBadgeEffect
8034f2e4, StartWaitEvent. Successfulstylishbranches useM_A_3A/M_Z_1/M_A_3B/
M_C_3 andauthored jumps/rotations; keepthosebranchdependenciesexplicit rather
thanaddoptionalposeevents totheno-trypath.

Exactlevel1soundresources decodedfromoriginalstringbytes:

| Request | Key / pointer |
| --- | --- |
| Chargevoice | SFX_VOICE_MARIO_POWER2 /802eefa4 |
| Hold | SFX_MARIO_HAMMER_HOLD1 /802ef294 |
| Swing | SFX_MARIO_HAMMER_SWING1 /802ef2dc |
| Impact | **SFX_MARIO_HAMMER_HIT_GROUND2** /802ef348 |

Soundkeynamesdonotjustifyrewritingtheauthoredlevel1branch toGROUND1.
Originalvoice/sfxkeyloaders, sample/macro/backendownershipremainseparate.

Originalusedcallback census forpendingprovideridentification (fullbodies not
newlyrecoveredjustbecausemetadataisshown):

| Callback | US address / size |
| --- | --- |
| snd_se | 80104700 /bc |
| snd_voice | 80104508 /d8 |
| evt_snd_sfxon | 8015235c /64 |
| WeaponAftereffect | 801054e8 /20c |
| ACRStart /ACRGetResult | 80106dfc /b8;80106d7c /80 |
| ACSuccessEffect | 80106fa0 /344 |
| GetResultPrizeLv | 801075c4 /9c |
| AudienceDeclareACResult | 8010749c /ac |
| AudienceDeclareAcrobatResult | 801072e4 /1b8 |
| evt_audience_acrobat_notry | 80139898 /24 |
| evt_audience_ap_recovery | 80139af4 /8 |
| InviteApInfoReport | 8010436c /4c |
| _hammer_star_effect (badgebranch) | 80114b54 /1f4 |

Defaultordinarypointerdoesnotenterbadge_hammer_star_effect branch; retain it
asfuturecallback, nota blockerforthisadmittedordinarypath. Unboundactuallyused
callbacksremainexplicitrequests orpendingowners throughoriginaleventVM.

### Used swing SFX event wrapper closure

Pending8015235c at8035e128 isexacttwoRAWoperands: authoredkey802ef2dc,
optionaloutputliteral0. [Audio wrapper follow-up](../audio/BATTLE_POSITIONAL_SFX_START_WRAPPERS.md#original-evt_snd_sfxon-raw-event-wrapper)
pinsalwayspsndSFXOncall, noGetValue, raw0discard/nooutputwrite andnonzeroRAW
outputevtSetValue ofreturnedvoiceword. Literal0usedpath canretainauthoredsound
requestwithoutinventedhandle; actualaudio remains explicitpresentationboundary.
Nonzerooutputneedsrealnativevoiceownershiporretainsourcepending. No nativeedit.

## Native fresh ordinary binding (2026-10-13)

`TrioAttackRuntime` now admits original W80353de8/wrapper80416c40/core8035d6d4.
Its unchanged script controls approach, source ftomsec windup, StartAC/ResultAC,
swing WAIT7, failure/success damage and recovery. ID3 packed import/export
retains all documented fields/shared forced counter. General pad work atBf2c
updates independently of private defense atB1d1c; Hammer reads B1034 held-left.
Source main return0 clears only main; state1007 exports its own four-pointer
clear. Source Stop3 writes1005 and retains the original60-step cleanup protocol.
Original helper80115648 reads signed pouch99; eight parameters preserve resolved
sentinels/raw mode. Power uses actual selected weapon words and pouch98/99 plus
U2ff/U300 bonuses, rather than the previous Jump-only reader/constant level.
Fresh level1 pouch initialization remains an explicit snapshot adapter.

Only fresh ground Goomba(kind1)/Spiked(kind3), original body parts/counter0/1
and W80353de8 are admitted to the native Hammer damage branch. Original W78
resistance includes2, closing the early spike counter in this scope. Flying
Para(kind2), badges/statuses/modifiers and other weapons require their providers.
Original damage scripts/death/reward/registry removal and later Goombella
handoff remain executing; no HP0 victory bypass is added.

M_H_1A/B/C come from the original ISO and draw through the existing mesh/atlas
pipeline. Circle Pad prompts adapt controls without adding story dialogue.
Raw-output0 evt_snd_sfxon calls retain original requests; nonzero-output calls
still await voice ownership. Charge cue requests use original keys with adapter
handle0; actual voice/macro playback and the original gauge/display remain open.
No generated sounds or replacement artwork are introduced.

Integration verifies no-input timeout and early-release H1/code10117/SpikedHP1,
held-left stage5 release result2/class3/H2/code30717, original Spiked death and
registry/visual removal, source60 cleanup and Goombella menu. Isolated native
capture confirms original Hammer poses, target selection, adapted prompt and
no-input recovery. It does not prove native charge-window/gauge/audio or
original/hardware timing parity. Optional stylish, teaching/mask2, successful
Jump/Headbonk, nonfresh damage and complete victory/field return remain incomplete.
