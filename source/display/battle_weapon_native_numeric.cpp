#include "battle_weapon_native_numeric.hpp"
#include "field_camera_events.hpp"
#include <cmath>
#include <cstdlib>
namespace ttyd {
BattleWeaponNativeNumeric::BattleWeaponNativeNumeric(BattleWeaponNativeFrame&f,BattleActorServices a,uint32_t b,
 std::function<std::optional<uint32_t>(uint32_t)>w,std::function<bool()>c):frame_(f),actors_(a),base_(b),sourceWord_(std::move(w)),sourceCurrent_(std::move(c)){}
bool BattleWeaponNativeNumeric::current()const{
 if(!base_||uint64_t(base_)+13*0x400>0x100000000ULL||!sourceCurrent_||!sourceCurrent_())return false;
 const auto*c=frame_.camera();return c&&c->owns(base_)&&c->owns(uint32_t(uint64_t(base_)+13*0x400-1));
}
bool BattleWeaponNativeNumeric::owns(uint32_t a)const{return base_&&uint64_t(a)>=base_&&uint64_t(a)<uint64_t(base_)+13*0x400;}
std::optional<uint32_t>BattleWeaponNativeNumeric::word(uint32_t a)const{
 if(!current())return {};
 const auto value=owns(a)?frame_.camera()->word(a):sourceWord_?sourceWord_(a):std::nullopt;
 return current()?value:std::nullopt;
}
std::optional<uint32_t>BattleWeaponNativeNumeric::camera(std::optional<uint32_t>i)const{
 if(!current())return {};
 const auto value=frame_.camera()->camera(i);return current()?value:std::nullopt;
}
bool BattleWeaponNativeNumeric::matrix(BattleAudienceMatrixRequest&q)const{
 if(!current())return false;
 auto copy=q;if(!frame_.camera()->matrix(copy)||!current())return false;q.output=copy.output;return true;
}
std::optional<uint32_t>BattleWeaponNativeNumeric::capture(uint32_t i){
 if(i!=8||!current())return {};
 const auto at=camera(i);if(!at||*at!=base_+8*0x400)return {};
 std::array<uint32_t,12>v;
 for(unsigned j=0;j<v.size();++j){const auto w=word(*at+0x11c+4*j);if(!w)return {};v[j]=*w;}
 if(!current())return {};
 captured_=*at;view_=v;hasView_=true;return at;
}
uint32_t BattleWeaponNativeNumeric::totalWord(uint32_t a)const{
 // Raster total reads may consume only the actual twelve captured Camera8
 // words. Arbitrary optional camera reads use word(); unknown reserved words
 // must never fall through into ordinary actor/source storage.
 if(!current())std::abort();
 if(owns(a)){const uint32_t first=captured_+0x11c;if(!hasView_||a<first||uint64_t(a)>=uint64_t(first)+48||(a-first)%4)std::abort();return view_[(a-first)/4];}
 if(!actors_.memory.word)std::abort();
 return actors_.memory.word(actors_.memory.user,a);
}
std::optional<int32_t>BattleWeaponNativeNumeric::truncate(float v)const{
 if(!current()||!std::isfinite(v)||double(v)<-2147483648.||double(v)>=2147483648.)return {};
 const auto n=int32_t(std::trunc(double(v)));return current()?std::optional<int32_t>(n):std::nullopt;
}
std::optional<float>BattleWeaponNativeNumeric::fma(float a,float b,float c)const{
 if(!current()||!std::isfinite(a)||!std::isfinite(b)||!std::isfinite(c))return {};
 const float value=std::fma(a,b,c);return current()&&std::isfinite(value)?std::optional<float>(value):std::nullopt;
}
std::optional<BattleActMatrix>BattleWeaponNativeNumeric::identity()const{BattleAudienceMatrixRequest q;q.function=0x8029dd04;return matrix(q)?std::optional<BattleActMatrix>(q.output):std::nullopt;}
BattleActDrawServices BattleWeaponNativeNumeric::draw(){
 BattleActDrawServices s;s.memory=actors_;s.user=this;
 s.camera=[](void*p,uint32_t i){return bool(static_cast<BattleWeaponNativeNumeric*>(p)->capture(i));};
 s.center=[](void*p)->std::optional<std::array<float,3>>{auto&a=*static_cast<BattleWeaponNativeNumeric*>(p);if(!a.current())return {};const auto v=a.frame_.camera()->ringCenter();return a.current()?v:std::nullopt;};
 s.offset=[](void*p,float angle)->std::optional<std::array<float,2>>{auto&a=*static_cast<BattleWeaponNativeNumeric*>(p);if(!a.current())return {};const auto v=a.frame_.camera()->ringOffset(angle);return a.current()?v:std::nullopt;};
 s.interpolate=[](void*p,int32_t mode,int32_t step,int32_t duration,float start,float end)->std::optional<float>{auto&a=*static_cast<BattleWeaponNativeNumeric*>(p);if(!a.current())return {};const auto v=cameraInterpolation(mode,step,duration,start,end);return a.current()&&v&&std::isfinite(*v)?v:std::nullopt;};
 s.translation=[](void*p,const std::array<float,3>&v)->std::optional<BattleActMatrix>{BattleAudienceMatrixRequest q;q.function=0x8029e230;q.xyz=v;return static_cast<BattleWeaponNativeNumeric*>(p)->matrix(q)?std::optional<BattleActMatrix>(q.output):std::nullopt;};
 s.rotation=[](void*p,float angle,uint32_t axis)->std::optional<BattleActMatrix>{BattleAudienceMatrixRequest q;q.function=0x8029dff0;q.radians=angle;q.axis=axis;return static_cast<BattleWeaponNativeNumeric*>(p)->matrix(q)?std::optional<BattleActMatrix>(q.output):std::nullopt;};
 s.scale=[](void*p,float x,float y,float z)->std::optional<BattleActMatrix>{BattleAudienceMatrixRequest q;q.function=0x8029e2b0;q.xyz={x,y,z};return static_cast<BattleWeaponNativeNumeric*>(p)->matrix(q)?std::optional<BattleActMatrix>(q.output):std::nullopt;};
 s.concat=[](void*p,const BattleActMatrix&l,const BattleActMatrix&r)->std::optional<BattleActMatrix>{BattleAudienceMatrixRequest q;q.function=0x8029dd64;q.left=l;q.right=r;return static_cast<BattleWeaponNativeNumeric*>(p)->matrix(q)?std::optional<BattleActMatrix>(q.output):std::nullopt;};return s;
}
BattleClassRasterServices BattleWeaponNativeNumeric::raster(){
 BattleClassRasterServices s;s.user=this;s.math=draw();s.memory=actors_;s.memory.byte=nullptr;s.memory.audienceLifecycle={};s.memory.memory={};s.memory.memory.user=this;
 s.memory.memory.word=[](void*p,uint32_t a){return static_cast<BattleWeaponNativeNumeric*>(p)->totalWord(a);};
 if(actors_.memory.half)s.memory.memory.half=[](void*p,uint32_t a){auto&n=*static_cast<BattleWeaponNativeNumeric*>(p);if(!n.current()||n.owns(a))std::abort();return n.actors_.memory.half(n.actors_.memory.user,a);};
 s.request=[](void*p,const BattleClassRasterRequest&r)->std::optional<uint32_t>{if(r.function!=BattleClassRasterService::Camera||r.integers!=std::vector<uint32_t>{8}||!r.reals.empty()||r.matrix)return {};return static_cast<BattleWeaponNativeNumeric*>(p)->capture(8);};
 s.truncate=[](void*p,float v){return static_cast<BattleWeaponNativeNumeric*>(p)->truncate(v);};s.identity=[](void*p){return static_cast<BattleWeaponNativeNumeric*>(p)->identity();};return s;
}
BattleWeaponDrawServices BattleWeaponNativeNumeric::weapon(){
 BattleWeaponDrawServices s;s.math=draw();s.user=this;s.current=[](void*p){return static_cast<BattleWeaponNativeNumeric*>(p)->current();};
 s.truncate=[](void*p,float v){return static_cast<BattleWeaponNativeNumeric*>(p)->truncate(v);};s.fma=[](void*p,float a,float b,float c){return static_cast<BattleWeaponNativeNumeric*>(p)->fma(a,b,c);};s.identity=[](void*p){return static_cast<BattleWeaponNativeNumeric*>(p)->identity();};return s;
}
}
