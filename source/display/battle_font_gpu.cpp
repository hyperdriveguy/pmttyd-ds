#include "battle_font_gpu.hpp"
#include <cmath>
#include <cstdlib>
namespace ttyd {namespace {
// Exact admitted US font catalog: 162 glyphs, height 22, 512x256 atlas. These are
// package identity/layout checks, not a glyph cap for arbitrary source BFN data.
bool metadata(const MessageFont& f){
 if(f.glyphs.size()!=162)return false;
 for(auto m:f.mapping)if(m>=f.glyphs.size())return false;
 for(const auto&g:f.glyphs)if(g.height!=22||uint32_t(g.x)+g.width>512||uint32_t(g.y)+g.height>256)return false;
 return true;
}
// Tilted native top-screen raster uses 240x400 viewport/scissor coordinates.
// Reversed PICA depth admits scale [-1,0], offset [0,1]; all projections stay finite.
bool rect(const NativeCameraRect&r){return r.width&&r.height&&r.x<=240&&r.y<=400&&uint64_t(r.x)+r.width<=240&&uint64_t(r.y)+r.height<=400;}
bool raster(const BattleFontRasterView&r){
 if(!rect(r.viewport)||!rect(r.scissor)||!std::isfinite(r.depthScale)||!std::isfinite(r.depthOffset)||r.depthScale>0||r.depthScale< -1||r.depthOffset<0||r.depthOffset>1)return false;
 for(const auto&row:r.projection)for(float f:row)if(!std::isfinite(f))return false;
 return true;
}
bool matrix(const BattleActMatrix&m){for(float f:m)if(!std::isfinite(f))return false;return true;}
// Conservative native finite domain: validate the exact composed 4x4 and all
// shader clip lanes before glyph RNG/admission. Do not divide by W or cull:
// singular, offscreen, zero-area and transparent source glyphs remain valid.
bool geometry(const BattleFontGlyphPacket&p){
 CameraProjectionMatrix transform{};
 for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c){
  float sum=0;
  for(unsigned k=0;k<3;++k){
   const volatile float product=p.raster.projection[r][k]*p.matrix[4*k+c];
   if(!std::isfinite(product))return false;
   const volatile float next=sum+product;if(!std::isfinite(next))return false;sum=next;
  }
  if(c==3){const volatile float next=sum+p.raster.projection[r][3];if(!std::isfinite(next))return false;sum=next;}
  transform[r][c]=sum;
 }
 // Four glyph corners use source integer XY with Z=0, W=1. Test every
 // product/sum BEFORE RNG, without perspective division or visibility culling.
 const auto&q=p.quad;
 const float points[4][4]={{float(q.left),float(q.top),0,1},{float(q.right),float(q.top),0,1},
                           {float(q.right),float(q.bottom),0,1},{float(q.left),float(q.bottom),0,1}};
 for(const auto&point:points)for(unsigned r=0;r<4;++r){
  float sum=0;
  for(unsigned c=0;c<4;++c){
   const volatile float product=transform[r][c]*point[c];if(!std::isfinite(product))return false;
   const volatile float next=sum+product;if(!std::isfinite(next))return false;sum=next;
  }
 }
 return true;
}
bool same(const WindowFontQuad&a,const WindowFontQuad&b){return a.left==b.left&&a.top==b.top&&a.right==b.right&&a.bottom==b.bottom&&a.u0==b.u0&&a.v0==b.v0&&a.u1==b.u1&&a.v1==b.v1&&a.rgba==b.rgba&&a.originX==b.originX&&a.originY==b.originY&&a.pitchX==b.pitchX&&a.pitchY==b.pitchY;}
}
bool BattleFontLease::current()const{return generation&&generation->resident_&&font&&font==generation->font_&&texture&&texture==generation->texture_;}
BattleFontGpuResource::~BattleFontGpuResource(){retire();}
// Native import format 0=RGBA8, one whole unrotated 512x256 image, UV0/1/1/0.
// Adoption copies metadata and creates a fresh opaque generation; texture memory
// remains borrowed. Retired leases cannot revive when an address is reused.
bool BattleFontGpuResource::adopt(const MessageFont&f,const BattleClassSurfaceImport&i){
 if(generation_||!metadata(f)||!i.identity||i.width!=512||i.height!=256||i.format!=0||i.count!=1||i.subWidth!=512||i.subHeight!=256||i.rotated||i.uv!=std::array<float,4>{0,1,1,0})return false;
 auto generation=std::shared_ptr<BattleFontGeneration>(new BattleFontGeneration);
 generation->font_=std::make_shared<const MessageFont>(f);generation->texture_=i.identity;generation->resident_=true;
 generation_=generation;lease_={generation,generation->font_,i.identity};return true;
}
void BattleFontGpuResource::retire(){if(generation_)generation_->resident_=false;lease_={};generation_.reset();}
BattleFontLease BattleFontGpuResource::lease()const{return lease_.current()?lease_:BattleFontLease{};}
bool BattleFontGpuResource::accepts(const BattleFontLease&l)const{return l.current()&&lease_.current()&&l.generation==generation_&&l.font==lease_.font&&l.texture==lease_.texture;}
bool validBattleFontGlyphPacket(const BattleFontGlyphPacket&p){
 const auto&q=p.quad;
 return p.font.current()&&matrix(p.matrix)&&raster(p.raster)&&geometry(p)&&q.u0<=q.u1&&q.v0<=q.v1&&q.u1<=512&&q.v1<=256&&q.originX==0&&q.originY==0&&q.pitchX==0&&q.pitchY==0;
}
CameraProjectionMatrix battleFontGlyphTransform(const BattleFontGlyphPacket&p){
 // PICA projection is already tilted. Append only the exact loaded3x4;
 // using nativeCameraTransform here would tilt/apply camera a second time.
 CameraProjectionMatrix out{};
 for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c){
  for(unsigned k=0;k<3;++k)out[r][c]+=p.raster.projection[r][k]*p.matrix[4*k+c];
  if(c==3)out[r][c]+=p.raster.projection[r][3];
 }
 return out;
}
// Publication lifetime can outlast the source callback/camera pass. Once a
// checked lifetime fails, invalid_ is sticky; later true predicates cannot revive it.
bool BattleFontGlyphSink::current()const{
 if(!bound_||invalid_)return false;
 if(!binding_.font.current()||!binding_.publicationCurrent||!binding_.publicationCurrent()){invalid_=true;return false;}
 return true;
}
// Source operations additionally check short currentness and poison on failure.
// snapshot() checks only long publication currentness, not the finished camera pass.
bool BattleFontGlyphSink::source(){if(!current()||!binding_.sourceCurrent||!binding_.sourceCurrent()){invalid_=true;return false;}return true;}
// Reserve capacity before source RNG; ordered admission/emit must be paired.
// Sink is one-shot: bound_ stays set after retirement, so it cannot rebind itself.
bool BattleFontGlyphSink::bind(BattleFontGlyphBinding b){
 if(bool(b.orderedAdmit)!=bool(b.orderedEmit)||bound_||running_||!b.font.current()||!b.resources||!b.capacity||!raster(b.raster)||!b.sourceCurrent||!b.publicationCurrent||!b.ordinaryProfile||!b.random||!b.publicationCurrent()||!b.sourceCurrent())return false;
 building_.reserve(b.capacity);binding_=std::move(b);bound_=true;invalid_=false;return true;
}
// Source matrix slot 0 and TEV color-register slot 1 are distinct retained loads.
// No default identity or color is synthesized before glyph admission.
bool BattleFontGlyphSink::loadMatrix(const BattleActMatrix&m,uint32_t slot){
 if(running_||pending_||published_||slot!=0||!matrix(m)||!source())return false;
 matrix_=m;return true;
}
bool BattleFontGlyphSink::tevColor(uint32_t slot,uint32_t rgba){
 if(running_||pending_||published_||slot!=1||!source())return false;
 color_=rgba;return true;
}
// Ordinary source GX profile and full geometry/storage admission precede the
// two irand(64) calls owned by the glyph kernel. Offscreen/blank/transparent
// glyphs still retain source effects; capacity is native policy, not source count.
bool BattleFontGlyphSink::admit(const WindowFontQuad&q){
 if(pending_||published_||!matrix_||!color_||q.rgba!=*color_||building_.size()>=binding_.capacity||!source()||!binding_.ordinaryProfile()||!source())return false;
 BattleFontGlyphPacket p{binding_.font,q,*matrix_,binding_.raster};if(!validBattleFontGlyphPacket(p))return false;
 if(binding_.orderedAdmit&&!binding_.orderedAdmit(binding_.orderedUser,p))return false;
 pending_=std::move(p);return true;
}
void BattleFontGlyphSink::emit(const WindowFontQuad&q){
 // Atomic admission contract: actual RNG/void emission cannot retire, fail,
 // reenter or mutate resources. A violated owner contract is a native bug;
 // never silently drop an admitted glyph after consuming its two RNG calls.
 if(!invoking_||!pending_||!same(q,pending_->quad)||!current()||building_.size()>=building_.capacity())std::abort();
 if(binding_.orderedEmit)binding_.orderedEmit(binding_.orderedUser,*pending_);
 building_.push_back(std::move(*pending_));pending_.reset();
}
SourceMatrixFontAtomicGlyphServices BattleFontGlyphSink::glyphServices(){
 SourceMatrixFontAtomicGlyphServices s;s.font=binding_.font.font.get();s.actors=binding_.actors;s.user=this;s.lifetime=binding_.resources;s.invoking=&invoking_;
 s.current=[](void*p){return static_cast<BattleFontGlyphSink*>(p)->source();};
 s.color=[](void*p)->std::optional<uint32_t>{auto&c=*static_cast<BattleFontGlyphSink*>(p);return c.source()?c.color_:std::nullopt;};
 s.admit=[](void*p,const WindowFontQuad&q){return static_cast<BattleFontGlyphSink*>(p)->admit(q);};
 s.random=[](void*p,uint32_t bound){return static_cast<BattleFontGlyphSink*>(p)->binding_.random(bound);};
 s.emit=[](void*p,const WindowFontQuad&q){static_cast<BattleFontGlyphSink*>(p)->emit(q);};return s;
}
// Pin resource context through the invocation. Retirement before admission can
// hold; retirement/reentry after accepted RNG/emit violates the atomic contract.
// Cleanup requested during the call is deferred until this guard unwinds.
bool BattleFontGlyphSink::glyph(const SourceMatrixFontGlyph&q){
 if(running_||retireRequested_)return false;
 const auto resources=binding_.resources;running_=true;
 struct Guard{BattleFontGlyphSink&self;~Guard(){self.running_=false;if(self.retireRequested_)self.clear();}}guard{*this};
 if(!source())return false;
 return drawSourceMatrixFontAtomicGlyph(q,glyphServices());
}
// Publication moves one completed immutable leaf; it does not finish the outer
// mixed packet sequence, source display callback, camera queue or GPU submission.
bool BattleFontGlyphSink::publish(){
 if(running_||publishing_||pending_||published_||!source())return false;
 publishing_=true;published_=std::make_shared<const std::vector<BattleFontGlyphPacket>>(std::move(building_));publishing_=false;return true;
}
std::shared_ptr<const std::vector<BattleFontGlyphPacket>> BattleFontGlyphSink::snapshot()const{return current()?published_:nullptr;}
void BattleFontGlyphSink::clear(){pending_.reset();building_.clear();published_.reset();matrix_.reset();color_.reset();binding_={};}
void BattleFontGlyphSink::retire(){const auto resources=binding_.resources;invalid_=true;retireRequested_=true;if(!running_)clear();}
bool BattleFontGlyphSink::active()const{return bound_&&!invalid_;}
}
