#include <memory>
#include <array>
#include <algorithm>
#include "battle_audience_turn_end_continuation.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
struct Fixture {
 static constexpr uint32_t B=0x10000000,C=0x11000000,U=0x20000000,P=0x21000000,I=0x22000000,S=0x22000100,G=0x22000200,T=0x22000300,K=0x23000000;
 static const EventScriptAssets& assets(){static const auto v=[](){EventScriptAssets a;assert(a.load("romfs/trio-attack-events.pmevt"));return a;}();return v;}
 const EventScriptAssets& source=assets();std::map<uint32_t,uint8_t> data;
 struct Write {uint32_t address,value;};std::vector<Write> writes;std::vector<uint32_t> reads;
 unsigned divisionAttempts=0,randomAttempts=0,randomConsumed=0,conversionAttempts=0,addAttempts=0;
 bool divideReady=true,randomReady=true,convertReady=true,addReady=true;
 int32_t randomValue=99;std::vector<float> conversions;
 const BattleAudienceAddNumInvocation* addIdentity=nullptr;
 std::vector<std::array<int32_t,2>> divisions;
 std::vector<BattleAudienceAddNumInvocation> additions;
 std::function<void(uint32_t)> onWord,onStore;
 std::function<void(unsigned)> onConvert,onRandom;
 BattleAudienceTurnEndCursor* borrowed=nullptr;
 Fixture(){word(0x8041eb90,B);word(0x8041eb00,P);owner(B);owner(C);half(B,1);half(C,1);half(P+0x8c,0);writes.clear();reads.clear();}
 void byte(uint32_t a,uint8_t n){data[a]=n;}
 uint8_t byte(uint32_t a){auto i=data.find(a);if(i!=data.end())return i->second;
  if((a>=B && a<B+0x19100) || (a>=C && a<C+0x19100) || (a>=U && a<K+0x10000))return 0;
  uint32_t n;assert(source.readWord(a&~3u,n));return uint8_t(n>>(24-8*(a&3)));}
 void word(uint32_t a,uint32_t n){for(unsigned j=0;j<4;++j)byte(a+j,uint8_t(n>>(24-8*j)));}
 uint32_t word(uint32_t a){reads.push_back(a);uint32_t n=0;for(unsigned j=0;j<4;++j)n=(n<<8)|byte(a+j);if(onWord)onWord(a);return n;}
 void half(uint32_t a,uint16_t n){byte(a,uint8_t(n>>8));byte(a+1,uint8_t(n));}
 uint16_t half(uint32_t a){reads.push_back(a);return uint16_t(uint16_t(byte(a))<<8|byte(a+1));}
 void owner(uint32_t b){const auto u=U+(b==C?0x1000:0);word(b+0x20,u);word(u+8,222);byte(u+0xc,0);byte(u+0xd,1);
  word(b+0x2738,I);word(I+0xc,S);word(S+0x18,G);word(G,4);word(G+4,T);
  const uint8_t levels[]{7,8,7,1};for(unsigned j=0;j<4;++j){word(T+0x30*j,K+0x100*j);byte(K+0x100*j+0xe,levels[j]);}
  word(b+0x15fd0,bits(8));word(b+0x15fd4,bits(10));word(b+0x15fd8,0);word(b+0x15fe8,200);
 }
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;
  a.byte=[](void* u,uint32_t p){auto& f=*static_cast<Fixture*>(u);f.reads.push_back(p);return f.byte(p);};
  a.memory.word=[](void* u,uint32_t p){return static_cast<Fixture*>(u)->word(p);};
  a.memory.half=[](void* u,uint32_t p){return static_cast<Fixture*>(u)->half(p);};
  a.memory.storeWord=[](void* u,uint32_t p,uint32_t n){auto& f=*static_cast<Fixture*>(u);f.writes.push_back({p,n});f.word(p,n);if(f.onStore)f.onStore(p);};return a;
 }
 BattleAudienceTurnEndServices services(){BattleAudienceTurnEndServices s;s.actors=actors();s.user=this;
  s.divide=[](void* u,int32_t n,int32_t d)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(u);++f.divisionAttempts;f.divisions.push_back({n,d});return f.divideReady && d?std::optional<int32_t>(n/d):std::nullopt;};
  s.truncate=[](void* u,float n)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(u);const auto index=++f.conversionAttempts;f.conversions.push_back(n);if(f.onConvert)f.onConvert(index);
   if(!f.convertReady || !std::isfinite(n) || n<-2147483648.f || n>=2147483648.f)return {};
   return int32_t(n);};
  s.random=[](void* u,int32_t bound)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(u);assert(bound==100);const auto index=++f.randomAttempts;if(f.onRandom)f.onRandom(index);
   if(!f.randomReady)return {};
   ++f.randomConsumed;return f.randomValue;};
  s.addAudience=[](void* u,const BattleAudienceAddNumInvocation& q){auto& f=*static_cast<Fixture*>(u);++f.addAttempts;
   if(f.addIdentity)assert(f.addIdentity==&q);else f.addIdentity=&q;
   if(f.borrowed){assert(!f.borrowed->reset());assert(!resumePackedBattleAudienceTurnEnd(f.services(),*f.borrowed));}
   if(!f.addReady)return false;
   f.additions.push_back(q);return true;};return s;
 }
 std::vector<uint32_t> categories(uint32_t b=B){std::vector<uint32_t> v;const auto a=b+0x2858;for(uint32_t j=0;j<word(a+0x13904);++j)v.push_back(word(a+0x138cc+4*j));return v;}
 unsigned stores(uint32_t a)const{unsigned n=0;for(auto w:writes)if(w.address==a)++n;return n;}
};
void ordinary(){
 for(int turn:{1,45,46,47})for(int draw:{0,99}){Fixture f;f.half(Fixture::B,uint16_t(turn));f.randomValue=draw;BattleAudienceTurnEndCursor c;
  assert(resumePackedBattleAudienceTurnEnd(f.services(),c));assert((f.randomConsumed==1 && f.divisions==std::vector<std::array<int32_t,2>>{{46,1}}));
  std::vector<uint32_t> expected;if(turn==46)expected.push_back(6);
  if(!draw){if(turn>=46)expected.push_back(1);for(auto k:{4u,9u,10u,11u,12u,13u})expected.push_back(k);}
  assert(f.categories()==expected);assert(f.word(Fixture::B+0x16160)==uint32_t(draw?3:0));
  assert(scalar(f.word(Fixture::B+0x15fd4))==(turn>=46?9.6f:10.f));
  assert(f.conversionAttempts==(turn>=46?2u:1u));assert(f.additions.empty());
 }
 Fixture disabled;disabled.word(Fixture::B+0x2858,0x40000);BattleAudienceTurnEndCursor dc;
 assert(resumePackedBattleAudienceTurnEnd(disabled.services(),dc));assert(disabled.writes.empty() && !disabled.divisionAttempts && !disabled.randomAttempts);
 const auto position=[&](uint32_t address){const auto i=std::find(disabled.reads.begin(),disabled.reads.end(),address);assert(i!=disabled.reads.end());return i-disabled.reads.begin();};
 assert(position(0x8041eb00)<position(Fixture::B+0x2738) && position(Fixture::B+0x2738)<position(Fixture::B+0x2858));
 assert(position(Fixture::B+0x2858)<position(Fixture::I+0xc) && position(Fixture::I+0xc)<position(Fixture::S+0x18) && position(Fixture::S+0x18)<position(Fixture::G+4));
 bool sawSets=false,sawPouch=false;for(auto a:disabled.reads){sawSets|=a==Fixture::G+4;sawPouch|=a==0x8041eb00;}assert(sawSets && sawPouch);
 Fixture saved;saved.onWord=[&](uint32_t address){if(address==Fixture::G+4)saved.word(Fixture::B+0x2858,0x40000);};BattleAudienceTurnEndCursor savedCursor;
 assert(resumePackedBattleAudienceTurnEnd(saved.services(),savedCursor) && saved.divisionAttempts==1 && saved.randomConsumed==1 && saved.conversionAttempts==0);
 Fixture chance;chance.word(Fixture::B+0x16160,99);BattleAudienceTurnEndCursor cc;assert(resumePackedBattleAudienceTurnEnd(chance.services(),cc));assert(chance.word(Fixture::B+0x16160)==0); // draw99<clamped100
 Fixture wrap;wrap.word(Fixture::B+0x16160,0x7fffffff);BattleAudienceTurnEndCursor wc;assert(resumePackedBattleAudienceTurnEnd(wrap.services(),wc));assert(wrap.word(Fixture::B+0x16160)==0x80000002u && wrap.categories().empty());
 Fixture minimum;minimum.word(Fixture::G,0);BattleAudienceTurnEndCursor mc;assert(resumePackedBattleAudienceTurnEnd(minimum.services(),mc));assert((minimum.divisions==std::vector<std::array<int32_t,2>>{{0,1}}));
}
void holds(){
 Fixture d;d.divideReady=false;BattleAudienceTurnEndCursor dc;
 assert(!resumePackedBattleAudienceTurnEnd(d.services(),dc) && dc.pending()==0x8019f334);
 const auto stores=d.writes.size(),reads=d.reads.size();d.byte(Fixture::U+0xd,9);
 assert(!resumePackedBattleAudienceTurnEnd(d.services(),dc) && d.writes.size()==stores && d.reads.size()==reads);
 assert((d.divisions.back()==std::array<int32_t,2>{46,1}));d.divideReady=true;assert(resumePackedBattleAudienceTurnEnd(d.services(),dc));
 Fixture target;target.half(Fixture::B,46);target.convertReady=false;BattleAudienceTurnEndCursor tc;
 assert(!resumePackedBattleAudienceTurnEnd(target.services(),tc));const auto prefix=target.writes.size();
 assert(target.stores(Fixture::B+0x16160)==1 && !target.randomConsumed);
 assert(!resumePackedBattleAudienceTurnEnd(target.services(),tc) && target.writes.size()==prefix);
 target.convertReady=true;assert(resumePackedBattleAudienceTurnEnd(target.services(),tc));assert(target.stores(Fixture::B+0x16160)==1 && target.randomConsumed==1 && target.categories()==std::vector<uint32_t>{6});
 Fixture rng;rng.half(Fixture::B,46);rng.randomReady=false;BattleAudienceTurnEndCursor rc;
 assert(!resumePackedBattleAudienceTurnEnd(rng.services(),rc) && rc.pending()==0x80005da0);const auto before=rng.writes.size();
 assert(!resumePackedBattleAudienceTurnEnd(rng.services(),rc) && rng.writes.size()==before && !rng.randomConsumed);
 rng.randomReady=true;assert(resumePackedBattleAudienceTurnEnd(rng.services(),rc) && rng.randomConsumed==1 && rng.categories()==std::vector<uint32_t>{6});
 Fixture conversion;conversion.convertReady=false;BattleAudienceTurnEndCursor nc;
 assert(!resumePackedBattleAudienceTurnEnd(conversion.services(),nc) && nc.pending()==0x801a1c90);const auto changes=conversion.writes.size();conversion.word(Fixture::B+0x15fd4,bits(99));
 assert(!resumePackedBattleAudienceTurnEnd(conversion.services(),nc) && conversion.writes.size()==changes && conversion.conversions.back()==10);
 conversion.convertReady=true;assert(resumePackedBattleAudienceTurnEnd(conversion.services(),nc) && conversion.randomConsumed==1);
 Fixture add;add.half(Fixture::P+0x8c,200);add.addReady=false;BattleAudienceTurnEndCursor ac;
 assert(!resumePackedBattleAudienceTurnEnd(add.services(),ac) && ac.pending()==0x801a1708);const auto written=add.writes.size(),read=add.reads.size();
 assert(!resumePackedBattleAudienceTurnEnd(add.services(),ac) && add.writes.size()==written && add.reads.size()==read && add.randomConsumed==1);
 auto changed=add.services();changed.user=nullptr;assert(!resumePackedBattleAudienceTurnEnd(changed,ac));
 add.word(0x8041eb90,Fixture::C);add.half(Fixture::P+0x8c,0);add.addReady=true;add.borrowed=&ac;
 assert(resumePackedBattleAudienceTurnEnd(add.services(),ac) && add.additions.size()==1);
 const auto q=add.additions[0];assert(q.battle==Fixture::B && q.audience==Fixture::B+0x2858 && q.pouch==Fixture::P && q.amount==10 && q.capacity==200);
}
void mutation(){
 Fixture owners;owners.onRandom=[&](unsigned){owners.word(0x8041eb90,Fixture::C);};owners.randomValue=0;BattleAudienceTurnEndCursor oc;
 assert(resumePackedBattleAudienceTurnEnd(owners.services(),oc));assert(owners.categories(Fixture::B).empty() && owners.categories(Fixture::C)==std::vector<uint32_t>({4,9,10,11,12,13}));
 assert(owners.word(Fixture::B+0x16160)==0 && owners.word(Fixture::C+0x16160)==0); // clear captured parent A, never substitute helper owner
 Fixture late;late.half(Fixture::B,46);late.onConvert=[&](unsigned n){if(n==1)late.half(Fixture::B,47);};late.randomValue=0;BattleAudienceTurnEndCursor lc;
 assert(resumePackedBattleAudienceTurnEnd(late.services(),lc));assert(late.categories()==std::vector<uint32_t>({1,4,9,10,11,12,13})); // fresh turn skipscategory6
 Fixture fresh;fresh.onStore=[&](uint32_t a){if(a==Fixture::B+0x16160)fresh.word(a,100);};BattleAudienceTurnEndCursor fc;
 assert(resumePackedBattleAudienceTurnEnd(fresh.services(),fc) && fresh.word(Fixture::B+0x16160)==100); // callback modifies store before fresh clamp/RNG reads
 Fixture count;count.half(Fixture::P+0x8c,200);
 const auto a=Fixture::B+0x2858;
 for(unsigned j=0;j<6;++j){const auto r=a+0x1bc+0x134*j;count.word(r,j==1?0x101:j==2?3:j==3?0x11:1);count.byte(r+0x19,j==4?12:0);}
 BattleAudienceTurnEndCursor pc;assert(resumePackedBattleAudienceTurnEnd(count.services(),pc));assert(count.additions.size()==1 && count.additions[0].amount==7); // slots0,3,5 count (control10 doesnotexclude)
 Fixture recapture;recapture.half(Fixture::P+0x8c,200);const auto record=Fixture::B+0x2858+0x1bc;
 recapture.word(record,1);recapture.byte(Fixture::C+0x2858+0x1bc+0x19,12);bool flipped=false;
 recapture.onWord=[&](uint32_t p){if(p==record && !flipped){flipped=true;recapture.word(0x8041eb90,Fixture::C);}};
 BattleAudienceTurnEndCursor rec;assert(resumePackedBattleAudienceTurnEnd(recapture.services(),rec));
 assert(flipped && recapture.additions.size()==1 && recapture.additions[0].amount==10 && recapture.additions[0].battle==Fixture::C);
 Fixture missing;missing.half(Fixture::P+0x8c,200);auto ms=missing.services();ms.addAudience=nullptr;BattleAudienceTurnEndCursor missingCursor;
 assert(!resumePackedBattleAudienceTurnEnd(ms,missingCursor) && missingCursor.pending()==0x801a1708);
 const auto prefix=missing.writes.size(),reads=missing.reads.size();assert(!resumePackedBattleAudienceTurnEnd(ms,missingCursor) && missing.writes.size()==prefix && missing.reads.size()==reads);
 Fixture duplicate;duplicate.randomValue=0;const auto da=Fixture::B+0x2858;duplicate.word(da+0x13904,1);duplicate.word(da+0x138cc,4);BattleAudienceTurnEndCursor dup;
 assert(resumePackedBattleAudienceTurnEnd(duplicate.services(),dup) && duplicate.categories()==std::vector<uint32_t>({4,9,10,11,12,13}));
 Fixture reloaded;reloaded.randomValue=0;bool touched=false;const auto ra=Fixture::B+0x2858;
 reloaded.onStore=[&](uint32_t p){if(p==ra+0x138cc && !touched){touched=true;reloaded.word(ra+0x13904,2);}};
 BattleAudienceTurnEndCursor reload;assert(resumePackedBattleAudienceTurnEnd(reloaded.services(),reload));
 assert(touched && reloaded.word(ra+0x13904)==8 && reloaded.word(ra+0x138cc)==4 && reloaded.word(ra+0x138cc+12)==9);
 Fixture zero;zero.word(Fixture::B+0x15fd4,0);zero.half(Fixture::P+0x8c,200);auto zs=zero.services();zs.addAudience=nullptr;BattleAudienceTurnEndCursor zc;assert(resumePackedBattleAudienceTurnEnd(zs,zc));
 Fixture gate;gate.word(Fixture::B+0xef4,0x100);gate.half(Fixture::P+0x8c,200);auto gs=gate.services();gs.addAudience=nullptr;BattleAudienceTurnEndCursor gc;assert(resumePackedBattleAudienceTurnEnd(gs,gc));
}
}
int main(){ordinary();holds();mutation();std::cout<<"Whole original TurnEnd source graph, retained target/RNG/population waits and independent owners passed\n";}
