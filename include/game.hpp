#pragma once
#include "item_selection_presentation.hpp"
#include "goomba_hand_in.hpp"
#include "field_party.hpp"
#include <array>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>
#include <optional>
#include "prologue_pouch.hpp"
#include <unordered_map>
#include "npc_collision.hpp"
#include "npc_wander.hpp"
#include "water_recovery.hpp"
#include "transition_fade.hpp"
#include "road_motion.hpp"
#include "field_jump.hpp"
#include "field_step.hpp"
#include "field_idle.hpp"
#include "field_key.hpp"
#include "field_shadow.hpp"
#include "battle_timing_a.hpp"
#include "battle_hammer.hpp"
#include "battle_defense.hpp"
#include "battle_damage.hpp"
#include "battle_damage_application.hpp"
#include "battle_weapon_power.hpp"
#include "physical_input.hpp"
#include "field_battle.hpp"
#include "field_movement.hpp"
#include "field_wall.hpp"
#include "field_road.hpp"
#include "field_door.hpp"
#include "field_camera_state.hpp"
#include "field_camera_shift.hpp"
#include "camera_road_state.hpp"
#include "field_camera_events.hpp"
#include "mario_presentation.hpp"
#include "partner_presentation.hpp"
#include "partner_follow.hpp"
#include "harbor_story.hpp"
#include "opening_field_departure.hpp"
#include "harbor_encounter.hpp"
#include "harbor_tutorial.hpp"
#include "professor_story.hpp"
#include "trio_battle_commands.hpp"
#include "trio_attack.hpp"
#include "field_actors.hpp"
#include "field_object_collision.hpp"
#include "field_save_objects.hpp"
namespace ttyd {
struct NpcTalkPlayer;
constexpr int TickRate = 60;
// Provisional native step allowance; original movement remains to be measured.
constexpr float StepHeight = .25f;
// Original normal-Mario searchFrontWall: half20-unit width and two ray heights.
constexpr float BodyRadius = .1f;
constexpr float WalkSpeed=FieldMovement::Walk, DashSpeed=FieldMovement::Dash;
// Circle Pad magnitude maps to the original clamped70-unit stick range.
constexpr float DashThreshold=FieldMovement::DashStick;
constexpr float FloorFootRadius=.075f;
constexpr float VoidThreshold=-20.f;
constexpr float WallLowerRay = .11f, WallUpperRay = .2664f;
// Original normal road-entry event restores party homing distance50 units.
constexpr float PartnerDistance = .5f;
struct Vec2 { float x = 0, z = 0; };
struct GroundContact {uint32_t attributes=0, sourceNode=0;};
struct FieldHeadQuery {
    bool contact=false;
    float correction=0,clearance=-.01f;
    GroundContact node;
    uint32_t objectKey=0; // Dynamic owner namespace; never a static REL node offset.
};
struct FieldSupport {
    bool found=false;
    float height=0,delta=-.01f;
    GroundContact contact;
    uint32_t objectKey=0;
    CameraPoint point; // Accepted Q1000 ray XYZ in native units, not root XYZ.
};
struct ReturnPoint {float x=0,y=0,z=0;};
struct Rect { float x0, z0, x1, z1; };
inline constexpr std::array<Rect, 4> Obstacles{{
    {-9,-4.5f,-4,-1.5f}, {3.5f,-4.5f,9,-1.5f},
    {-1.1f,-1.1f,1.1f,0.6f}, {-8.7f,2.2f,-7.2f,3.5f}
}};
inline constexpr Vec2 CrumpPosition{3, 3.5f};
inline constexpr Vec2 NpcPosition{-2.5f, 3.5f};
enum class Scene : uint8_t { Explore, Dialogue, Battle, Victory, Defeat, FieldDefeat };
enum class Phase : uint8_t { Select, Windup, Impact, EnemyWindup, EnemyImpact };
enum class Menu : uint8_t { Status, Map, Items, Settings };
enum class Sound : uint8_t { None, Confirm, Jump, Hit, Success };
// Encounter scheduling/damage and tutorial/weapon eligibility remain provisional.
// Jump's selected difficulty remains a harness choice. Hammer uses the ordinary
// default weapon's recovered N4/period25/difficulty3, not a tutorial binding.
struct BattleRules {
    static constexpr int CrumpHP = 5;
    static constexpr int JumpImpact = 48, JumpDifficulty = 3;
    static constexpr int EnemyImpact = 54;
};
struct Input {
    float x = 0, z = 0;
    bool confirm = false, cancel = false, jumpHeld = false;
    bool cancelHeld = false, confirmReleased = false, cancelReleased = false;
    bool physicalSnapshot=false;uint16_t physicalButtons=0;
    bool left = false, right = false, menuConfirm = false;
    int touchX = -1, touchY = -1; // press edge, never repeated held input
};
// Latest hit's original service requests, recorded for unbound presentation/VM
// integration. This trace is not an executing or automatically drained event queue.
struct DamageServiceRequest {
    enum class Kind {Effect,Sound,Notice,HitEvent} kind=Kind::Effect;
    DamageUnit* unit=nullptr;
    DamageEffect effect{DamageEffectKind::Hit,{}};
    DamagePosition position;
    const char* soundKey=nullptr;
    DamageNotice notice{DamageNoticeKind::ActionCount};
    uint32_t code=0;
};
struct RoadWarp {
    uint32_t source,target;
    float x,z,spawnX,spawnZ,spawnY;
    uint32_t sourceNode,arrivalNode,direction,arrivalDirection;
    float centerY;
};
const char* roomAsset(uint32_t room);
const char* roomName(uint32_t room);
bool loadWarps(const char* path);
struct Progress {
    FieldActorTalkState originalNpcTalkState;
    std::shared_ptr<ProloguePouch> originalPouch; // Legacy checkpoints omit it; v8 retains packed original storage.
    uint32_t room = 0;
    float elevation = 0;
    Vec2 position{1, 3.5f};
    int hp = 10, mushrooms = 1;
    bool crumpDefeated = false, sound = true;
    // GSW0 is a full source word. Legacy/new harness state cannot infer it.
    std::optional<int32_t> originalStory;
    bool originalFlag1207=false; // Original GSF: Frankly has departed through the east pipe.
    bool originalFlag233=false,originalFlag236=false; // Save-block first/post-story notices.
    uint8_t originalSaveMode=0; // Original GSW4, independent of native file existence.
    uint8_t interiorMask=0; // Native same-map door state; separate from GSW0.
};
struct FieldNpcInstance {
    NpcSideBody body;
    NpcWanderWork wander;
    NpcWanderActor movement;
    NpcPresentation presentation;
    NpcPoseClock poseClock;
    std::string pose;
    uint64_t placement=0,seen=0,clock=0,poseAt=0;
    int posePhase=0;
    uint32_t secondaryFlags=0;
    float savedYaw=0;
    bool scripted=false,initialized=false,paused=false,presentationReady=false;
};
struct FieldNpcSoundRequest {std::string key;uint32_t mask=0;FieldWallVector root;};
class Game {
public:
    explicit Game(bool initializeAnimation=true) {
        if(initializeAnimation)if(auto bank=fieldMarioAnimationBank())fieldParties.initializeMario(fieldMarioBattle,std::move(bank),0);
    }
    Progress progress;
    OpeningFieldDeparture openingDeparture;
    bool retainOpeningDeparture(const OpeningHarborScene&);
    Vec2 partner{1.4f,2.5f};
    float groundHeight = 0, partnerHeight = 0;
    float groundMetadataHeight=0; // Source +1c4; distinct from actual rootY.
    bool overheadContact=false;
    uint32_t headObjectKey=0,groundObjectKey=0,wallObjectKey=0;
    GroundContact headContact;
    float headClearance=-.01f;
    bool fieldHeadNarrowCone=false; // Original separate +4 flag owner remains unbound.
    GroundContact groundContact,partnerContact;
    GroundContact previousGroundContact; // Source +1ec, separate from current support.
    CameraScreenCache fieldScreen; // Mario +168/+16c/+170; refreshed after MoveMain.
    MarioCameraTarget marioCamera;
    FieldCameraTarget fieldCamera;
    FieldCameraFrame fieldCameraFrame;
    CameraPoint marioCameraCollision,fieldCameraCollisionTarget; // Source retained f8/fc/100 and camera ac/b0/b4 (engine units).
    FieldCameraShift fieldCameraShift;
    bool fieldCameraCandidateActive=false;
    float fieldCameraCandidateShiftRate=0; // Previous candidate +5c, not current selected road.
    uint32_t fieldCameraFlags=0,fieldCameraDisplayFlags=0;
    bool fieldCameraViewAdapter=true; // Automatic view; false lets tests/events provide a frame.
    CameraRoadWork cameraRoadWork;
    FieldCameraEvents fieldCameraEvents;
    MarioPresentation marioPresentation;
    PartnerPresentation partnerPresentation;
    PartnerFollow partnerFollow;
    HarborStory harborStory;
    HarborEncounter harborEncounter;
    HarborTutorial harborTutorial;
    size_t harborCameraCursor=0;
    bool harborResume=false;
    bool beginHarborEncounter(bool resume);
    ProfessorStory professorStory;
    PartnerFollow professorFollow;
    void updateTemporaryProfessor();
    size_t professorCameraCursor=0;
    FieldActorTalk fieldActorTalk;
    std::shared_ptr<ItemSelectionPresentation> itemSelection;
    std::shared_ptr<FieldSaveObjects> saveObjects; // Stable key-service lifetime; replaced on room transactions.
    bool saveLocked() const{return saveObjects && saveObjects->locked();}
    void updateSaveObjects();
    FieldIdleWork fieldIdle;
    // Native partner ability/status services are still adapters. These explicit
    // inputs allow source eligibility instead of collapsing it to input enabled.
    FieldKeyPartner fieldKeyPartner;
    int fieldChristineStatus=0,fieldBomheiKeyCheck=0,fieldYoshiStatus=0,fieldHeldItem=0;
    int fieldShipChanging=0,fieldRollEnabled=1,fieldSlitEnabled=1;
    std::vector<int> fieldIdleSounds; // Original source IDs; macro playback unbound.
    // Basic source camera-event services. Coordinates are engine units;
    // EVT operand/address marshaling remains a separate runtime binding.
    bool setCameraTarget(CameraPoint xyz,float percent,int32_t durationMs,uint8_t curve);
    bool releaseCameraTarget(int32_t durationMs,uint8_t curve);
    bool setCameraView(CameraPoint eye,CameraPoint at,int32_t durationMs,uint8_t curve);
    bool releaseCameraView(int32_t durationMs,uint8_t curve);
    bool fieldCameraAuthored=false;
    bool fieldAirDampingBypass=false,fieldKeysOff=false;
    GroundContact wallContact;
    GroundContact roadPassContact; // Source +1f0, separate from blocking/front metadata.
    float wallContactDistance=10; // Source wrapper initial1000, native units*.01.
    uint32_t fieldViewFlags=0; // Original init clears+14; later view writers unbound.
    GroundContact returnContact;
    WaterRecovery water;
    TransitionFade fade;
    RoadMotion roadMotion;
    RoadCaseAdmission roadAdmission;
    FieldDoorState fieldDoor;
    bool roadCasesEnabled=true;
    RoadPolicy roadPolicy=RoadPolicy::Generic;
    RoadCallback roadCallback=RoadCallback::None;
    uint32_t fieldSystemFlags=0; // Native system-word adapter, not saved-story inference.
    bool arriving=false;
    void beginRoadArrival();
    void cancelRoad();
    bool reloadRequested=false;
    bool airborne() const{return fieldJump.airborne || fieldStep.active();}
    const FieldJump& fieldAirMotion() const{return fieldJump;}
    const FieldMovement& fieldTravel() const{return fieldMovement;}
    bool recoveryReturn(ReturnPoint& point) const;
    float cameraZ = 3.5f, cameraHeight = 0;
    float cameraX = 1, jumpHeight = 0, facing = 1, turnScale = 1;
    bool trioBattleActive=false;ProfessorStory trioBattleIntro;TrioBattleCommands trioCommands;TrioAttack trioAttack;size_t trioBattleCameraCursor=0;
    std::shared_ptr<BattleSpotlightState> battleSpotlight=std::make_shared<BattleSpotlightState>();
    std::shared_ptr<BattleRandom> trioRandom=std::make_shared<BattleRandom>(1); // Source boot seed; field/story interleaving remains incomplete.
    const ProfessorStory& storyPresentation() const {return trioBattleActive?trioBattleIntro:professorStory;}
    Scene scene = Scene::Explore;
    Phase phase = Phase::Select;
    Menu menu = Menu::Status;
    int selection = 0, enemyHP = BattleRules::CrumpHP, phaseTick = 0;
    int dialoguePage = 0, dialogueKind = 0, lastDamage = 0;
    uint64_t ticks = 0;
    bool actionSuccess = false, guarded = false, saveRequested = false;
    Sound sound = Sound::None;
    int pipeRequest=-1;
    uint32_t pipeTarget() const;
    bool tryFieldPipe(int8_t mainY);
    bool finishPipeWarp();
    void cancelPipeWarp();
    int warpRequest = -1;
    bool finishWarp();
    uint32_t warpTarget() const;
    void tick(const Input& input);
    void restore(const Progress& saved);
    bool checkpointGround(float& height);
    void startBattle();
    const HammerCommand& hammerAction() const { return hammerCommand; }
    const TimingACommand& jumpAction() const {return jumpCommand;}
    int defenseResult() const { return defenseCommand.result; }
    int actionHelpRequest() const {return lastActionHelpRequest;}
    uint32_t damageCode() const { return lastDamageCode; }
    const DamageUnit& playerBattleState() const { return playerBattleUnit; }
    const DamageUnit& enemyBattleState() const { return enemyBattleUnit; }
    const std::vector<DamageServiceRequest>& damageRequests() const { return pendingDamageRequests; }
    void publishPhysicalInput(const std::array<RawPad,4>& pads) {inputProducer.update(pads,TickRate,false);}
    void retainPhysicalInput(const Game& previous) {inputProducer=previous.inputProducer;}
    const GlobalInputChannel& physicalChannel(size_t i=0) const { assert(i<4);return inputProducer.channels[i]; }
    const BattlePadWork& generalPad(size_t i=0) const {assert(i<4);return generalBattlePads[i];}
    const BattlePadWork& privatePad() const {return actionCommandPad;}
    const FieldBattleWork& fieldBattleState() const {return fieldBattleWork;}
    const FieldMarioBattleState& fieldMarioBattleState() const {return fieldMarioBattle;}
    void setFieldMarioCameraId(uint32_t id) {fieldMarioBattle.cameraId=id;} // Source raw word134 store; viewport ownership is separate.
    const FieldPartyRegistry& fieldPartyState() const {return fieldParties;}
    FieldPartyAppearanceServices fieldPartyAppearanceServices();
    bool fieldControlEnabled() const {return marioFieldControl && partnerFieldControl;}
    const char* dialogue() const;
    const char* prompt() const;
    bool near(Vec2 point, float distance = 1.5f) const;
    const FieldActorRecord* nearbyFieldActor() const;
    uint32_t fieldNpcContact() const{return fieldNpcHit;}
    const FieldNpcInstance* fieldNpcState(uint32_t key) const;
    bool fieldNpcTalkActive() const {return goombaHandIn.active() || fieldActorTalk.message.active();}
    const std::vector<FieldNpcSoundRequest>& fieldNpcSounds() const{return npcSoundRequests;}
private:
    NpcTalkPlayer fieldNpcTalkPlayer() const;
    FieldActorTalkInventory fieldNpcTalkInventory(const FieldActorRecord&) const;
    bool fieldNpcTalkBound(const FieldActorRecord&) const;
    GoombaHandIn goombaHandIn;
    bool fieldNpcTalkCandidateAlive() const;
    uint32_t npcTalkCandidate=0;
    uint32_t npcTalkPending=0;
    bool npcTalkWaiting=false;
    uint64_t npcTalkGeneration=0;
    void updateFieldNpcTalk(const Input&);
    uint32_t npcRandomSeed=1; // Deterministic native seed; original shared seed/interleaving remains unbound.
    uint32_t fieldNpcEventCandidate=0;
    std::vector<FieldNpcSoundRequest> npcSoundRequests;
    void synchronizeFieldNpcs();
    void updateFieldNpcEvents();
    void updateFieldNpcCandidates();
    void updateFieldNpcPresentation();
    std::unordered_map<uint32_t,FieldNpcInstance> fieldNpcs;
    uint64_t fieldNpcGeneration=0;
    uint32_t fieldNpcRoom=~0u;
    uint32_t fieldNpcHit=0;
    void updateFieldNpcCollision();
    void checkFieldRoadCases();
    FieldBattleWork fieldBattleWork;
    FieldMarioBattleState fieldMarioBattle;
    FieldPartyRegistry fieldParties;
    int32_t fieldSequence=2;
    bool marioFieldControl=true,partnerFieldControl=true;
    // Recorded requests: original prepare/post/audio internals remain unbound.
    bool fieldPrepareRequested=false,fieldPostRequested=false,fieldBgmStopRequested=false;
    uint32_t fieldSoundFlags=0;
    FieldBattleServices fieldBattleServices();
    bool returnFromBattle(int32_t harnessResult);
    void updatePartner();
    FieldJump fieldJump;
    FieldMovement fieldMovement;
    FieldStep fieldStep;
    float fieldVerticalVelocity=0;
    float fieldInputMagnitude=0;
    float jumpBase=0;
    Vec2 moveDirection{1,0};
    ReturnPoint roomRespawn{1,0,3.5f};
    bool beginVoid();
    void advanceVertical(bool held,bool dispatch=true);
    bool integrateVertical(bool held,float& previousY,bool update=true);
    FieldHeadQuery queryOverhead(float rootY,float velocity,float extension);
    std::vector<FieldObjectTriangle> fieldObjectTriangles;
    void refreshFieldObjects();
    uint64_t fieldObjectGeneration=UINT64_MAX;
    uint32_t fieldObjectRoom=UINT32_MAX,fieldObjectInterior=0,fieldObjectSystem=0;
    std::optional<int32_t> fieldObjectStory;
    void dispatchFieldAir();
    void registerRoadCases(bool enabled);
    bool resolveRoadCallbacks(const RoadWarp& road);
    void rebaseFieldCamera();
    void beginFieldCameraTick();
    void updateMarioPresentation();
    bool fieldIdleWasActive=false;
    void publishFieldCameraTarget();
    void cacheFieldScreen();
    void publishFieldCameraCollision(const FieldSupport& support);
    bool fieldCameraInitialized=false;
    int8_t retainedFieldMainX=0;
    uint64_t fieldCameraSimulationTicks=0; // Process clock bridge survives room/Save restore.
    void finishVertical(float previousY);
    int turnTick = 0;
    std::array<BattlePadWork,4> generalBattlePads;
    BattlePadWork actionCommandPad;
    PhysicalInputProducer inputProducer;
    DefenseCommand defenseCommand;
    uint32_t lastDamageCode=0;
    int lastActionHelpRequest=0; // Records service requests; original help UI remains unbound.
    DamageUnit playerBattleUnit, enemyBattleUnit;
    DamagePart playerBattlePart, enemyBattlePart;
    uint8_t jumpLevelFixture=1,hammerLevelFixture=1; // Pouch/loadout initialization is not yet bound.
    std::vector<DamageServiceRequest> pendingDamageRequests;
    TimingACommand jumpCommand;
    HammerCommand hammerCommand;
    // Explicit development seed; no claim about original seed/load ownership.
    BattleRandom battleRandom{1};
    int dialoguePages() const;
    void move(const Input&,bool launched=false);
    void battle(const Input&);
    void attackDone();
    void applyBattleDamage(bool enemyTarget,int32_t hp,int32_t fp,uint32_t code);
    void openDialogue(int kind);
    bool beginWater();
    Vec2 arrivalStart,arrivalEnd;
    Vec2 departureOrigin;
    float departureHeight=0;
    bool departureOriginValid=false;
    bool departureDone=false;
    bool departureParentStarted=false;
    int departurePause=0;
};
struct CollisionSnapshot;
// Stages a room's collision; destruction restores the previous data unless committed.
class CollisionTransition {
public:
    CollisionTransition();
    ~CollisionTransition();
    CollisionTransition(const CollisionTransition&)=delete;
    CollisionTransition& operator=(const CollisionTransition&)=delete;
    bool load(const char* prefix);
    void commit();
private:
    std::unique_ptr<CollisionSnapshot> previous;
    bool staged=false,committed=false;
};
bool walkable(Vec2 p,float elevation=0);
bool loadCollision(const char* path);
bool loadGround(const char* path);
bool loadHead(const char* path);
bool headCollision(Vec2 p,Vec2 direction,float worldY,float velocity,float& clampedRoot);
bool headCollision(Vec2 p,Vec2 direction,float worldY,float velocity,float& clampedRoot,float rayExtension);
FieldHeadQuery searchFieldHead(Vec2 p,Vec2 direction,float rootY,float velocity,float extension,
                              FieldSlope* slopes=nullptr,bool narrowCone=false,
                              const std::vector<FieldObjectTriangle>* objects=nullptr);
bool loadGroundReturns(const char* path);
bool groundReturn(const GroundContact&,ReturnPoint& point);
bool groundAt(Vec2 p, float reference, float& height,GroundContact* contact=nullptr,float* slope=nullptr,
              const std::vector<FieldObjectTriangle>* objects=nullptr,uint32_t* objectKey=nullptr);
bool frontStepGround(Vec2 p,Vec2 direction,float rootY,float& height,float& slope);
bool groundedFootprint(Vec2 p,Vec2 direction,float rootY,float& height,GroundContact* contact=nullptr);
FieldSupport searchFieldSupport(Vec2 p,Vec2 direction,float rootY,float velocity,FieldSlope& slope,float depth=.11f,
                                const std::vector<FieldObjectTriangle>* objects=nullptr);
// Center adapter; native marioChkLandon uses the shared five-foot support service.
bool landingGround(Vec2 p,float worldY,float velocity,float& height,GroundContact* contact=nullptr);
bool landingFootprint(Vec2 p,Vec2 direction,float worldY,float velocity,float& height,GroundContact* contact=nullptr);
bool roadStartGround(Vec2 p,float sourceY,float& height);
void clearCollision();
const std::vector<Rect>& collisionWalls();
Rect collisionBounds();
bool wallAtLevel(size_t index,float elevation);
// Uses complete source-winding .head mesh; legacy fixture wall segments are a
// separate geometry adaptation when a complete mesh is unavailable.
FieldWallHit fieldWallRay(FieldWallVector origin,FieldWallVector direction,float length,
                          const std::vector<FieldObjectTriangle>* objects=nullptr);
FieldShadowHit fieldShadowRay(FieldWallVector origin,FieldWallVector direction,float length);
}
