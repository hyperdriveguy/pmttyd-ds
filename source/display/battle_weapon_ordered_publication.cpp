#include "battle_weapon_ordered_publication.hpp"
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cstring>
namespace ttyd {namespace {
bool finiteClip(const CameraProjectionMatrix&m,const std::array<std::array<float,3>,4>&positions){
 for(const auto&point:positions)for(unsigned r=0;r<4;++r){float sum=0;for(unsigned c=0;c<4;++c){
  const volatile float product=m[r][c]*(c==3?1.f:point[c]);if(!std::isfinite(product))return false;
  const volatile float next=sum+product;if(!std::isfinite(next))return false;sum=next;
 }}return true;
}
bool iconGeometry(const RetainedIconPacket&p){return validBattleEnemyHpIconPacket(p)&&finiteClip(p.transform,p.quad.positions);}
bool submenuGeometry(const BattleSubmenuPacket&p){
 const auto vertices=battleSubmenuVertices(p);if(!vertices)return false;
 std::array<std::array<float,3>,4> positions{};for(unsigned i=0;i<4;++i)positions[i]={(*vertices)[i].x,(*vertices)[i].y,(*vertices)[i].z};
 const CameraViewMatrix identity{{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};
 return finiteClip(nativeCameraTransform(p.camera.projection,identity),positions);
}
bool same(const BattleEnemyHpIconResidency&a,const BattleEnemyHpIconResidency&b){return a.texture==b.texture&&a.generation==b.generation&&a.image==b.image&&a.width==b.width&&a.height==b.height&&a.gpuWidth==b.gpuWidth&&a.gpuHeight==b.gpuHeight&&a.format==b.format;}
bool sameFloat(float a,float b){return std::memcmp(&a,&b,sizeof(float))==0;}
template<size_t N>bool sameFloats(const std::array<float,N>&a,const std::array<float,N>&b){for(size_t i=0;i<N;++i)if(!sameFloat(a[i],b[i]))return false;return true;}
bool sameProjection(const CameraProjectionMatrix&a,const CameraProjectionMatrix&b){for(size_t i=0;i<4;++i)if(!sameFloats(a[i],b[i]))return false;return true;}
bool same(const BattleFontGlyphPacket&a,const BattleFontGlyphPacket&b){
 const auto&q=a.quad;const auto&r=b.quad;
 return a.font.generation==b.font.generation&&a.font.font==b.font.font&&a.font.texture==b.font.texture&&sameFloats(a.matrix,b.matrix)&&
 q.left==r.left&&q.top==r.top&&q.right==r.right&&q.bottom==r.bottom&&q.u0==r.u0&&q.u1==r.u1&&q.v0==r.v0&&q.v1==r.v1&&q.rgba==r.rgba&&q.originX==r.originX&&q.originY==r.originY&&sameFloat(q.pitchX,r.pitchX)&&sameFloat(q.pitchY,r.pitchY)&&
 sameProjection(a.raster.projection,b.raster.projection)&&sameFloat(a.raster.depthScale,b.raster.depthScale)&&sameFloat(a.raster.depthOffset,b.raster.depthOffset)&&
 a.raster.viewport.x==b.raster.viewport.x&&a.raster.viewport.y==b.raster.viewport.y&&a.raster.viewport.width==b.raster.viewport.width&&a.raster.viewport.height==b.raster.viewport.height&&
 a.raster.scissor.x==b.raster.scissor.x&&a.raster.scissor.y==b.raster.scissor.y&&a.raster.scissor.width==b.raster.scissor.width&&a.raster.scissor.height==b.raster.scissor.height;
}
}
bool BattleWeaponOrderedPublication::enter(){if(running_)return false;running_=true;return true;}
void BattleWeaponOrderedPublication::leave(){running_=false;}
void BattleWeaponOrderedPublication::retire(){invalid_=true;}
bool BattleWeaponOrderedPublication::live(){if(!bound_||invalid_)return false;
 if(!binding_.current(binding_.user,binding_.allocation,binding_.generation))invalid_=true;
 return !invalid_;}
bool BattleWeaponOrderedPublication::source(uint64_t pass){return live()&&pass&&pass_==pass&&binding_.sourceCurrent(binding_.user,pass)&&live();}
bool BattleWeaponOrderedPublication::bind(BattleWeaponOrderedBinding b){
 if(!enter())return false;
 Guard guard{*this};
 if(bound_||invalid_||!b.allocationOwner||!b.sourceResourceOwner||!b.resources||!b.allocation||!b.generation||!b.capacity||!b.current||!b.sourceCurrent||!b.sourceComplete||!b.font||!b.submenu||!b.icon||!b.ready||!b.draw)return false;
 binding_=std::move(b);bound_=true;building_.reserve(binding_.capacity);return live();
}
bool BattleWeaponOrderedPublication::begin(uint64_t pass){
 if(!enter())return false;
 Guard guard{*this};
 if(!live()||!pass)return false;
 if(pass_==pass)return !sealed_;
 if(pass<=lastPass_||pending_||(pass_&&!submitted_))return false;
 if(!binding_.sourceCurrent(binding_.user,pass)||!live())return false;
 pass_=lastPass_=pass;submitted_=false;sealed_.reset();building_.clear();return true;
}
bool BattleWeaponOrderedPublication::reserveFont(uint64_t pass,const BattleFontGlyphPacket&p){
 if(!enter())return false;
 Guard guard{*this};
 if(!source(pass)||sealed_||!binding_.font->accepts(p.font)||!validBattleFontGlyphPacket(p)||!live())return false;
 if(pending_)return same(*pending_,p);
 if(building_.size()>=binding_.capacity)return false;
 pending_=p;return true;
}
void BattleWeaponOrderedPublication::commitFont(uint64_t pass,const BattleFontGlyphPacket&p){
 // This is the same no-failure atomic boundary as the font sink's own emit.
 if(running_||invalid_||pass!=pass_||!pending_||!same(*pending_,p)||building_.size()>=binding_.capacity)std::abort();
 building_.emplace_back(std::move(*pending_));pending_.reset();
}
bool BattleWeaponOrderedPublication::appendSubmenu(uint64_t pass,BattleSubmenuPacketPtr p){
 if(!enter())return false;
 Guard guard{*this};
 if(!source(pass)||sealed_||pending_||!p||building_.size()>=binding_.capacity||p->sourceResourceOwner!=binding_.sourceResourceOwner||p->sourceResourceOwner.owner_before(binding_.sourceResourceOwner)||binding_.sourceResourceOwner.owner_before(p->sourceResourceOwner)||p->camera.pass!=pass||p->camera.generation!=binding_.generation||!validateBattleSubmenuPacket(*p,*binding_.submenu)||!submenuGeometry(*p)||!source(pass))return false;
 building_.emplace_back(std::move(p));return true;
}
bool BattleWeaponOrderedPublication::appendWindow(uint64_t pass,BattleWindowPacketPtr p){
 if(!enter())return false;
 Guard guard{*this};
 if(!source(pass)||sealed_||pending_||!binding_.window||!p||building_.size()>=binding_.capacity||p->camera.pass!=pass||p->camera.generation!=binding_.generation||!validateBattleWindowPacket(*p,*binding_.window)||!source(pass))return false;
 // Windowdrv's genuine global Setup/palette/object owner is independent of
 // the weapon common bank. Its own long proof is retained by the typed packet.
 building_.emplace_back(std::move(p));return true;
}
bool BattleWeaponOrderedPublication::appendClassSurface(uint64_t pass,BattleClassSurfacePacketPtr p){
 if(!enter())return false;
 Guard guard{*this};
 if(!source(pass)||sealed_||pending_||!binding_.classes||!p||building_.size()>=binding_.capacity||
 p->sourceResourceOwner!=binding_.sourceResourceOwner||p->sourceResourceOwner.owner_before(binding_.sourceResourceOwner)||binding_.sourceResourceOwner.owner_before(p->sourceResourceOwner)||
 p->camera.pass!=pass||p->camera.generation!=binding_.generation||!validateBattleClassSurfacePacket(*p,*binding_.classes)||!source(pass))return false;
 building_.emplace_back(std::move(p));return true;
}
bool BattleWeaponOrderedPublication::appendIcon(uint64_t pass,const RetainedIconPacket&p){
 if(!enter())return false;
 Guard guard{*this};
 if(!source(pass)||sealed_||pending_||building_.size()>=binding_.capacity||!iconGeometry(p))return false;
 const auto r=binding_.icon(binding_.user,p.image);
 if(!r||r->image!=p.image||!validBattleEnemyHpIconResidency(*r)||!source(pass))return false;
 building_.emplace_back(BattleEnemyHpIconPacket{p,*r});return true;
}
bool BattleWeaponOrderedPublication::preflight(const std::vector<BattleWeaponOrderedPacket>&packets){
 for(const auto&q:packets){
  if(const auto*p=std::get_if<BattleFontGlyphPacket>(&q)){if(!binding_.font->accepts(p->font)||!validBattleFontGlyphPacket(*p))return false;}
  else if(const auto*p=std::get_if<BattleSubmenuPacketPtr>(&q)){if(!*p||!validateBattleSubmenuPacket(**p,*binding_.submenu)||!submenuGeometry(**p))return false;}
  else if(const auto*icon=std::get_if<BattleEnemyHpIconPacket>(&q)){if(!iconGeometry(icon->packet)||!validBattleEnemyHpIconResidency(icon->residency))return false;const auto r=binding_.icon(binding_.user,icon->packet.image);if(!r||!same(*r,icon->residency))return false;}
  else if(const auto*cp=std::get_if<BattleClassSurfacePacketPtr>(&q)){if(!*cp||!binding_.classes||(*cp)->camera.pass!=pass_||(*cp)->camera.generation!=binding_.generation||!validateBattleClassSurfacePacket(**cp,*binding_.classes))return false;}
  else {const auto&p=std::get<BattleWindowPacketPtr>(q);if(!p||!binding_.window||p->camera.pass!=pass_||p->camera.generation!=binding_.generation||!validateBattleWindowPacket(*p,*binding_.window))return false;}
  if(!live())return false;
 }
 return live();
}
std::shared_ptr<const BattleWeaponOrderedBatch> BattleWeaponOrderedPublication::seal(uint64_t pass){
 if(!enter())return {};
 Guard guard{*this};
 if(!source(pass)||pending_)return {};
 if(sealed_)return sealed_;
 if(!binding_.sourceComplete(binding_.user,pass)||!source(pass)||!preflight(building_))return {};
 auto p=std::make_shared<BattleWeaponOrderedBatch>();p->allocationOwner=binding_.allocationOwner;p->sourceResourceOwner=binding_.sourceResourceOwner;p->allocation=binding_.allocation;p->generation=binding_.generation;p->pass=pass;p->packets=building_;sealed_=p;return sealed_;
}
bool BattleWeaponOrderedPublication::validate(const std::shared_ptr<const BattleWeaponOrderedBatch>&p){
 if(!enter())return false;
 Guard guard{*this};return live()&&p&&p==sealed_&&!p.owner_before(sealed_)&&!sealed_.owner_before(p)&&p->pass==pass_&&preflight(p->packets);
}
bool BattleWeaponOrderedPublication::submit(uint64_t pass,const std::shared_ptr<const BattleWeaponOrderedBatch>&p){
 if(!enter())return false;
 Guard guard{*this};
 if(!source(pass)||!p||p!=sealed_||p.owner_before(sealed_)||sealed_.owner_before(p)||pending_||!preflight(p->packets))return false;
 if(submitted_)return true;
 if(!binding_.ready(binding_.user,*p)||!source(pass)||!preflight(p->packets))return false;
 // All actual native packet/resource checks preceded the first GPU write.
 binding_.draw(binding_.user,*p);submitted_=true;return live();
}
BattleWeaponNativeSubmission BattleWeaponOrderedPublication::submission(){return {this,
 [](void*p,uint64_t pass){return static_cast<BattleWeaponOrderedPublication*>(p)->begin(pass);},
 [](void*p,uint64_t pass)->std::shared_ptr<const void>{return static_cast<BattleWeaponOrderedPublication*>(p)->seal(pass);},
 [](void*p,uint64_t pass,const std::shared_ptr<const void>&batch){auto&s=*static_cast<BattleWeaponOrderedPublication*>(p);if(!s.sealed_||batch.get()!=s.sealed_.get()||batch.owner_before(s.sealed_)||s.sealed_.owner_before(batch))return false;return s.submit(pass,s.sealed_);},
 [](void*p,const std::shared_ptr<const void>&batch){auto&s=*static_cast<BattleWeaponOrderedPublication*>(p);if(!s.sealed_||batch.get()!=s.sealed_.get()||batch.owner_before(s.sealed_)||s.sealed_.owner_before(batch))return false;return s.validate(s.sealed_);}};}
bool BattleWeaponOrderedPublication::retains(uint32_t image)const{
 const auto has=[&](const auto&packets){return std::any_of(packets.begin(),packets.end(),[&](const auto&q){const auto*p=std::get_if<BattleEnemyHpIconPacket>(&q);return p&&p->packet.image==image;});};
 return has(building_)||(sealed_&&has(sealed_->packets));
}
}
