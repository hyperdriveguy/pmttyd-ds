#include "hit_n64_gpu.hpp"
#include "battle_audience_aux_callback.hpp"
#include "battle_stage_display.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Effect=0x2000,Payload=0x3000,Camera=0x4000;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
bool same(const SourceIconGXCall& a,const SourceIconGXCall& b){return a.function==b.function && a.integers==b.integers && a.reals==b.reals && a.matrix==b.matrix;}
struct Fixture {
    std::shared_ptr<const HitN64TextureResources> resource;bool alive=true,resident=true;
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
    std::vector<SourceIconGXCall> calls;std::vector<BattleAudienceMatrixRequest> math;
    std::vector<std::array<uint16_t,18>> geometry;
    int holdCall=-1,holdMath=-1,holdIndices=-1,holdTruncate=-1;unsigned held=0,truncations=0;
    HitN64DrawServices s;BattleAudienceAuxServices adapter;BattleActMatrix* mutateBase=nullptr;
    HitN64DrawCursor* recursive=nullptr;bool recursed=false;
    Fixture(uint32_t kind=0,float progress=.25f,uint32_t age=1){
        words[Effect+0xc]=Payload;words[Effect+8]=2;words[Payload]=kind;words[Payload+0x28]=bits(1.5f);
        words[Payload+4]=bits(1);words[Payload+8]=bits(2);words[Payload+12]=bits(3);
        words[Payload+0x2c]=0x123;words[Payload+0x34]=age;words[Payload+0x38]=bits(progress);
        const uint8_t color[]={255,128,64,32,16,8};for(unsigned i=0;i<6;++i)bytes[Payload+0x3c+i]=color[i];
        bytes[0x804198ac]=255;bytes[0x804198ad]=255;
        for(unsigned i=0;i<3;++i)words[0x802fb0ac+i*4]=bits(1);
        words[0x802fb0b8]=0x43300000;words[0x802fb0bc]=0x80000000;
        const float constants[]={5,4,1.f/32,1.f/128,0,96,.01745329238474369f,256,1.f/256,1};
        for(unsigned i=0;i<10;++i)words[0x80425458+i*4]=bits(constants[i]);
        words[Camera+0x114]=bits(0);
        const BattleActMatrix view={1,0,0,0,0,1,0,0,0,0,1,0};for(unsigned i=0;i<12;++i)words[Camera+0x11c+4*i]=bits(view[i]);
        words[Payload+0x44+4]=bits(10);words[Payload+0x44+8]=0;words[Payload+0x44+12]=0;
        words[Payload+0x44+0x1c]=bits(360);words[Payload+0x44+0x24]=bits(17);
        bindAudienceAuxMathAdapter(adapter);s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->words[a];};
        s.byte=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);auto i=f.bytes.find(a);if(i!=f.bytes.end())return i->second;auto b=f.resource?f.resource->byte(a):std::optional<uint8_t>{};assert(b);return *b;};
        s.multiply=[](void*,float a,float b){volatile float n=a*b;return n;};
        s.truncate=[](void* p,float n)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);
            if(int(f.truncations)==f.holdTruncate && f.held++<2)return {};
            ++f.truncations;if(!std::isfinite(n) || n<-2147483648.f || n>=2147483648.f)return {};return int32_t(n);};
        s.matrix=[](void* p,BattleAudienceMatrixRequest& r){auto& f=*static_cast<Fixture*>(p);
            if(int(f.math.size())==f.holdMath && f.held++<2)return false;
            if(f.mutateBase){(*f.mutateBase)[3]=37;f.mutateBase=nullptr;}
            f.math.push_back(r);
            if(r.function==0x8029e1c0){BattleStageMath q;q.function=r.function;q.axis=r.xyz;q.angle=r.radians;if(!nativeBattleStageMath(q))return false;r.output=q.matrix;return true;}
            return f.adapter.matrix(f.adapter.user,r);};
        s.call=[](void* p,const SourceIconGXCall& q)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);
            if(f.recursive && !f.recursed){f.recursed=true;assert(!resumeHitN64Draw(4,Effect,f.s,*f.recursive));}
            if(int(f.calls.size())==f.holdCall && f.held++<2)return {};
            f.calls.push_back(q);if(q.function==0x8000f2fc)return Camera;
            if(q.function==0x801d78b0){assert(q.integers[0]==74);return 0x7788;} // Explicit recorded fixture object.
            return 0;};
        s.indices=[](void* p,const std::array<uint16_t,18>& v){auto& f=*static_cast<Fixture*>(p);
            if(int(f.geometry.size())==f.holdIndices && f.held++<2)return false;
            f.geometry.push_back(v);return true;};
    }
    void unusedRun(HitN64DrawCursor& c){unsigned waits=0;while(!resumeHitN64Draw(4,Effect,s,c))assert(++waits<=3);assert(!c.active());}
    void equal(const Fixture& base){assert(calls.size()==base.calls.size() && math.size()==base.math.size() && geometry==base.geometry && truncations==base.truncations);for(size_t i=0;i<calls.size();++i)assert(same(calls[i],base.calls[i]));for(size_t i=0;i<math.size();++i){assert(math[i].function==base.math[i].function && math[i].axis==base.math[i].axis && math[i].xyz==base.math[i].xyz && math[i].radians==base.math[i].radians && math[i].left==base.math[i].left && math[i].right==base.math[i].right);}}
};
HitN64GpuLifetime lifetime(Fixture&f){HitN64GpuLifetime l;l.owner=std::make_shared<int>(1);l.user=&f;
 l.current=[](void*p){return static_cast<Fixture*>(p)->alive;};
 l.textureCurrent=[](void*p,const std::shared_ptr<const HitN64TextureResources>&r,uint64_t g){auto&f=*static_cast<Fixture*>(p);return f.resident&&r==f.resource&&g==17;};return l;}
