#include "battle_hit_registry.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {
using Point=std::array<float,3>;
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
Point difference(const Point& a,const Point& b){return {sub(a[0],b[0]),sub(a[1],b[1]),sub(a[2],b[2])};}
// Native finite SDK numeric adaptation; paired-single/rsqrt exception and
// bit parity remain open. Keep explicit single-result FMA for source gates.
float dot(const Point& a,const Point& b){return add(std::fma(a[0],b[0],mul(a[1],b[1])),mul(a[2],b[2]));}
Point cross(const Point& a,const Point& b){return {std::fma(a[1],b[2],-mul(b[1],a[2])),-std::fma(a[0],b[2],-mul(b[0],a[2])),-std::fma(a[1],b[0],-mul(b[1],a[0]))};}
float squareMag(const Point& p){return add(std::fma(p[2],p[2],mul(p[0],p[0])),mul(p[1],p[1]));}
bool finite(const Point& p){for(float v:p)if(!std::isfinite(v))return false;return true;}
template<class T> bool read(FILE* f,T& value){return std::fread(&value,sizeof(value),1,f)==1;}
bool name(FILE* f,std::string& out){char value[64];if(std::fread(value,64,1,f)!=1 || !std::memchr(value,0,64) || !value[0])return false;out=value;return true;}
}
bool BattleHitRegistry::load(const char* path){
    FILE* f=path?std::fopen(path,"rb"):nullptr;if(!f)return false;
    BattleHitRegistry next;char magic[8];uint32_t maps=0,totalTriangles=0;
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMBHIT01",8) && read(f,maps) && maps>0 && maps<=8;
    for(uint32_t map=0;ok && map<maps;++map){
        std::string mapName,rootName;uint32_t count=0;
        ok=name(f,mapName) && name(f,rootName) && read(f,count) && count<=4096 && next.objects_.size()+count<=4096;
        for(uint32_t i=0;ok && i<count;++i){
            BattleHitObject object;uint32_t triangles=0;object.map=map;
            ok=name(f,object.name) && read(f,object.node) && read(f,object.parts) && read(f,object.flags) && read(f,object.cull) && object.cull<=3 && read(f,object.attributes) && read(f,object.center) && finite(object.center) && read(f,object.radius) && std::isfinite(object.radius) && object.radius>=0 && read(f,triangles) && triangles<=32767 && totalTriangles+triangles<=100000 && (object.parts || !triangles);
            if(!ok)break;
            for(const auto& old:next.objects_)if(old.map==map && (old.node==object.node || old.name==object.name))ok=false;
            object.triangles.resize(ok?triangles:0);totalTriangles+=triangles;
            for(auto& t:object.triangles){
                ok=ok && read(f,t.vertices);for(const auto& v:t.vertices)ok=ok && finite(v);
                if(!ok)break;
                t.edges={difference(t.vertices[2],t.vertices[0]),difference(t.vertices[0],t.vertices[1]),difference(t.vertices[1],t.vertices[2])};
                t.normal=cross(t.edges[0],t.edges[1]);ok=finite(t.normal);
            }
            next.objects_.push_back(std::move(object));
        }
    }
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
    next.known_=true;next.publish();*this=std::move(next);return true;
}
bool BattleHitRegistry::flags(uint32_t i,uint16_t flags){if(i>=objects_.size())return false;objects_[i].flags=flags;return true;}
bool BattleHitRegistry::attributes(uint32_t i,uint32_t attributes){if(i>=objects_.size())return false;objects_[i].attributes=attributes;return true;}
bool BattleHitRegistry::mobjFlags(uint32_t i,std::optional<uint32_t> flags){if(i>=objects_.size())return false;objects_[i].mobjFlags=flags;return true;}
void BattleHitRegistry::publish(){
    active_.clear();for(uint32_t i=0;i<objects_.size();++i)if(objects_[i].parts && !(objects_[i].flags&0x81))active_.push_back(i);
}
ShadowRayResult BattleHitRegistry::ray(const ShadowCollisionRay& ray,ShadowCollisionHit& out,BattleHitWinner* result) const{
    if(!known_ || ray.filter!=0x8002d380 || ray.direction!=Point{0,-1,0} || !finite(ray.origin) || !std::isfinite(ray.length) || ray.length<0)return ShadowRayResult::Unavailable;
    // Unresolved tagged owners prevent admission of this complete atomic
    // native query; never guess a filter result or silently choose farther floor.
    for(auto i:active_)if((objects_[i].attributes&0x80000000u) && !objects_[i].mobjFlags)return ShadowRayResult::Unavailable;
    const float half=mul(.5f,ray.length);Point midpoint=ray.origin;midpoint[1]=add(midpoint[1],-half);
    if(!finite(midpoint))return ShadowRayResult::Unavailable;
    float length=ray.length;std::optional<BattleHitWinner> winner;Point normal{},point{};
    for(auto i:active_){
        const auto& o=objects_[i];
        if((o.attributes&0x80000000u) && (*o.mobjFlags&0x800))continue;
        const auto delta=difference(midpoint,o.center);
        const float squared=dot(delta,delta);if(!std::isfinite(squared))return ShadowRayResult::Unavailable;
        if(std::sqrt(squared)>add(half,o.radius))continue;
        for(uint32_t j=0;j<o.triangles.size();++j){
            const auto& t=o.triangles[j];const float ny=t.normal[1];
            if(o.cull==1 && !(ny>0))continue;
            const auto d0=difference(ray.origin,t.vertices[0]);const float q=dot(t.normal,d0);
            if(!std::isfinite(q))return ShadowRayResult::Unavailable;
            if(o.cull==1){if(q<0)continue;}else if(!(mul(ny,q)>0))continue;
            bool inside=true;
            for(unsigned edge=0;edge<3;++edge){
                const auto d=difference(ray.origin,t.vertices[edge]);const auto& e=t.edges[edge];
                const float k=std::fma(d[2],e[0],-mul(d[0],e[2]));
                if(!std::isfinite(k))return ShadowRayResult::Unavailable;
                if((o.cull==1?k:mul(q,k))<0){inside=false;break;}
            }
            if(!inside)continue;
            const float distance=div(-q,-ny);if(!std::isfinite(distance))return ShadowRayResult::Unavailable;
            if(distance>=length)continue;
            length=distance;point={ray.origin[0],sub(ray.origin[1],distance),ray.origin[2]};
            normal=q>=0?t.normal:Point{mul(t.normal[0],-1),mul(t.normal[1],-1),mul(t.normal[2],-1)};
            winner=BattleHitWinner{i,j,distance};
        }
    }
    if(!winner)return ShadowRayResult::Miss;
    const float magnitude=std::sqrt(squareMag(normal));if(!(magnitude>0) || !std::isfinite(magnitude))return ShadowRayResult::Unavailable;
    const float inverse=div(1,magnitude);for(float& v:normal)v=mul(v,inverse);
    out={point,normal};if(result)*result=*winner;return ShadowRayResult::Hit;
}
}
