#include "battle_enemy_hp_publication.hpp"
#include <cassert>
#include <map>
using namespace ttyd;
namespace {
struct Fixture {
 BattleEnemyHpGpuResource hp;BattleEnemyHpOrderedPresentation owner;
 std::map<uint32_t,BattleEnemyHpIconResidency> icons;
 uint64_t allocation=17,completed=0;bool alive=true,iconReady=true,viewReady=true,retireOnIcon=false,loseOnComplete=false;
 Fixture(const char* root){
   BattleEnemyHpTextureServices s;s.user=this;s.import=[](void* p,const char*)->std::optional<BattleEnemyHpTextureAdmission>{return BattleEnemyHpTextureAdmission{p,128,32,0};};s.release=[](void*,const void*){};assert(hp.load(root,s));
   for(unsigned image:{100u,101u})icons[image]={this,std::make_shared<const uint8_t>(0),image,16,20,32,32,0};
 }
 BattleEnemyHpPublicationServices services(){
   BattleEnemyHpPublicationServices s;s.user=this;s.hp=&hp;
   s.current=[](void* p,uint64_t a){auto& f=*static_cast<Fixture*>(p);return f.alive && f.allocation==a;};
   s.complete=[](void* p,uint64_t pass){auto& f=*static_cast<Fixture*>(p);if(f.loseOnComplete)f.alive=false;return pass==f.completed;};
   s.icon=[](void* p,uint32_t image)->std::optional<BattleEnemyHpIconResidency>{auto& f=*static_cast<Fixture*>(p);if(f.retireOnIcon)f.owner.retire();const auto i=f.icons.find(image);return f.iconReady && i!=f.icons.end()?std::optional<BattleEnemyHpIconResidency>(i->second):std::nullopt;};
   s.view=[](void* p)->std::optional<BattleEnemyHpRasterView>{auto& f=*static_cast<Fixture*>(p);if(!f.viewReady)return {};BattleEnemyHpRasterView v;v.viewport=v.scissor={0,0,240,400};v.projection={{{{1,0,0,0}},{{0,1,0,0}},{{0,0,1,0}},{{0,0,0,1}}}};return v;};return s;
 }
 RetainedIconPacket icon(uint32_t image){RetainedIconPacket q;q.image=image;q.camera=8;q.viewport=q.scissor={0,0,240,400};q.transform={{{{1,0,0,0}},{{0,1,0,0}},{{0,0,1,0}},{{0,0,0,1}}}};return q;}
 BattleEnemyHpGpuQuad quad(uint32_t index){
   BattleEnemyHpGpuBatch batch;const BattleActMatrix m{1,0,0,0,0,1,0,0,0,0,1,0};
   const std::array<std::array<float,3>,4> p{{{-20,8,0},{20,8,0},{20,-8,0},{-20,-8,0}}};
   const std::array<std::array<float,2>,4> uv{{{0,0},{1,0},{1,1},{0,1}}};
   assert(hp.quad(index,index?BattleEnemyHpMaterial::TexturedModulate:BattleEnemyHpMaterial::UntexturedRaster,m,p,uv,{1,2,3,4},batch.sink()));return batch.quads[0];
 }
 bool bind(){return owner.bind(allocation,services());}
 void unit(uint64_t pass,uint32_t image,bool solid){assert(owner.appendIcon(pass,icon(image)));assert(owner.appendHp(pass,quad(17)));if(solid)assert(owner.appendHp(pass,quad(0)));assert(owner.appendHp(pass,quad(18)));}
};
}
int main(int argc,char** argv){
 const char* root=argc>1?argv[1]:"romfs";
 Fixture f(root);auto missing=f.services();missing.current=nullptr;assert(!f.owner.bind(17,missing));assert(!f.owner.begin(1));assert(f.bind());assert(f.owner.begin(1));assert(f.owner.begin(1));assert(!f.owner.begin(2));
 f.unit(1,100,true);f.unit(1,101,false);assert(!f.owner.snapshot());assert(f.owner.retains(100) && f.owner.retains(101));assert(!f.owner.publish(1));f.completed=1;assert(f.owner.publish(1));
 auto publication=f.owner.snapshot();assert(publication && publication->packets.size()==7 && f.owner.validate(*publication));
 const auto& packets=publication->packets;assert(std::get<BattleEnemyHpIconPacket>(packets[0]).packet.image==100);assert(std::get<BattleEnemyHpRasterPacket>(packets[1]).quad.index==17);assert(std::get<BattleEnemyHpRasterPacket>(packets[2]).quad.material==BattleEnemyHpMaterial::UntexturedRaster);assert(std::get<BattleEnemyHpRasterPacket>(packets[3]).quad.index==18);assert(std::get<BattleEnemyHpIconPacket>(packets[4]).packet.image==101);assert(std::get<BattleEnemyHpRasterPacket>(packets[5]).quad.index==17);assert(std::get<BattleEnemyHpRasterPacket>(packets[6]).quad.index==18);
 auto forged=*publication;assert(!f.owner.validate(forged));assert(!f.owner.begin(1));assert(f.owner.begin(2));f.unit(2,101,true);assert(f.owner.snapshot()==publication && f.owner.validate(*publication));
 // The old sink can never append into the new source pass.
 BattleEnemyHpOrderedHpSink old{&f.owner,1};assert(!old.services().append(old.services().user,f.quad(18)));
 f.iconReady=false;assert(!f.owner.appendIcon(2,f.icon(100)));f.iconReady=true;f.viewReady=false;assert(!f.owner.appendHp(2,f.quad(18)));f.viewReady=true;
 // Last icon generation disappears while the next pass holds: whole new pass
 // remains unpublished and the old publication fails full resource preflight.
 auto oldGeneration=f.icons[101].generation;f.icons[101].generation=std::make_shared<const uint8_t>(0);f.completed=2;assert(!f.owner.publish(2) && f.owner.snapshot()==publication);assert(!f.owner.validate(*publication));f.icons[101].generation=oldGeneration;assert(f.owner.publish(2));auto second=f.owner.snapshot();assert(second!=publication && f.owner.validate(*second));assert(!f.owner.validate(*publication));
 // Actual allocation loss during a completion callback discards building AND
 // publication before any subsequent draw admission; no partial new frame.
 assert(f.owner.begin(3));f.unit(3,100,false);f.completed=3;f.loseOnComplete=true;assert(!f.owner.publish(3));assert(!f.owner.snapshot() && !f.owner.retains(100));assert(!f.owner.validate(*second));
 Fixture stale(root);assert(stale.bind() && stale.owner.begin(1));stale.unit(1,100,true);stale.completed=1;stale.hp.retire();assert(!stale.owner.publish(1) && !stale.owner.snapshot());
 Fixture reentrant(root);assert(reentrant.bind() && reentrant.owner.begin(1));reentrant.retireOnIcon=true;assert(!reentrant.owner.appendIcon(1,reentrant.icon(100)));assert(!reentrant.owner.snapshot() && !reentrant.owner.buildingPass());
 Fixture empty(root);assert(empty.bind() && empty.owner.begin(1));empty.completed=1;assert(empty.owner.publish(1) && empty.owner.snapshot()->packets.empty() && empty.owner.validate(*empty.owner.snapshot()));
 empty.owner.retire();f.hp.retire();reentrant.hp.retire();empty.hp.retire();
}
