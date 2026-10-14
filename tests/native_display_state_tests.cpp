#include "native_display_state.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Fixture {NativeDisplayState state;std::vector<NativeDisplayPipeline> packets;bool ready=true;
    NativeDisplaySubmit services(){return {this,[](void* user,const NativeDisplayPipeline& p){auto& f=*static_cast<Fixture*>(user);if(!f.ready)return false;f.packets.push_back(p);return true;}};}
};
int main(){
    Fixture f;DisplayStateRequest color{DisplayGXState::ColorUpdate,{1}};
    assert(!f.state.request(color,f.services()) && f.packets.empty());
    f.state.afterDummy();f.ready=false;
    assert(!f.state.request(color,f.services()) && !f.state.state()->color);
    f.ready=true;assert(f.state.request(color,f.services()));
    assert(f.state.state()->color && !f.state.state()->alpha && !f.state.state()->depthWrite);
    assert(f.state.request({DisplayGXState::AlphaUpdate,{1}},f.services()));
    assert(f.state.state()->color && f.state.state()->alpha);
    assert(!f.state.request({DisplayGXState::ZScaleOffset,{},{{2,0}}},f.services()));
    assert(!f.state.request({DisplayGXState::AlphaCompare,{1,0,0,7,0}},f.services()));
    assert(!f.state.request({DisplayGXState::BlendMode,{2,1,0,7,0}},f.services()));

    // Every actual dispatch class, including inherited classes6/7/8, traverses
    // the real source dispatch cursor and captures the borrowed callback.
    for(int drawClass=0;drawClass<=10;++drawClass){
        Fixture p;p.state.afterDummy();DisplayQueue queue;
        assert(queue.enter(4,drawClass,0x1234,0x5678,0)==DisplayAdmission::Registered);queue.sort();
        DisplayDispatchServices services;services.user=&p;
        services.state=[](void* user,const DisplayStateRequest& request){auto& f=*static_cast<Fixture*>(user);return f.state.request(request,f.services());};
        unsigned callbacks=0;struct Call {Fixture* fixture;unsigned* count;} call{&p,&callbacks};
        // State and callback services share one fixture through this bridge.
        services.user=&call;
        services.state=[](void* user,const DisplayStateRequest& request){auto& f=*static_cast<Call*>(user)->fixture;return f.state.request(request,f.services());};
        services.callback=[](void* user,uint32_t camera,uint32_t id,uint32_t borrowed){auto& c=*static_cast<Call*>(user);++*c.count;assert(camera==4 && id==0x1234 && borrowed==0x5678);assert(c.fixture->state.state()->rasterAdmitted());return false;};
        DisplayDispatchCursor cursor;assert(!queue.dispatch(4,services,cursor));
        const auto* state=p.state.state();assert(state->color && !state->alpha);
        const auto count=p.packets.size();assert(count==(drawClass>=6 && drawClass<=8?3:7));
        assert(cursor.invoking && callbacks==1 && queue.count()==1);
        assert(!queue.dispatch(4,services,cursor) && p.packets.size()==count && callbacks==2);
        assert(p.state.restore(p.services()) && p.packets.size()==count+1);
        assert(p.state.state()->color && !p.state.state()->alpha); // Restore does not reseed dummy state.
    }
    // Unsupported early-alpha rejection with depth writes is not raster-admitted.
    f.state.request({DisplayGXState::ZCompareLocation,{1}},f.services());
    f.state.request({DisplayGXState::ZMode,{1,3,1}},f.services());
    assert(!f.state.state()->rasterAdmitted());
    assert(f.state.request({DisplayGXState::AlphaCompare,{7,0,0,7,0}},f.services()));
    assert(f.state.state()->rasterAdmitted());
}
