#include "battle_weapon_source_icon_pin.hpp"
#include <algorithm>
namespace ttyd {
namespace {struct Guard{bool&v;explicit Guard(bool&n):v(n){v=true;}~Guard(){v=false;}};}
BattleWeaponSourceIconPin::BattleWeaponSourceIconPin(const std::shared_ptr<BattleWeaponOrderedPublication>&p,std::shared_ptr<const void> lifetime,std::function<bool()> current):owner_(p),lifetime_(std::move(lifetime)),current_(std::move(current)){if(!p)invalid_=true;}
bool BattleWeaponSourceIconPin::valid()const{if(invalid_)return false;if(owner_.expired()||!lifetime_||!current_||!current_()){invalid_=true;return false;}return !invalid_;}
bool BattleWeaponSourceIconPin::markPending(uint32_t image){if(invoking_)return false;Guard guard(invoking_);if(!valid())return false;pending_.insert(image);return true;}
bool BattleWeaponSourceIconPin::acknowledge(uint32_t image){if(invoking_)return false;Guard guard(invoking_);if(!valid())return false;const auto p=owner_.lock();if(!p||!p->retains(image))return false;pending_.erase(image);return valid();}
bool BattleWeaponSourceIconPin::retains(uint32_t image)const{if(invoking_)return false;Guard guard(invoking_);if(!valid())return false;const auto p=owner_.lock();return p&&(pending_.count(image)||p->retains(image));}
bool BattleWeaponSourceIconPin::alive()const{if(invoking_)return false;Guard guard(invoking_);return valid();}
void BattleWeaponSourceIconPin::retire(){invalid_=true;if(invoking_)return;pending_.clear();owner_.reset();lifetime_.reset();current_={};}
bool BattleWeaponSourceIconPins::attach(const std::shared_ptr<BattleWeaponSourceIconPin>&p){
 if(invoking_)return false;
 Guard guard(invoking_);
 if(!p||!p->alive())return false;
 pins_.erase(std::remove_if(pins_.begin(),pins_.end(),[](const auto&w){auto p=w.lock();return !p||!p->alive();}),pins_.end());
 for(const auto&w:pins_)if(auto q=w.lock())if(q==p&&!q.owner_before(p)&&!p.owner_before(q))return true;
 pins_.push_back(p);return true;
}
bool BattleWeaponSourceIconPins::retains(uint32_t image)const{if(invoking_)return false;Guard guard(invoking_);for(const auto&w:pins_)if(auto p=w.lock())if(p->retains(image))return true;return false;}
void BattleWeaponSourceIconPins::clear(bool retirePins){if(invoking_)return;Guard guard(invoking_);if(retirePins)for(const auto&w:pins_)if(auto p=w.lock())p->retire();pins_.clear();}
}
