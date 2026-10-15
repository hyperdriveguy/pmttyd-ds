#include <memory>
#include "battle_weapon_cursor.hpp"
#include "source_icon_resources.hpp"
#include "battle_audience_aux_callback.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Draw{BattleActMatrix matrix;SourceMatrixIcon icon;};
struct Fixture{
 SourceIconResources resources;std::map<uint32_t,uint32_t> words;std::vector<Draw> draws;std::vector<std::array<float,3>> translations;
 std::array<uint32_t,3> window{bits(-60),bits(35),0x7fc00000};
 std::shared_ptr<const void> token=std::make_shared<int>(1);BattleWeaponCursor cursor;
 unsigned events=0,pauseAt=0;bool reenter=false,retireWord=false;
 bool event(){return ++events!=pauseAt;}
 bool alive=true,holdInterpolation=false,holdFma=false,holdDraw=false,retireDraw=false;
 unsigned interpolations=0,fmas=0,stores=0,matrices=0,cameras=0;int32_t step=0;
 Fixture(){assert(resources.load("romfs/source-icons"));
  words={{0x8041eb90,0x10000},{0x11c78,0x20000},{0x20000,0x30000},{0x2011c,2},
   {0x30000,3},{0x30004,1},{0x30008,10},{0x80415fd0,0x40000},{0x40004,60},{0x4001c,0},
   {0x802ef780,0x43300000},{0x802ef784,0x80000000},{0x5114,0}};
  for(auto p:std::initializer_list<std::pair<uint32_t,float>>{{0x8042242c,0},{0x8042246c,10},{0x80422474,15},{0x80422470,155},{0x80422478,.75f},{0x8042244c,20},{0x8042247c,164},{0x80422480,24},{0x8042245c,1}})words[p.first]=bits(p.second);
  BattleActMatrix view{1,0,0,0,0,1,0,0,0,0,1,0};for(unsigned i=0;i<12;++i)words[0x511c+4*i]=bits(view[i]);
 }
 BattleWeaponCursorServices services(){BattleWeaponCursorServices s;s.user=this;s.lifetime=token;
  s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};
  s.actors.memory.user=this;
  s.actors.memory.word=[](void*p,uint32_t a){auto&f=*static_cast<Fixture*>(p);auto i=f.words.find(a);assert(i!=f.words.end());return i->second;};
  s.actors.memory.storeWord=[](void*p,uint32_t a,uint32_t v){auto&f=*static_cast<Fixture*>(p);f.words[a]=v;++f.stores;};
  s.interpolate=[](void*p,int32_t mode,int32_t step,int32_t duration,float a,float b)->std::optional<float>{auto&f=*static_cast<Fixture*>(p);++f.interpolations;assert(mode==4&&duration==6&&a==0&&b==10);f.step=step;if(f.holdInterpolation)return {};return 2;};
  s.fma=[](void*p,float a,float b,float c)->std::optional<float>{auto&f=*static_cast<Fixture*>(p);++f.fmas;if(f.holdFma)return {};return std::fma(a,b,c);};
  s.icons.user=this;s.icons.currentCamera=[](void*p)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);++f.cameras;if(!f.event())return {};return 0x5000;};
  s.icons.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(f.retireWord)f.alive=false;if(!f.event())return {};auto i=f.words.find(a);return i==f.words.end()?f.resources.word(a):std::optional<uint32_t>(i->second);};
  s.icons.half=[](void*p,uint32_t a)->std::optional<uint16_t>{auto&f=*static_cast<Fixture*>(p);if(!f.event())return {};return f.resources.half(a);};
  s.icons.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&f=*static_cast<Fixture*>(p);++f.matrices;if(f.reenter){assert(!f.cursor.reset()&&!f.run());}if(!f.event())return false;if(q.function==0x8029e230)f.translations.push_back(q.xyz);BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,q);};
  s.icons.draw=[](void*p,const BattleActMatrix&m,const SourceMatrixIcon&i){auto&f=*static_cast<Fixture*>(p);if(f.retireDraw)f.alive=false;if(!f.event())return false;if(f.holdDraw)return false;f.draws.push_back({m,i});return true;};return s;
 }
 bool run(){return resumePackedBattleWeaponCursor(window,services(),cursor);}
};
void check(const Draw&d,uint32_t id,float x,float y,float scale){assert(d.icon.id==id&&d.icon.flags==(id==504?20:16)&&(d.icon.rgba&255)==255);assert(d.matrix[3]==x&&d.matrix[7]==y&&d.matrix[11]==0&&d.matrix[0]==scale);}
}
int main(){
 {Fixture f;assert(f.run()&&!f.cursor.active());assert(f.words[0x2011c]==3&&f.stores==1&&f.interpolations==1&&f.fmas==1&&f.draws.size()==3);check(f.draws[0],445,95,52,.75f);check(f.draws[1],446,95,-111,.75f);check(f.draws[2],504,-60,-13,1);}
 // Each optional nested camera/read/math/draw boundary holds without prefix replay.
 {Fixture baseline;assert(baseline.run());for(unsigned at=1;at<=baseline.events;++at){Fixture f;f.pauseAt=at;assert(!f.run()&&f.stores==1);assert(f.run()&&f.draws.size()==3&&f.stores==1&&f.interpolations==1&&f.fmas==1);for(unsigned i=0;i<3;++i){assert(f.draws[i].matrix==baseline.draws[i].matrix&&f.draws[i].icon.id==baseline.draws[i].icon.id);}}}
 {Fixture f;f.reenter=true;assert(f.run()&&f.draws.size()==3);}
 {Fixture f;f.retireWord=true;assert(!f.run()&&f.cursor.stale()&&f.draws.empty()&&f.matrices==0);}
 // Interpolation captures old operands and C, then increments the freshly loaded timer.
 {Fixture f;f.holdInterpolation=true;assert(!f.run()&&f.cursor.pending()==0x80006c8c&&f.stores==0);f.words[0x2011c]=11;f.words[0x20000]=0xdeadbeef;f.words[0x40004]=60;f.holdInterpolation=false;assert(f.run()&&f.step==2&&f.words[0x2011c]==0&&f.stores==2&&f.draws.size()==3);}
 // Exact signed wrap32 rate*12 /60 reset: no invented positive duration.
 for(uint32_t rate:{0u,0xffffffffu,0x40000000u}){Fixture f;f.words[0x40004]=rate;assert(f.run()&&f.words[0x2011c]==0&&f.stores==2);}
 // First icon held: source later branches/window fields stay fresh, existing request stable.
 {Fixture f;f.holdDraw=true;assert(!f.run()&&f.cursor.pending()==0x800197f8&&f.stores==1);unsigned matrices=f.matrices;f.window[0]=bits(4);f.window[1]=bits(50);f.words[0x30008]=6;f.words[0x30000]=100;f.holdDraw=false;assert(f.run()&&f.stores==1&&f.interpolations==1&&f.matrices==matrices+6&&f.draws.size()==2);check(f.draws[0],445,95,52,.75f);check(f.draws[1],504,4,-70,1);}
 for(uint32_t selection:{0u,6u,0xffffffffu,0x80000000u}){Fixture f;f.words[0x30004]=0;f.words[0x30008]=6;f.words[0x30000]=selection;assert(f.run()&&f.draws.size()==1);float index=selection==6?5:0;check(f.draws[0],504,-60,35-24*index,1);}
 // Fused operation holds after arrow completion. Source scale loads AFTER fma.
 {Fixture f;f.holdFma=true;assert(!f.run()&&f.draws.size()==2&&f.stores==1);f.words[0x8042245c]=bits(.5f);f.window[0]=bits(999);f.words[0x30000]=100;f.holdFma=false;assert(f.run()&&f.draws.size()==3&&f.stores==1&&f.interpolations==1&&f.fmas==2);check(f.draws.back(),504,-60,-13,.5f);}
 // fnmsubs negates the rounded fused result (zero sign is observable).
 {Fixture f;f.words[0x30004]=0;f.words[0x30008]=0;f.words[0x30000]=0;f.window[1]=bits(0);assert(f.run()&&std::signbit(f.translations[0][1]));}
 // Actual unloaded-icon early return is original behavior, not provider absence.
 {Fixture f;f.words[0x8041e638]=0;assert(f.run()&&f.draws.empty()&&f.cameras==3&&f.fmas==1);}
 {Fixture f;auto s=f.services();s.interpolate=nullptr;assert(!resumePackedBattleWeaponCursor(f.window,s,f.cursor)&&f.cursor.pending()==0x80006c8c&&f.stores==0);}
 {Fixture f;auto s=f.services();s.fma=nullptr;assert(!resumePackedBattleWeaponCursor(f.window,s,f.cursor)&&f.cursor.pending()==0x80117bec&&f.draws.size()==2&&f.stores==1);}
 {Fixture f;auto s=f.services();s.icons.draw=nullptr;assert(!resumePackedBattleWeaponCursor(f.window,s,f.cursor)&&f.draws.empty()&&f.stores==1);}
 // Reject owner/table/borrow identity changes, and permanently retire stale held work.
 {Fixture f;f.holdDraw=true;assert(!f.run());auto s=f.services();auto other=f.window;assert(!resumePackedBattleWeaponCursor(other,s,f.cursor));s.lifetime=std::make_shared<int>(1);assert(!resumePackedBattleWeaponCursor(f.window,s,f.cursor));f.alive=false;assert(!f.run()&&f.cursor.stale());f.alive=true;f.holdDraw=false;assert(!f.run()&&f.draws.empty()&&f.stores==1);assert(f.cursor.reset()&&f.run());}
 {Fixture f;f.retireDraw=true;assert(!f.run()&&f.cursor.stale()&&f.draws.size()==1&&f.stores==1&&f.fmas==0);}
}