void finish(HitN64GpuCapture&c,Fixture&f){unsigned waits=0;while(!c.resume(4,Effect,f.s))assert(++waits<4);}
}
int main(int argc,char**argv){
 HitN64GpuCapture unavailable;Fixture noResource;assert(!unavailable.begin({},17,lifetime(noResource)));
 NativeHitN64Gpu gpu;assert(!gpu.prepare(nullptr,0,1)&&!gpu.beginFrame());gpu.release();
 const char* path=argc>1?argv[1]:"romfs/hit-n64-texture74.bin";
 auto resource=HitN64TextureResources::load(path);if(!resource){std::cout<<"PASS missing Hit GPU ownership; private source fixture skipped\n";return 0;}
 Fixture f;f.resource=resource;HitN64GpuCapture capture;assert(capture.begin(resource,17,lifetime(f)));finish(capture,f);
 auto b=capture.publication();assert(b&&b->current()&&b->packets().size()==3);
 const auto&p=b->packets()[0];assert(p.color0==0xff804023&&p.color1==0x20100840);assert(p.vertices[0].position[0]==-16);
 assert(p.vertices[0].uv0[0]==p.vertices[0].uv1[0]);assert(std::fabs(p.vertices[0].uv0[1]-p.vertices[0].uv1[1]+.75f)<1e-6f);
 assert(b->packets()[0].matrix!=b->packets()[1].matrix&&b->packets()[1].matrix!=b->packets()[2].matrix);
 NativeHitN64Texture native;HitN64NativeRaster raster;assert(!gpu.preflight(*b,native,raster)&&!gpu.draw(*b,native,raster));
 f.alive=false;assert(!b->current()&&!capture.publication()&&!capture.resume(4,Effect,f.s));f.alive=true;assert(!b->current()&&!capture.publication());f.resident=false;assert(!b->current());
 for(unsigned which=0;which<3;++which){Fixture held;held.resource=resource;held.holdCall=int(which);HitN64GpuCapture c;assert(c.begin(resource,17,lifetime(held)));finish(c,held);assert(c.publication()->packets().size()==3);}
 for(unsigned i=0;i<f.math.size();++i){Fixture held;held.resource=resource;held.holdMath=int(i);HitN64GpuCapture c;assert(c.begin(resource,17,lifetime(held)));finish(c,held);assert(c.publication()->packets().size()==3);}
 Fixture alias;alias.resource=resource;alias.bytes[0x803a0458+56]^=1;HitN64GpuCapture a;assert(a.begin(resource,17,lifetime(alias)));assert(!a.resume(4,Effect,alias.s)&&!a.publication()&&a.pending()==0x801d76a0);alias.bytes.erase(0x803a0458+56);finish(a,alias);assert(a.publication()->packets().size()==3);
 Fixture loss;loss.resource=resource;loss.holdCall=2;HitN64GpuCapture l;assert(l.begin(resource,17,lifetime(loss)));assert(!l.resume(4,Effect,loss.s));assert(!l.begin(resource,17,lifetime(loss)));loss.alive=false;assert(!l.resume(4,Effect,loss.s)&&!l.publication());l.discard();
 Fixture collapsed(0,.9f);collapsed.resource=resource;HitN64GpuCapture cc;assert(cc.begin(resource,17,lifetime(collapsed)));finish(cc,collapsed);for(const auto&q:cc.publication()->packets())for(const auto&v:q.vertices)assert(v.uv1[0]==0&&v.uv1[1]==1);
 Fixture invisible(0,1.f);invisible.resource=resource;HitN64GpuCapture empty;assert(empty.begin(resource,17,lifetime(invisible)));finish(empty,invisible);assert(empty.publication()->packets().empty());
 std::cout<<"PASS original Hit ordered capture, all matrix/query holds, source alias rejection, lifetime/residency loss, three passes and native-unavailable rejection\n";
}
