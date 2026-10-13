#pragma once
#include "trio_battle_commands.hpp"
#include <memory>
namespace ttyd {
bool loadTrioAttackAssets(const char* root);
struct TrioAttackRuntime;
// Executes original event data; unsupported callbacks keep their event waiting.
class TrioAttack {
public:
    bool begin(const TrioBattleCommands&,ProfessorStory&,int16_t marioHp);
    void tick(uint64_t milliseconds,ProfessorStory&,uint32_t triggers=0);
    bool active() const;
    bool finished() const;
    int32_t commandActor() const;
    bool prepareCommands(TrioBattleCommands&,const ProfessorStory&);
    uint32_t pendingFunction() const;
    const std::vector<ProfessorServiceRequest>& requests() const;
    bool readWord(uint32_t address,uint32_t& value) const;
private:
    std::shared_ptr<TrioAttackRuntime> runtime_;
};
}
