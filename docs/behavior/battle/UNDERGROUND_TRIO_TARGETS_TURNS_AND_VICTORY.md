# Underground trio target admission, ordinary turns and victory result

US G8ME01 disc0 revision0 read-only static evidence,2026-10-13. Scoped to
[OW-ROAD-01](../../requests/OW-ROAD-01.md),[AC-01](../../requests/AC-01.md),
[EVT-01](../../requests/EVT-01.md). Complements
[setup4](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md) and
[introduction/player bindings](UNDERGROUND_SETUP4_INTRO_PHASE_STAGE_AND_PLAYER_COMMANDS.md).
No original execution, image comparison, timing measurement or full native
battle equivalence claim.

## Used source masks and initial candidate derivation

The [complete sampler](BATTLE_ENEMY_CANDIDATE_SAMPLING.md) owns actual geometry,
ordered row compaction and permutation publication. Source default weapon data:

| Command | Weapon | AdmissionA(+0x64) | Attributes/occlusionC(+0x68) |
| --- | --- | --- | --- |
| MarioJump | 0x803538a8 | 0x01001260 | 0x20001004 |
| GoombellaHeadbonk | 0x80393ea8 | 0x01001260 | 0x20001004 |
| MarioHammer | 0x80353de8 | 0x01001260 | 0x21002000 |

Source A includes0x20(kind216..217 exclusion),0x40(kind218..221 exclusion),
0x200(opponent/neutral belong admission),0x1000(self exclusion),and0x01000000
UI navigation. Mask01000000 has no direct row-admission check in this body.
NoA100000/200000 bodypeer suppression branch applies to these weapons.

Initial fresh source setup: Mario/Goombella belong0, opponent Goomba/Spiked/
Paragoomba/Frankly belong1; source sidebytes from B+8+8*g+2 are+1/-1 for
player/opponent. The incomplete local decomp header's friendly alliance macros
are not authority over original setupbyte+4. Player sampler direction is+1.

Row construction first removes status27 units, unitattribute0x40 andpartswith
notTargetable0x10000. Franklypart0x00890009 therefore creates no candidate;
his noncombat coordinator still retains its battleunit/event ownership.
Ordinary trio parts have ID1. Initial currentXYZ and zero extra hit-X/bias
produce physical records in registryorderGoomba3,Spiked4,Paragoomba5.
AllsourceZ values(-10,0,10) yield lane1 because row+0x14=z+5>=-30.
Their ascending positive-direction permutation keys are30,70,110;
physical rows and permutation indices initially0,1,2. These are derived scoped
initial cases; source motion/status/side/part offsets can change later sampling.

| Command | Initial admitted unit/part pairs | Initial permutation | Defaultselectedpair |
| --- | --- | --- | --- |
| Jump/Headbonk | (3,1),(4,1),(5,1) | 0,1,2 | (3,1)Goomba |
| Hammer | (3,1) | 0 | (3,1)Goomba |

Jump/Headbonk C0x4 excludes ceilingunitattribute2; C0x1000 excludespart
jumplikeCannotTarget0x100000. TrioSpikedpartattribute9 lacks that flag.
**A spike is selectable for Jump and Headbonk.** Its counterattribute1 is
consulted afterselection by damage/counter logic, with original conditional
Frankly warning092_06(Mario) or092_07(Goombella). Do not disable this target
or redirect such a selection to Hammer automatically.

Hammer C0x2000 rejects Paragoombapartattribute0x200000. C0x01000000 then
performs same-lane frontocclusion. GoombasortX30 blocksSpiked70; the airborne
part is already rejected. AfterGoomba isremoved bystatus27, Spiked becomes the
frontHammercandidate. A surviving Paragoomba losingwings changes togrounded
parts/kind1 through sourcefall event; it thenparticipates in ordinaryground
occlusion. DefaultHammerC has no spikecounter rejection. The source occlusion
pass uses retained rows andpartflags0x40/0x400000, not nearestscreenpixel.

When actorsmove, targetrefresh must rerun the original sort-coordinate/lane/
status/currentpart rules rather than retain this initialtable. Death is a
status27 service result here, not a bareHP<=0 comparison invented forsampling.
Sourceparttargetability, counterflags and damageprecheck are distinct owners.

