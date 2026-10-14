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
namespace ttyd {
bool loadTrioAttackAssets(const char* root);
std::shared_ptr<const ActorAnimationPack> fieldMarioAnimationBank();
struct TrioAttackRuntime;
struct TrioRewardIcon {std::array<float,3> position{};float scale=1;uint8_t alpha=255;bool hidden=true;uint64_t animationTick=0;};
struct TrioAudienceAudioRequest {std::optional<uint32_t> key;int32_t right=0,left=0;std::optional<int32_t> handle;};
// Executes original event data; unsupported callbacks keep their event waiting.
class TrioAttack {
public:
    bool begin(const TrioBattleCommands&,ProfessorStory&,int16_t marioHp,std::shared_ptr<ProloguePouch> entryPouch={},uint32_t* fieldNpcBattleFlags=nullptr,FieldMarioBattleState* fieldMario=nullptr,FieldPartyRegistry* fieldParties=nullptr,const FieldPartyAppearanceServices* appearance=nullptr,std::shared_ptr<BattleSpotlightState> spotlight={},std::shared_ptr<BattleRandom> random={});
    void tick(uint64_t milliseconds,ProfessorStory&,uint32_t triggers=0,const BattlePadSample* physical=nullptr);
    bool active() const;
    bool finished() const;
    int32_t commandActor() const;
    bool prepareCommands(TrioBattleCommands&,const ProfessorStory&);
    uint32_t pendingFunction() const;
    const std::vector<ProfessorServiceRequest>& requests() const;
    const std::vector<TrioAudienceAudioRequest>& audienceAudioRequests() const;
    bool readWord(uint32_t address,uint32_t& value) const;
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
