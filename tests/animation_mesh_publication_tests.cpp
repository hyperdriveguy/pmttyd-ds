#include "animation_mesh_publication.hpp"
#include "native_field_animation.hpp"
#include "field_party.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
using namespace ttyd;
namespace {
struct Fixture {
 std::shared_ptr<NativeFieldAnimation> native;std::shared_ptr<BattlePosePool> pool;bool valid=true;uint64_t pass=91;int32_t handle=0;
 Fixture(bool fast=true){assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform);assert(native);pool=native->pool();auto h=pool->enter(fieldPlayerPoseResources()[0],2,{});assert(h && pool->selectName(*h,"M_W_1",true,{}));handle=*h;for(unsigned i=0;i<17;++i)assert(native->tick(false));AnimationFacingAddressPolicy a;a.base=0x81900000;a.unmapped=[this](uint32_t p,uint32_t n){return memory().unmappedRange(p,n);};assert(process().configureMain({},a,[this](uint64_t p){return valid && p==pass;},fast));}
 AnimationStartupProcess& process(){return native->process();}EventAddressSpace& memory(){return process().vm().memory;}BattlePose& pose(){return *pool->pose(handle);}
 uint32_t read(uint32_t a){uint32_t v;assert(memory().readWord(a,v));return v;}
 uint32_t model(){auto& p=pose();return read(read(read(read(read(0x80415dc0))+16*p.word(0x10)+8)+0xa0));}
 void source(){assert(process().reservePresentation(uint32_t(handle),pass));for(unsigned i=0;i<10000;++i){assert(!process().present(uint32_t(handle),pass,8192));if(process().presentationOwner()->phase()==1)return;}assert(false);}
 void finish(unsigned budget=8192){for(unsigned i=0;i<100000;++i)if(process().present(uint32_t(handle),pass,budget))return;assert(false);}
};
bool same(const ActorAnimationSample& a,const ActorAnimationSample& b){
 if(a.last!=b.last || a.next!=b.next || a.fraction!=b.fraction || a.selectedTime!=b.selectedTime || a.positions!=b.positions || a.normals!=b.normals || a.nodes!=b.nodes || a.visibility!=b.visibility || a.textures!=b.textures || a.vertices.size()!=b.vertices.size() || a.colors!=b.colors || a.batches.size()!=b.batches.size() || a.surfaces.size()!=b.surfaces.size())return false;
 for(unsigned i=0;i<a.vertices.size();++i)if(std::memcmp(&a.vertices[i],&b.vertices[i],sizeof(ActorMeshVertex)))return false;
 for(unsigned i=0;i<a.batches.size();++i)if(std::memcmp(&a.batches[i],&b.batches[i],sizeof(ActorSurfaceBatch)))return false;
 for(unsigned i=0;i<a.surfaces.size();++i)if(a.surfaces[i].transform!=b.surfaces[i].transform || a.surfaces[i].wrapS!=b.surfaces[i].wrapS || a.surfaces[i].wrapT!=b.surfaces[i].wrapT)return false;
 return true;
}
void actualAndEveryVisit(){
 Fixture f;BattlePoseClock clock;assert(f.native->clock(clock) && f.pool->evaluate(f.handle,clock));const auto expected=f.pose().shape;f.pose().shape.positions[0]={999,888,777};const auto old=f.pose().shape;f.pose().word(0x3c,0xffffffff);f.source();assert(f.process().mainOwner()->bulkCopies()>0);const auto q=f.process().quantization().writes();assert(f.process().presentationPending() && !f.process().mainPending());
 unsigned visits=0;for(;visits<100000;++visits){if(f.process().present(uint32_t(f.handle),f.pass,1))break;assert(same(f.pose().shape,old));const auto reads=f.process().presentationOwner()->reads();f.valid=false;assert(!f.process().present(uint32_t(f.handle),f.pass,32) && f.process().presentationOwner()->reads()==reads && same(f.pose().shape,old));f.valid=true;assert(!f.process().present(uint32_t(f.handle)+1,f.pass,1));assert(!f.process().present(uint32_t(f.handle),f.pass+1,1));}assert(visits<100000 && same(f.pose().shape,expected) && f.process().quantization().writes()==q && f.process().presentationOwner()->publications()==1);
 // Two reusable sample slots retain capacities after warmup.
 f.source();f.finish();auto capacities=[](const ActorAnimationSample& s){return std::array<size_t,13>{{s.positions.capacity(),s.normals.capacity(),s.nodes.capacity(),s.textures.capacity(),s.visibility.capacity(),s.vertices.capacity(),s.colors.capacity(),s.surfaces.capacity(),s.batches.capacity(),s.world.capacity(),s.seen.capacity(),s.active.capacity(),s.textureTransforms.capacity()}};};const auto capacity=capacities(f.process().presentationOwner()->candidate());const auto frames=f.process().mainOwner()->frameAllocations();f.source();f.finish();assert(capacities(f.process().presentationOwner()->candidate())==capacity && f.process().mainOwner()->frameAllocations()==frames);
}
void fastParityAndFallback(){
 {Fixture slow(false),fast;slow.source();fast.source();
 bool done=false;for(unsigned i=0;i<10000 && !done;++i){const unsigned budget=i%4==3?8192:i%4==2?31:i%4==1?7:1;const bool a=slow.process().present(uint32_t(slow.handle),slow.pass,budget),b=fast.process().present(uint32_t(fast.handle),fast.pass,budget);assert(a==b && slow.process().presentationOwner()->phase()==fast.process().presentationOwner()->phase() && slow.process().presentationOwner()->reads()==fast.process().presentationOwner()->reads() && same(slow.pose().shape,fast.pose().shape));done=a;}
 assert(done && !slow.process().presentationOwner()->fastSlices() && fast.process().presentationOwner()->fastSlices());}
 {Fixture f;f.source();while(f.process().presentationOwner()->phase()!=20)assert(!f.process().present(uint32_t(f.handle),f.pass,1));
 const auto old=f.pose().shape;const auto* publication=f.process().presentationOwner();const auto& candidate=publication->candidate();
 constexpr uint32_t alias=0x8c000000;
 assert(f.memory().registerExternalRam(alias,reinterpret_cast<uint8_t*>(const_cast<ActorVector*>(candidate.positions.data())),sizeof(ActorVector)));
 assert(!f.process().present(uint32_t(f.handle),f.pass,7) && !publication->fastSlices() && same(old,f.pose().shape));
 assert(f.memory().unregisterExternalRam(alias) && f.memory().registerExternalRam(alias,reinterpret_cast<uint8_t*>(const_cast<AnimationMeshPublication*>(publication)),1));
 assert(!f.process().present(uint32_t(f.handle),f.pass,7) && !publication->fastSlices());assert(f.memory().unregisterExternalRam(alias));
 f.finish();assert(publication->fastSlices());}
 // Missing later channels retain a readable prefix through scalar fallback.
 {Fixture missing;missing.source();const auto pointer=missing.pose().word(0x54);std::vector<uint8_t> normals(missing.read(missing.model()+0xf8)*12);for(unsigned i=0;i<normals.size();++i)assert(missing.memory().readByte(pointer+i,normals[i]));missing.pose().word(0x54,0x8d000000);
 assert(!missing.process().present(uint32_t(missing.handle),missing.pass,8192));const auto* m=missing.process().presentationOwner();assert(m->phase()==20 && !m->fastSlices() && m->reads()>=missing.read(missing.model()+0xf0)*3);
 assert(missing.memory().registerRam(0x8d000000,std::move(normals)));missing.finish();assert(m->fastSlices());}
 // Nonfinite values hold at the same lane; repair resumes fresh raw reads.
 {Fixture finite;finite.source();const auto output=finite.pose().word(0x4c),before=finite.read(output+4);const auto shape=finite.pose().shape;
 assert(finite.memory().writeWord(output+4,0x7fc00000) && !finite.process().present(uint32_t(finite.handle),finite.pass,8192));const auto* n=finite.process().presentationOwner();const auto reads=n->reads(),slices=n->fastSlices();assert(n->phase()==20 && slices && same(shape,finite.pose().shape));
 finite.valid=false;assert(!finite.process().present(uint32_t(finite.handle),finite.pass,8192) && n->reads()==reads && n->fastSlices()==slices);finite.valid=true;
 assert(finite.memory().writeWord(output+4,before));finite.finish();assert(n->fastSlices()>slices);}
}
void postMainGuardsAndStaticMismatch(){
 Fixture f;f.source();const auto old=f.pose().shape;const auto q=f.process().quantization().writes();const auto epoch=f.pose().timestamp();assert(!f.native->tick(false) && !f.native->release(f.handle) && !f.pool->enter(fieldPlayerPoseResources()[0],2,{}));assert(!f.process().main(uint32_t(f.handle),f.pass,8192));assert(!f.process().advance());const auto generation=f.pose().nativeGeneration;f.pose().nativeGeneration++;assert(!f.process().present(uint32_t(f.handle),f.pass,8192) && same(f.pose().shape,old));f.pose().nativeGeneration=generation;
 const auto model=f.model(),count=f.read(model+0xf0);assert(f.memory().writeWord(model+0xf0,count-1));for(unsigned i=0;i<64;++i)assert(!f.process().present(uint32_t(f.handle),f.pass,1));assert(f.process().presentationOwner()->phase()==15 && same(f.pose().shape,old));assert(f.memory().writeWord(model+0xf0,count));assert(!f.process().present(uint32_t(f.handle),f.pass,1));
 // Repairing the already captured count does not silently recapture metadata.
 assert(f.process().presentationOwner()->phase()==15 && same(f.pose().shape,old) && f.process().quantization().writes()==q && f.pose().timestamp()==epoch);
}
void globalAndMissingDecode(){
 Fixture f;BattlePoseClock clock;assert(f.native->clock(clock) && f.pool->evaluate(f.handle,clock));const auto expected=f.pose().shape;const auto work=f.read(0x80415dc0);const uint32_t p=0x83000000,n=0x83100000;assert(f.memory().registerRam(p,std::vector<uint8_t>(expected.positions.size()*12)) && f.memory().registerRam(n,std::vector<uint8_t>(expected.normals.size()*12)));assert(f.memory().writeWord(work+0xe8,1) && f.memory().writeWord(work+0xf8,p) && f.memory().writeWord(work+0xfc,n));f.pose().word(0x4c,0);f.pose().word(0x54,0);f.source();const auto q=f.process().quantization().writes();const auto old=f.pose().shape;
 std::vector<uint8_t> bytes;assert(f.memory().unregisterRam(p,&bytes));for(unsigned i=0;i<128;++i){assert(!f.process().present(uint32_t(f.handle),f.pass,1));if(f.process().presentationOwner()->phase()==20)break;}assert(f.process().presentationOwner()->phase()==20 && same(f.pose().shape,old));const auto reads=f.process().presentationOwner()->reads();assert(!f.process().present(uint32_t(f.handle),f.pass,16) && f.process().presentationOwner()->reads()==reads);assert(f.memory().registerRam(p,std::move(bytes)));f.pose().word(0x18,0x12345678);f.pose().word(0x1c,0x87654321);f.finish();assert(same(f.pose().shape,expected) && f.pose().word(0x18)==0x12345678 && f.process().quantization().writes()==q);
}
void assemblyAtomicityAndFreshCapture(){
 Fixture f;f.source();const auto old=f.pose().shape;const auto model=f.model(),table=f.read(model+0x1a8),original=f.read(table+0x4c);assert(f.memory().writeWord(table+0x4c,original^1));for(unsigned i=0;i<64;++i)assert(!f.process().present(uint32_t(f.handle),f.pass,1));assert(f.process().presentationOwner()->phase()==15 && same(f.pose().shape,old));assert(f.memory().writeWord(table+0x4c,original));f.finish();
 // Fresh capture sees post-Main output redirection rather than Main's old output.
 f.source();const uint32_t p=0x83200000;std::vector<uint8_t> bytes(f.pose().shape.positions.size()*12);assert(f.memory().registerRam(p,bytes));f.pose().word(0x4c,p);f.finish();for(const auto& v:f.pose().shape.positions)assert(v==ActorVector({0,0,0}));
}
void marioAndGameBarrier(){
 Fixture f;const auto foreign=f.process().mainConfiguration();assert(f.native->configurePresentation({}) && f.process().mainConfiguration()>foreign);MarioPresentation request;request.supported=true;request.poseUploaded=true;request.selected="M_W_1";request.poseUploadSerial=3;std::array<int32_t,3> handles{{f.handle,f.handle,f.handle}};f.native->setMarioRoot({1,2,3});const auto old=f.pose().shape;assert(!f.native->mario(request,handles,1) && f.native->presentationPending());auto changed=request;changed.poseUploadSerial++;assert(!f.native->mario(changed,handles,8192) && same(f.pose().shape,old));assert(!f.native->tick(false) && !f.native->release(f.handle));for(unsigned i=0;i<10000 && f.native->presentationPending();++i)f.native->resumeMario(8192);assert(!f.native->presentationPending() && f.native->marioPose() && f.native->marioRoot()==ActorVector({1,2,3}));
 auto owner=std::make_unique<Game>(false);auto& game=*owner;EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};assert(game.initializeNativeFieldAnimation("romfs",NativeFileCache::create(),platform));auto native=game.fieldPartyState().nativeAnimation;native->publicationBudget(1);Input input;input.x=1;game.tick(input);assert(game.nativeAnimationPending());const auto ticks=game.ticks;const auto eye=game.cameraManager->camera(4)->eye;const auto position=game.progress.position;BattlePoseClock before,after;assert(native->clock(before));game.tick(input);assert(game.ticks==ticks && game.cameraManager->camera(4)->eye==eye && game.progress.position.x==position.x && game.progress.position.z==position.z && native->clock(after) && before.modeZero==after.modeZero);auto save=game.progress;const auto hp=save.hp;save.hp=hp-1;game.restore(save);assert(game.progress.hp==hp);auto candidate=std::make_unique<Game>(false);assert(!candidate->retainNativeFieldAnimation(game,true));native->publicationBudget(8192);for(unsigned i=0;i<10000 && game.nativeAnimationPending();++i)game.drainNativeAnimation();assert(!game.nativeAnimationPending() && game.ticks==ticks);
}
void rawAndReleaseAdmission(){
 Fixture f;assert(!f.native->main(uint32_t(f.handle),f.pass,1) && f.process().mainPending());assert(!f.process().present(uint32_t(f.handle)+1,f.pass,8192) && !f.process().presentationPending());assert(!f.process().reservePresentation(uint32_t(f.handle),f.pass));
 Fixture release;auto& p=release.pose();p.word(0,p.word(0)|0x40);assert(!release.native->release(release.handle) && release.native->releasePending()==0x8002f878);const auto state=p.state;MarioPresentation request;request.supported=true;request.poseUploaded=true;request.selected="M_W_1";assert(!release.native->mario(request,{{release.handle,release.handle,release.handle}}) && !release.process().presentationPending() && p.state==state);
}
void failedAssembly(){
 Fixture f;f.source();const auto old=f.pose().shape;const auto q=f.process().quantization().writes();const auto* pose=f.pose().resource->bank->pose(f.pose().resource->names[f.pose().word(0x14)].c_str());bool bad=false;
 for(unsigned t=0;t<pose->textures.size();++t){for(unsigned i=0;i<256;++i)if(pose->textures[t].cells[i]>=64){assert(f.memory().writeByte(f.pose().word(0x6c)+24*t,uint8_t(i)));bad=true;break;}}
 assert(bad);
 for(unsigned i=0;i<1000 && f.process().presentationOwner()->phase()!=30;++i){assert(!f.process().present(uint32_t(f.handle),f.pass,32));}
 assert(f.process().presentationOwner()->phase()==30);assert(!f.process().present(uint32_t(f.handle),f.pass,1) && f.process().presentationOwner()->phase()==30 && same(f.pose().shape,old) && f.process().quantization().writes()==q);
}

