#include "stage_map_objects.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
namespace ttyd {
namespace {
using M=StageMapObjects::Matrix;using P=StageMapObjects::Point;
M identity(){return {1,0,0,0,0,1,0,0,0,0,1,0};}
M product(const M& a,const M& b) {
    M out{};
    for(unsigned r=0;r<3;++r)for(unsigned c=0;c<4;++c)
        out[4*r+c]=std::fma(a[4*r],b[c],std::fma(a[4*r+1],b[4+c],std::fma(a[4*r+2],b[8+c],c==3?a[4*r+3]:0.f)));
    return out;
}
P transform(const M& m,const P& p){P out{};for(unsigned r=0;r<3;++r)out[r]=std::fma(m[4*r],p[0],std::fma(m[4*r+1],p[1],std::fma(m[4*r+2],p[2],m[4*r+3])));return out;}
M rotation(unsigned axis,float degrees) {
    const float angle=degrees*.01745329238474369f,c=std::cos(angle),s=std::sin(angle);
    M m=identity();const unsigned a=(axis+1)%3,b=(axis+2)%3;
    m[4*a+a]=c;m[4*a+b]=-s;m[4*b+a]=s;m[4*b+b]=c;return m;
}
}
bool StageMapObjects::load(const char* path) {
    FILE* f=path?std::fopen(path,"rb"):nullptr;if(!f)return false;
    char magic[8];uint32_t vertices=0,count=0;
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMMAP001",8) && std::fread(&vertices,4,1,f)==1 && vertices && vertices<=1000000 && std::fread(&count,4,1,f)==1 && count && count<=4096;
    std::vector<Object> next;std::vector<uint32_t> mapping(ok?vertices:0,UINT32_MAX);
    for(uint32_t i=0;ok && i<count;++i) {
        Object object;char name[64];uint32_t size=0;
        ok=std::fread(&object.parent,4,1,f)==1 && object.parent>=-1 && (object.parent<0 || uint32_t(object.parent)<i) && std::fread(name,64,1,f)==1 && std::memchr(name,0,64) && name[0] && std::fread(object.local.data(),4,12,f)==12 && std::fread(&size,4,1,f)==1 && size<=vertices;
        for(float value:object.local)ok=ok && std::isfinite(value);
        if(!ok)break;
        object.name=name;object.additional=identity();object.bindings.resize(size);
        for(auto& binding:object.bindings) {
            ok=std::fread(&binding.index,4,1,f)==1 && binding.index<vertices && mapping[binding.index]==UINT32_MAX && std::fread(binding.local.data(),4,3,f)==3;
            for(float value:binding.local)ok=ok && std::isfinite(value);
            if(!ok)break;
            mapping[binding.index]=i;
        }
        next.push_back(std::move(object));
    }
    if(ok)for(auto owner:mapping)if(owner==UINT32_MAX){ok=false;break;}
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
    objects=std::move(next);owners=std::move(mapping);vertexCount=vertices;dirty=true;visibilityDirty=true;visible.clear();generation=0;recalculate();return true;
}
StageMapObjects::Object* StageMapObjects::find(const std::string& name){for(auto& o:objects)if(o.name==name)return &o;return nullptr;}
const StageMapObjects::Object* StageMapObjects::find(const std::string& name)const{for(const auto& o:objects)if(o.name==name)return &o;return nullptr;}
void StageMapObjects::concatenate(Object& o,const Matrix& matrix) {
    if(!(o.flags&0x200))o.additional=identity();
    o.additional=product(o.additional,matrix);o.flags|=0x200000;o.flags|=0x200;dirty=true;
}
bool StageMapObjects::translate(const std::string& name,const Point& p) {
    auto* o=find(name);if(!o)return false;auto t=identity();for(unsigned a=0;a<3;++a)t[4*a+3]=p[a]/10.f;concatenate(*o,t);return true;
}
bool StageMapObjects::rotate(const std::string& name,const Point& angles) {
    auto* o=find(name);if(!o)return false;
    if(!(o->flags&0x200))o->additional=identity();
    // Both source helpers reset once per series, then accumulate each axis.
    for(int axis=2;axis>=0;--axis)o->additional=product(o->additional,rotation(unsigned(axis),angles[unsigned(axis)]));
    o->flags|=0x200000;o->flags|=0x200;dirty=true;return true;
}
bool StageMapObjects::flag(const std::string& name,uint32_t mask,bool on) {
    const auto* target=find(name);if(!target)return false;const auto index=uint32_t(target-objects.data());
    for(uint32_t i=index;i<objects.size();++i) {
        auto ancestor=int32_t(i);while(ancestor>=0 && uint32_t(ancestor)!=index)ancestor=objects[uint32_t(ancestor)].parent;
        if(ancestor<0)continue;
        auto& o=objects[i];const auto value=on?o.flags|mask:o.flags&~mask;
        if(value!=o.flags){if((value^o.flags)&1)visibilityDirty=true;o.flags=value;dirty=true;}
    }
    return true;
}
std::optional<uint32_t> StageMapObjects::flags(const std::string& name)const{const auto* o=find(name);return o?std::optional<uint32_t>(o->flags):std::nullopt;}
const char* StageMapObjects::root()const{return objects.empty()?nullptr:objects.front().name.c_str();}
const StageMapObjects::Matrix* StageMapObjects::world(const std::string& name)const{const auto* o=find(name);return o?&o->world:nullptr;}
void StageMapObjects::recalculate() {
    if(!dirty)return;
    M root=identity();root[0]=root[5]=root[10]=.1f; // Source model*.1 native coordinate bridge.
    for(auto& o:objects) {
        o.world=product(o.parent<0?root:objects[uint32_t(o.parent)].world,o.local);
        if(o.flags&0x200000)o.world=product(o.world,o.additional);
        o.flags&=~uint32_t(0x200);
    }
    dirty=false;++generation;
}
void StageMapObjects::apply(const Writer& write)const{if(write)for(const auto& o:objects)for(const auto& v:o.bindings)write(v.index,transform(o.world,v.local));}
void StageMapObjects::refreshVisibility()const {
    if(!visibilityDirty)return;
    visible.clear();uint32_t start=0;
    while(start<vertexCount) {
        while(start<vertexCount && (objects[owners[start]].flags&1))++start;
        auto stop=start;while(stop<vertexCount && !(objects[owners[stop]].flags&1))++stop;
        if(stop>start)visible.push_back({start,stop-start});
        start=stop;
    }
    visibilityDirty=false;
}
void StageMapObjects::visibleRanges(uint32_t first,uint32_t count,const std::function<void(uint32_t,uint32_t)>& draw)const {
    if(!draw || first>vertexCount || count>vertexCount-first)return;
    refreshVisibility();const auto end=first+count;
    for(const auto& range:visible) {
        if(range[0]>=end)break;
        const auto a=range[0]>first?range[0]:first,b=range[0]+range[1]<end?range[0]+range[1]:end;
        if(b>a)draw(a,b-a);
    }
}
}
