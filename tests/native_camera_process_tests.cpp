#include "native_camera_process.hpp"
#include "native_shadow_process.hpp"
#include "native_ui_startup.hpp"
#include "native_field_animation.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
float number(EventAddressSpace& m,uint32_t a){auto b=word(m,a);float v;std::memcpy(&v,&b,4);return v;}
uint64_t bits(double v){uint64_t b;std::memcpy(&b,&v,8);return b;}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float f){return int32_t(f);};p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};p.roundDoubleFloat=[](void*,double d){volatile float v=float(d);return v;};return p;}
struct Fixture {
 std::shared_ptr<NativeFieldAnimation> field;
 std::shared_ptr<NativeUiStartup> ui;
 std::shared_ptr<NativeShadowProcess> shadows;
 std::shared_ptr<NativeCameraProcess> cameras;
 Fixture(const char* root){field=NativeFieldAnimation::create(root,NativeFileCache::create(),platform());assert(field);ui=NativeUiStartup::create(field,root);assert(ui&&ui->resumeFont());shadows=NativeShadowProcess::create(ui,root);assert(shadows&&shadows->initialize());}
 EventAddressSpace& memory(){return field->process().vm().memory;}
};
void actualBank(const char* root){
 Fixture f(root);auto& m=f.memory();auto numerical=nativeCameraLibmNumerics();bool distance=false,tangent=false;
 NativeCameraNumerics held;
 held.reciprocalSquareRoot=[&](float v)->std::optional<double>{return distance?numerical.reciprocalSquareRoot(v):std::nullopt;};
 held.tangent=[&](uint32_t fn,double v)->std::optional<double>{return tangent?numerical.tangent(fn,v):std::nullopt;};
 f.cameras=NativeCameraProcess::create(f.shadows,root,held);assert(f.cameras&&f.cameras->current()&&!f.cameras->ready());
 const auto uiCounts=f.ui->counts();assert(word(m,NativeCameraProcess::RenderPointer)==0);
 assert(!f.cameras->initialize(1)&&f.cameras->busy());assert(word(m,NativeCameraProcess::RenderPointer)==0);
 const auto originalMode=word(m,NativeCameraProcess::NtscInterlaced);assert(m.writeWord(NativeCameraProcess::NtscInterlaced,0x12345678));
 assert(!f.cameras->initialize()&&f.cameras->pending()==0x8000fe64); // First real Distance child.
 auto c=f.cameras->counts();assert(c.allocations==1&&c.clears==1&&c.distances==0);
 assert(word(m,NativeCameraProcess::RenderPointer)==NativeCameraProcess::RenderObject);
 assert(word(m,NativeCameraProcess::RenderObject)==originalMode);assert(m.writeWord(NativeCameraProcess::NtscInterlaced,originalMode)); // Retained first original load, no copy replay.
 for(unsigned i=0;i<NativeCameraProcess::RenderBytes;++i){uint8_t a=0,b=0;assert(m.readByte(NativeCameraProcess::NtscInterlaced+i,a)&&m.readByte(NativeCameraProcess::RenderObject+i,b)&&a==b);}
 assert(!f.cameras->initialize()&&f.cameras->counts().allocations==1);
 distance=true;assert(!f.cameras->initialize()&&f.cameras->pending()==0x8029e824);
 c=f.cameras->counts();assert(c.allocations==5&&c.clears==5&&c.copies==4&&c.distances==4);
 for(unsigned i=0;i<3;++i)assert(!f.cameras->initialize()&&f.cameras->counts().allocations==5);
 tangent=true;assert(f.cameras->initialize()&&f.cameras->ready()&&!f.cameras->busy());
 c=f.cameras->counts();assert(c.allocations==13&&c.clears==13&&c.copies==13&&c.distances==13&&c.tangents==12);
 assert(f.cameras->roads()&&f.cameras->roads()->current());
 auto bank=std::make_shared<CameraManager>();bank->initializeBoot(608,480);assert(f.cameras->publishBoot(bank)&&f.cameras->bootPublishedFor(bank));
 const char* names[]{"off","off2","shadow","bg","3d","3deff_A","3dimg","3deff_B","2d","fade","fade2","dbg","dbg3d"};
 for(unsigned i=0;i<13;++i){const auto at=word(m,NativeCameraProcess::Table+4*i);assert(at&&number(m,at+0x104)==608&&number(m,at+0x108)==480&&std::fabs(number(m,at+0x118)-1000)<.001f);
  const auto* typed=bank->camera(i);assert(typed&&!typed->viewPublished&&!std::strcmp(typed->name,names[i])&&typed->drawCamera==8&&typed->scissor[2]==608&&typed->scissor[3]==480);
  for(unsigned j=0;j<12;++j)assert(word(m,at+0x11c+4*j)==0);
  for(unsigned j=0;j<16;++j)assert(word(m,at+0x1a0+4*j)==0);
  assert(typed->projectionKind==((i<=3||i==11)?1u:0u));
 }
 auto camera=f.cameras->fieldCamera();assert(camera&&camera->entryAddress==word(m,FieldCameraRoadOwner::CameraSlot)&&m.hasRamBacking(camera->entryAddress,camera->entryBacking,NativeCameraProcess::CameraBytes));
 bank->camera(4)->nearPlane=77;assert(f.cameras->publishBoot(bank)&&bank->camera(4)->nearPlane==77); // No cold replay over live semantic state.
 assert(f.cameras->initialize()&&f.cameras->counts().allocations==13&&!NativeCameraProcess::create(f.shadows,root,numerical));
 assert(f.ui->current()&&f.ui->counts().reads==uiCounts.reads&&f.ui->counts().heapAllocations==uiCounts.heapAllocations&&f.shadows->ready());
 // Real SDK heap loss invalidates raw camera ownership despite unchanged bytes.
 auto descriptors=word(m,0x8041f070),heap=word(m,0x8041e844),head=word(m,descriptors+12*heap+8);
 assert(m.writeWord(descriptors+12*heap+8,0));assert(!f.cameras->current()&&!f.cameras->initialize()&&!f.cameras->fieldCamera());assert(m.writeWord(descriptors+12*heap+8,head)&&f.cameras->ready());
 // Equal bytes at a foreign mapping cannot satisfy actual camera4 borrowing.
 std::vector<uint8_t> same(NativeCameraProcess::CameraBytes);for(unsigned i=0;i<same.size();++i)assert(m.readByte(camera->entryAddress+i,same[i]));
 assert(m.unregisterExternalRam(camera->entryAddress)&&m.registerRam(camera->entryAddress,same));
 assert(!f.cameras->current()&&!f.cameras->roads()&&!f.cameras->fieldCamera());f.cameras.reset();assert(word(m,camera->entryAddress+0x1e8)==8);
}
void progressiveAndRetirement(const char* root){
 FieldMapSourceCall road;
 Fixture f(root);f.cameras=NativeCameraProcess::create(f.shadows,root,nativeCameraLibmNumerics(),NativeCameraProcess::NtscProgressive);assert(f.cameras&&f.cameras->initialize());assert(word(f.memory(),NativeCameraProcess::RenderObject)==2);
 auto bank=std::make_shared<CameraManager>();bank->initializeBoot(608,480);assert(f.cameras->publishBoot(bank));bank->initializeBoot(608,480);assert(!f.cameras->bootPublishedFor(bank)&&!f.cameras->publishBoot(bank));
 road.function=FieldMapSource::CameraLoad;road.arguments[0]=4;assert(!f.cameras->roads()->service(road,{}));assert(f.cameras->busy());
 f.ui->retire();assert(!f.cameras->current()&&!f.cameras->initialize());
}
// Direct source-matrix child proves binary64 tangent results survive until the
// original fdivs. Early binary32 narrowing changes this deliberately chosen K.
void wideMatrix(){
 struct Memory {EventNativeRuntime vm{0x83000000,platform()};EventAddressSpace& m=vm.memory;bool allow=false;unsigned calls=0;};
 auto memory=std::make_unique<Memory>();auto& h=*memory;
 assert(h.m.registerRam(0x81000000,std::vector<uint8_t>(4096)));
 assert(h.m.registerRam(0x80429378,std::vector<uint8_t>{0x3f,0x80,0,0,0x40,0,0,0,0,0,0,0,0xbf,0x80,0,0,0x3f,0,0,0,0x3c,0x8e,0xfa,0x35}));
 SourceCameraServices s;s.user=&h;s.word=[](void* p,uint32_t a,uint32_t& v){return static_cast<Memory*>(p)->m.readWord(a,v);};s.store=[](void* p,uint32_t a,uint32_t v){return static_cast<Memory*>(p)->m.writeWord(a,v);};
 s.call=[](void* p,SourceCameraCall& q){auto& h=*static_cast<Memory*>(p);assert(q.function==0x80272864);if(!h.allow)return false;++h.calls;q.floatingResult=bits(1.+3.*std::ldexp(1.,-25));return true;};
 SourceCameraCall q;q.function=SourceCameraBootstrap::Perspective;q.arguments[0]=0x81000000;q.scalars[0]=bits(25);q.scalars[1]=bits(1);q.scalars[2]=bits(1);q.scalars[3]=bits(32768);
 SourceCameraCursor c;assert(!resumeSourceCamera(q,0x81000f00,s,c)&&c.pc==0x8029e824);auto changed=q;changed.scalars[0]=bits(30);assert(!resumeSourceCamera(changed,0x81000f00,s,c)&&c.pc==0x8029e824);
 h.allow=true;assert(resumeSourceCamera(q,0x81000f00,s,c));assert(h.calls==1&&word(h.m,0x81000014)==0x3f7fffff);
}
}
int main(){wideMatrix();actualBank("romfs");progressiveAndRetirement("romfs");}
