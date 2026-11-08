#include <memory>
#include "battle_weapon_source_icon_pin.hpp"
#include "battle_weapon_icon_evidence.hpp"
#include <cassert>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <limits>
#include <fstream>
#include <map>
#include <cmath>
#include <cstring>
#include "source_palette_texture_object.hpp"
using namespace ttyd;
namespace {
// Explicit TEST process owner. Real cold/history capture executes; this is not
// a receipt for native GXInit or the normal source scheduler.
std::shared_ptr<SourceGxInheritedState> testHistory(std::shared_ptr<const void> process){
 auto h=std::make_shared<SourceGxInheritedState>();SourceGxInheritedBinding b;b.process=std::move(process);b.current=[](void*){return true;};b.coldCurrent=b.current;assert(h->bind(b)&&h->initializeCold());return h;
}
std::shared_ptr<BattleWeaponIconEvidence> testEvidence(const std::shared_ptr<SourceGxInheritedState>&h,std::shared_ptr<const void> process){
 auto e=BattleWeaponIconEvidence::create(h,4097,std::move(process),[]{return true;});assert(e&&e->retain(h,h->capture()));return e;
}
struct Fixture {
 BattleWeaponOrderedPublication owner;BattleFontGpuResource font;BattleSubmenuSurfaceGpuResource submenu;
 std::shared_ptr<const uint8_t> allocation=std::make_shared<const uint8_t>(1);
 std::shared_ptr<const void> resources=std::make_shared<const int>(2);
 std::shared_ptr<const uint8_t> iconGeneration=std::make_shared<const uint8_t>(3);
 std::shared_ptr<SourceGxInheritedState> history;std::shared_ptr<BattleWeaponIconEvidence> evidence;
 std::filesystem::path dir;
 bool alive=true,executing=true,complete=false,ready=false,iconAvailable=true,reenter=false,retireOnReady=false;
 unsigned draws=0;std::vector<size_t> order;
 Fixture(){history=testHistory(resources);evidence=testEvidence(history,resources);
  MessageFont original;assert(original.load("romfs/font.pmfont"));assert(font.adopt(original,{this,512,256,0,1,512,256,false,{0,1,1,0}}));
  dir=std::filesystem::temp_directory_path()/("pm-weapon-ordered-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));std::filesystem::create_directory(dir);
  FILE*f=std::fopen((dir/"battle-submenu-surfaces.bin").c_str(),"wb");assert(f);uint32_t count=2;std::fwrite("PMBSUB01",8,1,f);std::fwrite(&count,4,1,f);
  for(unsigned i=0;i<2;++i){uint32_t id=i+2;unsigned x=i?22:2,w=i?8:16;BattleClassRegion r{w,24,float(x)/32,1-2.f/32,float(x+w)/32,1-26.f/32};std::fwrite(&id,4,1,f);std::fwrite(&r,sizeof(r),1,f);}std::fclose(f);
  BattleClassSurfaceTextureServices gpu;gpu.user=this;gpu.import=[](void*p,uint32_t,const char*)->std::optional<BattleClassSurfaceImport>{return BattleClassSurfaceImport{p,32,32,0,1,32,32,false,{0,1,1,0}};};gpu.release=[](void*,uint32_t,const void*){};assert(submenu.load(dir.string().c_str(),gpu));
 }
 ~Fixture(){submenu.retire();font.retire();std::filesystem::remove_all(dir);}
 BattleWeaponOrderedBinding binding(size_t capacity=8){
  BattleWeaponOrderedBinding b;b.allocationOwner=allocation;b.sourceResourceOwner=b.resources=resources;b.allocation=0x85600200;b.generation=7;b.capacity=capacity;b.font=&font;b.submenu=&submenu;b.user=this;
  b.current=[](void*p,uint32_t a,uint64_t g){return static_cast<Fixture*>(p)->alive&&a==0x85600200&&g==7;};
  b.sourceCurrent=[](void*p,uint64_t pass){return static_cast<Fixture*>(p)->executing&&(pass==1||pass==2);};
  b.sourceComplete=[](void*p,uint64_t){return static_cast<Fixture*>(p)->complete;};
  b.icon=[](void*p,uint32_t image)->std::optional<BattleEnemyHpIconResidency>{auto&f=*static_cast<Fixture*>(p);if(f.reenter){assert(!f.owner.begin(1));assert(!f.owner.appendIcon(1,f.icon()));assert(!f.owner.seal(1));}if(!f.iconAvailable)return {};return BattleEnemyHpIconResidency{p,f.iconGeneration,image,16,20,32,32,0};};
  b.iconEvidence=[](void*p,uint64_t)->std::shared_ptr<const BattleWeaponIconEvidence>{return static_cast<Fixture*>(p)->evidence;};
  b.ready=[](void*p,const BattleWeaponOrderedBatch&){auto&f=*static_cast<Fixture*>(p);if(f.retireOnReady)f.owner.retire();return f.ready;};
  b.draw=[](void*p,const BattleWeaponOrderedBatch&batch){auto&f=*static_cast<Fixture*>(p);++f.draws;for(const auto&q:batch.packets)f.order.push_back(q.index());};return b;
 }
 BattleFontGlyphPacket glyph(){BattleFontGlyphPacket p;p.font=font.lease();p.matrix={1,0,0,0,0,1,0,0,0,0,1,0};p.quad.left=2;p.quad.right=10;p.quad.top=12;p.quad.bottom=-10;p.quad.u1=8;p.quad.v1=22;p.quad.rgba=0x12345600;p.raster.viewport=p.raster.scissor={0,0,240,400};for(unsigned i=0;i<4;++i)p.raster.projection[i][i]=1;return p;}
 RetainedIconPacket icon(){RetainedIconPacket q;q.image=334;q.camera=8;q.viewport=q.scissor={0,0,240,400};for(unsigned i=0;i<4;++i)q.transform[i][i]=1;return q;}
 BattleSubmenuPacketPtr panel(){auto p=std::make_shared<BattleSubmenuPacket>();p->surface=submenu.lease();p->camera.lifetime=allocation;p->camera.user=this;p->camera.current=[](void*p,const BattleSubmenuCameraProof&){return static_cast<Fixture*>(p)->alive;};p->camera.generation=7;p->camera.pass=1;p->camera.camera=8;p->camera.viewport={0,0,608,480,0,1};p->camera.scissor={0,0,608,480};for(unsigned i=0;i<4;++i)p->camera.projection[i][i]=1;
  auto&t=p->originalTexture;t.index=2;t.width=16;t.height=24;t.format=14;t.data=0x80010000;t.imageHeader=0x80008000;t.minFilter=t.magFilter=1;p->loadedObject[2]=(23u<<10)|15;p->loadedObject[5]=14;p->sourceResourceOwner=resources;p->sourceResourceUser=this;p->sourceResourceCurrent=[](void*p){return static_cast<Fixture*>(p)->alive;};p->matrix={1,0,0,0,0,1,0,0,0,0,1,0};p->positions={{{-8,12,0},{8,12,0},{8,-12,0},{-8,-12,0}}};p->sourceUV={{{0,0},{1,0},{1,1},{0,1}}};return p;
 }
 void start(size_t capacity=8){assert(owner.bind(binding(capacity)));assert(!owner.bind(binding(capacity)));assert(owner.begin(1));assert(owner.begin(1));}
 void mixed(){const auto g=glyph();assert(owner.reserveFont(1,g));assert(owner.reserveFont(1,g));assert(!owner.appendIcon(1,icon()));assert(!owner.seal(1));owner.commitFont(1,g);assert(owner.appendSubmenu(1,panel()));assert(owner.appendIcon(1,icon()));assert(owner.reserveFont(1,g));owner.commitFont(1,g);}
};
}

int main(){
 auto f=std::make_shared<Fixture>();f->start();auto owner=std::shared_ptr<BattleWeaponOrderedPublication>(f,&f->owner);std::weak_ptr<Fixture>weak=f;
 auto pin=std::make_shared<BattleWeaponSourceIconPin>(owner,f->resources,[weak]{auto p=weak.lock();return p&&p->alive;});BattleWeaponSourceIconPins registry;
 assert(registry.attach(pin)&&registry.attach(pin));assert(!registry.retains(334));assert(pin->markPending(334)&&registry.retains(334));assert(!pin->acknowledge(334));
 f->iconAvailable=false;assert(!owner->appendIcon(1,f->icon())&&registry.retains(334));f->iconAvailable=true;assert(owner->appendIcon(1,f->icon()));assert(pin->acknowledge(334)&&registry.retains(334));
 f->complete=true;auto sealed=owner->seal(1);assert(sealed&&registry.retains(334));f->executing=false;assert(registry.retains(334)); // Long source owner survives camera finish.
 assert(pin->markPending(445)&&registry.retains(445));f->alive=false;assert(!pin->alive()&&!registry.retains(334)&&!registry.retains(445));f->alive=true;assert(!pin->alive()); // No readiness revival.
 auto pin2=std::make_shared<BattleWeaponSourceIconPin>(owner,f->resources,[weak]{auto p=weak.lock();return p&&p->alive;});assert(registry.attach(pin2)&&pin2->markPending(446));registry.clear(true);assert(!pin2->alive()&&!pin2->markPending(446));
 auto pin3=std::make_shared<BattleWeaponSourceIconPin>(owner,f->resources,[]{return true;});assert(registry.attach(pin3)&&pin3->markPending(500));std::weak_ptr<BattleWeaponSourceIconPin>pinWeak=pin3;pin3.reset();assert(pinWeak.expired()&&!registry.retains(500));
 auto expired=std::make_shared<BattleWeaponSourceIconPin>(owner,f->resources,[]{return true;});assert(expired->markPending(501)&&registry.attach(expired));owner.reset();f.reset();assert(!expired->alive()&&!registry.retains(501));
 auto unbound=std::make_shared<BattleWeaponOrderedPublication>();auto noCurrent=std::make_shared<BattleWeaponSourceIconPin>(unbound,std::make_shared<int>(1),std::function<bool()>{});assert(!noCurrent->alive()&&!registry.attach(noCurrent));
 auto noLifetime=std::make_shared<BattleWeaponSourceIconPin>(unbound,std::shared_ptr<const void>{},[]{return true;});assert(!noLifetime->markPending(3));
 auto unrelated=std::make_shared<int>(2);auto nullAlias=std::shared_ptr<BattleWeaponOrderedPublication>(unrelated,nullptr);auto noOwner=std::make_shared<BattleWeaponSourceIconPin>(nullAlias,unrelated,[]{return true;});assert(!noOwner->alive());
 std::shared_ptr<BattleWeaponSourceIconPin>recursive;recursive=std::make_shared<BattleWeaponSourceIconPin>(unbound,std::make_shared<int>(1),[&]{assert(!recursive->alive());assert(!recursive->markPending(4));assert(!recursive->acknowledge(4));return true;});assert(recursive->markPending(4)&&recursive->retains(4));recursive->retire();assert(!recursive->alive());
 std::shared_ptr<BattleWeaponSourceIconPin>retired;retired=std::make_shared<BattleWeaponSourceIconPin>(unbound,std::make_shared<int>(1),[&]{retired->retire();return true;});assert(!retired->markPending(4));
 return 0;
}
