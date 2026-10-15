#include "battle_enemy_hp_publication.hpp"
#include <algorithm>
#include <cmath>
namespace ttyd {
namespace {
bool rect(const NativeCameraRect& r){return r.width && r.height && r.x<=240 && r.y<=400 && r.width<=240-r.x && r.height<=400-r.y;}
template<class Matrix> bool finite(const Matrix& m){for(const auto& row:m)for(auto n:row)if(!std::isfinite(n))return false;return true;}
uint32_t backing(uint32_t n){if(!n || n>1020)return 0;uint32_t p=8;while(p<n+4)p*=2;return p;}
bool viewValid(const BattleEnemyHpRasterView& v){return finite(v.projection) && rect(v.viewport) && rect(v.scissor) && std::isfinite(v.depthScale) && std::isfinite(v.depthOffset);}
bool same(const BattleEnemyHpIconResidency& a,const BattleEnemyHpIconResidency& b){return a.texture==b.texture && a.generation==b.generation && a.image==b.image && a.width==b.width && a.height==b.height && a.gpuWidth==b.gpuWidth && a.gpuHeight==b.gpuHeight && a.format==b.format;}
}
bool validBattleEnemyHpIconResidency(const BattleEnemyHpIconResidency& r){return r.texture && r.generation && r.format==0 && backing(r.width) && backing(r.height) && r.gpuWidth==backing(r.width) && r.gpuHeight==backing(r.height);}
bool validBattleEnemyHpIconPacket(const RetainedIconPacket& p){
 const auto& s=p.pipeline;const auto& m=p.material;
 if(p.camera!=8 || !rect(p.viewport) || !rect(p.scissor) || !finite(p.transform) || !finite(p.quad.positions) || !finite(p.quad.uv) || !std::isfinite(p.depthScale) || !std::isfinite(p.depthOffset) || !s.rasterAdmitted() || (!s.depth && s.depthWrite))return false;
 if(m.mode!=NativeIconPlan::Normal && m.mode!=NativeIconPlan::Grayscale && m.mode!=NativeIconPlan::Blended)return false;
 if(m.cull>2 || (s.compare!=1 && s.compare!=3 && s.compare!=6 && s.compare!=7))return false;
 const auto blend=[&](int32_t source,int32_t destination){return s.sourceBlend==source && s.destinationBlend==destination;};
 if(!s.logicOr && !blend(0,1) && !blend(1,0) && !blend(4,5) && !blend(1,1) && !blend(2,1) && !blend(6,7))return false;
 for(const auto& uv:p.quad.uv)for(auto n:uv)if(n<0 || n>1)return false;
 return true;
}
BattleEnemyHpQuadSink BattleEnemyHpOrderedHpSink::services(){return {this,[](void* u,const BattleEnemyHpGpuQuad& q){const auto& s=*static_cast<BattleEnemyHpOrderedHpSink*>(u);return s.owner && s.owner->appendHp(s.pass,q);}};}
bool BattleEnemyHpOrderedPresentation::enter(){if(running_)return false;running_=true;return true;}
void BattleEnemyHpOrderedPresentation::leave(){running_=false;if(retireRequested_ || invalid_)clear();}
void BattleEnemyHpOrderedPresentation::clear(){allocation_=pass_=lastPass_=0;active_=false;retireRequested_=invalid_=false;owner_.reset();services_={};building_.clear();published_.reset();}
void BattleEnemyHpOrderedPresentation::retire(){if(running_){retireRequested_=true;return;}clear();}
bool BattleEnemyHpOrderedPresentation::live(){if(invalid_ || retireRequested_ || !owner_ || !services_.current || !services_.current(services_.user,allocation_)){invalid_=true;return false;}return !retireRequested_;}
bool BattleEnemyHpOrderedPresentation::bind(uint64_t allocation,BattleEnemyHpPublicationServices s){
 if(!enter())return false;
 Guard guard{*this};
 if(!allocation || !s.current || !s.complete || !s.icon || !s.view || !s.hp)return false;
 if(!s.current(s.user,allocation) || retireRequested_)return false;
 clear();allocation_=allocation;services_=s;owner_=std::make_shared<const uint8_t>(0);return live();
}
bool BattleEnemyHpOrderedPresentation::begin(uint64_t pass){
 if(!enter())return false;
 Guard guard{*this};if(!live() || !pass)return false;
 if(active_)return pass==pass_;
 if(pass<=lastPass_)return false;
 pass_=lastPass_=pass;active_=true;building_.clear();return true;
}
bool BattleEnemyHpOrderedPresentation::appendIcon(uint64_t pass,const RetainedIconPacket& q){
 if(!enter())return false;
 Guard guard{*this};
 const auto captured=q;
 if(!live() || !active_ || pass!=pass_ || !validBattleEnemyHpIconPacket(captured))return false;
 const auto residency=services_.icon(services_.user,captured.image);
 if(!live() || !residency || residency->image!=captured.image || !validBattleEnemyHpIconResidency(*residency))return false;
 building_.push_back(BattleEnemyHpIconPacket{captured,*residency});return true;
}
bool BattleEnemyHpOrderedPresentation::appendHp(uint64_t pass,const BattleEnemyHpGpuQuad& q){
 if(!enter())return false;
 Guard guard{*this};
 const auto captured=q;
 if(!live() || !active_ || pass!=pass_ || !services_.hp->accepts(captured) || !live())return false;
 const auto view=services_.view(services_.user);
 if(!live() || !view || !viewValid(*view))return false;
 building_.push_back(BattleEnemyHpRasterPacket{captured,*view});return true;
}
bool BattleEnemyHpOrderedPresentation::preflight(const std::vector<BattleEnemyHpOrderedPacket>& packets){
 for(const auto& packet:packets){
   if(const auto* p=std::get_if<BattleEnemyHpIconPacket>(&packet)){
     if(!validBattleEnemyHpIconPacket(p->packet) || !validBattleEnemyHpIconResidency(p->residency) || p->residency.image!=p->packet.image)return false;
     const auto current=services_.icon(services_.user,p->packet.image);
     if(!live() || !current || !same(*current,p->residency))return false;
   }else {const auto& hp=std::get<BattleEnemyHpRasterPacket>(packet);if(!viewValid(hp.view) || !services_.hp->accepts(hp.quad))return false;}
 }
 return live();
}
bool BattleEnemyHpOrderedPresentation::publish(uint64_t pass){
 if(!enter())return false;
 Guard guard{*this};
 if(!live() || !active_ || pass!=pass_ || !services_.complete(services_.user,pass) || !live())return false;
 if(!preflight(building_) || !services_.complete(services_.user,pass) || !live())return false;
 auto publication=std::make_shared<BattleEnemyHpPublication>();publication->allocation=allocation_;publication->pass=pass_;publication->owner=owner_;publication->packets=building_;
 published_=std::move(publication);building_.clear();active_=false;return true;
}
bool BattleEnemyHpOrderedPresentation::validate(const BattleEnemyHpPublication& p){
 if(!enter())return false;
 Guard guard{*this};
 return live() && published_.get()==&p && p.owner==owner_ && p.allocation==allocation_ && p.pass && preflight(p.packets);
}
bool BattleEnemyHpOrderedPresentation::retains(uint32_t image)const{
 const auto has=[&](const auto& packets){return std::any_of(packets.begin(),packets.end(),[&](const auto& packet){const auto* p=std::get_if<BattleEnemyHpIconPacket>(&packet);return p && p->packet.image==image;});};
 return has(building_) || (published_ && has(published_->packets));
}
}
