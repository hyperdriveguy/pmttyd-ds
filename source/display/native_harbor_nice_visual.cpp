#include "native_harbor_nice_visual.hpp"
#include <cmath>
#include <limits>
namespace ttyd {
NativeHarborNiceVisual::NativeHarborNiceVisual(BattleStartupResources&r,std::shared_ptr<const void>life,std::function<bool()>current):resources_(&r),lifetime_(std::move(life)),consumerCurrent_(std::move(current)){}
bool NativeHarborNiceVisual::current()const{return !retired_&&lifetime_&&consumerCurrent_&&consumerCurrent_();}
std::optional<uint64_t>NativeHarborNiceVisual::spawn(const NativeHarborNiceVisualRequest&q){
 if(invoking_||!current()||q.effect.type>4||q.context!=1||q.language!=1||!std::isfinite(q.modelScale)||!std::isfinite(q.fadeStart)||q.milliseconds<last_)return {};
 for(float p:q.effect.position)if(!std::isfinite(p))return {};
 const auto*r=resources_->resource(3+q.effect.type);
 if(!r||r->sourceKey!=q.sourceKey||!resources_->resourceReady(q.sourceKey,1))return {};
 const auto*p=r->animations.pose("A_1");
 if(!p||p->loop||p->end<=p->start||q.fadeStart>=p->end||!next_||next_==std::numeric_limits<uint64_t>::max())return {};
 auto free=instances_.end();for(auto i=instances_.begin();i!=instances_.end();++i)if(!i->id){free=i;break;}
 if(free==instances_.end())return {};
 Instance candidate;candidate.request=q;
 if(!p->evaluate(p->start,candidate.sample)||candidate.sample.vertices.size()>VerticesPerInstance)return {};
 if(!current())return {};
 candidate.id=next_++;*free=std::move(candidate);return free->id;
}
bool NativeHarborNiceVisual::scale(uint64_t id,float scale){
 if(invoking_||!current()||!std::isfinite(scale))return false;
 for(auto&i:instances_)if(i.id==id){if(i.scaled)return i.scale==scale;i.scale=scale;i.scaled=true;return true;}
 return false;
}
bool NativeHarborNiceVisual::advance(uint64_t milliseconds){
 if(invoking_||!current()||milliseconds<last_)return false;
 invoking_=true;struct Guard{bool&v;~Guard(){v=false;}}guard{invoking_};
 // Prepare every live instance before replacing any sample. A lost resource
 // cannot publish a prefix of a new native simulation-time sample.
 auto next=instances_;
 for(auto&i:next)if(i.id){
  const auto*r=resources_->resource(3+i.request.effect.type);
  if(!r||r->sourceKey!=i.request.sourceKey||!resources_->resourceReady(r->sourceKey,1)||milliseconds<i.request.milliseconds)return false;
  const auto*p=r->animations.pose("A_1");if(!p)return false;
  constexpr float AuthoredFramesPerMillisecond=.06f;
  const float frame=p->start+float(milliseconds-i.request.milliseconds)*AuthoredFramesPerMillisecond;
  if(frame>=p->end){i=Instance{};continue;}
  if(!p->evaluate(frame,i.sample)||i.sample.vertices.size()>VerticesPerInstance)return false;
  i.alpha=frame>i.request.fadeStart?std::max(0.f,1-(frame-i.request.fadeStart)/(p->end-i.request.fadeStart)):1;
 }
 if(!current())return false;
 instances_=std::move(next);last_=milliseconds;return true;
}
void NativeHarborNiceVisual::retire(){if(invoking_)return;retired_=true;instances_={};}
}
