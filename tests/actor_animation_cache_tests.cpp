#include "actor_animation_cache.hpp"
#include <cassert>
#include <utility>
using namespace ttyd;
struct Stores {
    std::vector<std::pair<char,float>> values;
    ActorAnimationCacheStores callbacks(){return {this,
        [](void* u,int32_t v){static_cast<Stores*>(u)->values.emplace_back('i',float(v));},
        [](void* u,float v){static_cast<Stores*>(u)->values.emplace_back('f',v);}};}
};
int main(){
    ActorAnimationPose p;p.positions={{{10,0,0}}};p.normals={{{20,0,0}}};p.nodes={0};p.visibility={250};
    p.textures.resize(1);p.textures[0].initial=250;p.surfaces.resize(1);p.keys.resize(3);
    for(size_t i=0;i<3;++i)p.keys[i].time=float(i*10);
    auto& a=p.keys[0];a.positions={{0,16,0,0,0}};a.normals={{0,16,0,0,0}};a.nodes={{0,16,0,0,0}};
    a.visibility={{0,10,{0,0,0}}};a.textures={{0,10,{0,0,0}}};a.shifts={{.25f,.5f}};
    auto& b=p.keys[1];b.positions={{0,32,0,0,0}};b.nodes={{0,32,0,0,0}};
    b.visibility={{0,255,{0,0,0}}};b.textures={{0,255,{0,0,0}}};b.shifts={{.5f,1.f}};
    p.keys[2].positions={{0,-16,0,0,0}};
    ActorAnimationCache cache;ActorAnimationSample out;Stores stores;allocateActorAnimationCache(p,cache);
    assert(sampleActorAnimationCache(p,cache,out,0,1,.5f,-1,0,stores.callbacks()));
    assert(cache.positions[0][0]==11 && out.positions[0][0]==12 && cache.nodes[0]==1 && out.nodes[0]==2);
    assert(out.visibility[0]==4 && out.textures[0]==4 && out.surfaces[0].transform[0]==.25f);
    assert((stores.values==std::vector<std::pair<char,float>>{{'i',0},{'i',0},{'f',.5f}}));
    stores.values.clear();
    assert(sampleActorAnimationCache(p,cache,out,0,1,.25f,0,.5f,stores.callbacks()));
    assert(cache.positions[0][0]==11 && out.positions[0][0]==11.5f && out.nodes[0]==1.3125f);
    assert((stores.values==std::vector<std::pair<char,float>>{{'f',0},{'i',0},{'f',.25f}}));
    assert(sampleActorAnimationCache(p,cache,out,1,2,.5f,0,.25f));
    assert(cache.positions[0][0]==13 && out.positions[0][0]==12.5f && cache.nodes[0]==3);
    assert(out.visibility[0]==3 && out.textures[0]==3 && out.surfaces[0].transform[0]==.75f);
    const auto prior=out.positions;
    assert(sampleActorAnimationCache(p,cache,out,2,0,0,1,.5f));
    assert(cache.positions[0][0]==12 && out.positions==prior); // Source loop seam retains fractional outputs.
    assert(sampleActorAnimationCache(p,cache,out,0,1,0,2,0));
    assert(cache.positions[0][0]==11 && out.positions[0][0]==11 && out.nodes[0]==1 && out.textures[0]==4);
    assert(sampleActorAnimationCache(p,cache,out,2,2,0,0,0));
    assert(out.positions[0][0]==12 && out.nodes[0]==3); // Nonloop endpoint copies the cumulative buffers.
    p.keys[1].nodes[0].c=89;p.keys[1].nodes[0].b=127;
    assert(sampleActorAnimationCache(p,cache,out,0,1,.5f,-1,0) && out.nodes[0]==1); // OUT guard precedes IN lookup.
    p.keys[1].nodes[0].c=0;
    assert(!sampleActorAnimationCache(p,cache,out,0,1,.5f,-1,0));
    assert(!sampleActorAnimationCache(p,cache,out,-1,0,0,-1,0));
}
