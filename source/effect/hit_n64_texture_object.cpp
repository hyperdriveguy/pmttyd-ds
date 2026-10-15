#include "hit_n64_texture_object.hpp"
namespace ttyd {
struct HitN64TextureObjectCursor::State {
    HitN64TextureObjectServices owner;uint32_t index=0,pending=0;SourceIconGXCall request;
};
HitN64TextureObjectCursor::HitN64TextureObjectCursor()=default;
HitN64TextureObjectCursor::~HitN64TextureObjectCursor()=default;
bool HitN64TextureObjectCursor::active()const{return bool(state_);}
uint32_t HitN64TextureObjectCursor::pending()const{return state_?state_->pending:0;}
bool HitN64TextureObjectCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<uint32_t> resumeHitN64TextureObject(uint32_t index,const HitN64TextureObjectServices& s,HitN64TextureObjectCursor& cursor){
    if(cursor.invoking_ || !s.memory.word)return {};
    if(cursor.state_ && (cursor.state_->index!=index || cursor.state_->owner.user!=s.user || cursor.state_->owner.memory.user!=s.memory.user || cursor.state_->owner.memory.word!=s.memory.word || cursor.state_->owner.construct!=s.construct))return {};
    struct Guard{bool& f;explicit Guard(bool& flag):f(flag){f=true;}~Guard(){f=false;}} guard(cursor.invoking_);
    if(!cursor.state_){
        cursor.state_=std::make_unique<HitN64TextureObjectCursor::State>();auto& c=*cursor.state_;c.owner=s;c.index=index;
        const auto state=s.memory.word(s.memory.user,0x80419a40),loaded=s.memory.word(s.memory.user,state+4);
        if(!loaded){c.request.function=0x802b72b0;c.request.integers={0x80419a60,1,1,0,0,0,0};}
        else{const auto palette=s.memory.word(s.memory.user,state);c.request.function=0x80037a80;c.request.integers={palette,index};}
        c.pending=c.request.function;
    }
    auto& c=*cursor.state_;if(!s.construct)return {};
    const auto object=s.construct(s.user,c.request);if(!object)return {};
    cursor.state_.reset();return object;
}
}
