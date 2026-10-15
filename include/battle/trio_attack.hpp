#pragma once
#include "trio_battle_commands.hpp"
#include <memory>
#include <optional>
#include "battle_defense.hpp"
#include "prologue_pouch.hpp"
#include "point_get.hpp"
#include "field_party.hpp"
#include "field_party_appearance.hpp"
#include "stage_map_objects.hpp"
#include "battle_spotlight.hpp"
#include "positional_sfx_registry.hpp"
#include "camera_depth.hpp"
#include "retained_icon_frame.hpp"
#include "camera_manager.hpp"
#include "camera_letterbox.hpp"
#include "camera_letterbox_draw.hpp"
#include "battle_enemy_hp_draw.hpp"
#include "battle_enemy_hp_frame.hpp"
#include "trio_class_menu_frame_binding.hpp"
#include "trio_weapon_menu_frame_binding.hpp"
#include "battle_stock_exp_draw.hpp"
#include "battle_break_slot_draw.hpp"
#include "field_bero_exec.hpp"
#include "original_sequence.hpp"
#include "game_outer_main.hpp"
#include "physical_input.hpp"
#include "source_sound_flags.hpp"
#include "display_queue.hpp"
#include "shadow_work.hpp"
#include "shadow_display.hpp"
#include "battle_hit_registry.hpp"
#include "battle_display_footer.hpp"
#include "battle_audience_draw.hpp"
#include "battle_audience_callback.hpp"
#include "battle_stage_display.hpp"
#include "battle_audience_aux_callback.hpp"
#include "battle_part_display.hpp"
namespace ttyd {
class MessageAssets;
// Native providers retain this owner through every held source callback. Every
// external callback must become unavailable before its GPU/Game owner retires.
// Allocation/pass are native lifetime/publication IDs, not guest addresses or
// render frame numbers. begin/publish are whole external admissions: when false,
// their provider must retain any partial effects for retry under the same owner.
struct TrioEnemyHpNativeServices {
    std::shared_ptr<const void> lifetime;
    void* user=nullptr;
    SourceIconNumberServices number;
    BattleTexturePlaneServices raster;
    bool (*begin)(void*,uint64_t allocation,uint64_t pass)=nullptr;
    bool (*publish)(void*,uint64_t allocation,uint64_t pass)=nullptr;
};
class BattleAudienceIncarnationOwner;
bool loadTrioAttackAssets(const char* root);
std::shared_ptr<const ActorAnimationPack> fieldMarioAnimationBank();
std::shared_ptr<class NativeFileCache> fieldAnimationFileCache();
struct TrioAttackRuntime;
struct BattlePose;
class BattlePosePool;
struct BattleCameraFrame;
// Source presentation projections: 19 reward slots, alpha 255 fully opaque.
// Pose draw camera 4/class 1 identifies the source queued callback domain; the
// depth XYZ still needs an admitted camera snapshot to compute queue priority.
struct TrioRewardIcon {std::array<float,3> position{};float scale=1;uint8_t alpha=255;bool hidden=true;uint64_t animationTick=0;};
struct TrioAudienceAudioRequest {std::optional<uint32_t> key;int32_t right=0,left=0;std::optional<int32_t> handle;};
struct TrioShadowDrawRequest {std::array<float,3> position{};float radius=0;};
struct TrioPoseDrawRequest {uint32_t part=0;int32_t camera=4,drawClass=1;std::array<float,3> depth{};};
// Executes original event data; unsupported callbacks keep their event waiting.
class TrioAttack {
public:
    // Returned views/pointers borrow runtime-owned work unless the return type
    // explicitly shares ownership. Keep scene/runtime and real providers stable
    // across held draws; a queued request is not current callback admission.
    std::optional<TrioPoseDrawRequest> poseDrawRequest() const;
    // Explicit native numerical/lens adapter; does not assert inherited camera4
    // roll, projection-extra or complete original field history.
    void setCameraDepthLensAdapter(float fov,float aspect,float nearPlane,float farPlane);
    void setCameraDepthSnapshot(std::optional<CameraDepthSnapshot>);
    const CameraDepthSnapshot* cameraDepthSnapshot() const;
    const DisplayQueue* displayQueue() const;
    const CameraManager* cameras() const;
    // Material/texture entry points resume their own native cursors. Caller
    // owns provider identity and any linked partial effects; false with a pending
    // function token is unavailable, not a successful empty material.
    bool resumeTexture(const BattlePose&,uint32_t,const struct AnimationTextureServices&,uint32_t& pending) const;
    bool resumeMaterialTev(const struct MapTevInput&,const struct MapTevServices&,uint32_t& pending) const;
    const struct MapMaterialGroup* materialGroup() const;
    bool resumeMaterialColor(uint32_t,uint32_t,uint32_t,uint32_t,const struct MapMaterialColorServices&,uint32_t& pending) const;
    const struct MapMaterialLightWork* materialLights() const;
    bool resumeMaterialLight(uint32_t,std::array<float,3>,const struct MapMaterialLightServices&,uint32_t& pending) const;
    void setCameraLetterboxServices(const CameraLetterboxServices&);
    void setOriginalSequence(std::shared_ptr<OriginalSequence>);
    void setSoundFlags(std::shared_ptr<SourceSoundFlags>);
    bool setPhysicalInputOwner(PhysicalInputProducer*); // Stable Game owner until runtime retirement.
    bool makeOuterKey(const std::array<RawPad,4>&) const;
    bool outerMainPending() const;
    bool resumeOuterMain(const GameOuterMainServices&) const;
    bool finishingDrawPending() const;
    void resumeFinishingDraw(const CameraDrawServices&) const;
    bool resumeShadowDisplay(uint32_t camera,uint32_t userdata,const ShadowDisplayServices&) const;
    void invalidateBattleHitRegistry(); // Unknown collision/context writers must explicitly reopen their producer.
    const BattleHitRegistry* battleHitRegistry() const;
    std::optional<TrioShadowDrawRequest> shadowDrawRequest() const;
    const ShadowWork* shadowWork() const;
    const BattleCommandPalette* commandPalette() const;
    // Borrowed actual-VM lifecycle metadata, not scene/visibility admission.
    const BattleAudienceIncarnationOwner* audienceIncarnations() const;
    const BattleCameraFrame* cameraFrameState() const;
    bool cameraHistoryComplete() const;
    // Fresh creation admits only original Mario Jump/Hammer or Goombella
    // default weapon. Calling begin again reuses only the ready same-actor owner;
    // fresh-only shared/borrowed resource arguments do not rebind that runtime.
    bool begin(const TrioBattleCommands&,ProfessorStory&,int16_t marioHp,std::shared_ptr<ProloguePouch> entryPouch={},uint32_t* fieldNpcBattleFlags=nullptr,FieldMarioBattleState* fieldMario=nullptr,FieldPartyRegistry* fieldParties=nullptr,const FieldPartyAppearanceServices* appearance=nullptr,std::shared_ptr<BattleSpotlightState> spotlight={},std::shared_ptr<BattleRandom> random={},std::shared_ptr<PositionalSfxRegistry> sounds={},std::shared_ptr<ShadowWork> shadows={},std::shared_ptr<BattleCommandPalette> palette={},std::shared_ptr<CameraManager> cameras={},std::shared_ptr<FieldBeroExec> bero={},std::shared_ptr<const MessageAssets> classMessages={});
    bool hasBattleRuntime() const; // Actual existing VM owner, independent of action/finishing state.
    // Native millisecond/sample guards differ from VM visits and presentation
    // clocks. A retained Main keeps its original physical sample across holds;
    // repeated redraws do not imply another action-command simulation tick.
    void tick(uint64_t milliseconds,ProfessorStory&,uint32_t triggers=0,const BattlePadSample* physical=nullptr);
    bool statusWindowVisible() const;
    // active/finished describe native action readiness (!done/done), NOT whole
    // battle teardown. commandActor/prepareCommands additionally require actual
    // pending/phase/actor context before exposing another command.
    bool active() const;
    bool finished() const;
    int32_t commandActor() const;
    bool prepareCommands(TrioBattleCommands&,const ProfessorStory&);
    uint32_t pendingFunction() const;
    const std::vector<ProfessorServiceRequest>& requests() const;
    const std::vector<TrioAudienceAudioRequest>& audienceAudioRequests() const;
    bool readWord(uint32_t address,uint32_t& value) const;
    // Icon pass combines native owner token, sequence and completed draw bit.
    // Audience incarnation metadata is a separate lifecycle proof, not visibility
    // or a complete touch hit-test. Shared allocation alone cannot admit a draw.
    SourceIconPass sourceIconPass() const;
    std::optional<uint32_t> sourceIconWord(uint32_t) const;
    std::optional<uint32_t> sourceIconCamera(std::optional<uint32_t> index={}) const;
    bool prepareAudienceDrawing(const BattleAudienceDrawServices&) const;
    // Draw wrappers admit only the exact currently INVOKING source callback,
    // camera and userdata/part. Most bridges borrow this runtime for the call;
    // the modern HP overload additionally pins/rechecks its explicit owners.
    bool resumeAudienceCallback(uint32_t camera,uint32_t userdata,const BattleAudienceCallbackServices&) const;
    bool resumeStageDisplay(uint32_t camera,uint32_t callback,uint32_t userdata,const BattleStageDisplayServices&) const;
    bool resumeLetterboxDraw(uint32_t camera,uint32_t userdata,const CameraLetterboxDrawServices&) const;
    bool resumeEnemyHpDraw(uint32_t camera,uint32_t userdata,const BattleEnemyHpDrawServices&) const;
    bool resumeEnemyHpDraw(uint32_t camera,uint32_t userdata,const TrioEnemyHpNativeServices&) const;
    // Stable actual VM binding; linked math/footer/GPU providers must own their
    // retained child invocations. This does not activate source command Main.
    // Factories pin VM/allocation resources but their current predicates also
    // borrow this facade. Keep its address/lifetime fixed until bindings retire.
    // UI identity,whole camera Main publication and exact invoking queue record
    // are separate required proofs; no getter creates a pass or imports a GPU asset.
    std::optional<TrioClassMenuFrameBinding> classMenuFrameBinding(const BattleActDrawServices& linked) const;
    bool resumeClassMenuFrame(uint32_t camera,uint32_t userdata,BattleClassNativeFrame&,BattleClassNativeSubmission={}) const;
    std::optional<TrioWeaponMenuFrameBinding> weaponMenuFrameBinding(const BattleWeaponDrawServices& linked) const;
    bool resumeWeaponMenuFrame(uint32_t camera,uint32_t userdata,BattleWeaponNativeFrame&) const;
    std::optional<uint64_t> enemyHpAllocation() const;
    std::shared_ptr<const uint8_t> enemyHpAllocationOwner() const;
    bool enemyHpAllocationCurrent(uint64_t) const;
    bool enemyHpFrameComplete(uint64_t pass) const;
    std::optional<uint32_t> enemyHpResourceWord(uint32_t) const;
    std::optional<uint32_t> enemyHpCamera(std::optional<uint32_t> index={}) const;
    bool enemyHpMatrix(BattleAudienceMatrixRequest&) const;
    bool resumeStockExpDraw(uint32_t camera,uint32_t userdata,const SourceIconMatrixServices&) const;
    bool resumeBreakSlotDraw(uint32_t camera,uint32_t userdata,const BattleBreakSlotDrawServices&) const;
    bool resumeAudienceAux(uint32_t camera,uint32_t callback,uint32_t userdata,const BattleAudienceAuxServices&) const;
    bool resumePartDisplay(uint32_t camera,uint32_t part,const BattlePartDisplayServices&) const;
    // Pose accessor verifies guest part handle still equals native binding.
    // Other getters are borrowed projections, not permission to mutate source
    // resources or conclude the pending battle/field handoff.
    const BattlePose* battlePose(uint32_t part) const;
    const BattlePosePool* battlePosePool() const;
    const std::array<TrioRewardIcon,19>* rewardIcons() const;
    const PointGetEffect* rewardEffect() const;
    bool rewardPresentationActive() const;
    uint8_t endingFadeAlpha() const;
    // Published canonical pouch requires the ending export latch. Reward/fade
    // accessors expose presentation only; neither a visible icon nor exported
    // EXP proves completed sequence/field return.
    std::shared_ptr<ProloguePouch> exportedPouch() const;
    const StageMapObjects* stageMap(bool background) const;
private:
    // Draw/provider hooks can replace this public owner reentrantly. Active
    // dispatches must pin their runtime and revalidate allocation/pass identity;
    // shared ownership alone does not establish live source/GPU admission.
    std::shared_ptr<TrioAttackRuntime> runtime_;
};
}