## Target UI confirmation and retained pair

[Weapon decision](WEAPON_MENU_SETUP_AND_DECISION.md) obtainsselectedweapon,
passes A/C/currentactor/enemybelong/side to BattleSamplingEnemy and setsselected
index0. C0x20000000 preference selects firstother-side target whereavailable.
Allthree defaultweapons allow directional UI through A0x01000000.

[State11 target input](COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md)
captures selectedunit/part before BattleSamplingEnemyUpdate. It scans fresh
permutation for the firstmatching pair; ifabsent resetsselection0. It preserves
the pair, notoldphysicalrowordinal. Forwardrepeatmask0x00080000 increments/
wrapsselection; reverserepeat0x00040000 decrements/wraps. Both requests can
executeononecall. Atcount>=2, each acceptednavigationrequests originalcursor
sound; one-targetHammerhasnosuchnavigation sound.

Canceltrigger0x0200 takespriority and omitsconfirmquery. Confirmtrigger0x0100
requestsSFX2520,stagecommandlightoff, storesstate29 beforetargetmenuend,
ORsbattleflag4, thenAChelpdisplay2. The subsequent audiencequery stillruns;
confirmation is notanearlyreturn. [Attack dispatch](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md)
ownsstate29→commandcompletion and originalselectedevent pointer.
[Selected-target query](BATTLE_SELECTED_TARGET_AND_WEAPON.md) translatesretained
selectionthroughpermutation andoutputsunit/part; no newrandom/nearestchoice.
Nativecontrollerbutton mapping mayadaptpresentation, butkeepnavigation,
selection, cancellation andconfirmation ownership.

## Minimal ordinary move-count lifecycle and source phase order

[Complete unit entry](BATTLE_UNIT_ENTRY_COMPLETE.md) initially setsbasecount
unit+0x21=1,currentcount+0x22=1. Unitphaseword+0x2c copies setupword+8:
Mario/Goombella0x04000002, trioopponents/Frankly0x04000004.
Frankly'sinit SetMaxMoveCount0 writesbase0 andcapspositivecurrentto0;
he remainsphasecoordinator andneverbecomesanordinaryattackingfoe.

[Move reset](BATTLE_MOVE_COUNTS.md): turnfirst scansallnonnullregistryunits,
incrementsbyte+0x24, resetsstate+0x20=0, copiesbase→remaining, appliesFast/
Slow, copiesremaining→previous+0x23; exactzero setsstate10. It thenassigns
priorityfromkindbytes+0x88/89 withsourceirand. Used ordinaryMario/Goombella/
trio kinds haveprioritybytes0/0, so their zerobound consumesnodraw. Frankly's
kindbyte88=0,byte89=0 likewise; count0 excludeshim fromacting despitephasehook.

[Phase actor tables](BATTLE_PHASE_TABLES.md) admitmatchingphaseword andexclude
attribute0x80000, thenstable-sort abs(homeX)ascending followedbyprioritydescending.
Atunswappedsourcehomes andnormalcounts, playerphase2 orderMario(abs100),
Goombella(abs140); enemyphase4 orderGoomba30,Spiked70,Paragoomba110.
These are scoped ordinaryderivedorders, notforcedmenuorder: originalparty/
orderchange, movementhome, statuses andruntimeprioritywriters canchangeit.
Firstphase1 stillrunsFranklyinstruction beforeplayerphase2 through the
orderedphaseevent/barrier protocol documentedinintrocontract.

[Movecontroller](BATTLE_MOVE_CONTROLLER.md) selectsremainingcount>0 actors
andcalls[actioncontroller](BATTLE_ACTION_CONTROLLER.md). Attackevent mustfinish
trackedhandles/reactions/statusannouncements beforethepostactioncountregion.
Normalstate0 decrementscount; remaining<=0 setsstate10. Sourceflags/state3/4,
multi-itemstate5..9 andspecialbypass canalterconsumption, so countmustnotbe
spent attargetconfirmation orattackanimationstart. BattlePhaseEndCheck waits
allactorentries exhausted/nonacting/status27 beforephaseprogression.
Preserve simulation/eventcadence; no renderframe-basedturnclock isestablished.

