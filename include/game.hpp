#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>
#include <optional>
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
namespace ttyd {
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
};
struct FieldSupport {
    bool found=false;
    float height=0,delta=-.01f;
    GroundContact contact;
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
    uint32_t room = 0;
    float elevation = 0;
    Vec2 position{1, 3.5f};
    int hp = 10, mushrooms = 1;
    bool crumpDefeated = false, sound = true;
    // GSW0 is a full source word. Legacy/new harness state cannot infer it.
    std::optional<int32_t> originalStory;
    uint8_t interiorMask=0; // Native same-map door state; separate from GSW0.
};
class Game {
public:
    Progress progress;
    Vec2 partner{1.4f,2.5f};
    float groundHeight = 0, partnerHeight = 0;
    float groundMetadataHeight=0; // Source +1c4; distinct from actual rootY.
    bool overheadContact=false;
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
    Scene scene = Scene::Explore;
    Phase phase = Phase::Select;
    Menu menu = Menu::Status;
    int selection = 0, enemyHP = BattleRules::CrumpHP, phaseTick = 0;
    int dialoguePage = 0, dialogueKind = 0, lastDamage = 0;
    uint64_t ticks = 0;
    bool actionSuccess = false, guarded = false, saveRequested = false;
    Sound sound = Sound::None;
    int warpRequest = -1;
    bool finishWarp();
    uint32_t warpTarget() const;
    void tick(const Input& input);
    void restore(const Progress& saved);
    void startBattle();
    const HammerCommand& hammerAction() const { return hammerCommand; }
    const TimingACommand& jumpAction() const {return jumpCommand;}
    int defenseResult() const { return defenseCommand.result; }
    int actionHelpRequest() const {return lastActionHelpRequest;}
    uint32_t damageCode() const { return lastDamageCode; }
    const DamageUnit& playerBattleState() const { return playerBattleUnit; }
    const DamageUnit& enemyBattleState() const { return enemyBattleUnit; }
    const std::vector<DamageServiceRequest>& damageRequests() const { return pendingDamageRequests; }
    const GlobalInputChannel& physicalChannel(size_t i=0) const { assert(i<4);return inputProducer.channels[i]; }
    const BattlePadWork& generalPad(size_t i=0) const {assert(i<4);return generalBattlePads[i];}
    const BattlePadWork& privatePad() const {return actionCommandPad;}
    const FieldBattleWork& fieldBattleState() const {return fieldBattleWork;}
    bool fieldControlEnabled() const {return marioFieldControl && partnerFieldControl;}
    const char* dialogue() const;
    const char* prompt() const;
    bool near(Vec2 point, float distance = 1.5f) const;
private:
    FieldBattleWork fieldBattleWork;
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
                              FieldSlope* slopes=nullptr,bool narrowCone=false);
bool loadGroundReturns(const char* path);
bool groundReturn(const GroundContact&,ReturnPoint& point);
bool groundAt(Vec2 p, float reference, float& height,GroundContact* contact=nullptr,float* slope=nullptr);
bool frontStepGround(Vec2 p,Vec2 direction,float rootY,float& height,float& slope);
bool groundedFootprint(Vec2 p,Vec2 direction,float rootY,float& height,GroundContact* contact=nullptr);
FieldSupport searchFieldSupport(Vec2 p,Vec2 direction,float rootY,float velocity,FieldSlope& slope,float depth=.11f);
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
FieldWallHit fieldWallRay(FieldWallVector origin,FieldWallVector direction,float length);
FieldShadowHit fieldShadowRay(FieldWallVector origin,FieldWallVector direction,float length);
}
