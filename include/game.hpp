#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>
namespace ttyd {
constexpr int TickRate = 60;
// Provisional native step allowance; original movement remains to be measured.
constexpr float StepHeight = .25f;
struct Vec2 { float x = 0, z = 0; };
struct GroundContact {uint32_t attributes=0, sourceNode=0;};
struct Rect { float x0, z0, x1, z1; };
inline constexpr std::array<Rect, 4> Obstacles{{
    {-9,-4.5f,-4,-1.5f}, {3.5f,-4.5f,9,-1.5f},
    {-1.1f,-1.1f,1.1f,0.6f}, {-8.7f,2.2f,-7.2f,3.5f}
}};
inline constexpr Vec2 CrumpPosition{3, 3.5f};
inline constexpr Vec2 NpcPosition{-2.5f, 3.5f};
enum class Scene : uint8_t { Explore, Dialogue, Battle, Victory, Defeat };
enum class Phase : uint8_t { Select, Windup, Impact, EnemyWindup, EnemyImpact };
enum class Menu : uint8_t { Status, Map, Items, Settings };
enum class Sound : uint8_t { None, Confirm, Jump, Hit, Success };
// All windows are provisional prototype values, not measured TTYD behavior.
struct BattleRules {
    static constexpr int CrumpHP = 5, CrumpAttack = 1;
    static constexpr int JumpImpact = 48, JumpWindow = 5;
    static constexpr int HammerRelease = 60, HammerWindow = 6;
    static constexpr int EnemyImpact = 54, GuardWindow = 5;
};
struct Input {
    float x = 0, z = 0;
    bool confirm = false, cancel = false, jumpHeld = false;
    bool hammerHeld = false, hammerReleased = false;
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
    GroundContact groundContact,partnerContact;
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
    const char* dialogue() const;
    const char* prompt() const;
    bool near(Vec2 point, float distance = 1.5f) const;
private:
    std::vector<Vec2> followerPath;
    int jumpTick = 0, turnTick = 0;
    bool jumpAttempted = false, hammerArmed = false, guardAttempted = false;
    int dialoguePages() const;
    void move(const Input&);
    void battle(const Input&);
    void attackDone(int damage);
    void openDialogue(int kind);
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
bool groundAt(Vec2 p, float reference, float& height,GroundContact* contact=nullptr);
void clearCollision();
const std::vector<Rect>& collisionWalls();
Rect collisionBounds();
bool wallAtLevel(size_t index,float elevation);
}
