#include "actor_animation_cache.hpp"
#include <optional>
namespace ttyd {
namespace {
// Source binary32 store boundaries; the tangent table uses native double tan,
// not measured Gekko/libm parity. .0625 is1/16 compressed delta scale.
float rounded(float v){volatile float result=v;return result;}
std::optional<float> cubic(const ActorSparseUpdate& r,float t,float h){
    if(r.b<-89 || r.b>88)return {}; // Original adjacent-table domain needs its provider.
    //178 tangent samples indexed by signed angle+89 cover -89..88 degrees.
    // c outside(-89,89) is the source no-cubic gate; b must index the table.
    static const auto table=[](){
        std::array<float,178> values{};
        for(int i=0;i<178;++i)values[i]=float(std::tan(double(rounded(float(i-89)*.017453288659453392f))));
        return values;
    }();
    const float t2=rounded(t*t),t3=rounded(t*t2);
    const float h10=rounded(t-std::fma(2.f,t2,-t3));
    const float h01=std::fma(-2.f,t3,rounded(3.f*t2));
    float x=rounded(h10*rounded(h*table[size_t(int(r.b)+89)]));
    x=std::fma(h01,rounded(.0625f*float(r.a)),x);
    x=std::fma(rounded(t3-t2),rounded(h*table[size_t(int(r.c)+89)]),x);
    return x;
}
void triples(std::vector<ActorVector>& values,const std::vector<ActorSparseUpdate>& records,float scale){
    for(const auto& r:records){auto& p=values[r.index];p[0]=std::fma(float(r.a),scale,p[0]);p[1]=std::fma(float(r.b),scale,p[1]);p[2]=std::fma(float(r.c),scale,p[2]);}
}
}
void allocateActorAnimationCache(const ActorAnimationPose& p,ActorAnimationCache& c){
    c.positions.resize(p.positions.size());c.normals.resize(p.normals.size());
    c.visibility.resize(p.visibility.size());c.nodes.resize(p.nodes.size());
    c.textures.resize(p.textures.size());c.surfaces.resize(p.surfaces.size());
}
// Cumulative discrete/current-key cache and fractional output are distinct.
// previous -1 or backward current rebuilds bases; forward current applies only
// newly crossed keys; same-key backwards fraction publishes a zero phase first.
// Validated sparse indices belong to the pack; this is not an untrusted loader.
bool sampleActorAnimationCache(const ActorAnimationPose& p,ActorAnimationCache& c,ActorAnimationSample& out,
    int32_t current,int32_t next,float fraction,int32_t previous,float previousFraction,ActorAnimationCacheStores stores){
    if(current<0 || next<0 || size_t(current)>=p.keys.size() || size_t(next)>=p.keys.size() || previous<-1)return false;
    auto index=[&](int32_t v){if(stores.index)stores.index(stores.user,v);};
    auto phase=[&](float v){if(stores.fraction)stores.fraction(stores.user,v);};
    const bool reset=previous==-1 || previous>current;
    if(reset){
        c.positions=p.positions;c.normals=p.normals;c.visibility=p.visibility;c.nodes=p.nodes;
        c.textures.resize(p.textures.size());for(size_t i=0;i<p.textures.size();++i)c.textures[i]=uint8_t(p.textures[i].initial);
        c.surfaces=p.surfaces;
        if(previous==-1)index(0);
    }else if(c.positions.size()!=p.positions.size() || c.normals.size()!=p.normals.size() || c.visibility.size()!=p.visibility.size() || c.nodes.size()!=p.nodes.size() || c.textures.size()!=p.textures.size() || c.surfaces.size()!=p.surfaces.size())return false;
    if(previous==current && previousFraction>fraction)phase(0);
    const size_t first=reset?0:size_t(previous+1);
    for(size_t k=first;k<=size_t(current);++k){
        const auto& key=p.keys[k];
        triples(c.positions,key.positions,.0625f);triples(c.normals,key.normals,.0625f);
        for(const auto& r:key.visibility)c.visibility[r.index]=uint8_t(c.visibility[r.index]+r.delta);
        for(const auto& r:key.nodes)c.nodes[r.index]=std::fma(.0625f,float(r.a),c.nodes[r.index]);
        for(size_t j=0;j<key.textures.size();++j){
            const auto& r=key.textures[j];c.textures[r.index]=uint8_t(c.textures[r.index]+r.delta);
            if(j<key.shifts.size()){auto& s=c.surfaces[r.index].transform;s[0]=rounded(s[0]+key.shifts[j].s);s[1]=rounded(s[1]+key.shifts[j].t);}
        }
    }
    // Across loop seam current>next retains prior continuous output rather
    // than interpolating key0. Discrete visibility/texture/surface cache still
    // publishes below. Do not overwrite retained seam output with raw bases.
    if(current<=next){
        out.positions=c.positions;out.normals=c.normals;out.nodes=c.nodes;
        if(current<next){
            const auto& key=p.keys[size_t(next)];const float scale=rounded(.0625f*fraction);
            triples(out.positions,key.positions,scale);triples(out.normals,key.normals,scale);
            const float duration=rounded(p.keys[size_t(next)].time-p.keys[size_t(current)].time);
            for(const auto& r:key.nodes){
                if(r.c>=89 || r.c<=-89)continue;
                const auto delta=cubic(r,fraction,duration);if(!delta)return false;
                out.nodes[r.index]=rounded(out.nodes[r.index]+*delta);
            }
        }
    }
    // These native views consume the actual discrete cache buffers.
    out.visibility=c.visibility;out.textures=c.textures;out.surfaces=c.surfaces;
    out.last=size_t(current);out.next=size_t(next);out.fraction=fraction;
    index(current);phase(fraction);return true;
}
}
