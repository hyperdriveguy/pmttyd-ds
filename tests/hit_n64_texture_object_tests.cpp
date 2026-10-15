#include "hit_n64_texture_object.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::map<uint32_t,uint32_t> memory;std::vector<uint32_t> reads;std::vector<SourceIconGXCall> calls;
    bool ready=false;HitN64TextureObjectServices s;HitN64TextureObjectCursor* recursive=nullptr;
    Fixture(bool loaded){memory[0x80419a40]=0x1000;memory[0x1004]=loaded?1:0;memory[0x1000]=0x9000;s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.memory[a];};
        s.construct=[](void* p,const SourceIconGXCall& q)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);if(f.recursive)assert(!resumeHitN64TextureObject(74,f.s,*f.recursive));f.calls.push_back(q);if(!f.ready)return {};return 0x5577;};}
};
int main(){
    for(bool loaded:{false,true}){
        Fixture f(loaded);HitN64TextureObjectCursor c;f.recursive=&c;
        assert(!resumeHitN64TextureObject(74,f.s,c));assert(c.active());
        const auto read=f.reads;f.memory[0x80419a40]=0x2000;f.memory[0x1004]=loaded?0:1;f.memory[0x1000]=0x9900;
        assert(!resumeHitN64TextureObject(74,f.s,c));assert(f.reads==read);
        assert(!resumeHitN64TextureObject(75,f.s,c));auto changed=f.s;changed.construct=nullptr;assert(!resumeHitN64TextureObject(74,changed,c));
        f.ready=true;const auto object=resumeHitN64TextureObject(74,f.s,c);assert(object && *object==0x5577 && !c.active());assert(f.calls.size()==3);
        for(const auto& q:f.calls){if(loaded){assert(q.function==0x80037a80 && (q.integers==std::array<uint32_t,8>{0x9000,74}));}
            else{assert(q.function==0x802b72b0 && (q.integers==std::array<uint32_t,8>{0x80419a60,1,1,0,0,0,0}));}}
        assert((read==std::vector<uint32_t>(loaded?std::vector<uint32_t>{0x80419a40,0x1004,0x1000}:std::vector<uint32_t>{0x80419a40,0x1004})));
    }
    std::cout<<"PASS whole original N64texture-object real-state branches and retained source captures\n";
}
