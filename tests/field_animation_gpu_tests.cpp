#include "field_animation_gpu.hpp"
#include "native_field_animation.hpp"
#include "field_npc_main_entry_binding.hpp"
#include "field_party.hpp"
#include "animation_native_inner.hpp"
#include "animation_render_sdk_binding.hpp"
#include "animation_shape_render.hpp"
#include <cassert>
#include <cstring>
#include <limits>
#include <cmath>
using namespace ttyd;
namespace {
using namespace field_animation_gx;using K=AnimationRenderKind;using A=AnimationRenderArgument;
constexpr uint32_t Data=0x82000000,AttributeSpacing=0x100;
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
BattleActMatrix identity(){return {1,0,0,0,0,1,0,0,0,0,1,0};}
AnimationRenderCall request(uint32_t fn,std::initializer_list<A> args){AnimationRenderCall q;q.function=fn;q.count=args.size();unsigned i=0;for(auto a:args)q.arguments[i++]=a;return q;}
struct Fixture {
 EventEntryAddresses entries;EventContextAddresses contexts{entries};EventRuntimeGlobals globals{0,1,1,0};EventSchedule schedule;EventRuntimeAddresses runtime{globals,schedule};EventGpAddresses gp;EventAddressSpace memory{entries,contexts,runtime,gp};
 bool valid=true,physicalReady=true,submitReady=true;unsigned cameras=0,culls=0,submits=0,delegates=0;uint32_t lastCull=99;const FieldAnimationVertex* held=nullptr;std::vector<FieldAnimationVertex> captured;FieldAnimationMatrices capturedMatrices;uint32_t capturedUv=0;
 AnimationRenderCall* mutate=nullptr;CameraState cameraState;AnimationRenderLocals local;uint32_t result=99;
 FieldAnimationPhysical physical{this,[](void* u,const NativeCameraPacket&,const NativeCameraDraw&){auto& f=*static_cast<Fixture*>(u);++f.cameras;return f.physicalReady;},[](void* u,uint32_t mode){auto& f=*static_cast<Fixture*>(u);++f.culls;f.lastCull=mode;if(f.mutate)f.mutate->arguments[0].value=2;return f.physicalReady;}};
 FieldAnimationSubmit submit{this,[](void* u,const FieldAnimationBatch& b){auto& f=*static_cast<Fixture*>(u);++f.submits;if(f.held)assert(f.held==b.vertices);f.held=b.vertices;f.captured.assign(b.vertices,b.vertices+b.count);f.capturedMatrices=*b.matrices;f.capturedUv=b.uvCount;return f.submitReady;}};
 AnimationSourceRenderChildren fallback{this,[](void* u,const AnimationRenderCall&,AnimationRenderLocals&,uint32_t&){++static_cast<Fixture*>(u)->delegates;return false;},nullptr};
 FieldAnimationGpu gpu{memory,physical,submit,fallback,[this]{return valid;}};
 bool call(const AnimationRenderCall& q){auto s=gpu.services();return s.call(s.user,q,local,result);}
 bool fifo(uint16_t n=0){auto s=gpu.services();return s.fifo(s.user,Fifo,n);}
 void projection(){cameraState.projectionKind=1;for(unsigned i=0;i<4;++i)cameraState.matrices.projection[i][i]=1;assert(gpu.camera({CameraDrawRequestKind::Projection,4,&cameraState}));}
 void matrices(){projection();local.matrices[0]=identity();local.matrices[1]=identity();assert(call(request(PositionMatrix,{{K::Matrix,0},{K::Scalar,0}})));assert(call(request(NormalMatrix,{{K::Matrix,1},{K::Scalar,0}})));assert(call(request(CurrentMatrix,{{K::Scalar,0}})));assert(call(request(Cull,{{K::Scalar,0}})));}
 void binding(unsigned i,bool allocate=true){const uint32_t attr=i<3?Position+i:FirstUv+i-3,stride=i==2?4:i<2?12:8,base=Data+AttributeSpacing*i;
  assert(call(request(Descriptor,{{K::Scalar,attr},{K::Scalar,Index16}})));assert(call(request(Format,{{K::Scalar,0},{K::Scalar,attr},{K::Scalar,i==1?0u:1u},{K::Scalar,i==2?Rgba8:Float32},{K::Scalar,0}})));assert(call(request(Array,{{K::Scalar,attr},{K::Guest,base},{K::Scalar,stride}})));
  if(allocate){assert(memory.registerRam(base,std::vector<uint8_t>(stride)));if(i==2){for(unsigned c=0;c<4;++c)assert(memory.writeByte(base+c,uint8_t(16+c)));}else for(unsigned c=0;c<stride/4;++c)assert(memory.writeWord(base+4*c,bits(float(10*i+c+1))));}
 }
 void prepare(unsigned uv=0){matrices();for(unsigned i=0;i<3+uv;++i)binding(i);}
 bool begin(uint32_t n){if(!gpu.active())held=nullptr;return call(request(Begin,{{K::Scalar,TriangleFan},{K::Scalar,0},{K::Scalar,n}}));}
 void fan(unsigned n,unsigned uv=0){for(unsigned i=0;i<n*(3+uv);++i)assert(fifo());}
};
void matrixAndCullRetention(){
 auto f=std::make_unique<Fixture>();auto& t=*f;auto pos=request(PositionMatrix,{{K::Matrix,0},{K::Scalar,0}});t.local.matrices[0]=identity();(*t.local.matrices[0])[3]=7;
 assert(!t.call(pos) && !t.gpu.matrices().position);(*t.local.matrices[0])[3]=99;t.projection();t.physicalReady=false;assert(!t.call(pos));assert(!t.call(request(CurrentMatrix,{{K::Scalar,0}})));t.physicalReady=true;assert(t.call(pos) && (*t.gpu.matrices().position)[3]==7);
 assert(!t.call(request(NormalMatrix,{{K::Matrix,1},{K::Scalar,0}})));t.local.matrices[1]=identity();(*t.local.matrices[1])[0]=std::numeric_limits<float>::infinity();assert(t.call(request(NormalMatrix,{{K::Matrix,1},{K::Scalar,0}})));assert(bits((*t.gpu.matrices().normal)[0])==0x7f800000);
 assert(t.call(request(Cull,{{K::Scalar,0}})));auto cull=request(Cull,{{K::Scalar,1}});t.physicalReady=false;assert(!t.call(cull));assert(!t.call(request(Cull,{{K::Scalar,2}})));assert(!t.gpu.camera({CameraDrawRequestKind::Projection,4,&t.cameraState}));assert(!t.call(request(ClearDescriptors,{})));t.physicalReady=true;assert(t.gpu.restore() && t.lastCull==0 && t.gpu.matrices().cull==0);t.mutate=&cull;assert(t.call(cull));assert(t.lastCull==1 && t.gpu.matrices().cull==1 && cull.arguments[0].value==2);
 t.valid=false;const auto before=t.cameras;assert(!t.gpu.restore() && !t.call(cull) && !t.fifo() && t.cameras==before);
}
void allLanesAndSubmissionHold(){
 auto f=std::make_unique<Fixture>();auto& t=*f;t.prepare(UvCount);assert(t.begin(3));t.submitReady=false;
 for(unsigned i=0;i<3*AttributeCount-1;++i)assert(t.fifo());assert(!t.fifo() && t.gpu.active() && t.submits==1 && t.gpu.stagedVertices()==3);
 assert(t.capturedUv==8 && t.captured.size()==3);const auto saved=t.captured;assert(saved[0].position[0]==1 && saved[0].normal[2]==13 && saved[0].color[3]==19 && saved[0].uv[7][1]==102);
 assert(!t.fifo(1) && t.submits==1);assert(!t.call(request(ClearDescriptors,{})) && !t.begin(4));assert(!t.gpu.camera({CameraDrawRequestKind::Projection,4,&t.cameraState}));
 assert(t.memory.writeWord(Data,bits(999)));assert(t.memory.writeWord(Data+10*AttributeSpacing,bits(999)));assert(!t.fifo() && t.submits==2);assert(std::memcmp(saved.data(),t.captured.data(),saved.size()*sizeof(FieldAnimationVertex))==0);
 assert(t.gpu.restore() && t.submits==2);t.submitReady=true;assert(t.fifo() && !t.gpu.active() && t.gpu.submittedFans()==1 && t.gpu.submittedVertices()==3);
 assert(t.call(request(ClearDescriptors,{}))); // Formats and arrays survive GXClearVtxDesc.
 for(unsigned a:{Position,Normal,Color})assert(t.call(request(Descriptor,{{K::Scalar,a},{K::Scalar,Index16}})));
 assert(t.begin(3));t.fan(3);assert(t.gpu.submittedFans()==2 && t.capturedUv==0 && t.captured[0].position[0]==999);
}
void partialArrayRepair(){
 auto f=std::make_unique<Fixture>();auto& t=*f;t.matrices();t.binding(0,false);t.binding(1);t.binding(2);
 assert(t.memory.registerRam(Data,{0x3f,0x80,0,0}));assert(t.begin(3));assert(!t.fifo());assert(!t.fifo(1));assert(t.memory.writeWord(Data,bits(99)));assert(t.memory.registerRam(Data+4,std::vector<uint8_t>(8)));assert(t.memory.writeWord(Data+4,bits(2)) && t.memory.writeWord(Data+8,bits(3)));assert(t.fifo());
 for(unsigned i=1;i<9;++i)assert(t.fifo());assert(t.captured[0].position[0]==1 && t.captured[0].position[1]==2 && t.captured[1].position[0]==99);
}
void countsCapacityAndBoundaries(){
 auto f=std::make_unique<Fixture>();auto& t=*f;assert(t.begin(0x10000) && !t.gpu.active() && t.gpu.capacity()==0 && !t.fifo());assert(!t.begin(1));t.prepare();
 assert(t.begin(1));t.fan(1);assert(t.submits==0);assert(t.begin(2));t.fan(2);assert(t.submits==0);
 assert(t.begin(MaxVertices) && t.gpu.capacity()==MaxVertices);t.fan(MaxVertices);assert(t.captured.size()==MaxVertices && t.submits==1);
 const auto capacity=t.gpu.capacity();assert(t.begin(65));t.fan(65);assert(t.gpu.capacity()==capacity && t.gpu.submittedVertices()==MaxVertices+65);
 assert(t.call(request(Cull,{{K::Scalar,3}})));assert(t.begin(3));t.fan(3);assert(t.submits==2);
 assert(!t.call(request(CurrentMatrix,{{K::Scalar,1}})));assert(!t.call(request(Descriptor,{{K::Scalar,FirstUv+UvCount},{K::Scalar,Index16}})));assert(!t.call(request(Descriptor,{{K::Scalar,Position},{K::Scalar,2}})));
 assert(!t.call(request(0x800208c0,{})) && t.delegates==1);t.valid=false;assert(!t.begin(0) && !t.call(request(0x800208c0,{})) && t.delegates==1);
}
constexpr uint32_t B=0x803d8e70,N=0x81f00000;
struct NativeFixture {
 std::shared_ptr<NativeFieldAnimation> native;bool valid=true;int token=0;uint64_t pass=95;uint32_t handle=0;
 NativeFixture(){assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),p);assert(native);const auto h=native->pool()->enter(fieldPlayerPoseResources()[0],2,{});assert(h);handle=uint32_t(*h);assert(native->pool()->selectName(*h,"M_W_1",true,{}));pose().word(0,pose().word(0)&~8u);pose().word(0xf4,0xffffff80);for(unsigned i=0;i<17;++i)assert(native->tick(false));}
 EventAddressSpace& memory(){return native->process().vm().memory;}
 BattlePose& pose(){return *native->pool()->pose(int32_t(handle));}
 uint32_t word(uint32_t a){uint32_t w;assert(memory().readWord(a,w));return w;}
 void store(uint32_t a,uint32_t w){assert(memory().writeWord(a,w));}
 bool draw(){FieldNpcDrawRequest q;q.function=0x8003b4d8;q.args={handle,0,2};q.scalars={0,1};q.matrix={1,0,0,4,0,1,0,5,0,0,1,6};FieldNpcDrawResponse r;auto s=native->npcAnimationServices();return s.request(s.user,memory(),q,r);}
 void simple(bool transformed=true){assert(memory().registerRam(N,std::vector<uint8_t>(96)));for(unsigned i=0;i<24;++i)store(N+4*i,bits(i>=3 && i<=5?1.f:0.f));if(transformed){for(unsigned i=0;i<3;++i){store(N+4*i,bits(3.f+i));store(N+0xc+4*i,bits(2.f+i));}}store(0x8041e89c,0);store(0x8041e8a0,B);const float initial[]{1,0,0,4,0,1,0,5,0,0,1,6};for(unsigned i=0;i<12;++i)store(B+4*i,bits(initial[i]));}
};

