#include "animation_native_main.hpp"
#include "native_field_animation.hpp"
#include "field_party.hpp"
#include "field_camera_publication.hpp"
#include "field_camera.hpp"
#include <cassert>
#include <cstring>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t CO[]={0xf0,0xf8,0x13c,0x140,0x12c},PO[]={0x48,0x50,0x58,0x60,0x68},DW[]={12,12,1,4,24};
struct Fixture {
 std::shared_ptr<NativeFieldAnimation> native;std::shared_ptr<BattlePosePool> pool;std::shared_ptr<CameraManager> cameras=std::make_shared<CameraManager>();
 bool valid=true;uint64_t currentPass=37;std::vector<int32_t> handles;
 explicit Fixture(unsigned count=1,bool camera=true){
  assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform);assert(native);pool=native->pool();
  for(unsigned i=0;i<count;++i){auto h=pool->enter(fieldPlayerPoseResources()[0],2,{});assert(h && pool->selectName(*h,"M_W_1",true,{}));handles.push_back(*h);}
  for(unsigned i=0;i<17;++i)assert(native->tick(false));
  cameras->initializeBoot(608,480);if(camera)assert(publishNativeFieldCamera(*cameras,adaptedFieldCameraFrame(0,0,0)));
  assert(process().configureMain(cameras,policy(),[this](uint64_t pass){return valid && pass==currentPass;}));
 }
 AnimationStartupProcess& process(){return native->process();}EventAddressSpace& memory(){return process().vm().memory;}
 AnimationFacingAddressPolicy policy(){AnimationFacingAddressPolicy p;p.base=0x81900000;p.unmapped=[this](uint32_t a,uint32_t n){return memory().unmappedRange(a,n);};return p;}
 uint32_t read(uint32_t a){uint32_t v;assert(memory().readWord(a,v));return v;}
 uint32_t timelineSlot(unsigned i=0){auto* p=pool->pose(handles[i]);const uint32_t w=read(0x80415dc0),models=read(w),entry=read(models+16*p->word(0x10)+8),model=read(read(entry+0xa0));return read(model+0x1ac)+64*p->word(0x14)+0x3c;}
 uint32_t model(){auto* p=pool->pose(handles[0]);return read(read(read(read(read(0x80415dc0))+16*p->word(0x10)+8)+0xa0));}
 bool advance(unsigned visits=1){return native->main(uint32_t(handles[0]),currentPass,visits);}
 void finish(){for(unsigned i=0;i<100000;++i)if(advance(64))return;assert(false);}
};
void quantizationAndActualMain(){
 AnimationQuantizationState bank;assert(!bank.read(4) && !bank.read(8) && !bank.write(911,1) && !bank.write(920,1));assert(bank.write(912,123) && bank.read(0)==123 && bank.writes()==1);
 Fixture f;auto* p=f.pool->pose(f.handles[0]);BattlePoseClock clock;assert(f.native->clock(clock) && f.pool->evaluate(f.handles[0],clock));const auto current=p->word(0x3c),fraction=p->word(0x40);const auto model=f.model();std::array<uint32_t,8> pointers{},lengths{};std::array<std::vector<uint8_t>,8> expected;
 for(unsigned c=0;c<5;++c){pointers[c]=p->word(PO[c]);lengths[c]=f.read(model+CO[c])*DW[c];}pointers[5]=p->word(0x4c);pointers[6]=p->word(0x54);pointers[7]=p->word(0x64);lengths[5]=lengths[0];lengths[6]=lengths[1];lengths[7]=lengths[3];for(unsigned c=0;c<8;++c){expected[c].resize(lengths[c]);for(unsigned i=0;i<lengths[c];++i)assert(f.memory().readByte(pointers[c]+i,expected[c][i]));}
 p->shape.positions[0]={999,888,777};const auto oldShape=p->shape.positions;const auto epoch=p->timestamp();p->word(0x3c,0xffffffff);p->scalar(0x40,.875f);assert(!f.process().quantization().read(4));assert(!f.native->main(uint32_t(f.handles[0]),f.currentPass,0) && !f.process().mainPending());assert(!f.advance() && f.process().mainPending());const auto q=f.process().quantization().writes();assert(q==4);for(unsigned i=2;i<6;++i)assert(f.process().quantization().read(i)==0x00040004u+(i-2)*0x10001u);
 assert(f.process().quantization().write(916,0));bool held=false;for(unsigned i=0;i<20000;++i){assert(!f.advance());const auto progress=f.process().mainOwner()->progress();if(progress.main==21 && progress.geometry==0 && progress.cache==1 && (progress.replay==41 || progress.replay==50)){held=true;break;}}assert(held);const auto state=p->state;assert(!f.advance(32) && p->state==state && f.process().quantization().writes()==q+1);
 assert(f.process().quantization().write(916,0x00060006));f.finish();assert(!f.process().mainPending() && p->word(0x3c)==current && p->word(0x40)==fraction && p->timestamp()==epoch && p->shape.positions==oldShape && f.process().quantization().writes()==q+2);
 for(unsigned c=0;c<8;++c)for(unsigned i=0;i<lengths[c];++i){uint8_t v;assert(f.memory().readByte(pointers[c]+i,v) && v==expected[c][i]);}
 // A later invocation owns new GQR writes; no once-per-tick dedup is invented.
 const auto allocations=f.process().mainOwner()->frameAllocations();f.finish();assert(f.process().quantization().writes()==q+6 && f.process().mainOwner()->frameAllocations()==allocations);
 // Global W outputs use actual mapped RAM, independent of typed R4c/R54.
 const uint32_t globalP=0x83000000,globalN=0x83100000,work=f.read(0x80415dc0),oldP=p->word(0x4c),oldN=p->word(0x54);assert(f.memory().registerRam(globalP,std::vector<uint8_t>(lengths[5])) && f.memory().registerRam(globalN,std::vector<uint8_t>(lengths[6])));assert(f.memory().writeWord(work+0xe8,1) && f.memory().writeWord(work+0xf8,globalP) && f.memory().writeWord(work+0xfc,globalN));p->word(0x4c,0);p->word(0x54,0);f.finish();for(unsigned lane=0;lane<2;++lane)for(unsigned i=0;i<lengths[5+lane];++i){uint8_t v;assert(f.memory().readByte((lane?globalN:globalP)+i,v) && v==expected[5+lane][i]);}assert(p->shape.positions==oldShape);p->word(0x4c,oldP);p->word(0x54,oldN);assert(f.memory().writeWord(work+0xe8,0));

}
void pendingIdentityAndProducerGuards(){
 Fixture f;auto* p=f.pool->pose(f.handles[0]);p->word(0x3c,0xffffffff);assert(!f.advance());const auto state=p->state;const auto q=f.process().quantization().writes();const auto depth=f.process().mainOwner()->depth();BattlePoseClock before,after;assert(f.native->clock(before));
 assert(!f.native->tick(false) && !f.native->release(f.handles[0]) && !f.native->mario({},{{-1,-1,-1}}));assert(!f.pool->enter(fieldPlayerPoseResources()[0],2,{}));assert(!f.process().enter(0x802c18a0,2,fieldPlayerPoseResources()[0]));assert(!f.process().advance());assert(!f.process().configureMain(f.cameras,f.policy(),[](uint64_t){return true;}));assert(!f.native->main(uint32_t(f.handles[0])+1,f.currentPass,64) && !f.native->main(uint32_t(f.handles[0]),f.currentPass+1,64));assert(f.native->clock(after) && before.modeZero==after.modeZero && before.modeNonzero==after.modeNonzero && p->state==state && f.process().quantization().writes()==q && f.process().mainOwner()->depth()==depth);
 f.valid=false;assert(!f.advance(100) && p->state==state);f.valid=true;const auto generation=p->nativeGeneration;p->nativeGeneration+=1;assert(!f.advance(100) && p->state==state);p->nativeGeneration=generation;
 assert(f.memory().registerRam(0x81900000,std::vector<uint8_t>(4)));assert(!f.advance(64) && p->state==state);assert(f.memory().unregisterRam(0x81900000));f.finish();assert(f.native->tick(false));
 // Configuration is replaceable only after the complete raw invocation retires.
 assert(f.process().configureMain({},f.policy(),[&](uint64_t pass){return f.valid && pass==f.currentPass;}));f.finish();
}
void nullAndReachedCamera(){
 Fixture null(1,false);auto* p=null.pool->pose(null.handles[0]);p->word(0,p->word(0)|8);const auto slot=null.timelineSlot(),timeline=null.read(slot);assert(null.memory().writeWord(slot,0));const auto state=p->state;assert(null.advance() && !null.process().mainPending() && p->state==state && null.process().quantization().writes()==4);assert(null.memory().writeWord(slot,timeline));
 p->word(0,p->word(0)&~8u);null.finish();assert(null.process().quantization().writes()==8); // No published camera needed for clear8 tail.
 p->word(0,p->word(0)|8);p->word(0x3c,0xffffffff);bool facing=false;for(unsigned i=0;i<20000;++i){assert(!null.advance(1));if(null.process().mainOwner()->phase()==22){facing=true;break;}}assert(facing);const auto sampled=p->state;const auto epoch=p->timestamp();const auto q=null.process().quantization().writes();assert(!null.advance(32) && p->state==sampled);p->word(0x18,0x12345678);p->word(0x1c,0x87654321);p->word(0x3c,77);assert(publishNativeFieldCamera(*null.cameras,adaptedFieldCameraFrame(0,0,0)));assert(null.advance(64) && !null.process().mainPending() && p->word(0x3c)==77 && p->timestamp()!=epoch && null.process().quantization().writes()==q);
 for(unsigned i=0x20;i<0x70;++i)if(i<0x3c || i>=0x40)assert(p->state[i]==sampled[i]);
}
void linkedFramesAndCaptures(){
 Fixture f(3);auto* root=f.pool->pose(f.handles[0]);auto* child=f.pool->pose(f.handles[1]);auto* grand=f.pool->pose(f.handles[2]);root->word(0x90,uint32_t(f.handles[1]));child->word(0x90,uint32_t(f.handles[2]));root->word(0x3c,0xffffffff);child->word(0x3c,0xffffffff);grand->word(0x3c,0xffffffff);
 const auto positions=root->word(0x4c),normals=root->word(0x54),slot=f.timelineSlot(),timeline=f.read(slot);assert(!f.advance() && f.process().mainOwner()->depth()==2 && f.process().quantization().writes()==4);assert(!f.advance() && f.process().mainOwner()->depth()==3 && f.process().quantization().writes()==8);
 // Parent's model/timeline/outputs were captured before child work. Mutate only
 // parent fields and the shared selector slot; both suspended parents already captured.
 root->word(0x4c,0);root->word(0x54,0);assert(f.memory().writeWord(slot,0));f.finish();assert(f.process().quantization().writes()==12 && root->word(0x4c)==0 && root->word(0x54)==0 && root->word(0x3c)!=0xffffffff && child->word(0x3c)!=0xffffffff);assert(f.read(positions)!=0 || f.read(positions+4)!=0);assert(f.read(normals)!=0 || f.read(normals+4)!=0);
 root->word(0x4c,positions);root->word(0x54,normals);assert(f.memory().writeWord(slot,timeline));
 // Original linked flag2 skips that child entirely, with only root GQR writes.
 child->word(4,child->word(4)|2);f.finish();assert(f.process().quantization().writes()==16);
 // A cycle consumes explicit frames within the visit budget; it never returns
 // fabricated success, recursively blows C++ stack, or silently skips a child.
 child->word(4,child->word(4)&~2u);child->word(0x90,uint32_t(f.handles[0]));assert(!f.advance(7) && f.process().mainOwner()->depth()==8 && f.process().quantization().writes()==44);
}
}
int main(){quantizationAndActualMain();pendingIdentityAndProducerGuards();nullAndReachedCamera();linkedFramesAndCaptures();}
