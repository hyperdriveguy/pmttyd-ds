#include "battle_drop_material.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <iostream>
#include <vector>
using namespace ttyd;
struct Fixture {
    EventNativeRuntime vm{0x81000000,{}};
    std::vector<int32_t> draws,bounds;unsigned cursor=0,badgeQueries=0;int32_t story=11,badges=0;
    Fixture(){
        for(auto block:std::vector<std::pair<uint32_t,unsigned>>{{0x8041eb90,4},{0x1000,0x600},{0x2000,0x400},{0x802eea40,100}})assert(vm.memory.registerRam(block.first,std::vector<uint8_t>(block.second)));
        w(0x8041eb90,0x1000);w(0x1024,0x2000);w(0x2008,222);w(0x110c,0x1300);w(0x1318,0x1400);w(0x1404,0x1500);w(0x1414,0x1480);w(0x1418,0x14e0);
        h(0x210c,8);h(0x2108,10);h(0x2112,5);h(0x210e,5); // Mario resources, separate from setup records.
        for(unsigned i=0;i<5;++i){const int threshold[]={20,30,50,80,100};const int heart[]={70,60,50,40,30};
            for(auto table:{0x1480u,0x14e0u}){w(table+16*i,threshold[i]);w(table+16*i+4,table==0x1480?heart[i]:i==4?30:40);w(table+16*i+8,2);w(table+16*i+12,table==0x1480?(i<2?50:i<4?40:30):40);}}
        w(0x1408,10);w(0x140c,20);w(0x1410,70);
        for(unsigned i=0;i<25;++i)w(0x802eea40+4*i,900+i);
    }
    void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}
    void h(uint32_t a,int32_t v){assert(vm.memory.writeHalf(a,uint16_t(v)));}
    uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
    void run(std::initializer_list<int32_t> input){draws=input;cursor=0;bounds.clear();badgeQueries=0;BattleDropMaterialServices s;s.actors=nativeBattleActorServices(vm);s.user=this;
        s.random=[](void* self,int32_t bound){auto& f=*static_cast<Fixture*>(self);assert(f.cursor<f.draws.size());const auto result=f.draws[f.cursor++];assert(result>=0 && result<bound);f.bounds.push_back(bound);return result;};
        s.story=[](void* self){return static_cast<Fixture*>(self)->story;};s.equippedBadgeCount=[](void* self,int32_t id){auto& f=*static_cast<Fixture*>(self);assert(id==314);++f.badgeQueries;return f.badges;};
        battleInformationSetDropMaterial(0x1100,s);assert(cursor==draws.size());
    }
};
int main(){
    Fixture early;early.w(0x130c,0xffffffff);early.w(0x1310,0xffffffff);early.w(0x1314,0xffffffff); // Old drop outputs must be overwritten.
    early.h(0x210c,8);early.h(0x2108,10);early.h(0x2112,5);early.h(0x210e,5);
    early.run({0,39,40,29,39,40,17});assert(early.w(0x1310)==1 && early.w(0x1314)==1 && early.w(0x130c)==0);
    assert(early.bounds==std::vector<int32_t>({100,100,100,100,100,100,100}) && early.badgeQueries==2);
    Fixture upper;upper.h(0x210c,25);upper.run({30,30,99});assert(upper.w(0x1310)==0 && upper.w(0x1314)==0 && upper.w(0x130c)==0);
    Fixture lower;lower.h(0x210c,-1);lower.run({69,49,50,30,0});assert(lower.w(0x1310)==1 && lower.w(0x1314)==0);
    Fixture back;back.story=20;back.w(0x152c,0x1580);back.w(0x1580,123);back.h(0x1586,1);back.w(0x1588,456);back.h(0x158e,2);
    back.run({40,30,5,1});assert(back.w(0x130c)==456 && back.bounds==std::vector<int32_t>({100,100,100,3}) && back.badgeQueries==1);
    Fixture held;held.story=20;held.w(0x1408,50);held.w(0x140c,0);held.w(0x1410,50);held.w(0x131c,777);held.w(0x1338,888);
    held.run({40,30,0,1});assert(held.w(0x130c)==888 && held.bounds==std::vector<int32_t>({100,100,100,2}));assert(held.w(0x131c)==777 && held.w(0x1338)==888);
    Fixture fallback;fallback.badges=2;fallback.run({40,30,99,17});assert(fallback.w(0x130c)==917 && fallback.bounds==std::vector<int32_t>({100,100,100,25}) && fallback.badgeQueries==2);
    Fixture multiplier;multiplier.story=20;multiplier.badges=1;multiplier.w(0x1408,50);multiplier.w(0x140c,0);multiplier.w(0x1410,50);multiplier.w(0x131c,777);
    multiplier.run({40,30,75,0});assert(multiplier.w(0x130c)==777 && multiplier.bounds==std::vector<int32_t>({100,100,150,1}));
    std::cout<<"Original resource ratios/drop RNG order, early story gate, weighted back/held items and badge fallback passed\n";
}