void originalGraphRawArrays(){
 NativeFixture f;auto& n=*f.native;unsigned material=0,submitted=0;bool ready=false;
 FieldAnimationPhysical physical{nullptr,[](void*,const NativeCameraPacket&,const NativeCameraDraw&){return true;},[](void*,uint32_t){return true;}};
 struct Probe {bool* ready;unsigned* submitted;unsigned* material;} probe{&ready,&submitted,&material};
 FieldAnimationSubmit submit{&probe,[](void* u,const FieldAnimationBatch& batch){auto& p=*static_cast<Probe*>(u);assert(batch.count>=3 && batch.uvCount<=8 && batch.matrices->position && batch.matrices->normal);for(unsigned i=0;i<batch.count;++i)for(float x:batch.vertices[i].position)assert(std::isfinite(x));if(!*p.ready)return false;++*p.submitted;return true;}};
 AnimationSourceRenderChildren fallback{&probe,[](void* u,const AnimationRenderCall&,AnimationRenderLocals&,uint32_t&){++*static_cast<Probe*>(u)->material;return true;},nullptr};
 FieldAnimationGpu gpu(f.memory(),physical,submit,fallback,[&]{return f.valid;});
 auto camera=std::make_shared<CameraManager>();camera->initializeBoot(608,480);
 for(unsigned i=0;i<13;++i)camera->camera(i)->publication.flags=1;camera->camera(4)->publication.flags=0;
 auto queue=std::make_unique<DisplayQueue>();CameraDrawCursor cursor;CameraDrawServices cs;cs.letterbox=[](void*,DisplayQueue&){return true;};cs.request=[](void*,const CameraDrawRequest&){return false;};assert(!camera->draw(*queue,cs,cursor) && camera->currentIndex()==4);
 auto& c=*camera->camera(4);c.viewPublished=true;for(unsigned r=0;r<3;++r)for(unsigned col=0;col<4;++col)c.matrices.view[r][col]=r==col?1.f:0.f;
 assert(gpu.camera({CameraDrawRequestKind::Projection,4,&c}));
 NativeNpcInnerBackend backend;NativeNpcGroupBackend groups;groups.nativeRenderSdk=true;groups.renderChildren=gpu.services();backend.groups=groups;backend.gx=[](void*,EventAddressSpace&,const BattleAudienceGXRequest&){return true;};backend.flush=[](void*,EventAddressSpace&,uint32_t,uint32_t){return true;};backend.invalidate=[](void*,EventAddressSpace&){return true;};assert(n.configureNpcInnerBackend(backend));assert(n.beginNpcPass(&f.token,++f.pass,camera,[&]{return f.valid;}));
 for(unsigned i=0;i<10000 && !gpu.active();++i)assert(!f.draw());
 assert(gpu.active() && gpu.stagedVertices()>0 && !submitted && material);assert(!n.endNpcPass(&f.token,f.pass) && !n.tick(false) && !n.configureNpcInnerBackend(backend));
 f.valid=false;assert(!f.draw() && !submitted);f.valid=true;ready=true;bool done=false;for(unsigned i=0;i<10000 && !done;++i)done=f.draw();assert(done && submitted && gpu.submittedFans()==submitted && n.endNpcPass(&f.token,f.pass));
} // Original VM arrays and fan indices; material/texture and raster acknowledgement are explicit test fixtures.

}
int main(){matrixAndCullRetention();allLanesAndSubmissionHold();partialArrayRepair();countsCapacityAndBoundaries();originalGraphRawArrays();}
