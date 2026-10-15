#include "battle_weapon_cost_continuation.hpp"
#include <cstring>
namespace ttyd {namespace {
int32_t signedWord(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
double real(uint32_t hi,uint32_t lo){uint64_t bits=uint64_t(hi)<<32|lo;double v;std::memcpy(&v,&bits,8);return v;}
// Source signed-word double conversion: exponent4330 and low-word sign XOR.
// copies-1 wraps32 BEFORE conversion; bias comes from original RAM,not host pow.
double convert(uint32_t n,double bias){const uint64_t bits=0x4330000000000000ULL|(n^0x80000000u);double v;std::memcpy(&v,&bits,8);volatile double out=v-bias;return out;}
// Exact original badge/item ID memberships select stacked-copy cost policy.
// Exponential uses baseFP*pow(sourceBase,copies-1); linear uses wrapping32
// baseFP*copies. IDs outside BOTH lists retain the captured base cost.
bool exponential(int32_t id){return (id>=240&&id<=243)||(id>=247&&id<=249)||id==252||id==256||id==257||(id>=332&&id<=334)||id==337||id==338;}
bool linear(int32_t id){return (id>=244&&id<=246)||id==251||id==253;}
bool same(const PackedWeaponCostServices&a,const PackedWeaponCostServices&b){return a.user==b.user&&a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.current==b.current&&a.word==b.word&&a.byte==b.byte&&a.equippedCount==b.equippedCount&&a.power==b.power&&a.truncate==b.truncate;}
struct Running{bool&v;explicit Running(bool&b):v(b){v=true;}~Running(){v=false;}};
}
// Native phases retain source operands and successful children,not menu time.
// Context loss is sticky; shared owner/control block and every callback remain
// fixed. Reset drops metadata only; it cannot undo an equipped-count child.
struct PackedWeaponCostCursor::State {
 PackedWeaponCostServices services;uint32_t unit=0,weapon=0,phase=0,pending=0,item=0,base=0,copies=0,cost=0,hi=0,lo=0;
 double bias=0,powerBase=0,exponent=0,factor=0,product=0;bool invalid=false;
 bool valid(){if(invalid)return false;if(!services.current||!services.current(services.user))invalid=true;return !invalid;}
 bool read(uint32_t pc,uint32_t address,uint32_t&out){pending=pc;const auto v=services.word(services.user,address);if(!valid()||!v)return false;out=*v;return true;}
 bool byte(uint32_t pc,uint32_t address,uint32_t&out){pending=pc;const auto v=services.byte(services.user,address);if(!valid()||!v)return false;out=*v;return true;}
};
PackedWeaponCostCursor::PackedWeaponCostCursor()=default;PackedWeaponCostCursor::~PackedWeaponCostCursor()=default;
bool PackedWeaponCostCursor::active()const{return bool(state_);}bool PackedWeaponCostCursor::stale()const{return state_&&state_->invalid;}uint32_t PackedWeaponCostCursor::pending()const{return state_?state_->pending:0;}bool PackedWeaponCostCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<uint32_t>resumePackedWeaponCost(uint32_t unit,uint32_t weapon,const PackedWeaponCostServices&s,PackedWeaponCostCursor&cursor){
 if(cursor.invoking_||!s.lifetime||!s.current||!s.word||!s.byte)return {};
 if(cursor.state_){const auto&c=*cursor.state_;if(c.unit!=unit||c.weapon!=weapon||!same(c.services,s))return {};}
 Running running(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<PackedWeaponCostCursor::State>();cursor.state_->services=s;cursor.state_->unit=unit;cursor.state_->weapon=weapon;}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return {};switch(c.phase){
 // Weapon+8 item ID is captured before unsigned byte+11 base FP. Item0
 // skips pouch; nonzero item queries its original count even for baseFP0.
 case 0:if(!c.read(0x80126980,weapon+8,c.item))return {};c.phase=1;continue;
 case 1:if(!c.byte(0x80126984,weapon+0x11,c.base))return {};c.cost=c.base;c.phase=c.item?2:13;continue;
 case 2:{c.pending=0x800d3300;if(!s.equippedCount)return {};const auto v=s.equippedCount(s.user,c.item);if(!c.valid()||!v)return {};c.copies=*v;c.phase=3;continue;}
 // Reload item AFTER pouch child for policy selection; keep earlier base/copies.
 // Concurrent source item changes must not retarget the already completed count.
 case 3:if(!c.read(0x80126994,weapon+8,c.item))return {};if(exponential(signedWord(c.item)))c.phase=4;else{if(linear(signedWord(c.item)))c.cost=c.base*c.copies;c.phase=13;}continue;
 // Original double bias802f00b8/bc feeds copies-1; power base802f00d0/d4
 // is independently loaded. After pow,RELOAD bias for captured base conversion.
 // Double multiplication and actual fctiwz are separate reached providers.
 case 4:if(!c.read(0x80126a34,0x802f00b8,c.hi))return {};c.phase=5;continue;
 case 5:if(!c.read(0x80126a34,0x802f00bc,c.lo))return {};c.bias=real(c.hi,c.lo);c.exponent=convert(c.copies-1,c.bias);c.phase=6;continue;
 case 6:if(!c.read(0x80126a3c,0x802f00d0,c.hi))return {};c.phase=7;continue;
 case 7:if(!c.read(0x80126a3c,0x802f00d4,c.lo))return {};c.powerBase=real(c.hi,c.lo);c.phase=8;continue;
 case 8:{c.pending=0x80272620;if(!s.power)return {};const auto v=s.power(s.user,c.powerBase,c.exponent);if(!c.valid()||!v)return {};c.factor=*v;c.phase=9;continue;}
 case 9:if(!c.read(0x80126a5c,0x802f00b8,c.hi))return {};c.phase=10;continue;
 case 10:if(!c.read(0x80126a5c,0x802f00bc,c.lo))return {};c.bias=real(c.hi,c.lo);{const volatile double value=convert(c.base,c.bias)*c.factor;c.product=value;}c.phase=11;continue;
 case 11:{c.pending=0x80126a70;if(!s.truncate)return {};const auto v=s.truncate(s.user,c.product);if(!c.valid()||!v)return {};c.cost=uint32_t(*v);c.phase=13;continue;}
 // Only nonzero intermediate cost reads unit2f5 reduction. Subtract wraps32,
 // signed<=0 floors1,then signed>=1000 caps999. Free0 stays0; this is not
 // unsigned saturation and no cost is written back into guest weapon storage.
 case 13:if(c.cost){uint32_t reduction;if(!c.byte(0x80126a8c,unit+0x2f5,reduction))return {};c.cost-=reduction;if(signedWord(c.cost)<=0)c.cost=1;}c.phase=14;continue;
 case 14:{if(signedWord(c.cost)>=1000)c.cost=999;const auto result=c.cost;cursor.state_.reset();return result;}
 default:return {};
 }}
}
}
