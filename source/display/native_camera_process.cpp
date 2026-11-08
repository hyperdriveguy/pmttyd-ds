#include "native_camera_process.hpp"
#include "native_shadow_process.hpp"
#include "native_ui_startup.hpp"
#include "native_field_animation.hpp"
#include "event_heap_allocation.hpp"
#include "field_map_sdk.hpp"
#include <cmath>
#include <cstring>
#include <string>
namespace ttyd {
namespace {
constexpr uint32_t StackAddress=0x8d020000,StackBytes=32*1024,StackGuard=32;
constexpr uint32_t HeapAllocate=0x800300f0,Fill=0x800050b4,CopyString=0x8026d5fc,Distance=0x8029eeb4,Tangent=0x802724e8,FloatTangent=0x80272864;
const char* const Names[]{"off","off2","shadow","bg","3d","3deff_A","3dimg","3deff_B","2d","fade","fade2","dbg","dbg3d"};
double real(uint64_t b){double v;std::memcpy(&v,&b,sizeof(v));return v;}
uint64_t bits(double v){uint64_t b;std::memcpy(&b,&v,sizeof(b));return b;}
float single(uint32_t b){float v;std::memcpy(&v,&b,sizeof(v));return v;}
struct Literal {uint32_t address;std::vector<uint8_t> bytes;};
}
NativeCameraNumerics nativeCameraLibmNumerics(){NativeCameraNumerics n;
 n.tangent=[](uint32_t function,double input)->std::optional<double>{if((function!=Tangent&&function!=FloatTangent)||!std::isfinite(input))return {};return std::tan(input);};
 n.reciprocalSquareRoot=[](float input)->std::optional<double>{if(!std::isfinite(input)||input<=0)return {};return 1./std::sqrt(double(input));};return n;
}
struct NativeCameraProcess::State {
 std::shared_ptr<NativeShadowProcess> shadows;
 NativeCameraNumerics numeric;
 std::array<uint8_t,CameraCount*sizeof(uint32_t)> table{};
 std::array<uint8_t,RenderBytes> render{};
 std::array<uint8_t,sizeof(uint32_t)> renderPointer{};
 std::array<uint8_t,StackBytes> stack{};
 struct Entry {uint32_t address=0;std::vector<uint8_t> bytes;bool mapped=false;};
 std::array<Entry,CameraCount> entries;
 unsigned allocated=0,owned=0,literalCount=0;
 std::vector<Literal> literals;
 uint32_t descriptor=0,heapHandle=0,heapStart=0,heapEnd=0,sdkDescriptors=0;bool renderDone=false,initialized=false;
 Counts counts;
 SourceCameraCursor cursor;
 std::unique_ptr<FieldMapSdk> sdk;
 std::unique_ptr<FieldCameraRoadOwner> road;
 FieldMapSourceCall distance;
 bool distanceActive=false;
 std::weak_ptr<CameraManager> published;bool publishedOnce=false;
 EventNativeRuntime& vm()const{return shadows->ui()->fieldProcess()->process().vm();}
 ~State(){road.reset();sdk.reset();for(auto& e:entries)if(e.mapped)vm().memory.unregisterExternalRam(e.address,e.bytes.data());while(owned){--owned;const uint32_t a[]={Table,RenderObject,RenderPointer,StackAddress};const uint8_t* b[]={table.data(),render.data(),renderPointer.data(),stack.data()};vm().memory.unregisterExternalRam(a[owned],b[owned]);}while(literalCount){auto& l=literals[--literalCount];vm().memory.unregisterExternalRam(l.address,l.bytes.data());}}
 bool heapCurrent()const{
  auto& m=vm().memory;uint32_t handle=0,first=0,last=0,descriptors=0,size=0,head=0;
  if(!m.readWord(0x8041e844,handle)||handle!=heapHandle||!m.readWord(0x8041e830,first)||first!=heapStart||!m.readWord(0x8041e81c,last)||last!=heapEnd||!m.readWord(0x8041f070,descriptors)||descriptors!=sdkDescriptors||!m.readWord(descriptors+12*handle,size)||size!=heapEnd-heapStart||!m.readWord(descriptors+12*handle+8,head))return false;
  if(!allocated)return true;
  // Actual SDK allocated-list membership, not merely equal payload bytes.
  // Stop after all camera nodes are found; unrelated older allocations need
  // not be traversed on each source primitive. Bounds bound malformed cycles.
  unsigned found=0;uint32_t mask=0,previous=0;unsigned budget=(heapEnd-heapStart)/32;
  while(head&&budget--){uint32_t prev=0,next=0,bytes=0;
   if(head<heapStart||uint64_t(head)+32>heapEnd||!m.readWord(head,prev)||prev!=previous||!m.readWord(head+4,next)||!m.readWord(head+8,bytes)||bytes<32||uint64_t(head)+bytes>heapEnd)return false;
   for(unsigned i=0;i<allocated;++i)if(entries[i].address==head+32){if(bytes<CameraBytes+32||(mask&(1u<<i)))return false;mask|=1u<<i;++found;}
   if(found==allocated)return true;
   previous=head;head=next;
  }return false;
 }
 bool current()const{
  if(!shadows||!shadows->ready()||!shadows->ui()->fieldProcess()->process().heapGlobalsCurrent())return false;
  auto& m=vm().memory;const uint32_t a[]={Table,RenderObject,RenderPointer,StackAddress};const uint8_t* b[]={table.data(),render.data(),renderPointer.data(),stack.data()};const uint32_t n[]={uint32_t(table.size()),RenderBytes,4,StackBytes};
  for(unsigned i=0;i<owned;++i)if(!m.hasRamBacking(a[i],b[i],n[i]))return false;
  for(unsigned i=0;i<literalCount;++i)if(!m.hasRamBacking(literals[i].address,literals[i].bytes.data(),literals[i].bytes.size()))return false;
  for(unsigned i=0;i<allocated;++i){const auto& e=entries[i];if(!e.mapped||!m.hasRamBacking(e.address,e.bytes.data(),CameraBytes))return false;}
  if(!heapCurrent())return false;
  if(renderDone){uint32_t pointer=0;if(!m.readWord(RenderPointer,pointer)||pointer!=RenderObject)return false;}
  if(initialized)for(unsigned i=0;i<CameraCount;++i){uint32_t pointer=0;if(!m.readWord(Table+4*i,pointer)||pointer!=entries[i].address)return false;}
  return !road||road->current();
 }
 bool install(const char* root){
  auto& m=vm().memory;EventScriptAssets source,roads;if(!source.load((std::string(root)+"/source-camera-literals.pmevt").c_str())||!roads.load((std::string(root)+"/field-camera-road-literals.pmevt").c_str())||!source.merge(roads))return false;
  for(const auto& segment:source.segments())for(uint32_t i=0;i<segment.size;++i){const uint32_t at=segment.address+i;uint8_t expected=0,actual=0;if(!source.readByte(at,expected))return false;if(m.readByte(at,actual)){if(actual!=expected)return false;continue;}if(!m.unmappedRange(at,1))return false;if(literals.empty()||uint64_t(literals.back().address)+literals.back().bytes.size()!=at)literals.push_back({at,{}});literals.back().bytes.push_back(expected);}
  for(auto& l:literals){if(!m.registerExternalRam(l.address,l.bytes.data(),l.bytes.size()))return false;++literalCount;}
  const uint32_t a[]={Table,RenderObject,RenderPointer,StackAddress};uint8_t* b[]={table.data(),render.data(),renderPointer.data(),stack.data()};const uint32_t n[]={uint32_t(table.size()),RenderBytes,4,StackBytes};
  for(unsigned i=0;i<4;++i){if(!m.registerExternalRam(a[i],b[i],n[i]))return false;++owned;}return true;
 }
 bool child(SourceCameraCall& q){if(!current())return false;auto& m=vm().memory;const auto& a=q.arguments;
  if(q.function==HeapAllocate){if(a[0]!=0||a[1]!=CameraBytes||allocated>=CameraCount)return false;
   auto& e=entries[allocated];
   if(!e.address){e.address=eventHeapAllocate(0,CameraBytes,vm().memoryBinding.services());++counts.allocations;if(!e.address){q.result=0;return current();}}
   // Isolate the complete real allocation before a constructor/provider can
   // borrow it. SDK headers remain in their actual heap at unchanged addresses.
   if(e.bytes.empty()&&!m.extractRam(e.address,CameraBytes,e.bytes))return false;
   if(!m.registerExternalRam(e.address,e.bytes.data(),e.bytes.size()))return false;
   e.mapped=true;++allocated;q.result=e.address;return current();
  }
  if(q.function==Fill){if(a[1]||a[2]!=CameraBytes||!m.contiguousRam(a[0],a[2]))return false;q.result=eventFillMemory(a[0],0,a[2],vm().memoryBinding.services());++counts.clears;return current();}
  if(q.function==CopyString){
   // All source names fit their actual12-byte field. Read every source byte
   // before this atomic finite copy; unavailable data yields without stores.
   std::array<uint8_t,12> name{};unsigned length=0;
   for(;length<name.size();++length){if(!m.readByte(a[1]+length,name[length]))return false;if(!name[length])break;}
   if(length==name.size()||!m.contiguousRam(a[0],length+1))return false;
   for(unsigned i=0;i<=length;++i)if(!m.writeByte(a[0]+i,name[i]))return false;
   q.result=a[0];++counts.copies;return current();
  }
  if(q.function==Distance){
   if(!distanceActive){distance={};distance.function=Distance;distance.arguments=q.arguments;distanceActive=true;}
   if(distance.arguments!=q.arguments||!sdk->advance(distance)||!distance.floatingResult||!current())return false;
   q.floatingResult=bits(double(single(*distance.floatingResult)));distanceActive=false;++counts.distances;return true;
  }
  if(q.function==Tangent||q.function==FloatTangent){if(!numeric.tangent)return false;const auto value=numeric.tangent(q.function,real(q.scalars[0]));if(!value||!current())return false;q.floatingResult=bits(*value);++counts.tangents;return true;}
  return false;
 }
 SourceCameraServices services(){SourceCameraServices s;s.user=this;
  s.word=[](void* p,uint32_t a,uint32_t& v){auto& s=*static_cast<State*>(p);return s.current()&&s.vm().memory.readWord(a,v);};
  s.half=[](void* p,uint32_t a,uint16_t& v){auto& s=*static_cast<State*>(p);return s.current()&&s.vm().memory.readHalf(a,v);};
  s.store=[](void* p,uint32_t a,uint32_t v){auto& s=*static_cast<State*>(p);return s.current()&&s.vm().memory.writeWord(a,v);};
  s.storeHalf=[](void* p,uint32_t a,uint16_t v){auto& s=*static_cast<State*>(p);return s.current()&&s.vm().memory.writeHalf(a,v);};
  s.call=[](void* p,SourceCameraCall& q){return static_cast<State*>(p)->child(q);};return s;
 }
};
NativeCameraProcess::NativeCameraProcess()=default;NativeCameraProcess::~NativeCameraProcess()=default;
std::shared_ptr<NativeCameraProcess> NativeCameraProcess::create(std::shared_ptr<NativeShadowProcess> shadows,const char* root,NativeCameraNumerics numeric,uint32_t descriptor){
 if(!shadows||!root||!shadows->ready()||shadows->busy()||(descriptor!=NtscInterlaced&&descriptor!=NtscProgressive))return {};
 auto s=std::make_shared<State>();s->shadows=std::move(shadows);s->numeric=std::move(numeric);s->descriptor=descriptor;
 if(!s->vm().memory.readWord(0x8041e844,s->heapHandle)||!s->vm().memory.readWord(0x8041e830,s->heapStart)||!s->vm().memory.readWord(0x8041e81c,s->heapEnd)||!s->vm().memory.readWord(0x8041f070,s->sdkDescriptors)||s->heapEnd<=s->heapStart||!s->install(root)||!s->current())return {};
 s->sdk=std::make_unique<FieldMapSdk>(s->vm().memory,[state=s.get()]{return state->current();},[state=s.get()](float x)->std::optional<double>{return state->numeric.reciprocalSquareRoot?state->numeric.reciprocalSquareRoot(x):std::nullopt;});
 auto n=std::shared_ptr<NativeCameraProcess>(new NativeCameraProcess);n->state_=std::move(s);return n;
}
bool NativeCameraProcess::initialize(unsigned visits){
 if(invoking_||!current())return false;
 invoking_=true;struct Guard{bool& b;~Guard(){b=false;}}g{invoking_};auto& s=*state_;
 if(!s.initialized){
 SourceCameraCall input;
 if(!s.renderDone){input.function=SourceCameraBootstrap::SelectedModeCopy;input.arguments[0]=s.descriptor;if(!resumeSourceCamera(input,StackAddress+StackBytes-StackGuard,s.services(),s.cursor,visits))return false;s.renderDone=true;if(!s.current())return false;}
 input={};input.function=SourceCameraBootstrap::Init;if(!resumeSourceCamera(input,StackAddress+StackBytes-StackGuard,s.services(),s.cursor,visits))return false;
 s.initialized=true;
 }
 if(!s.road){auto& e=s.entries[FieldCameraRoadOwner::FieldCamera];s.road=FieldCameraRoadOwner::bind(s.vm().memory,StackAddress+2*StackBytes,{s.table.data()+4*FieldCameraRoadOwner::FieldCamera,e.address,e.bytes.data()});}
 return ready();
}
bool NativeCameraProcess::current()const{return state_&&state_->current();}
bool NativeCameraProcess::ready()const{return current()&&state_->initialized&&state_->road&&state_->road->current();}
bool NativeCameraProcess::busy()const{return invoking_||(state_&&(state_->cursor.active||(state_->road&&state_->road->pending())));}
uint32_t NativeCameraProcess::pending()const{return state_?state_->cursor.pc:0;}
FieldCameraRoadOwner* NativeCameraProcess::roads()const{return ready()?state_->road.get():nullptr;}
NativeCameraProcess::Counts NativeCameraProcess::counts()const{return state_?state_->counts:Counts{};}
std::shared_ptr<NativeShadowProcess> NativeCameraProcess::shadows()const{return state_?state_->shadows:nullptr;}
std::optional<FieldCameraEntryBorrow> NativeCameraProcess::fieldCamera()const{if(!ready())return {};auto& s=*state_;auto& e=s.entries[FieldCameraRoadOwner::FieldCamera];return FieldCameraEntryBorrow{s.table.data()+4*FieldCameraRoadOwner::FieldCamera,e.address,e.bytes.data()};}
bool NativeCameraProcess::bootPublishedFor(const std::shared_ptr<CameraManager>& manager)const{return ready()&&manager&&manager->sourceBootPublished_&&state_->published.lock()==manager;}
bool NativeCameraProcess::publishBoot(const std::shared_ptr<CameraManager>& target){
 if(!ready()||!target)return false;
 if(state_->publishedOnce)return bootPublishedFor(target);
 auto& manager=*target;
 if(manager.current_||manager.sourceBootPublished_)return false;
 for(const auto& c:manager.cameras_)if(c.viewPublished)return false;
 std::array<CameraState,CameraCount> bank{};auto& m=state_->vm().memory;
 auto w=[&](uint32_t a,uint32_t& v){return m.readWord(a,v);};auto f=[&](uint32_t a,float& v){uint32_t b;if(!w(a,b))return false;v=single(b);return true;};
 for(unsigned i=0;i<CameraCount;++i){auto& c=bank[i];const auto a=state_->entries[i].address;
  for(unsigned j=0;j<12;++j){uint8_t ch;if(!m.readByte(a+0x254+j,ch)||ch!=uint8_t(Names[i][j]))return false;if(!ch)break;}
  c.name=Names[i];if(!m.readHalf(a,c.publication.flags)||!m.readHalf(a+2,c.mode))return false;
  for(unsigned j=0;j<3;++j)if(!f(a+0xc+4*j,c.eye[j])||!f(a+0x18+4*j,c.at[j])||!f(a+0x24+4*j,c.up[j])||!f(a+0x150+4*j,c.publication.translation[j]))return false;
  if(!f(a+0x30,c.nearPlane)||!f(a+0x34,c.farPlane)||!f(a+0x38,c.fov)||!f(a+0x3c,c.aspect)||!f(a+0x114,c.yaw)||!f(a+0x118,c.distance)||!f(a+0x14c,c.publication.rollDegrees)||!w(a+0x19c,c.projectionKind)||!w(a+0x1ec,c.callback)||!w(a+0x1e8,c.drawCamera))return false;
  for(unsigned j=0;j<6;++j)if(!f(a+0xfc+4*j,c.viewport[j]))return false;
  for(unsigned j=0;j<4;++j)if(!m.readHalf(a+0xf4+2*j,c.scissor[j]))return false;
  for(unsigned row=0;row<4;++row)for(unsigned col=0;col<4;++col)if(!f(a+0x15c+4*(4*row+col),c.matrices.projection[row][col])||!f(a+0x1a0+4*(4*row+col),c.publication.extra[row][col]))return false;
  for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col)if(!f(a+0x11c+4*(4*row+col),c.matrices.view[row][col]))return false;
 }
 std::array<uint8_t,4> dimensions{};for(unsigned i=0;i<4;++i)if(!m.readByte(RenderObject+4+i,dimensions[i]))return false;
 if(!current())return false;
 manager.cameras_=bank;manager.sourceRenderDimensions=dimensions;manager.renderDimensionsPublished_=true;manager.sourceBootPublished_=true;state_->published=target;state_->publishedOnce=true;return true;
}
}