## Exact victory result1 and completion publication

Read-only DOL btlseqEnd80215234/0x1470, scopedinitialstate0x07000000
8021529c..348. It resolvesMario, capturesMario's signedbelong andrequests
BtlUnit_GetEnemyBelong(Mario). Resultselection priority is:

1. Battleflags0x40 → FieldBattleInfo.result4.
2. Elsebattleflags0x20 → result5.
3. Else ownsideconcludedword B+0xc+8*MarioBelong nonzero → result3.
4. Else opponentsideconcludedword B+0xc+8*EnemyBelong nonzero → **result1**.
5. Elsebattleflags0x10 → result4; otherwise noresultwriteinthispath.

Thusresult1 is opponent-sidevictoryafterownside-losspriority, notgeneric
battlefinished/nonzero. Exactsettercalls are802152cc/e4/308/32c/344→
BattleInformationSetResult80114508. Entireendgraph alsoowns sourcecallbacks,
reactionanimations,rewards/status/pouchimport/cleanup andcannotbesubstituted
byimmediatelywriting1 whenthreeHPcountersreachzero.

[Conclusion helper](BATTLE_WAITS_AND_CONCLUSION.md) suppliesthegroupwords.
Setupgroup0config2 requires survivingunit withattribute0x40000 andwithout
0x10000000; group1config1 requires survivingunitwithout0x20000/0x10000000;
group2config0 isalreadyconcluded. Franklyunitattribute0x12000000 includes
0x10000000 andisexcluded fromenemyconclusion, sohisHP10doesnotpreventwin.
Status27 gates actualsurvival; preservingunitpresence/eventownership afterdeath
isdistinctfromlosingcandidate/conclusioneligibility.

battleSeqEndCheck800f903c/0x10 onlyextractsbit31fromcurrentB+0xef4.
[LowercasebattleMain](BATTLE_SEQUENCE_MANAGER_BANKS_AND_OUTER_HOOK.md) sets
thatbitwhenuppercaseBattleMainreturns0 aftertopbank4; itdoesnotreadresult1.
seq_battleMain later waitsits >30callcounter beforefieldreturnsequence2 under
its sourceglobalflag routing. Existing[fieldwait](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md)
requiressequence2 ANDresultnonzero, then restorescontrols/music/fieldstate.
Loss/escape/overrideflags andfullend/reward/resultteardownremain distinctnative
providers; a result1 compatibilityadapter mustreportitsboundedimplementation.

## Remaining bounded requests

Inboxdependencies remain: actualtik4 candidate/targetrefresh/confirmationtrace,
usedphase/turnwriter composition includingYorderchange, completeHeadbonk/
Hammer/Goombellaattack/hitcounter/guard clocks, HP→status27death producer and
partfall/counterevents, completeendstate/resultoverride/loss/escaperouting,
rewards/pouch/snapshot/fieldresume lifecycle andoriginalexecutioncomparison.
Sourcegenericcontracts already recovermanylocalfunctions; thisused-data
specialization doesnotclaimtheirfullnativecomposition. No nativeedits/commit,
buildtestsororiginalruntimeobservations inthisdocumentationtask.

## Concrete fresh Jump precheck, top-spike counter and geometry importer

Requested during original268commandJump binding. Existing complete
[hit precheck](BATTLE_HIT_PRECHECK.md) is the authority for orderedRNG/status/
badge routes; [counter selector](BATTLE_COUNTERS.md) and
[damageorchestration](BATTLE_DAMAGE_ORCHESTRATION.md) are separate laterowners.
For freshtrio, noactiveevasion/status/badges, stagewordB+0x182bc=0,
weaponaccuracybyte+0x10=100 andordinaryfirsthitflags0x100:

- DefaultJump ontoGoomba,Paragoomba ORSpiked returns precheck**1**, after
  exactlyonefinalirand(100). Accuracy100stillconsumesadraw.
