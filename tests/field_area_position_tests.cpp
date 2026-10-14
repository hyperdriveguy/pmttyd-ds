#include "field_area_event.hpp"
#include "animation_group_prefetch.hpp"
#include "event_address_space.hpp"
#include "event_lifecycle.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <set>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <cstdlib>
using namespace ttyd;
namespace {
float scalar(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
struct Harness {
 Game game{false};FieldAreaEvent area;FieldBeroExec exec;AnimationGroupPrefetch cache;
 FieldAreaEventServices s;EventEntry* normal=nullptr;EventAddressSpace* memory=nullptr;
 EventCreationServices creation;uint32_t child=0,root=0;unsigned time=0;
 Harness(){game.progress.room=2;game.fieldMapFlags.synchronize(2);game.fieldBeroInfo.synchronize(2);
  assert(cache.open("romfs/field-prefetch") && area.begin(2,0));FieldAreaGpState image{};
  std::memcpy(image.bytes.data()+0x12c-FieldAreaGpState::Begin,"gor_02",7);
  std::memcpy(image.bytes.data()+0x11c-FieldAreaGpState::Begin,"w_bero",7);
  image.bytes[0x176-FieldAreaGpState::Begin]=1;image.bytes[0x177-FieldAreaGpState::Begin]=146;
  const auto id=area.appendAreaInit(image);assert(id);root=*id;
  s.map=&game.fieldMapFlags;s.beroInfo=&game.fieldBeroInfo;s.beroExec=&exec;s.prefetch=&cache;
  s.mario=game.fieldMarioPreparationWorld();s.previousParty=game.fieldPreviousPartyWorld();s.user=this;
  s.call=[](void* p,FieldAreaNativeCall& r){auto& h=*static_cast<Harness*>(p);h.creation=r.creation;
   h.memory=&r.memory;if(r.function==0x80079b74){h.normal=&r.event;h.child=r.event.id;}return 0;};
  for(unsigned i=0;i<60 && !normal;++i)tick();
  assert(normal && !area.failed && area.pendingFunction==0x80079b74 && area.liveEventCount()==3);
 }
 void tick(){area.tick(++time*17,s);assert(!area.failed);}
 uint32_t word(uint32_t a){uint32_t v;assert(memory && memory->readWord(a,v));return v;}
 void bind(){s.geometry=game.fieldBeroGeometry();s.position=game.fieldPositionWorld();}
 void finish(){for(unsigned i=0;i<10 && area.eventAlive(child);++i)tick();assert(!area.eventAlive(child));
  assert(area.eventAlive(root) && area.liveEventCount()==3 && area.pendingFunction==0x8005323c);
  assert(game.fieldMarioBattleState().keyDepth==1);
  assert(!game.fieldBeroInitial); // The real grandchild returned in the existing VM.
 }
 ~Harness(){deleteEventId(root,creation);}
};
// Raw source registry is independently copied into each VM, then registered.
void battleRegistry(){
 Harness a,b;assert(a.word(0x8030536c)==0x90540da8 && a.word(0x80305370)==0x90540f84);
 assert(a.area.battleSetupConfiguration(0)==0x90540da8 && a.area.battleSetupConfiguration(-2)==0x90540da8 && a.area.battleSetupConfiguration(-1)==0);
 assert(!a.area.battleSetupConfiguration(1) && !a.area.battleSetupConfiguration(5));
 EventScriptAssets original;uint32_t v=1;assert(original.load("romfs/door-data.pmevt") && original.readWord(0x8030536c,v) && v==0);
 assert(a.memory->writeWord(0x8030536c,0));assert(!a.area.battleSetupConfiguration(0) && b.area.battleSetupConfiguration(0)==0x90540da8);
 assert(a.memory->writeWord(0x8030536c,0x90540da8));
 assert(a.memory->writeByte(0x8100012c,'x'));assert(!a.area.battleSetupConfiguration(0) && a.area.battleSetupConfiguration(-1)==0);assert(a.memory->writeByte(0x8100012c,'g'));
 // Admission safely rejects malformed live registry pointers before source calls.
 const auto first=a.word(0x803057ec);assert(a.memory->writeWord(0x803057ec,0xdead0000));assert(!a.area.battleSetupConfiguration(0));assert(a.memory->writeWord(0x803057ec,first));
 assert(a.memory->writeWord(0x80415fd0,0x82070000));for(unsigned i=0;i<3;++i)assert(a.memory->writeByte(0x8207012c+i,uint8_t("gor"[i])));assert(a.area.battleSetupConfiguration(0)==0x90540da8);assert(a.memory->writeWord(0x80415fd0,0x81000000));
 char temporary[]="/tmp/pm-area-battle-XXXXXX";const auto* made=mkdtemp(temporary);assert(made);const std::filesystem::path dir=made;
 for(const char* file:{"door-data.pmevt","field-area-init.bin","field-area-events.bin","field-area-battle.bin"})std::filesystem::copy_file(std::filesystem::path("romfs")/file,dir/file);
 // Unsupported row1 in candidate metadata cannot replace the valid global pack.
 {std::fstream f(dir/"field-area-battle.bin",std::ios::in|std::ios::out|std::ios::binary);f.seekp(12+16);const char one[4]={1,0,0,0};f.write(one,4);assert(f);}
 assert(!loadFieldAreaEventAssets(dir.c_str()));
 {std::fstream f(dir/"field-area-battle.bin",std::ios::in|std::ios::out|std::ios::binary);f.seekp(12+16);const char zero[4]={};f.write(zero,4);assert(f);}
 std::ifstream input(dir/"door-data.pmevt",std::ios::binary);std::vector<char> bytes{std::istreambuf_iterator<char>(input),std::istreambuf_iterator<char>()};input.close();
 auto u=[&](size_t a){return uint32_t(uint8_t(bytes[a]))<<24|uint32_t(uint8_t(bytes[a+1]))<<16|uint32_t(uint8_t(bytes[a+2]))<<8|uint8_t(bytes[a+3]);};
 auto patch=[&](uint32_t address,uint32_t value){bool found=false;for(unsigned i=0;i<u(8);++i){const auto at=12+12*i,base=u(at),size=u(at+4),offset=u(at+8);if(base<=address && uint64_t(address)+4<=uint64_t(base)+size){for(unsigned j=0;j<4;++j)bytes[offset+address-base+j]=char(value>>(24-8*j));found=true;break;}}assert(found);std::ofstream out(dir/"door-data.pmevt",std::ios::binary);out.write(bytes.data(),bytes.size());assert(out);};
 patch(0x90540fb4,0x9040a180);assert(!loadFieldAreaEventAssets(dir.c_str())); // Missing number-list terminator.
 patch(0x90540fb4,0);patch(0x90540f84,0xdead0000);assert(!loadFieldAreaEventAssets(dir.c_str())); // Unreadable number name.
 Harness retained;assert(retained.area.battleSetupConfiguration(0)==0x90540da8 && !a.area.failed && !b.area.failed);
 std::filesystem::remove_all(dir);
}
// Deliberate host-only world fixture. It proves real setup scheduling/metadata;
// it supplies no native Game registration, battle RNG, pose or resource service.
void sharedNpcSetup() {
 clearCollision();Harness h;{CollisionTransition room;assert(room.load("romfs/gor_02",2));h.bind();h.finish();}
 struct World {bool ready=false;unsigned getters=0,modes=0,battles=0;std::map<uint32_t,uint32_t> data,names;} world;
 for(unsigned n=0;n<15;++n){const auto row=0x90524f10+0x5c*n,name=h.word(row),actor=0x85000000+0x400*n;assert(name);
  world.names[name]=actor;world.data[actor]=3;world.data[actor+0x28]=0x85010000;
 }
 assert(h.word(0x90524f10+15*0x5c)==0);
 FieldNpcSetupWorld w;w.user=&world;
 w.fieldPointer=[](void* p){++static_cast<World*>(p)->getters;return 0x85020000u;};
 w.admit=[](void* p,const EventEntry&,uint32_t table,bool first,const FieldNpcSetupOperands&){assert(table==0x90524f10 && first);return static_cast<World*>(p)->ready;};
 w.npc=[](void* p,uint32_t name,bool asserting,const FieldNpcSetupOperands&){assert(!asserting);return static_cast<World*>(p)->names.at(name);};
 w.word=[](void* p,uint32_t address){return static_cast<World*>(p)->data[address];};
 w.store=[](void* p,uint32_t address,uint32_t value){static_cast<World*>(p)->data[address]=value;};
 w.battle=[](void* p,uint32_t,uint32_t setup){assert(setup==0);++static_cast<World*>(p)->battles;};
 w.copyString=[](void*,uint32_t,uint32_t){assert(false);};w.setAnimation=[](void*,uint32_t,uint32_t,int32_t){assert(false);};
 w.mode=[](void* p,int32_t mode){assert(mode==1);++static_cast<World*>(p)->modes;};h.s.npcSetup=w;
 h.tick();assert(world.getters==1 && world.modes==0 && world.battles==0 && h.area.eventAlive(h.root));
 world.ready=true;h.tick();assert(world.getters==1 && world.modes==1 && world.battles==15 && h.area.eventAlive(h.root));
 auto* gp=h.creation.manager.globals(h.creation.manager.user);auto* context=h.creation.manager.context(h.creation.manager.user,gp->battleContext!=0);
 std::set<uint32_t> ids;
 for(unsigned n=0;n<15;++n){const auto actor=0x85000000+0x400*n,id=world.data.at(actor+0x118);assert(id && ids.insert(id).second);
  assert(world.data.at(actor)==0x80603 && world.data.at(actor+0x124)==h.word(0x90524f10+0x5c*n+0x10));
  EventEntry* created=nullptr;for(int slot=0;slot<context->capacity;++slot)if((context->entries[slot].flags&1) && context->entries[slot].id==id)created=&context->entries[slot];
  assert(created && created->restartScript==h.word(0x90524f10+0x5c*n+0xc) && created->field170==actor && created->rawBytes[0]==0 && created->type==0xef && !(created->flags&0x20) && !created->waitingParent && !created->brotherOwner);
 }
 const auto beforeCancel=h.area.liveEventCount();deleteEventId(h.root,h.creation);assert(!h.area.eventAlive(h.root) && h.area.liveEventCount()==beforeCancel-1);
 for(auto id:ids)assert(h.area.eventAlive(id)); // Independent init roots survive caller retirement.
}

}
int main(){assert(loadFieldAreaEventAssets("romfs") && loadFieldMapFlagAssets("romfs") && loadFieldBeroInfoAssets("romfs") && loadWarps("romfs/rooms.bin"));
 battleRegistry();sharedNpcSetup();
 clearCollision();Harness h;h.bind();h.tick();assert(h.area.pendingFunction==0x80079b74 && !h.game.fieldRecoveryWords[0]);
 {CollisionTransition wrong;assert(wrong.load("romfs/map",0));Game other(false);other.fieldMapFlags.synchronize(0);h.s.geometry=other.fieldBeroGeometry();h.tick();assert(h.area.pendingFunction==0x80079b74 && !h.game.fieldRecoveryWords[0]);}h.s.geometry=h.game.fieldBeroGeometry();
 {CollisionTransition room;assert(room.load("romfs/gor_02",2));
  auto geometry=h.game.fieldBeroGeometry();FieldWallVector center;
  assert(geometry.center(geometry.user,"w_bero",center));assert(!geometry.center(geometry.user,"dokan",center));
  assert(h.game.fieldMapFlags.hitEnabled("w_bero",false));assert(geometry.center(geometry.user,"w_bero",center));
  assert(h.game.fieldMapFlags.hitFlags("w_bero",0x80,true));assert(!geometry.center(geometry.user,"w_bero",center));
  h.tick();assert(!h.game.fieldRecoveryWords[0]);assert(h.game.fieldMapFlags.hitFlags("w_bero",0x80,false));
  assert(h.game.fieldMapFlags.hitEnabled("w_bero",true));h.game.fieldCameraShift.current=25;h.game.fieldCameraShift.previousMilliseconds=12345;
  h.finish();
  const auto sx=scalar(h.word(0x8041e9f8)),sy=scalar(h.word(0x8041e9f4)),sz=scalar(h.word(0x8041e9f0));
  const auto ex=scalar(h.word(0x8041e9ec)),ey=scalar(h.word(0x8041e9e8)),ez=scalar(h.word(0x8041e9e4));
  assert(std::fabs(ex-sx-60.f)<.001f && ey==sy && std::fabs(ez-sz)<.001f);
  assert(h.game.fieldRecoveryWords[0]==h.word(0x8041e9ec) && scalar(*h.game.fieldRecoveryWords[1])==37.f+ey && h.game.fieldRecoveryWords[2]==h.word(0x8041e9e4));
  const auto start=fieldBeroStartPublication({sx,sy,sz});assert(start);
  assert(h.game.progress.position.x==start->position.x*.01f && h.game.progress.position.z==start->position.z*.01f);
  assert(h.game.fieldCamera.point.y==start->position.y+.01f && h.game.marioCamera.retained.y==start->position.y);
  assert(h.game.fieldCameraShift.current==0 && h.game.fieldCameraShift.previousMilliseconds==12345 && !h.game.fieldRecoveryReload);
 }
 // Explicit VM input fixture: non-sentinel zero-distance must not dereference
 // an invalid name or require geometry. Oversized start holds before publication.
 clearCollision();Harness literal;assert(literal.memory->writeWord(0x82070000,2097152) && literal.memory->writeWord(0x82070004,8) && literal.memory->writeWord(0x82070008,9));
 literal.normal->locals[6]=int32_t(0x82070000);literal.normal->locals[3]=0;literal.normal->locals[10]=-230000000;
 literal.bind();literal.s.geometry={};literal.tick();assert(!literal.game.fieldRecoveryWords[0] && literal.word(0x8041e9f8)==0);
 assert(literal.memory->writeWord(0x82070000,50));const_cast<FieldJump&>(literal.game.fieldAirMotion()).motion=25;literal.finish();assert(literal.game.fieldRecoveryReload==1);assert(scalar(literal.word(0x8041e9f8))==50 && scalar(literal.word(0x8041e9ec))==50 && scalar(*literal.game.fieldRecoveryWords[1])==45);
 // Raw nonzero selects slot1 even without a known current index; genuine null
 // avoids every coordinate operand, float resolver and actor writer.
 Game unknown(false);unknown.fieldPartyCurrentIndex.reset();auto world=unknown.fieldPositionWorld();
 EventEntry e;uint32_t raw=0;FieldPositionOperands operands;operands.user=&raw;
 operands.word=[](void* p,uint32_t){return *static_cast<uint32_t*>(p);};
 assert(fieldPartySetPositionEvent(e,operands,world)==0);raw=1;assert(fieldPartySetPositionEvent(e,operands,world)==2);
 unknown.fieldPartyCurrentIndex=127;raw=0;assert(fieldPartySetPositionEvent(e,operands,world)==0);raw=1;assert(fieldPartySetPositionEvent(e,operands,world)==2);
 unknown.fieldPartyCurrentIndex=-1;raw=0;assert(fieldPartySetPositionEvent(e,operands,world)==2);
}
