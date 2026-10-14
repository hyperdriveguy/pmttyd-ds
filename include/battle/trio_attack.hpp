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
#include "camera_manager.hpp"
#include "camera_letterbox.hpp"
#include "field_bero_exec.hpp"
#include "original_sequence.hpp"
#include "source_sound_flags.hpp"
#include "display_queue.hpp"
#include "shadow_work.hpp"
#include "battle_display_footer.hpp"
namespace ttyd {
bool loadTrioAttackAssets(const char* root);
std::shared_ptr<const ActorAnimationPack> fieldMarioAnimationBank();
struct TrioAttackRuntime;
struct BattlePose;
class BattlePosePool;
struct BattleCameraFrame;
struct TrioRewardIcon {std::array<float,3> position{};float scale=1;uint8_t alpha=255;bool hidden=true;uint64_t animationTick=0;};
struct TrioAudienceAudioRequest {std::optional<uint32_t> key;int32_t right=0,left=0;std::optional<int32_t> handle;};
struct TrioShadowDrawRequest {std::array<float,3> position{};float radius=0;};
struct TrioPoseDrawRequest {uint32_t part=0;int32_t camera=4,drawClass=1;std::array<float,3> depth{};};
// Executes original event data; unsupported callbacks keep their event waiting.
class TrioAttack {
public:
    std::optional<TrioPoseDrawRequest> poseDrawRequest() const;
    // Explicit native numerical/lens adapter; does not assert inherited camera4
    // roll, projection-extra or complete original field history.
    void setCameraDepthLensAdapter(float fov,float aspect,float nearPlane,float farPlane);
    void setCameraDepthSnapshot(std::optional<CameraDepthSnapshot>);
    const CameraDepthSnapshot* cameraDepthSnapshot() const;
    const DisplayQueue* displayQueue() const;
    const CameraManager* cameras() const;
    void setCameraLetterboxServices(const CameraLetterboxServices&);
    void setOriginalSequence(std::shared_ptr<OriginalSequence>);
    void setSoundFlags(std::shared_ptr<SourceSoundFlags>);
    std::optional<TrioShadowDrawRequest> shadowDrawRequest() const;
    const ShadowWork* shadowWork() const;
    const BattleCommandPalette* commandPalette() const;
    const BattleCameraFrame* cameraFrameState() const;
    bool cameraHistoryComplete() const;
    bool begin(const TrioBattleCommands&,ProfessorStory&,int16_t marioHp,std::shared_ptr<ProloguePouch> entryPouch={},uint32_t* fieldNpcBattleFlags=nullptr,FieldMarioBattleState* fieldMario=nullptr,FieldPartyRegistry* fieldParties=nullptr,const FieldPartyAppearanceServices* appearance=nullptr,std::shared_ptr<BattleSpotlightState> spotlight={},std::shared_ptr<BattleRandom> random={},std::shared_ptr<PositionalSfxRegistry> sounds={},std::shared_ptr<ShadowWork> shadows={},std::shared_ptr<BattleCommandPalette> palette={},std::shared_ptr<CameraManager> cameras={},std::shared_ptr<FieldBeroExec> bero={});
    void tick(uint64_t milliseconds,ProfessorStory&,uint32_t triggers=0,const BattlePadSample* physical=nullptr);
    bool statusWindowVisible() const;
    bool active() const;
    bool finished() const;
    int32_t commandActor() const;
    bool prepareCommands(TrioBattleCommands&,const ProfessorStory&);
    uint32_t pendingFunction() const;
    const std::vector<ProfessorServiceRequest>& requests() const;
    const std::vector<TrioAudienceAudioRequest>& audienceAudioRequests() const;
    bool readWord(uint32_t address,uint32_t& value) const;
    const BattlePose* battlePose(uint32_t part) const;
    const BattlePosePool* battlePosePool() const;
    const std::array<TrioRewardIcon,19>* rewardIcons() const;
    const PointGetEffect* rewardEffect() const;
    bool rewardPresentationActive() const;
    uint8_t endingFadeAlpha() const;
    std::shared_ptr<ProloguePouch> exportedPouch() const;
    const StageMapObjects* stageMap(bool background) const;
private:
    std::shared_ptr<TrioAttackRuntime> runtime_;
};
}