void linkedAndNull(){
 Fixture linked;BattlePoseClock clock;assert(linked.native->clock(clock) && linked.pool->evaluate(linked.handle,clock));const auto expected=linked.pose().shape;auto child=linked.pool->enter(fieldPlayerPoseResources()[0],2,{});assert(child && linked.pool->selectName(*child,"M_W_1",true,{}));linked.pose().word(0x90,uint32_t(*child));linked.source();assert(linked.process().quantization().writes()==8);linked.finish();assert(same(linked.pose().shape,expected));
 Fixture null;assert(null.native->clock(clock) && null.pool->evaluate(null.handle,clock));const auto old=null.pose().shape;const auto directory=null.read(null.model()+0x1ac),slot=directory+64*null.pose().word(0x14)+0x3c;assert(null.memory().writeWord(slot,0));null.pose().word(0,null.pose().word(0)|8);null.source();const auto state=null.pose().state;null.finish();assert(same(null.pose().shape,old) && null.pose().state==state && null.process().quantization().writes()==4);
}

void sourceHeaderAliasGuard(){
 // Source output can alias the W header. A successful store must invalidate
 // the NEXT native primitive even inside one accelerated synchronous slice.
 {
  Fixture f;const auto work=f.read(0x80415dc0),models=f.read(work);auto& retained=f.pose();const auto old=retained.shape;
  assert(f.memory().writeWord(work+0xe8,1) && f.memory().writeWord(work+0xf8,work));
  assert(!f.process().present(uint32_t(f.handle),f.pass,8192));
  assert(f.read(work)!=models && !f.process().ready() && f.process().mainPending() && same(retained.shape,old));
  const auto q=f.process().quantization().writes();assert(q==4);
  assert(!f.process().present(uint32_t(f.handle),f.pass,8192) && f.process().quantization().writes()==q);
  assert(f.memory().writeWord(work,models) && f.memory().writeWord(work+0xe8,0));
 }
 {
  Fixture f;const auto work=f.read(0x80415dc0);assert(!f.process().present(uint32_t(f.handle),f.pass,1));
  const auto q=f.process().quantization().writes();assert(f.memory().writeWord(work+4,63));
  assert(!f.process().present(uint32_t(f.handle),f.pass,8192) && f.process().quantization().writes()==q);
  assert(f.memory().writeWord(work+4,64));f.finish();
 }
}

}
int main(){actualAndEveryVisit();fastParityAndFallback();postMainGuardsAndStaticMismatch();globalAndMissingDecode();assemblyAtomicityAndFreshCapture();marioAndGameBarrier();rawAndReleaseAdmission();failedAssembly();linkedAndNull();sourceHeaderAliasGuard();}
