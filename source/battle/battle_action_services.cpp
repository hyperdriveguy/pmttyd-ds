#include "battle_action_services.hpp"
#include <cassert>
#include <algorithm>

namespace ttyd {
namespace {
int32_t resolve(const ActionValueServices& s,int32_t value) {assert(s.resolve);return s.resolve(s.user,value);}
int output(const ActionValueServices& s,int32_t destination,int32_t value) {
    assert(s.store);s.store(s.user,destination,value);return 2;
}
int32_t signedWord(uint32_t value) {
    return value<=0x7fffffff?static_cast<int32_t>(value):static_cast<int32_t>(int64_t(value)-0x100000000LL);
}
void help(ActionCommandView& w,int32_t type) {assert(w.help);w.help(w.user,type);}
}
int setActionParameters(ActionCommandView& w,const std::array<int32_t,8>& operands,const ActionValueServices& s) {
    assert(w.parameters);
    for(unsigned i=0;i<8;++i) {
        const int32_t value=resolve(s,operands[i]);
        if(value!=-250000000)(*w.parameters)[i]=value;
    }
    return 2;
}
int setActionFlags(ActionCommandView& w,uint32_t rawMask) {assert(w.setupFlags);*w.setupFlags=rawMask;return 2;}
int startActionCommand(ActionCommandView& w,int32_t rawOn,const ActionValueServices& s) {
    const int32_t on=resolve(s,rawOn);assert(w.state);*w.state=100;
    if(on) {assert(w.actorFlags);if(!(*w.actorFlags&0x10))help(w,2);}
    return 2;
}
int32_t evaluateActionCommand(ActionCommandView& w) {
    if(w.evaluate)return w.evaluate(w.user);
    assert(w.result);return static_cast<int32_t>(*w.result&2);
}
int resultActionCommandEvent(ActionCommandView& w) {
    evaluateActionCommand(w);assert(w.result);
    if(*w.result&1)return 0;
    help(w,0);return 2;
}
int readActionResult(ActionCommandView& w,int32_t destination,const ActionValueServices& s) {
    assert(w.result);return output(s,destination,signedWord(*w.result));
}
int readActionCount(ActionCommandView& w,int32_t destination,const ActionValueServices& s) {
    assert(w.count);return output(s,destination,*w.count);
}
int setTimingOptions(ActionCommandView& w,int32_t rawOn,uint32_t rawMask,const ActionValueServices& s) {
    const int32_t on=resolve(s,rawOn);assert(w.timingOptions);
    if(on)*w.timingOptions|=rawMask;else *w.timingOptions&=~rawMask;
    return 2;
}
int readTimingSuccessWindow(ActionCommandView& w,int32_t destination,const ActionValueServices& s) {
    assert(w.successWindow);const int32_t window=w.successWindow(w.user);return output(s,destination,window);
}
int stopActionCommandEvent(ActionCommandView& w) {
    if(w.remove)w.remove(w.user);
    help(w,0);return 2;
}
int32_t actionPrizeLevel(void* work,const ActionPrizeActor* actor,int32_t input,const ActionPrizeServices& s) {
    if(actor && (actor->lastHitCode&0xff)==30)return -1;
    assert(s.prizeByte);
    const uint32_t result=static_cast<uint32_t>(input)+s.prizeByte(s.user,work)-1u;
    return std::clamp<int32_t>(signedWord(result),0,6);
}
int readActionPrize(const std::array<int32_t,3>& operands,const ActionPrizeServices& s) {
    assert(s.captureWork && s.resolve && s.store);
    void* work=s.captureWork(s.user);
    const int32_t actorId=s.resolve(s.user,operands[0]);ActionPrizeActor* actor=nullptr;
    if(actorId!=-1 && actorId!=-5) {
        assert(s.translate && s.unit);actor=s.unit(s.user,s.translate(s.user,actorId));
    }
    const int32_t input=s.resolve(s.user,operands[1]);
    const int32_t result=actionPrizeLevel(work,actor,input,s);
    s.store(s.user,operands[2],result);return 2;
}
}