- Spikedpartcounter1 isNOTprecheckcounter2. Thefrontcounter2 pathwouldreturn5
  whenunresisted; itdoesnotapplytothissourcevariant.
- Precheck4 requiresInvisible18, defenderunitattributes0x18 orpartflag0x40.
  Theseareabsentfreshtrio. Do not use4torepresentordinarytopspikes.
- DefaultJump special0x35807 lacksbypass8. Charge-relatedspecial4 setsattacker
  token8beforelaterchecks. Later-hitflags0x100100 havebypass0x100000, butonly
  aftertheearlierpart/invisible/frontcounter gates; thisavoidsthefinalaccuracy
  drawonthepassedsubsequent-hitroute. Keepitsoriginalorder.

Top-spike result appearslater: C=partcounter1, attackerSpikeShieldbyte+0x302=0,
weaponresistanceword+0x78 lacks2. BattleCheckCounterearlyrow800fa6b4 selects
workcode+20=43,+24=299,+2c=31, amount+28=1, clearsordinaryadmissionwork+00,
ORsflags0x100. Its classificationreturnis8, butBattleCheckDamageignores that
registerreturnandtestsworkfields. It skipsdefenderdamage, stopsAC/help,
BattleDamageDirect(-5,attacker,null,1,0,299,0,1), schedulesdefenderhitevent31,
returns**-1**. ThusSpikedHPisunchanged andattackertakescounter1 underthis
scopedunresistedcase; Frankly'sconditionalwarningisadownstreamsourceevent.
Counterdamage/status/reactionfields mustretainunit/eventowners, notbeexpressed
as a precheckmiss. Standardno-counterpositiveHPdamage returns18 through
normaldamageorchestration; actualHPamountrequirescallback/modifier/guardpolicy.

Source partgeometry constructor DOL80129470..80129564 supplies:

| Import field | Original source | Runtime field | Initialtrio values |
| --- | --- | --- | --- |
| LocalpartXYZ | partrecordfloats+c,+10,+14 | P+24,+28,+2c | (0,0,0)allfour |
| HitbaseXYZ | partrecordfloats+18,+1c,+20 | P+170,+174,+178 | Goomba/Paragoomba(0,22,0);Spiked/Frankly(0,25,0) |
| SamplingoffsetXYZ | partrecordfloats+24,+28,+2c | P+188,+18c,+190 | (0,30,0)allfour |
| ExtrasamplingdeltaXYZ | freshpartzero thenoriginalsingle0stores | P+194,+198,+19c | (0,0,0) |
| Forwardbias | LOW16(setupWORD+18),notpartrecordfield | S16(P+1a0) | 0allfour |
| Partfacingsign | sourcegroupdirectionbyteB+a+8*belong | S8(P+bd) | -1opponents; +1Mario/Goombella |
| Unitscalar | sourceunitinitialsingle1 | F32(U+114) | 1 |

BtlUnit_GetPartsWorldPos80128b10/0xe0 usespartownerP+4ec; ordinarybranch
GetPos(owner),GetPartsPos(part), thenX=ownerX+partX*S8(owner189),
Y=ownerY+partY,Z=ownerZ+partZ. Partattribute0x10000000 insteadcopies
P+18/+1c/+20directly; notusedbyfreshtrio. Source samplerthenadds partYoffset
againonlythroughitsdocumentednon-ceilingpolicyandformsindependenthitoffsets.
Forusedlocalpart0, initialsamplehitXYZ=(30,30,-10),(70,30,0),(110,70,10);
sortbaseXYZ=(30,0,-5),(70,0,5),(110,40,15). Hitbase22/25 mustnotreplace
samplingoffset30, andforwardbias mustnotbeestimatedfrombodywidth20.

HP→death/status27eventproducer anditsbarrierlifecycle remainrequested.
CheckStatus27readsactualstoredstatusdurationbyte135; itdoesNOTdirectlytest
HP. A nativeHPcounterreachingzero alone doesnotclosethissourceownership.
The scoped no-status/precheckcase mustbeinvalidatedifbadges/status/fog/weapon
resistance/sourceinitialization differ; fullgenericprovidersremainnecessary.
