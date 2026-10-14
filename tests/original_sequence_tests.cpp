#include "original_sequence.hpp"
#include "game.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Fixture {
    OriginalSequence owner;
    unsigned syncs=0;
    bool syncReady=true,initReady=true,mainReady=true,exitReady=true;
    bool mutateSync=false,mutateExit=false,mutateInit=false;
    std::vector<uint32_t> calls;
    uint32_t flags=0x80000003;
    OriginalSequenceServices services(){
        OriginalSequenceServices s;s.user=this;
        s.drawSync=[](void* p){auto& f=*static_cast<Fixture*>(p);++f.syncs;
            if(!f.syncReady)return false;
            if(f.mutateSync)f.owner.state.current=3;
            return true;};
        s.callback=[](void* p,uint32_t id,OriginalSequenceWork& w){
            auto& f=*static_cast<Fixture*>(p);f.calls.push_back(id);
            if(id==0x80007390 || id==0x80008724){
                if(!f.exitReady)return false;
                if(id==0x80007390)originalFieldSequenceExit(f.flags);
                if(f.mutateExit){f.owner.state.current=1;f.owner.request(4,17,23);}
                return true;
            }
            if(id==0x80073f20){
                assert(f.owner.state.current==4 && w[0]==4 && w[1]==0);
                for(unsigned i=4;i<8;++i)assert(w[i]==0);
                if(!f.initReady)return false;
                if(f.mutateInit)f.owner.state.current=6;
                return true;
            }
            return f.mainReady;
        };return s;
    }
};
int main(){
    OriginalSequence boot;
    assert(boot.state.current==-1 && boot.state.requested==-1 && boot.state.previous==-1);
    for(auto w:boot.state.work)assert(w==0);
    boot.request(4,0x12345678,0xffffffff);
    assert(boot.state.current==-1 && boot.state.requested==4);
    assert(!boot.main({}) && boot.pending()==0x80073f20);
    assert(boot.state.current==4 && boot.state.previous==-1);
    assert(boot.state.work[2]==0x12345678 && boot.state.work[3]==0xffffffff);

    Fixture f;f.owner.importNativeField();f.owner.state.work.fill(0xfeed);
    f.owner.request(4,11,13);f.syncReady=false;
    assert(!f.owner.main(f.services()) && f.owner.pending()==0x80005ce4);
    assert(f.owner.state.current==2 && f.owner.state.work[7]==0xfeed && f.calls.empty());
    f.syncReady=true;f.exitReady=false;
    assert(!f.owner.main(f.services()) && f.owner.pending()==0x80007390);
    assert(f.syncs==2 && f.owner.state.current==2);
    f.exitReady=true;f.initReady=false;
    assert(!f.owner.main(f.services()) && f.owner.pending()==0x80073f20);
    assert(f.syncs==2 && f.flags==0x80000001 && f.owner.state.previous==2);
    assert(f.owner.state.current==4 && f.owner.state.work[2]==11 && f.owner.state.work[3]==13);
    // A new request during held Init cannot change its captured callback/args.
    f.owner.request(5,99,101);f.owner.state.work[6]=0;
    assert(!f.owner.main(f.services()) && f.syncs==2);
    assert(f.owner.state.current==4 && f.owner.state.work[2]==11);
    f.initReady=true;f.mainReady=false;
    assert(!f.owner.main(f.services()) && f.owner.pending()==0x80073b24);
    const auto calls=f.calls.size();
    f.owner.state.current=6;f.mainReady=true;
    assert(f.owner.main(f.services()) && f.owner.pending()==0);
    assert(f.calls.size()==calls+1 && f.calls.back()==0x80073b24); // Captured Main survives wait.

    Fixture fresh;fresh.owner.importNativeField();fresh.owner.request(4,1,2);
    fresh.mutateSync=fresh.mutateExit=fresh.mutateInit=true;
    assert(fresh.owner.main(fresh.services()));
    assert(fresh.calls==std::vector<uint32_t>({0x80008724,0x80073f20,0x800f6f68}));
    assert(fresh.owner.state.previous==1 && fresh.owner.state.current==6);
    assert(fresh.owner.state.work[2]==17 && fresh.owner.state.work[3]==23);

    // Equal requests skip sync/Exit/Init and select fresh current Main only.
    Fixture equal;equal.owner.importNativeField();
    assert(equal.owner.main(equal.services()) && equal.syncs==0);
    assert(equal.calls==std::vector<uint32_t>({0x800073a4}));

    // All recovered rows; source -1/out-of-table accesses remain unavailable.
    const uint32_t inits[]={0x800874b0,0x80009d30,0x8000753c,0x8000874c,
        0x80073f20,0x80078a14,0x800f72b8,0x80252f8c};
    for(int id=0;id<8;++id){OriginalSequence o;o.request(id,0,0);
        assert(!o.main({}) && o.pending()==inits[id] && o.state.current==id);}
    OriginalSequence unmapped;unmapped.request(9,7,8);
    assert(!unmapped.main({}) && unmapped.state.current==9 && unmapped.state.work[0]==0);
    unmapped.request(4,0,0);assert(!unmapped.main({}) && unmapped.state.current==9);
    OriginalSequence invalidEqual;assert(!invalidEqual.main({}) && invalidEqual.state.current==-1);

    Game game;auto retained=game.originalSequence;
    retained->request(4,0,0);assert(retained->state.current==2);
    game.restore(game.progress);
    assert(game.originalSequence==retained && retained->state.current==2);
    assert(retained->state.requested==2 && retained->pending()==0);
}
