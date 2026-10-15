#include "battle_submenu_surface_gpu.hpp"
#include <cassert>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <string>
using namespace ttyd;
struct Fixture {
 BattleSubmenuSurfaceGpuResource resource;BattleClassSurfaceTextureServices services;
 int imports=0,releases=0;bool retireInImport=false,reenter=false;BattleSubmenuSurfaceLease observed;std::string root;
 BattleClassSurfaceImport descriptor{this,32,32,0,1,32,32,false,{0,1,1,0}};
 Fixture(){services.user=this;services.import=[](void*p,uint32_t slot,const char*)->std::optional<BattleClassSurfaceImport>{auto&f=*static_cast<Fixture*>(p);assert(slot==0);++f.imports;if(f.retireInImport)f.resource.retire();if(f.reenter)assert(!f.resource.load(f.root.c_str(),f.services));return f.descriptor;};
 services.release=[](void*p,uint32_t slot,const void*id){auto&f=*static_cast<Fixture*>(p);assert(slot==0&&id==f.descriptor.identity);++f.releases;assert(!f.observed.current());assert(!f.resource.lease().current());};}
};
static void metadata(const std::string&path){FILE*f=std::fopen(path.c_str(),"wb");assert(f);uint32_t count=2;assert(std::fwrite("PMBSUB01",8,1,f)==1);assert(std::fwrite(&count,4,1,f)==1);for(unsigned i=0;i<2;++i){uint32_t id=i+2;unsigned x=i?22:2,w=i?8:16;BattleClassRegion r{w,24,float(x)/32,1-2.f/32,float(x+w)/32,1-26.f/32};assert(std::fwrite(&id,4,1,f)==1);assert(std::fwrite(&r,sizeof(r),1,f)==1);}std::fclose(f);}
int main(int argc,char**argv){
 const auto dir=std::filesystem::temp_directory_path()/("pm-port-submenu-surface-tests-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));std::filesystem::create_directories(dir);metadata((dir/"battle-submenu-surfaces.bin").string());
 const auto root=argc>1?std::string(argv[1]):dir.string();
 {Fixture f;f.root=root;f.reenter=true;assert(f.resource.load(root.c_str(),f.services));f.observed=f.resource.lease();assert(f.observed.current());assert(f.observed.surface(2)->width==16&&f.observed.surface(3)->width==8);assert(!f.observed.surface(0)&&!f.observed.surface(4));assert(f.resource.accepts(f.observed));assert(!f.resource.load(root.c_str(),f.services));auto fake=f.observed;fake.texture=&fake;assert(!fake.current()&&!f.resource.accepts(fake));f.resource.retire();assert(f.releases==1&&!f.observed.current());assert(f.resource.load(root.c_str(),f.services));assert(!f.resource.accepts(f.observed));f.resource.retire();assert(f.releases==2);}
 for(unsigned field=0;field<9;++field){Fixture f;switch(field){case 0:f.descriptor.width=64;break;case 1:f.descriptor.height=64;break;case 2:f.descriptor.format=1;break;case 3:f.descriptor.count=2;break;case 4:f.descriptor.subWidth=16;break;case 5:f.descriptor.subHeight=24;break;case 6:f.descriptor.rotated=true;break;case 7:f.descriptor.uv[0]=.1f;break;case 8:f.descriptor.identity=nullptr;break;}assert(!f.resource.load(root.c_str(),f.services));assert(f.imports==1&&f.releases==int(field!=8));assert(!f.resource.lease().current());}
 {Fixture f;f.retireInImport=true;assert(!f.resource.load(root.c_str(),f.services));assert(f.imports==1&&f.releases==1);f.retireInImport=false;assert(f.resource.load(root.c_str(),f.services));}
 // Bad catalog/extra bytes are rejected before importing native memory.
 {FILE*f=std::fopen((dir/"battle-submenu-surfaces.bin").string().c_str(),"ab");assert(f);std::fputc(0,f);std::fclose(f);Fixture bad;assert(!bad.resource.load(dir.string().c_str(),bad.services));assert(bad.imports==0);}
 std::filesystem::remove_all(dir);return 0;
}
