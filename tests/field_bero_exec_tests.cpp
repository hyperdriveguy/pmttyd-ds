#include "field_bero_exec.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Fixture {
    FieldBeroExec a,b;FieldBeroExec* live=&a;std::vector<unsigned> reads;
    static uint32_t word(void* user,uint32_t address){auto& f=*static_cast<Fixture*>(user);f.reads.push_back(address);return address==100?0:0x80000001;}
    static int32_t value(void* user,EventEntry& e,int32_t raw){auto& f=*static_cast<Fixture*>(user);e.arguments=999;if(raw==0){f.b.onOff(0,0x10000);f.live=&f.b;}return raw;}
    static FieldBeroExec* owner(void* user){return static_cast<Fixture*>(user)->live;}
};
int main(){
    FieldBeroExec state;assert(state.read()==0);state.onOff(0,0x80000001);assert(state.read()==0x80000001u);
    state.onOff(-5,1);assert(state.read()==0x80000000u);state.unknown();assert(!state.read() && !state.read(1));
    state.onOff(1,1);assert(state.read(1)==0 && !state.read());state.onOff(0,0x10000);assert(state.read(0x10001)==0x10000);
    state.afterInfo();assert(state.read()==0);state.unknown(8);assert(state.read(1)==0 && !state.read());
    state.onOff(0,8);assert(state.read()==8); // Source mask writes restore knowledge only for their bits.
    Fixture f;EventEntry e;e.arguments=100;FieldBeroExecEventServices s{&f,Fixture::word,Fixture::value,Fixture::owner};
    assert(fieldBeroExecOnOffEvent(e,s)==2 && f.reads==std::vector<unsigned>({100,104}));
    assert(f.a.read()==0 && f.b.read()==0x80010001u); // Captured args, fresh post-resolver owner and live bits.
}
