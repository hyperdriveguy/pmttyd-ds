#pragma once
#include "battle_candidate_sampling.hpp"
#include "professor_story.hpp"
#include <optional>
namespace ttyd {
bool loadTrioCommandAssets(const char* root);
struct TrioAttackRequest {
    int32_t actor=-1,unit=-1,part=-1;
    uint32_t weapon=0;
};
// Native menu presentation around the source sampler/state11 selection rules.
// Attack dispatch retains ownership until an executing event backend accepts it.
class TrioBattleCommands {
public:
    enum class State {Closed,Command,Target,AttackReady};
    State state=State::Closed;
    unsigned command=0;
    int32_t actor=1;
    std::array<DamageUnit,7> units{};
    std::array<JumpVector,7> positions{};
    BattleCandidateBuffer candidates;
    std::optional<TrioAttackRequest> attack;
    bool begin(const ProfessorStory&,int32_t actorId=1);
    void syncPositions(const ProfessorStory&);
    void syncUnitState(const std::array<DamageUnit,7>&,const std::array<bool,7>&,const std::array<DamagePart,7>* parts=nullptr);
    void input(uint32_t triggers,uint32_t repeats);
    void chooseCommand(unsigned);
    void chooseTarget(unsigned);
    void refreshTargets();
    int count() const;
    std::array<int32_t,2> selected() const;
    std::array<int32_t,2> target(unsigned) const;
    const char* commandName(unsigned) const;
    const char* targetName(int32_t) const;
private:
    std::array<uint8_t,7> partCounts{};
    std::array<bool,7> present{};
    std::array<float,7> scales{};
    std::array<int8_t,7> facings{};
    std::array<BattlePositionUnit,7> positionUnits{};
    std::array<BattlePositionPart,7> positionParts{};
    std::array<SamplingUnit,7> samplingUnits{};
    std::array<SamplingPart,7> samplingParts{};
    CandidateSamplingServices services();
    unsigned weaponIndex() const;
};
}
