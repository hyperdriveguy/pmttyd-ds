#include "game_outer_main.hpp"
#include "original_sequence.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    uint32_t gp=0x81000000,hold=0,yields=0,inputCalls=0,seqReads=0;
    uint64_t now=0;
    std::map<uint32_t,uint32_t> words;
    std::map<uint32_t,uint8_t> bytes;
    std::vector<uint32_t> reads,calls,stores;
    std::vector<int32_t> seq{4};
    OriginalSequence sequence;
    bool useSequence=false;
    GameOuterMainServices services(){
        GameOuterMainServices s;s.user=this;
        s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);if(a==0x80415fd0)return f.gp;return f.words[a];};
        s.byte=[](void* p,uint32_t a)->std::optional<uint8_t>{auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.bytes[a];};
        s.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.stores.push_back(a);f.words[a]=v;return true;};
        s.request=[](void* p,uint32_t fn,uint32_t& pending){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(fn);
            if(fn==0x8029ccf4){++f.yields;f.gp+=0x10000;return true;}
            if(fn==0x80005ff8)++f.inputCalls;
            if(fn==0x8002e228 && f.useSequence){const bool ready=f.sequence.main({});if(!ready)pending=f.sequence.pending();return ready;}
            return fn!=f.hold;};
        s.time=[](void* p)->std::optional<uint64_t>{return static_cast<Fixture*>(p)->now;};
        s.divide=[](void*,int64_t a,uint32_t d)->std::optional<int64_t>{assert(d);return a/d;};
        s.sequence=[](void* p)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);return f.seq.at(f.seqReads++);};return s;
    }
};
int main(){
    {Fixture f;GameOuterMainCursor c;f.words[f.gp+0x10]=1;f.words[f.gp+0x10000+0x10]=1;
     assert(updateGameOuterMain(f.services(),c));assert(f.yields==2 && f.inputCalls==1);
     assert(std::vector<uint32_t>(f.calls.begin()+2,f.calls.end())==std::vector<uint32_t>(gameOuterMainCalls.begin(),gameOuterMainCalls.end()));
     assert(f.reads[0]==0x80415fd0 && f.reads[2]==0x80415fd0 && f.reads[4]==0x80415fd0);}
    {Fixture f;GameOuterMainCursor c;f.useSequence=true;f.sequence.request(4,17,23);
     assert(!updateGameOuterMain(f.services(),c) && c.pending==0x80073f20 && c.index==1);
     assert(f.sequence.state.current==4 && f.sequence.state.work[2]==17 && f.sequence.state.work[3]==23);
     assert(!updateGameOuterMain(f.services(),c) && f.inputCalls==1 && c.index==1);
     assert(f.calls.size()==3 && f.calls.back()==0x8002e228);}
    {Fixture f;GameOuterMainCursor c;f.hold=0x800368d0;
     assert(!updateGameOuterMain(f.services(),c) && c.index==11);f.hold=0;
     assert(updateGameOuterMain(f.services(),c) && f.inputCalls==1 && f.calls[12]==0x800368d0);}
    {Fixture f;GameOuterMainCursor c;f.words[f.gp]=0x1000;f.words[f.gp+0x1378]=1;f.now=0x123456789abcdef0ULL;
     assert(updateGameOuterMain(f.services(),c));assert(f.stores==std::vector<uint32_t>({0x8041e94c,0x8041e948}));
     assert(f.words[0x8041e94c]==0x9abcdef0 && f.words[0x8041e948]==0x12345678);
     assert(f.reads.back()==f.gp+0x1378);}
    for(int seconds:{-1,120,121}){Fixture f;GameOuterMainCursor c;f.words[f.gp]=0x1000;f.words[0x800000f8]=400;
     f.words[0x8041e94c]=20000;f.now=uint64_t(20000+seconds*100);f.seq={3,4};
     assert(updateGameOuterMain(f.services(),c));assert(f.seqReads==unsigned(seconds>120?2:0));
     assert(f.words[f.gp]==uint32_t(seconds>120?0x3000:0x1000));
     auto it=f.reads.end()-3-(seconds>120?2:0);assert(*it++==0x8041e94c && *it++==0x800000f8 && *it==0x8041e948);}
    {Fixture f;GameOuterMainCursor c;f.words[f.gp]=0x1000;f.bytes[f.gp+0x13cc+3]=1;f.now=5;
     assert(updateGameOuterMain(f.services(),c) && f.reads.back()==f.gp+0x13cc+3 && f.seqReads==0);}
    {Fixture f;GameOuterMainCursor c;f.words[f.gp]=0x1000;f.words[0x800000f8]=4;f.now=121;f.seq={2};
     assert(updateGameOuterMain(f.services(),c) && f.seqReads==1 && f.words[f.gp]==0x3000);}
}
