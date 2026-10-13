#pragma once
#include "trio_battle_commands.hpp"
#include <memory>
#include "battle_defense.hpp"
#include "prologue_pouch.hpp"
#include "point_get.hpp"
namespace ttyd {
bool loadTrioAttackAssets(const char* root);
struct TrioAttackRuntime;
struct TrioRewardIcon {std::array<float,3> position{};float scale=1;uint8_t alpha=255;bool hidden=true;uint64_t animationTick=0;};
// Executes original event data; unsupported callbacks keep their event waiting.
class TrioAttack {
public:
    bool begin(const TrioBattleCommands&,ProfessorStory&,int16_t marioHp,std::shared_ptr<ProloguePouch> entryPouch={});
    void tick(uint64_t milliseconds,ProfessorStory&,uint32_t triggers=0,const BattlePadSample* physical=nullptr);
    bool active() const;
    bool finished() const;
    int32_t commandActor() const;
    bool prepareCommands(TrioBattleCommands&,const ProfessorStory&);
    uint32_t pendingFunction() const;
    const std::vector<ProfessorServiceRequest>& requests() const;
    bool readWord(uint32_t address,uint32_t& value) const;
    const std::array<TrioRewardIcon,19>* rewardIcons() const;
    const PointGetEffect* rewardEffect() const;
    bool rewardPresentationActive() const;
private:
    std::shared_ptr<TrioAttackRuntime> runtime_;
};
}
