#include "battle_damage_orchestration.hpp"
#include <cassert>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <limits>
#include <iostream>
using namespace ttyd;
constexpr uint32_t B=0x1000,A=0x30000,D=0x32000,P=0x34000,W=0x35000;
struct Hit {int32_t attacker;uint32_t unit,part;int32_t hp,fp;uint32_t code;int32_t pattern,recovery;};
struct Fixture {
    std::unordered_map<uint32_t,uint8_t> ram;
    std::set<std::pair<uint32_t,int32_t>> statuses;
    std::vector<std::string> trace;
    std::vector<Hit> hits;std::vector<std::pair<uint32_t,uint32_t>> events;
    std::vector<DamageAudienceRequest> audiences;std::vector<int32_t> randomBounds,randomValues,defenses;
    std::vector<uint32_t> calcFlags,calcCounts,directSequences;std::vector<int32_t> counterTypes;
    int32_t hp=0,fp=0,possible=0;uint32_t code=30,inflicted=0;bool changeBattle=false,fpRewrite=false;
    unsigned defenseIndex=0,randomIndex=0;
    Fixture(){w(0x8041eb90,B);w(A,1);w(D,2);w(D+8,3);w(D+0x10,0x36000);w(0x2000c,0x20100);b(0x3608c,100);}
    uint8_t b(uint32_t at){return ram[at];}void b(uint32_t at,uint8_t v){ram[at]=v;}
    uint16_t h(uint32_t at){return uint16_t(b(at)<<8|b(at+1));}void h(uint32_t at,uint16_t v){b(at,v>>8);b(at+1,v);}
    uint32_t w(uint32_t at){return uint32_t(b(at))<<24|uint32_t(b(at+1))<<16|uint32_t(b(at+2))<<8|b(at+3);}
    void w(uint32_t at,uint32_t v){for(unsigned i=0;i<4;++i)b(at+i,v>>(24-8*i));}
    BattleDamageOrchestrationServices services(){
        BattleDamageOrchestrationServices s;s.user=this;s.memory.user=this;s.memory.memory.user=this;
        s.memory.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->w(a);};
        s.memory.memory.half=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->h(a);};
        s.memory.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->b(a);};
        s.memory.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->w(a,v);};
        s.memory.memory.storeHalf=[](void* p,uint32_t a,uint16_t v){static_cast<Fixture*>(p)->h(a,v);};
        s.memory.memory.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->b(a,v);};
        s.status=[](void* p,uint32_t u,int32_t type){return static_cast<Fixture*>(p)->statuses.count({u,type})!=0;};
        s.acPossible=[](void* p,uint32_t){return static_cast<Fixture*>(p)->possible;};
        s.defenseResult=[](void* p){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("defense");return f.defenses.at(f.defenseIndex++);};
        s.hitPosition=[](void*,uint32_t u,uint32_t part){assert(u==D && part==P);return DamagePosition{100,20,30};};
        s.niceEntry=[](void* p,int32_t mode,DamagePosition pos){assert(pos.x==40 && pos.y==65 && pos.z==20);auto& f=*static_cast<Fixture*>(p);f.trace.push_back(mode==2?"superguard":"guard");return uint32_t(0x20000);};
        s.systemSound=[](void* p,uint32_t key){assert(key==0x802ee064 || key==0x802ee078);static_cast<Fixture*>(p)->trace.push_back("sound");};
        s.audience=[](void* p,DamageAudienceRequest kind,int32_t){auto& f=*static_cast<Fixture*>(p);f.audiences.push_back(kind);f.trace.push_back("audience");};
        s.hpDamage=[](void* p,uint32_t a,uint32_t d,uint32_t part,uint32_t w,uint32_t& code,uint32_t flags){assert(a==A && d==D && part==P && w==W);auto& f=*static_cast<Fixture*>(p);f.trace.push_back("hp");f.calcFlags.push_back(flags);f.calcCounts.push_back(f.h(D+0x260));code=f.code;if(f.changeBattle)f.w(0x8041eb90,0x17000);return f.hp;};
        s.fpDamage=[](void* p,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t& code,uint32_t flags){auto& f=*static_cast<Fixture*>(p);if(f.fpRewrite)code=0xdeadbeef;f.trace.push_back("fp");f.calcFlags.push_back(flags);f.calcCounts.push_back(f.h(D+0x260));return f.fp;};
        s.weaponStatus=[](void* p,uint32_t,uint32_t d,uint32_t,uint32_t,uint32_t){auto& f=*static_cast<Fixture*>(p);assert(f.h(d+0x260)==uint16_t(f.calcCounts.back()+1));f.trace.push_back("status");return f.inflicted;};
        s.counterStatus=[](void* p,uint32_t& result,uint32_t u,uint32_t part,int32_t element,int32_t type,int32_t rate,int32_t chance,int32_t duration,int32_t strength){assert(result==0 && u==A && part==P && element==0 && rate==100 && chance==0 && duration==3 && strength==1);auto& f=*static_cast<Fixture*>(p);f.counterTypes.push_back(type);f.trace.push_back("counter-status");result=999;};
        s.emergencyRevival=[](void* p,uint32_t u,uint32_t& code){assert(u==D);static_cast<Fixture*>(p)->trace.push_back("revival");code|=0x20000000;};
        s.hitEvent=[](void* p,uint32_t u,uint32_t code){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("event");f.events.push_back({u,code});};
        s.hitEventDirect=[](void* p,uint32_t u,uint32_t code,uint32_t script){assert(u==A && code==311 && script==0x12345);static_cast<Fixture*>(p)->trace.push_back("direct-event");};
        s.directDamage=[](void* p,int32_t a,uint32_t u,uint32_t part,int32_t hp,int32_t fp,uint32_t code,int32_t pattern,int32_t recovery){auto& f=*static_cast<Fixture*>(p);f.trace.push_back(u==D?"defender":"attacker");f.hits.push_back({a,u,part,hp,fp,code,pattern,recovery});f.directSequences.push_back(f.w(A+0x280));};
        s.irand=[](void* p,int32_t bound){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("random");f.randomBounds.push_back(bound);return f.randomIndex<f.randomValues.size()?f.randomValues[f.randomIndex++]:0;};
        s.bombEntry=[](void* p,int32_t mode,DamagePosition pos,float scale){assert(mode==0 && pos.x==100 && pos.y==20 && pos.z==30 && scale==1);static_cast<Fixture*>(p)->trace.push_back("bomb");};
        s.spatialSound=[](void* p,uint32_t key,DamagePosition){assert(key==0x802ee08c);static_cast<Fixture*>(p)->trace.push_back("spatial-sound");};
        s.fogStop=[](void* p){static_cast<Fixture*>(p)->trace.push_back("fog-stop");};
        s.stopAction=[](void* p,uint32_t battle){assert(battle==B);static_cast<Fixture*>(p)->trace.push_back("stop");};
        s.helpType=[](void* p,int32_t type){assert(type==0);static_cast<Fixture*>(p)->trace.push_back("help");};return s;
    }
    int32_t run(uint32_t flags=0){return checkPackedBattleDamage(A,D,P,W,flags,services());}
};
int main(){
    Fixture empty;assert(checkPackedBattleDamage(0,0,0,0,0,empty.services())==18 && empty.trace.empty());
    for(int mode=0;mode<=5;++mode){Fixture f;f.possible=1;f.defenses={5};f.b(W+0x13,mode);f.w(A+0x280,255);f.w(A+0x2d0,mode==4?0x12345:0);
        assert(f.run()==(mode==2 || mode==3?21:-1));assert(f.w(A+0x280)==256 && f.b(D+0x273)==0);
        assert(f.directSequences[0]==256);assert(f.calcFlags.empty() && f.hits[0].code==(0x80000u|42|(mode==2?0:0x100)) && !f.hits[0].recovery);
        assert(f.w(0x2011c)==0x3f400000 && f.audiences==std::vector<DamageAudienceRequest>{DamageAudienceRequest::GuardGood});
        if(mode==4)assert(f.trace.back()=="direct-event" && f.hits.size()==1);
        else if(mode!=2 && mode!=3)assert(f.hits.size()==2 && f.hits[1].hp==1 && f.hits[1].code==311);
    }
    Fixture normal;normal.hp=3;normal.h(D+0x260,65535);assert(normal.run()==18);assert((normal.calcCounts==std::vector<uint32_t>{65535,65535}));assert(normal.h(D+0x260)==0 && normal.w(D+0x258)==P && normal.w(A+0x280)==1);
    assert(normal.directSequences[0]==0);assert(normal.hits.size()==1 && normal.hits[0].hp==3 && normal.audiences[0]==DamageAudienceRequest::EnemyDamage);
    Fixture zero;assert(zero.run()==19 && zero.hits.size()==1 && zero.hits[0].hp==0 && zero.audiences[0]==DamageAudienceRequest::EnemyNoDamage);
    Fixture fp;fp.fp=2;assert(fp.run()==18 && fp.hits[0].fp==2 && fp.audiences.empty());
    Fixture rewrite;rewrite.fp=2;rewrite.fpRewrite=true;assert(rewrite.run()==18 && rewrite.hits[0].code==0xdeadbeef);
    Fixture negative;negative.hp=-3;assert(negative.run()==18 && negative.audiences.empty());
    Fixture silent;silent.b(W+0x6d,27);assert(silent.run()==19 && silent.audiences.empty());
    Fixture disabled;disabled.w(W+0x74,0x20000);auto disabledServices=disabled.services();disabledServices.acPossible=nullptr;disabledServices.defenseResult=nullptr;assert(checkPackedBattleDamage(A,D,P,W,0,disabledServices)==19);
    Fixture guard;guard.possible=1;guard.defenses={3,4};guard.inflicted=15;assert(guard.run(0x40000100)==19);assert(guard.defenseIndex==2 && guard.calcFlags[0]&0x40000);assert(guard.hits[0].code==30);
    for(uint32_t bits:{1u,2u,4u,8u,12u,32u}){Fixture f;f.inflicted=bits;assert(f.run(0x100)==18);assert(f.hits[0].code==(0x100u|(bits&8?29:bits&4?28:bits&1?23:22)));assert(f.hits[0].recovery==(bits&32?0:1));}
    Fixture contact;contact.w(W+0x74,0x4000);contact.w(P+0x1ac,0x800);assert(contact.run(0x100)==18 && (contact.calcFlags[0]&0x400000) && (contact.hits[0].code&255)==23);
    Fixture vapor;vapor.inflicted=16;vapor.w(D+0x274,123);assert(vapor.run(0x100)==18 && vapor.hits.empty() && vapor.events[0].second==0x20000717 && vapor.w(D+0x274)==0);
    Fixture freeze;freeze.w(W+0x74,0x80);freeze.statuses.insert({D,9});freeze.b(W+0x6d,14);freeze.w(D+0x104,0x4000);assert(freeze.run(0x100)==19);assert((freeze.randomBounds==std::vector<int32_t>{100,100}) && freeze.events.empty() && freeze.hits[0].pattern==0);
    Fixture shatter;shatter.w(W+0x74,0x80);shatter.statuses.insert({D,9});shatter.randomValues={-1};shatter.w(D+0x274,123);assert(shatter.run(0x100)==18 && shatter.hits.empty() && shatter.events[0].second==0x20008717 && shatter.w(D+0x274)==123);
    for(int32_t damage:{5,-5,std::numeric_limits<int32_t>::max()-2}){Fixture f;f.hp=damage;f.w(P+0x1b0,0x100);f.statuses.insert({D,21});f.statuses.insert({D,22});f.b(D+0x2f0,1);f.changeBattle=true;
        assert(f.run()==-1 && f.hits.size()==2);assert((f.directSequences==std::vector<uint32_t>{0,1}));const int32_t expected=damage==5?6:damage==-5?-6:-3;assert(f.hits[1].hp==expected && f.hits[1].part==0 && f.hits[1].attacker==-5);
        const std::vector<std::string> tail={"audience","bomb","spatial-sound","fog-stop","stop","help","attacker"};assert(std::vector<std::string>(f.trace.end()-tail.size(),f.trace.end())==tail);
    }
    Fixture spike;spike.w(P+0x1b0,1);spike.w(D+0x274,99);assert(spike.run()==-1 && spike.calcFlags.empty() && spike.hits.size()==1 && spike.hits[0].unit==A && spike.hits[0].hp==1 && spike.events[0].second==31 && spike.w(D+0x274)==0);
    Fixture status;status.w(P+0x1b0,0x200);status.statuses.insert({D,6});assert(status.run()==-1 && (status.counterTypes==std::vector<int32_t>{4}));
    Fixture burn;burn.w(P+0x1b0,0x20);assert(burn.run()==-1 && (burn.counterTypes==std::vector<int32_t>{8}));
    Fixture returnEvent;returnEvent.w(P+0x1b0,0x2000);assert(returnEvent.run()==-1 && returnEvent.calcFlags.empty() && returnEvent.events[0].second==1817);
    for(uint32_t mask=0;mask<32;++mask){std::vector<unsigned> enabled;for(unsigned i=0;i<5;++i)if(mask&(1u<<i))enabled.insert(enabled.begin(),i);
        for(unsigned rank=0;rank<std::max<size_t>(1,enabled.size());++rank){Fixture f;f.w(W+0x74,0x800);f.w(B+0x163f4,mask<<8);f.randomValues={int32_t(rank)};uint32_t flags=0x40;
            const uint32_t expected=0x40|(mask?0x01000000u<<enabled[rank]:0);assert(packedBattleCheckPikkyoro(W,&flags,f.services())==expected && flags==expected);
            assert(f.randomBounds.size()==(enabled.size()>=2?1:0));}
    }
    Fixture noPointer;noPointer.w(W+0x74,0x800);noPointer.w(B+0x163f4,0x1100);assert(packedBattleCheckPikkyoro(W,nullptr,noPointer.services())==0x10000000 && noPointer.randomBounds[0]==2);
    std::cout<<"Original damage orchestration: fresh guard reads, zero/FP/negative damage, status priority, retained RNG, source counter tail and captured battle passed\n";
}
