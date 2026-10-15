#include "animation_scratch_memory.hpp"
#include "animation_sample_time.hpp"
#include "event_address_space.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
float rounded(float v){volatile float f=v;return f;}
int32_t signedWord(uint32_t v){return v<=0x7fffffffu?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
struct Memory {
 EventAddressSpace& m;EventAddressSpace::RawRamAccess* ram=nullptr;bool ok=true;
 uint32_t word(uint32_t a){uint32_t v=0;ok=(ram?ram->readWord(a,v):m.readWord(a,v))&&ok;return v;}
 uint8_t byte(uint32_t a){uint8_t v=0;ok=(ram?ram->readByte(a,v):m.readByte(a,v))&&ok;return v;}
 float real(uint32_t a){const uint32_t v=word(a);float f;std::memcpy(&f,&v,4);return f;}
 void word(uint32_t a,uint32_t v){const bool done=ram?ram->writeWord(a,v):m.writeWord(a,v);assert(done);ok=done&&ok;}
 void byte(uint32_t a,uint8_t v){const bool done=ram?ram->writeByte(a,v):m.writeByte(a,v);assert(done);ok=done&&ok;}
 void real(uint32_t a,float f){uint32_t v;std::memcpy(&v,&f,4);word(a,v);}
 bool backed(uint32_t a,uint32_t n){ok=(ram?ram->bind(a,n):m.contiguousRam(a,n))&&ok;return ok;}
 void copy(uint32_t dst,uint32_t src,uint32_t n){if(ram && ram->copy(dst,src,n))return;uint32_t o=0;for(;o+4<=n;o+=4)word(dst+o,word(src+o));for(;o<n;++o)byte(dst+o,byte(src+o));}
};
// Ordinary private/nonalias domain. Checks are native admission policy; original
// Main has no malformed-resource guards. Zero-size source pointers are ignored.
struct View {
 Memory mem;NativeFileCache& files;const BattlePose& p;uint32_t model=0,timeline=0,work=0;
 std::array<uint32_t,5> counts{},cache{},output{};
 static constexpr uint32_t widths[5]={12,12,1,4,24};
 static constexpr uint32_t countOffsets[5]={0xf0,0xf8,0x13c,0x140,0x12c};
 static constexpr uint32_t baseOffsets[5]={0x154,0x15c,0x1a0,0x1a4,0x190};
 static constexpr uint32_t cacheOffsets[5]={0x48,0x50,0x58,0x60,0x68};
 static constexpr uint32_t outputOffsets[5]={0x4c,0x54,0x5c,0x64,0x6c};
 static constexpr uint32_t pairOffsets[5]={4,12,28,36,20};
 static constexpr uint32_t streamOffsets[5]={0x2c,0x30,0x38,0x3c,0x34};
 static constexpr uint32_t capacityOffsets[5]={0xc,0x10,0x18,0x1c,0x14};
 static constexpr uint32_t strides[5]={4,4,2,4,12};
 bool file(uint32_t a,uint64_t n){return n==0 || (n<=0xffffffff && uint64_t(a)+n<=0x100000000ULL && files.ownsPayload(a,uint32_t(n)) && mem.backed(a,uint32_t(n)));}
 bool init(){
  if(!p.resource || !p.resource->draw || !p.resource->bank || p.word(0x10)>=64 || p.word(0x14)>=p.resource->names.size())return false;
  work=mem.word(0x80415dc0);const uint32_t models=mem.word(work),record=models+16*p.word(0x10);
  const uint32_t holder=mem.word(mem.word(record+8)+0xa0);model=mem.word(holder);
  if(!mem.ok || !model || !file(model,0x1b0) || mem.word(work+0xe8) || p.word(0x90)!=0xffffffff)return false;
  const uint32_t directory=mem.word(model+0x1ac);if(!file(directory+64*p.word(0x14),64))return false;
  timeline=mem.word(directory+64*p.word(0x14)+0x3c);
  if(!timeline || !file(timeline,0x44))return false;
  const uint32_t scratch=mem.word(work+0x104),end=mem.word(work+0x108);
  if(uint64_t(scratch)+0x180000>0xffffffff || end<scratch || end>scratch+0x180000)return false;
  struct Range{uint32_t begin,end;};std::array<Range,8> ranges{};unsigned rangeCount=0;
  for(unsigned c=0;c<5;++c){
   counts[c]=mem.word(model+countOffsets[c]);
   if(counts[c]!=p.resource->draw->word(countOffsets[c]))return false;
   cache[c]=p.word(cacheOffsets[c]);output[c]=p.word(outputOffsets[c]);
   if((c==2 || c==4) && cache[c]!=output[c])return false;
   const uint64_t n=uint64_t(counts[c])*widths[c];
   if(n>0x180000)return false;
   if(!n)continue;
   for(unsigned which=0;which<((c==2 || c==4)?1u:2u);++which){
    const uint32_t a=which?output[c]:cache[c];
    if(a<scratch || uint64_t(a)+n>end || (c!=2 && (a&3)))return false;
    for(unsigned i=0;i<rangeCount;++i)if(a<ranges[i].end && uint64_t(a)+n>ranges[i].begin)return false;
    ranges[rangeCount++]={a,uint32_t(a+n)};
    if(!mem.backed(a,uint32_t(n)))return false;
   }
  }
  return mem.ok;
 }
 bool snapshot(BattlePose& target,bool includeCache){
  const auto& p=target;
 auto decode=[&](auto& s,const std::array<uint32_t,5>& pointers){
  s.positions.resize(counts[0]);s.normals.resize(counts[1]);s.visibility.resize(counts[2]);s.nodes.resize(counts[3]);s.textures.resize(counts[4]);
  // Wrap metadata belongs to the static material sidecar, never scratch RAM.
  const auto* a=p.resource->bank->pose(p.resource->names[p.word(0x14)].c_str());if(!a)return false;s.surfaces=a->surfaces;
  for(unsigned c=0;c<2;++c){auto& values=c?s.normals:s.positions;for(uint32_t i=0;i<counts[c];++i)for(unsigned lane=0;lane<3;++lane)values[i][lane]=mem.real(pointers[c]+12*i+4*lane);}
  for(uint32_t i=0;i<counts[2];++i)s.visibility[i]=mem.byte(pointers[2]+i);
  for(uint32_t i=0;i<counts[3];++i)s.nodes[i]=mem.real(pointers[3]+4*i);
  for(uint32_t i=0;i<counts[4];++i){s.textures[i]=mem.byte(pointers[4]+24*i);if(!s.surfaces.empty()){if(i>=s.surfaces.size())return false;for(unsigned lane=0;lane<5;++lane)s.surfaces[i].transform[lane]=mem.real(pointers[4]+24*i+4+4*lane);}}
  return mem.ok;
 };
 return decode(target.shape,output) && (!includeCache || decode(target.cache,cache));
 }
 bool key(uint32_t key,unsigned c,bool cubic=false){
  const uint32_t offset=mem.word(key+pairOffsets[c]),n=mem.word(key+pairOffsets[c]+4);
  if(!n)return mem.ok;
  const uint32_t capacity=mem.word(timeline+capacityOffsets[c]),stream=mem.word(timeline+streamOffsets[c]);
  if(offset>capacity || n>capacity-offset || !file(stream,uint64_t(offset+n)*strides[c]))return false;
  uint64_t index=0;
  for(uint32_t i=0;i<n;++i){
   const uint32_t r=stream+(offset+i)*strides[c];index+=mem.byte(r);if(index>=counts[c])return false;
   if(cubic && c==3){const int a=int8_t(mem.byte(r+2)),b=int8_t(mem.byte(r+3));if(b>-89 && b<89 && (a<-89 || a>88))return false;}
  }
  return mem.ok;
 }
 void apply(uint32_t key,unsigned c,uint32_t target,float scale,bool fractional,float fraction,float duration,const AnimationTangentTable* tangents){
  const uint32_t offset=mem.word(key+pairOffsets[c]),n=mem.word(key+pairOffsets[c]+4);
  if(!n)return;
  const uint32_t stream=mem.word(timeline+streamOffsets[c]);
  for(uint32_t i=0;i<n;++i){
   const uint32_t r=stream+(offset+i)*strides[c];target+=mem.byte(r)*widths[c];
   if(c<2){for(unsigned lane=0;lane<3;++lane)mem.real(target+4*lane,std::fma(scale,float(int8_t(mem.byte(r+1+lane))),mem.real(target+4*lane)));}
   else if(c==2)mem.byte(target,uint8_t(mem.byte(target)+mem.byte(r+1)));
   else if(c==4){
    mem.byte(target,uint8_t(mem.byte(target)+mem.byte(r+1)));
    mem.real(target+4,rounded(mem.real(target+4)+mem.real(r+4)));
    mem.real(target+8,rounded(mem.real(target+8)+mem.real(r+8)));
   }else if(!fractional)mem.real(target,std::fma(.0625f,float(int8_t(mem.byte(r+1))),mem.real(target)));
   else {
    const int a=int8_t(mem.byte(r+2)),b=int8_t(mem.byte(r+3));if(b<=-89 || b>=89)continue;
    const float t2=rounded(fraction*fraction),t3=rounded(fraction*t2);
    const float h10=rounded(fraction-std::fma(2.f,t2,-t3)),h01=std::fma(-2.f,t3,rounded(3.f*t2));
    float x=rounded(h10*rounded(duration*tangents->at(unsigned(a+89))));
    x=std::fma(h01,rounded(.0625f*float(int8_t(mem.byte(r+1)))),x);
    x=std::fma(rounded(t3-t2),rounded(duration*tangents->at(unsigned(b+89))),x);
    mem.real(target,rounded(mem.real(target)+x));
   }
  }
 }
};
}
AnimationScratchMemory::AnimationScratchMemory(std::shared_ptr<BattlePosePool> pool,std::shared_ptr<NativeFileCache> files,EventAddressSpace& memory,const AnimationPoolMemory& arrays):pool_(pool),files_(std::move(files)),memory_(memory),arrays_(arrays){}
std::shared_ptr<AnimationScratchMemory> AnimationScratchMemory::bind(std::shared_ptr<BattlePosePool> pool,std::shared_ptr<NativeFileCache> files,EventAddressSpace& memory,const AnimationPoolMemory& arrays){
 if(!pool || !files || !arrays.aliases(*pool,memory) || !pool->animationTangents() || !pool->tangentSamplingAvailable())return {};
 return std::shared_ptr<AnimationScratchMemory>(new AnimationScratchMemory(std::move(pool),std::move(files),memory,arrays));
}
AnimationScratchMemory::~AnimationScratchMemory(){retire();}
bool AnimationScratchMemory::current() const{auto pool=pool_.lock();return active_ && pool && arrays_.aliases(*pool,memory_);}
void AnimationScratchMemory::retire(){
 if(!active_)return;
 const auto identity=weak_from_this(); // Control-block identity survives last-owner destruction and prevents address reuse.
 if(auto pool=pool_.lock())for(unsigned i=0;i<256;++i){auto* p=const_cast<BattlePose*>(pool->slot(i));if(p && p->sourceBuffers && p->sourceBuffers->identity_==this && !p->sourceBuffers->owner_.owner_before(identity) && !identity.owner_before(p->sourceBuffers->owner_) && p->sourceBuffers->generation_==p->nativeGeneration){if(snapshot(*p,true))p->sourceBuffers.reset();}}
 active_=false;
}
std::shared_ptr<AnimationScratchPose> AnimationScratchMemory::pose(unsigned handle,uint64_t generation){
 if(!current() || handle>=256)return {};
 return std::shared_ptr<AnimationScratchPose>(new AnimationScratchPose(shared_from_this(),handle,generation));
}
std::shared_ptr<AnimationScratchMemory> AnimationScratchPose::owner(const BattlePose& p) const{
 auto o=owner_.lock();if(!o || !o->current() || !generation_ || p.nativeGeneration!=generation_)return {};
 auto pool=o->pool_.lock();const auto* live=pool?pool->pose(int32_t(handle_)):nullptr;
 return live && live->nativeGeneration==generation_ && live->resource==p.resource?o:nullptr;
}
bool AnimationScratchPose::evaluate(BattlePose& p,BattlePoseClock clock) const{
 auto o=owner(p);if(!o)return false;auto pool=o->pool_.lock();const auto* tangents=pool->animationTangents();if(!tangents || !pool->tangentSamplingAvailable())return false;
 return o->memory_.withRawRamAccess([&](EventAddressSpace::RawRamAccess& ram){
 View v{{o->memory_,&ram},*o->files_,p};if(!v.init())return false;auto& m=v.mem;
 const uint32_t t=v.timeline,control=m.word(t+0x24),keys=m.word(t+0x28),discrete=m.word(t+0x40);
 const uint32_t nk=m.word(t+8),nv=m.word(t+0x20);
 if(!nk || nk>1024 || !nv || nv>65536 || !v.file(control,12) || !v.file(keys,44ull*nk) || !v.file(discrete,8ull*nv))return false;
 const uint32_t loop=m.word(control);const float start=m.real(control+4),end=m.real(control+8);
 if(loop>1 || !std::isfinite(start) || !std::isfinite(end) || end<start || !m.ok)return false;
 // Caller supplies the same millisecond quotient domain as the source clock
 // child. GQR/linked/ring/facing and arbitrary FP exception domains are separate.
 const uint64_t elapsed=(p.word(0xc)?clock.modeNonzero:clock.modeZero)-p.timestamp();
 p.scalar(0x20,rounded(rounded(rounded(float(elapsed))*60.f)/1000.f));p.scalar(0x20,rounded(p.scalar(0x20)*p.scalar(0x44)));
 const auto plan=animationSampleTimePlan(p.scalar(0x20),loop,start,end);p.scalar(0x84,plan.ratio);
 const auto selected=finishAnimationSampleTime(plan,p.scalar(0x84));if(!selected)return false;p.scalar(0x24,*selected);
 uint32_t k=0;while(k<nk && !(m.real(keys+44*k)>p.scalar(0x24)))++k;
 p.word(0x28,k-1);p.word(0x2c,k);if(!loop && k>=nk)p.word(0x2c,p.word(0x28));
 uint32_t d=0;while(d<nv && !(m.real(discrete+8*d)>p.scalar(0x24)))++d;
 p.word(0x34,d-1);p.word(0x38,d);if(!loop && d>=nv)p.word(0x38,p.word(0x34));
 if(!d || !k || !std::isfinite(p.scalar(0x24)) || !m.ok)return false;
 p.word(0x80,m.word(discrete+8*(d-1)+4));
 const uint32_t current=k-1;uint32_t next=p.word(0x2c);const float time=m.real(keys+44*current);
 float span=0;if(loop && next>=nk){next=0;p.word(0x2c,0);span=rounded(end-time);}else span=rounded(m.real(keys+44*next)-time);
 const float fraction=span==0?0:rounded(rounded(p.scalar(0x24)-time)/span);p.scalar(0x30,fraction);
 const int32_t previous=signedWord(p.word(0x3c));if(previous<-1 || !m.ok)return false;
 const bool reset=previous==-1 || previous>int32_t(current);const uint32_t first=reset?0:uint32_t(previous+1);
 // Preflight all dereferences before scratch writes. No waits/callbacks occur
 // inside the admitted nonalias phase, so source per-use reloads stay equivalent.
 if(reset)for(unsigned c=0;c<5;++c)if(!v.file(m.word(v.model+View::baseOffsets[c]),uint64_t(v.counts[c])*View::widths[c]))return false;
 for(uint32_t i=first;i<=current;++i)for(unsigned c=0;c<5;++c)if(!v.key(keys+44*i,c))return false;
 if(current<next)for(unsigned c:{0u,1u,3u})if(!v.key(keys+44*next,c,true))return false;
 if(!m.ok)return false;
 if(reset){
  for(unsigned c=0;c<5;++c)if(v.counts[c])m.copy(p.word(View::cacheOffsets[c]),m.word(v.model+View::baseOffsets[c]),v.counts[c]*View::widths[c]);
  if(p.word(0x3c)==0xffffffff)p.word(0x3c,0);
 }else if(previous==int32_t(current) && p.scalar(0x40)>fraction)p.scalar(0x40,0);
 for(uint32_t i=first;i<=current;++i)for(unsigned c=0;c<5;++c)v.apply(keys+44*i,c,p.word(View::cacheOffsets[c]),.0625f,false,0,0,tangents);
 if(current<=next)for(unsigned c:{0u,1u,3u}){
  if(v.counts[c])m.copy(p.word(View::outputOffsets[c]),p.word(View::cacheOffsets[c]),v.counts[c]*View::widths[c]);
  if(current<next)v.apply(keys+44*next,c,p.word(View::outputOffsets[c]),rounded(.0625f*fraction),true,fraction,rounded(m.real(keys+44*next)-time),tangents);
 }
 p.word(0x3c,current);p.scalar(0x40,fraction);
 // Recheck ownership/layout after source writes. Remapped external backing
 // can physically alias a MODEL field despite distinct guest addresses.
 if(!m.ok || !owner(p))return false;
 View result{{o->memory_,&ram},*o->files_,p};if(!result.init() || !result.snapshot(p,false))return false;
 p.shape.last=current;p.shape.next=next;p.shape.fraction=fraction;p.shape.selectedTime=p.scalar(0x24);
 const auto* animation=p.resource->bank->pose(p.resource->names[p.word(0x14)].c_str());return animation && animation->assemble(p.shape);
 });
}
// Readers validate the same lifetime/layout and decode BE source bytes at their
// own usepoint. They never write native floats back over original RAM.
std::optional<ActorVector> AnimationScratchPose::vector(const BattlePose& p,bool normal,uint32_t index) const{
 auto o=owner(p);if(!o)return {};View v{{o->memory_},*o->files_,p};if(!v.init() || index>=v.counts[normal?1:0])return {};
 ActorVector r;for(unsigned i=0;i<3;++i)r[i]=v.mem.real(v.output[normal?1:0]+12*index+4*i);return v.mem.ok?std::optional<ActorVector>(r):std::nullopt;
}
std::optional<float> AnimationScratchPose::node(const BattlePose& p,uint32_t index) const{
 auto o=owner(p);if(!o)return {};View v{{o->memory_},*o->files_,p};if(!v.init() || index>=v.counts[3])return {};const float f=v.mem.real(v.output[3]+4*index);return v.mem.ok?std::optional<float>(f):std::nullopt;
}
std::optional<uint8_t> AnimationScratchPose::byte(const BattlePose& p,bool texture,uint32_t index) const{
 auto o=owner(p);if(!o)return {};View v{{o->memory_},*o->files_,p};const unsigned c=texture?4:2;if(!v.init() || index>=v.counts[c])return {};const uint8_t b=v.mem.byte(v.output[c]+View::widths[c]*index);return v.mem.ok?std::optional<uint8_t>(b):std::nullopt;
}
std::optional<std::array<float,5>> AnimationScratchPose::transform(const BattlePose& p,uint32_t index) const{
 auto o=owner(p);if(!o)return {};View v{{o->memory_},*o->files_,p};if(!v.init() || index>=v.counts[4])return {};std::array<float,5> r;for(unsigned i=0;i<5;++i)r[i]=v.mem.real(v.output[4]+24*index+4+4*i);return v.mem.ok?std::optional<std::array<float,5>>(r):std::nullopt;
}
bool AnimationScratchPose::snapshot(BattlePose& p,bool includeCache) const{
 auto o=owner(p);return o && o->snapshot(p,includeCache);
}
bool AnimationScratchMemory::snapshot(BattlePose& p,bool includeCache) const{
 if(!current())return false;
 return memory_.withRawRamAccess([&](EventAddressSpace::RawRamAccess& ram){
  View v{{memory_,&ram},*files_,p};return v.init() && v.snapshot(p,includeCache);
 });
}
std::optional<ActorVector> animationOutputVector(const BattlePose& p,bool normal,uint32_t index){if(p.sourceBuffers)return p.sourceBuffers->vector(p,normal,index);const auto& v=normal?p.shape.normals:p.shape.positions;return index<v.size()?std::optional<ActorVector>(v[index]):std::nullopt;}
std::optional<float> animationOutputNode(const BattlePose& p,uint32_t index){if(p.sourceBuffers)return p.sourceBuffers->node(p,index);return index<p.shape.nodes.size()?std::optional<float>(p.shape.nodes[index]):std::nullopt;}
std::optional<uint8_t> animationOutputByte(const BattlePose& p,bool texture,uint32_t index){if(p.sourceBuffers)return p.sourceBuffers->byte(p,texture,index);const auto& v=texture?p.shape.textures:p.shape.visibility;return index<v.size()?std::optional<uint8_t>(v[index]):std::nullopt;}
std::optional<std::array<float,5>> animationOutputTransform(const BattlePose& p,uint32_t index){if(p.sourceBuffers)return p.sourceBuffers->transform(p,index);return index<p.shape.surfaces.size()?std::optional<std::array<float,5>>(p.shape.surfaces[index].transform):std::nullopt;}
}
