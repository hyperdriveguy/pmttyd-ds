#include "battle_jump_takeoff.hpp"
#include <cassert>
#include <cmath>

namespace ttyd {
namespace {
int8_t signedByte(uint32_t value) {
    const uint8_t bits=static_cast<uint8_t>(value);
    return static_cast<int8_t>(bits<128?bits:int32_t(bits)-256);
}
float sub(float a,float b) {return a-b;}
float add(float a,float b) {return a+b;}
BattlePositionUnit& unit(const TakeoffServices& s,void* work,int32_t id) {
    auto* u=s.unit(s.user,work,id);assert(u);return *u;
}
template<class Sample,class Choose>
int takeoff(const std::array<int32_t,6>& operands,const TakeoffServices& s,
            const BattleHitPositionServices& hit,Sample sample,Choose choose) {
    assert(s.captureWork && s.resolve && s.translate && s.unit && s.enemyBelong && s.sideByte && s.truncate && s.store);
    void* work=s.captureWork(s.user);
    const int32_t actorRaw=s.resolve(s.user,operands[0]);
    const int32_t targetRaw=s.resolve(s.user,operands[1]);
    const int32_t targetPart=s.resolve(s.user,operands[2]);
    const int32_t actorId=s.translate(s.user,actorRaw);auto& actor=unit(s,work,actorId);
    float currentX,ignored;getVisiblePosition(actor,&currentX,&ignored,&ignored);
    const int32_t targetId=s.translate(s.user,targetRaw);auto& target=unit(s,work,targetId);
    auto* requested=getPositionPart(target,targetPart);assert(requested);
    JumpVector position;getHomePosition(actor,&position.x,&position.y,&position.z);
    getHitPosition(target,*requested,&position.x,&ignored,&position.z,hit);
    const int8_t belong=signedByte(static_cast<uint32_t>(s.enemyBelong(s.user,actor)));
    const uint8_t side=s.sideByte(s.user,work,belong);
    const int8_t bias=signedByte(0u-static_cast<uint32_t>(side));
    TakeoffSample buffer;
    const int32_t sampled=sample(buffer,actorId,belong,bias);
    int32_t chosenUnit,chosenPart;
    choose(buffer,chosenUnit,chosenPart); // Register return is ignored.
    if(buffer.count>0) {
        chosenUnit=s.translate(s.user,chosenUnit);auto& candidate=unit(s,work,chosenUnit);
        auto* part=getPositionPart(candidate,chosenPart);assert(part);
        float chosenX;getHitPosition(candidate,*part,&chosenX,&ignored,&ignored,hit);
        if(std::fabs(sub(currentX,position.x))>std::fabs(sub(currentX,chosenX)))position.x=chosenX;
        const int32_t magnitude=requested==part?60:30;
        const int32_t factor=signedByte(s.sideByte(s.user,work,belong));
        const int32_t offset=(sampled!=0?-magnitude:magnitude)*factor;
        position.x=add(position.x,static_cast<float>(offset));
    } else {
        const int32_t factor=signedByte(s.sideByte(s.user,work,belong));
        position.x=add(position.x,static_cast<float>(60*factor));
    }
    const float values[]={position.x,position.y,position.z};
    for(unsigned i=0;i<3;++i) {
        const int32_t value=s.truncate(s.user,values[i]);s.store(s.user,operands[i+3],value);
    }
    return 2;
}
}
int jumpTakeoffPositionEvent(const std::array<int32_t,6>& operands,const TakeoffServices& s,
                             const BattleHitPositionServices& hit) {
    assert(s.sample && s.choose);
    return takeoff(operands,s,hit,[&](TakeoffSample& buffer,int32_t actor,int8_t belong,int8_t bias) {
        return s.sample(s.user,buffer,nullptr,actor,belong,0x01001240,0x01000000,bias);
    },[&](TakeoffSample& buffer,int32_t& unit,int32_t& part) {s.choose(s.user,buffer,4,unit,part);});
}
int jumpTakeoffPositionEvent(const std::array<int32_t,6>& operands,const TakeoffServices& s,
                             const BattleHitPositionServices& hit,const CandidateSamplingServices& sampling,
                             const CandidateChoiceServices& choice) {
    BattleCandidateBuffer privateBuffer;
    return takeoff(operands,s,hit,[&](TakeoffSample& view,int32_t actor,int8_t belong,int8_t bias) {
        const int32_t result=sampleBattleCandidates(privateBuffer,nullptr,actor,belong,0x01001240,0x01000000,bias,sampling);
        view.data=&privateBuffer;view.count=signedByte(privateBuffer.bytes[0xa6c]);return result;
    },[&](TakeoffSample& view,int32_t& unit,int32_t& part) {
        assert(view.data==&privateBuffer);
        struct Outputs {int32_t* unit;int32_t* part;} values{&unit,&part};
        CandidateChoiceOutputs outputs;outputs.user=&values;
        outputs.write=[](void* user,bool isUnit,int32_t value) {
            auto& out=*static_cast<Outputs*>(user);*(isUnit?out.unit:out.part)=value;
        };
        chooseBattleCandidate(privateBuffer,4,choice,outputs);
        view.count=signedByte(privateBuffer.bytes[0xa6c]);
    });
}
}
