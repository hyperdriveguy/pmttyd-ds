#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>
#include "water_recovery.hpp"
#include "transition_fade.hpp"
#include "road_motion.hpp"
#include "field_jump.hpp"
#include "field_step.hpp"
#include "battle_timing_a.hpp"
#include "battle_hammer.hpp"
#include "battle_defense.hpp"
namespace ttyd {
constexpr int TickRate = 60;
// Provisional native step allowance; original movement remains to be measured.
constexpr float StepHeight = .25f;
// Original normal-Mario searchFrontWall: half20-unit width and two ray heights.
constexpr float BodyRadius = .1f;
constexpr float WalkSpeed=.01f, DashSpeed=.0225f;
// Circle Pad magnitude maps to the original clamped70-unit stick range.
constexpr float DashThreshold=55.f/70.f;
constexpr float FloorFootRadius=.075f;
constexpr float VoidThreshold=-20.f;
constexpr float WallLowerRay = .11f, WallUpperRay = .2664f;
// Original normal road-entry event restores party homing distance50 units.
constexpr float PartnerDistance = .5f;
struct Vec2 { float x = 0, z = 0; };
struct GroundContact {uint32_t attributes=0, sourceNode=0;};
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
    static constexpr int CrumpHP = 5, CrumpAttack = 1;
    static constexpr int JumpImpact = 48, JumpDifficulty = 3;
    static constexpr int EnemyImpact = 54;
};
struct Input {
    float x = 0, z = 0;
    bool confirm = false, cancel = false, jumpHeld = false;
    bool cancelHeld = false, confirmReleased = false, cancelReleased = false;
    bool left = false, right = false, menuConfirm = false;
    int touchX = -1, touchY = -1; // press edge, never repeated held input
};
struct RoadWarp { uint32_t source,target; float x,z,spawnX,spawnZ,spawnY; };
const char* roomAsset(uint32_t room);
const char* roomName(uint32_t room);
bool loadWarps(const char* path);
struct Progress {
    uint32_t room = 0;
    float elevation = 0;
    Vec2 position{1, 3.5f};
    int hp = 10, mushrooms = 1;
    bool crumpDefeated = false, sound = true;
};
class Game {
public:
    Progress progress;
    Vec2 partner{1.4f,2.5f};
    float groundHeight = 0, partnerHeight = 0;
    bool overheadContact=false;
    GroundContact groundContact,partnerContact;
    GroundContact returnContact;
    WaterRecovery water;
    TransitionFade fade;
    RoadMotion roadMotion;
    bool arriving=false;
    void beginRoadArrival();
    void cancelRoad();
    bool reloadRequested=false;
    bool airborne() const{return fieldJump.airborne || fieldStep.active();}
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
    int warpRequest = -1, warpCooldown = 0;
    bool finishWarp();
    uint32_t warpTarget() const;
    void tick(const Input& input);
    void restore(const Progress& saved);
    void startBattle();
    const HammerCommand& hammerAction() const { return hammerCommand; }
    int defenseResult() const { return defenseCommand.result; }
    const char* dialogue() const;
    const char* prompt() const;
    bool near(Vec2 point, float distance = 1.5f) const;
private:
    std::vector<Vec2> followerPath;
    FieldJump fieldJump;
    FieldStep fieldStep;
    float jumpBase=0;
    Vec2 moveDirection{1,0};
    ReturnPoint roomRespawn{1,0,3.5f};
    bool beginVoid();
    void advanceVertical(bool held);
    bool integrateVertical(bool held,float& previousY);
    void finishVertical(float previousY);
    int turnTick = 0;
    BattlePadWork generalBattlePad, actionCommandPad;
    DefenseCommand defenseCommand;
    TimingACommand jumpCommand;
    HammerCommand hammerCommand;
    // Explicit development seed; no claim about original seed/load ownership.
    BattleRandom battleRandom{1};
    int dialoguePages() const;
    void move(const Input&);
    void battle(const Input&);
    void attackDone(int damage);
    void openDialogue(int kind);
    bool beginWater();
    Vec2 arrivalStart,arrivalEnd;
    Vec2 departureOrigin;
    float departureHeight=0;
    bool departureOriginValid=false;
    bool departureDone=false;
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
bool loadGroundReturns(const char* path);
bool groundReturn(const GroundContact&,ReturnPoint& point);
bool groundAt(Vec2 p, float reference, float& height,GroundContact* contact=nullptr,float* slope=nullptr);
bool frontStepGround(Vec2 p,Vec2 direction,float rootY,float& height,float& slope);
bool groundedFootprint(Vec2 p,Vec2 direction,float rootY,float& height,GroundContact* contact=nullptr);
// Center falling floor ray (marioChkLandon): abs(v)+.0001 below, abs(v)+.05 above.
bool landingGround(Vec2 p,float worldY,float velocity,float& height,GroundContact* contact=nullptr);
bool landingFootprint(Vec2 p,Vec2 direction,float worldY,float velocity,float& height,GroundContact* contact=nullptr);
void clearCollision();
const std::vector<Rect>& collisionWalls();
Rect collisionBounds();
bool wallAtLevel(size_t index,float elevation);
}
