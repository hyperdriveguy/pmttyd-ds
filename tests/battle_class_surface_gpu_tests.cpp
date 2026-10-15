#include "battle_class_surface_gpu.hpp"
#include "battle_class_backend.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <unistd.h>
using namespace ttyd;
namespace {
void u32(std::ofstream&f,uint32_t n){for(unsigned i=0;i<4;++i)f.put(char(n>>(8*i)));}
void f32(std::ofstream&f,float v){uint32_t n;std::memcpy(&n,&v,4);u32(f,n);}
void region(std::ofstream&f,unsigned w,unsigned h,unsigned x,unsigned y,unsigned aw,unsigned ah){u32(f,w);u32(f,h);f32(f,float(x)/aw);f32(f,1-float(y)/ah);f32(f,float(x+w)/aw);f32(f,1-float(y+h)/ah);}
void metadata(const std::string&root,bool changed=false){
 std::ofstream a(root+"/battle-class-surfaces.bin",std::ios::binary);a.write("PMBCLS01",8);u32(a,2);u32(a,0);region(a,40,40,changed?3:2,2,256,128);u32(a,1);region(a,128,80,46,2,256,128);
 std::ofstream b(root+"/battle-class-icons.bin",std::ios::binary);b.write("PMICON1",8);u32(b,10);for(unsigned i=0;i<10;++i){u32(b,334+i);u32(b,0);u32(b,1);u32(b,0);region(b,48,48,i%4*52+2,i/4*52+2,256,256);}
}
struct Import {
 unsigned calls=0,releases=0;int absent=-1,bad=-1,mutation=-1;bool alias=false,reenter=false;int retireDuring=-1;
 int ids[2]{};BattleClassSurfaceGpuResource* owner=nullptr;BattleClassSurfaceLease watched;
 std::string root;
 BattleClassSurfaceTextureServices services(){return {this,
 [](void*p,uint32_t slot,const char*path)->std::optional<BattleClassSurfaceImport>{auto&f=*static_cast<Import*>(p);++f.calls;assert(slot<2);assert(std::string(path)==f.root+(slot?"/battle-class-icons.t3x":"/battle-class-surfaces.t3x"));
  if(f.reenter){assert(!f.owner->load(f.root.c_str(),f.services()));}
  if(int(slot)==f.retireDuring)f.owner->retire();
  if(int(slot)==f.absent)return {};
  const unsigned h=slot?256:128;BattleClassSurfaceImport r{f.alias?&f.ids[0]:&f.ids[slot],256,h,0,1,256,h,false,{0,1,1,0}};
  if(int(slot)==f.bad)switch(f.mutation){
   case 0:r.identity=nullptr;break;case 1:--r.width;break;case 2:--r.height;break;case 3:r.format=3;break;case 4:r.count=2;break;case 5:--r.subWidth;break;case 6:--r.subHeight;break;case 7:r.rotated=true;break;case 8:r.uv[0]=.01f;break;case 9:r.uv[1]=.99f;break;case 10:r.uv[2]=.99f;break;case 11:r.uv[3]=.01f;break;case 12:r.uv[0]=std::numeric_limits<float>::quiet_NaN();break;default:assert(false);}
  return r;},
 [](void*p,uint32_t slot,const void*id){auto&f=*static_cast<Import*>(p);assert(id==(f.alias?&f.ids[0]:&f.ids[slot]));assert(!f.owner->ready());if(f.watched.generation)assert(!f.watched.current());++f.releases;}};}
};
}
int main(){
 char name[]="/tmp/ttyd-class-gpu-XXXXXX";const char*dir=mkdtemp(name);assert(dir);const std::string root=dir;metadata(root);
 NativeBattleClassSurfaceGpu native;assert(!native.load(root.c_str())&&!native.resource().ready());native.release();
 Import f;f.root=root;BattleClassSurfaceGpuResource r;f.owner=&r;assert(!r.ready()&&!r.lease().current());
 auto missing=f.services();missing.release=nullptr;assert(!r.load(root.c_str(),missing)&&!f.calls);assert(!r.load("/no/class/metadata",f.services())&&!f.calls);
 for(unsigned slot=0;slot<2;++slot)for(int mutation=0;mutation<13;++mutation){f.bad=slot;f.mutation=mutation;const auto old=f.releases;assert(!r.load(root.c_str(),f.services())&&!r.ready());assert(f.releases-old==slot+(mutation!=0));}
 f.bad=-1;f.absent=1;auto old=f.releases;assert(!r.load(root.c_str(),f.services())&&f.releases==old+1);f.absent=-1;
 f.alias=true;old=f.releases;assert(!r.load(root.c_str(),f.services())&&f.releases==old+2);f.alias=false;
 for(int slot=0;slot<2;++slot){f.retireDuring=slot;old=f.releases;assert(!r.load(root.c_str(),f.services())&&!r.ready()&&f.releases==old+slot+1);}f.retireDuring=-1;
 metadata(root,true);auto calls=f.calls;assert(!r.load(root.c_str(),f.services())&&f.calls==calls);metadata(root);
 f.reenter=true;assert(r.load(root.c_str(),f.services()));auto lease=r.lease();assert(lease.current()&&r.accepts(lease)&&lease.assets->surface(0)&&lease.assets->icon(343));
 calls=f.calls;assert(!r.load(root.c_str(),f.services())&&f.calls==calls);
 auto forged=lease;forged.textures[0]=&f;assert(!forged.current()&&!r.accepts(forged));forged=lease;forged.assets=std::make_shared<BattleClassAssets>();assert(!forged.current());
 f.watched=lease;old=f.releases;r.retire();assert(f.releases==old+2&&!r.ready()&&!lease.current()&&!r.accepts(lease));r.retire();assert(f.releases==old+2);
 f.watched={};assert(r.load(root.c_str(),f.services()));auto fresh=r.lease();assert(fresh.generation!=lease.generation&&fresh.textures==lease.textures&&!r.accepts(lease)&&r.accepts(fresh));r.retire();assert(!fresh.current());
 std::filesystem::remove_all(root);std::cout<<"PASS class GPU pair validation, rollback, exact catalog, reentrant cancel, retirement-before-free and address-reuse generations\n";
}
