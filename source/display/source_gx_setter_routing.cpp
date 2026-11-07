#include "source_gx_setter_routing.hpp"
#include "display_queue.hpp"
#include "battle_texture_plane.hpp"
#include "source_font_draw_start.hpp"
#include "source_icon_gx.hpp"
#include "window_primitive_continuation.hpp"
#include <algorithm>
#include <cstring>
namespace ttyd {namespace {
struct Guard{bool&v;explicit Guard(bool&x):v(x){v=true;}~Guard(){v=false;}};
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
unsigned count(uint32_t f){
 switch(f){
 case 0x802b8be0:case 0x802b8c3c:case 0x802b8ff4:case 0x802b9540:case 0x802b9440:case 0x802b957c:return 2;
 case 0x802b8c98:case 0x802b857c:case 0x802b9208:case 0x802b93d8:return 3;
 case 0x802b8ce0:return 5;case 0x802b932c:return 4;
 default:return 1;
 }
}
// Explicit leaf operations which do not modify ANY inherited field owned by
// this observer. Opaque parents (notably mapSetMaterialLight/Fog) are absent.
// Actual owning providers still perform/retain their complete linked effects.
bool transparent(uint32_t f){switch(f){
 case 0x8000f2e4:case 0x8000f2fc:case 0x8001d968:case 0x8002b438:case 0x80037a80:
 case 0x802b3c08:case 0x802b4520:case 0x802b4558:case 0x802b4e04:case 0x802b5084:
 case 0x802b5a6c:case 0x802b6e54:case 0x802b6f3c:case 0x802b6f78:case 0x802b72b0:
 case 0x802b7544:case 0x802b76c4:case 0x802b76d4:case 0x802b76e4:case 0x802b7880:
 case 0x802b8880:case 0x802b88d4:case 0x802b8960:case 0x802b89a4:case 0x802b89e8:case 0x802b8a50:
 case 0x802b8ab8:case 0x802b8d60:case 0x802b8e30:case 0x802b8fcc:
 case 0x802ba2fc:case 0x802ba39c:case 0x802ba3d0:return true;
 default:return false;
}}
SourceGxInheritedCall convert(uint32_t f,uint32_t site,const std::vector<uint32_t>&v){SourceGxInheritedCall q;q.function=f;q.callsite=site;q.count=unsigned(v.size());std::copy_n(v.begin(),std::min(v.size(),q.integers.size()),q.integers.begin());return q;}
}
SourceGxSetterRouting::SourceGxSetterRouting(std::shared_ptr<SourceGxInheritedState>h,std::shared_ptr<const void>s,std::function<bool()>c):history_(std::move(h)),scope_(std::move(s)),sourceCurrent_(std::move(c)){}
bool SourceGxSetterRouting::current()const{return alive_&&history_&&scope_&&sourceCurrent_&&sourceCurrent_()&&alive_&&history_->current();}
void SourceGxSetterRouting::retire(){alive_=false;}
SourceGxObserved SourceGxSetterRouting::record(const SourceGxInheritedCall&q){
 if(!current())return SourceGxObserved::Unavailable;
 if(SourceGxInheritedState::tracks(q.function)){
  if(history_->apply(q))return SourceGxObserved::Recorded;
 }else if(transparent(q.function))return SourceGxObserved::Ignored;
 // The actual provider already accepted this operation. Never manufacture its
 // unknown writes or tell its parent to replay a successful opaque prefix.
 return history_->invalidate()?SourceGxObserved::Invalidated:SourceGxObserved::Unavailable;
}
SourceGxObserved SourceGxSetterRouting::accepted(const SourceGxInheritedCall&q){if(invoking_)return SourceGxObserved::Unavailable;Guard g(invoking_);return record(q);}
SourceGxObserved SourceGxSetterRouting::accepted(const DisplayStateRequest&r){
 SourceGxInheritedCall q;
 switch(r.kind){
 case DisplayGXState::ColorUpdate:q.function=0x802b9380;q.callsite=0x80010944;q.count=1;break;
 case DisplayGXState::AlphaUpdate:q.function=0x802b93ac;q.callsite=r.values[0]==0?0x8001096c:0x80010978;q.count=1;break;
 case DisplayGXState::BlendMode:q.function=0x802b932c;q.count=4;break;
 case DisplayGXState::ZCompareLocation:q.function=0x802b940c;q.count=1;break;
 case DisplayGXState::ZMode:q.function=0x802b93d8;q.count=3;break;
 case DisplayGXState::AlphaCompare:q.function=0x802b8d60;q.count=5;break;
 case DisplayGXState::ZScaleOffset:{if(invoking_)return SourceGxObserved::Unavailable;Guard g(invoking_);return current()?SourceGxObserved::Ignored:SourceGxObserved::Unavailable;}
 }
 for(unsigned i=0;i<q.count;++i)q.integers[i]=uint32_t(r.values[i]);
 return accepted(q);
}
SourceGxObserved SourceGxSetterRouting::accepted(const BattleTexturePlaneRequest&r){
 if(r.kind!=BattleTexturePlaneKind::Call){if(invoking_)return SourceGxObserved::Unavailable;Guard g(invoking_);return current()?SourceGxObserved::Ignored:SourceGxObserved::Unavailable;}
 auto q=convert(uint32_t(r.function),0,r.integers);
 if(q.function==0x802b8ff4){q.fog=r.reals.size()==4;for(unsigned i=0;i<std::min(size_t(4),r.reals.size());++i)q.floatBits[i]=bits(r.reals[i]);}
 return accepted(q);
}
SourceGxObserved SourceGxSetterRouting::accepted(const SourceFontStateCall&r){return accepted(convert(r.function,r.callsite,r.integers));}
SourceGxObserved SourceGxSetterRouting::accepted(const SourceFontDrawStartFog&r){auto q=convert(r.function,r.callsite,{r.type,r.color});q.fog=true;for(unsigned i=0;i<4;++i)q.floatBits[i]=bits(r.reals[i]);return accepted(q);}
SourceGxObserved SourceGxSetterRouting::accepted(const SourceIconGXCall&r){
 SourceGxInheritedCall q;q.function=r.function;q.count=count(r.function);q.integers=r.integers;
 if(q.function==0x802b8ff4){q.fog=true;for(unsigned i=0;i<4;++i)q.floatBits[i]=bits(r.reals[i]);}
 return accepted(q);
}
SourceGxObserved SourceGxSetterRouting::accepted(const WindowPrimitiveRequest&r){
 if(r.kind!=WindowPrimitiveRequestKind::Call){if(invoking_)return SourceGxObserved::Unavailable;Guard g(invoking_);return current()?SourceGxObserved::Ignored:SourceGxObserved::Unavailable;}
 auto q=convert(r.function,r.callsite,r.integers);
 if(q.function==0x802b8ff4&&r.color&&q.count==1&&r.reals.size()==4){q.count=2;q.integers[1]=*r.color;q.fog=true;for(unsigned i=0;i<4;++i)q.floatBits[i]=bits(r.reals[i]);}
 return accepted(q);
}
std::shared_ptr<const SourceGxInheritedSnapshot>SourceGxSetterRouting::capture(unsigned stages)const{if(invoking_)return {};Guard g(invoking_);if(!current())return {};auto p=history_->capture(stages);return current()?p:nullptr;}
bool SourceGxSetterRouting::compatibleNow(unsigned stages)const{return bool(capture(stages));}
}
