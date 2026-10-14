#include "display_queue.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <random>
#include <vector>
using namespace ttyd;
void sequence(const std::vector<unsigned>& keys){
    DisplayQueue q;for(unsigned i=0;i<keys.size();++i)assert(q.enter(4,0,1,i,400000.f-float(keys[i]))==DisplayAdmission::Registered);
    q.sort();std::vector<unsigned> seen;float prior=-std::numeric_limits<float>::infinity();
    for(unsigned i=0;i<q.count();++i){const auto* r=q.ordered(i);assert(r && r->key>=prior);prior=r->key;seen.push_back(r->userdata);}
    std::sort(seen.begin(),seen.end());for(unsigned i=0;i<seen.size();++i)assert(seen[i]==i);
}
struct Dispatch {
    DisplayQueue* queue=nullptr;std::vector<DisplayGXState> states;std::vector<uint32_t> borrowed;bool ready=false,append=false;
    DisplayDispatchServices services(){return {this,
        [](void* p,const DisplayStateRequest& r){auto& d=*static_cast<Dispatch*>(p);d.states.push_back(r.kind);if(r.kind==DisplayGXState::AlphaUpdate)assert(r.values[0]==0);return true;},
        [](void* p,uint32_t camera,uint32_t identity,uint32_t userdata){auto& d=*static_cast<Dispatch*>(p);assert(camera==4 && identity==7 && d.queue->current()->userdata==userdata);if(!d.ready)return false;d.borrowed.push_back(userdata);if(d.append){d.append=false;d.queue->enter(4,1,7,99,0);}return true;}};}
};
int main(){
    for(unsigned n=0;n<=7;++n){unsigned cases=1;for(unsigned i=0;i<n;++i)cases*=3;
        for(unsigned c=0;c<cases;++c){std::vector<unsigned> keys(n);unsigned code=c;for(auto& k:keys){k=code%3;code/=3;}sequence(keys);}}
    std::mt19937 rng(0x6680);for(unsigned n:{8u,39u,40u,41u,63u,64u,65u,127u,128u,255u,256u,1024u}){
        for(unsigned sample=0;sample<20;++sample){std::vector<unsigned> keys(n);for(auto& k:keys)k=rng()%13;sequence(keys);}sequence(std::vector<unsigned>(n,1));}
    DisplayQueue ties;ties.enter(4,0,1,0,399999);ties.enter(4,0,1,1,399999);ties.enter(4,0,1,2,400000);ties.sort();
    assert(ties.ordered(0)->userdata==2 && ties.ordered(1)->userdata==1 && ties.ordered(2)->userdata==0);
    DisplayQueue q;assert(q.enter(4,1,7,10,std::numeric_limits<float>::quiet_NaN())==DisplayAdmission::RejectedNaN && q.count()==0);
    assert(q.enter(4,11,7,10,0)==DisplayAdmission::Unavailable);
    q.enter(4,1,7,10,-5000);assert(q.record(0)->key==405000);
    q.enter(3,1,7,20,-5000);q.sort();Dispatch d;d.queue=&q;DisplayDispatchCursor cursor;
    assert(!q.dispatch(4,d.services(),cursor) && cursor.index==0 && d.states.size()==7 && q.current()->userdata==10);
    d.ready=true;d.append=true;assert(q.dispatch(4,d.services(),cursor) && d.borrowed==std::vector<uint32_t>({10,99}) && d.states.size()==14);
    q.reinitialize();assert(q.count()==0 && q.current()->userdata==99);
    q.enter(260,2,7,30,-5000);assert(q.record(0)->camera==4 && q.record(0)->key==595000);
    DisplayDispatchCursor full;assert(q.dispatch(260,d.services(),full) && d.borrowed.size()==2); // Full draw argument is not byte narrowed.
    q.reinitialize();for(unsigned i=0;i<1024;++i)assert(q.enter(4,0,7,i,0)==DisplayAdmission::Registered);
    assert(q.enter(4,0,7,0,0)==DisplayAdmission::Unavailable);
    DisplayQueue resumed;resumed.enter(4,1,7,10,0);DisplayDispatchCursor waiting;
    struct Mutation {DisplayQueue* q;bool ready=false;std::vector<uint32_t> seen;} mutation{&resumed};
    DisplayDispatchServices change{&mutation,[](void*,const DisplayStateRequest&){return true;},
        [](void* p,uint32_t,uint32_t,uint32_t data){auto& m=*static_cast<Mutation*>(p);m.seen.push_back(data);if(!m.ready){m.q->reinitialize();return false;}return true;}};
    assert(!resumed.dispatch(4,change,waiting));mutation.ready=true;
    assert(resumed.dispatch(4,change,waiting) && mutation.seen==std::vector<uint32_t>({10,10})); // Resume captured args even after count reset.
}
