#include "field_position_event.hpp"
#include "game.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
uint32_t bits(float value){uint32_t out;std::memcpy(&out,&value,4);return out;}
float scalar(uint32_t word){float out;std::memcpy(&out,&word,4);return out;}
struct Fixture {
 std::map<uintptr_t,uint32_t> memory;
 std::vector<std::pair<char,uintptr_t>> trace;
 uintptr_t mario=0x1000,party=0x2000;
 int current=0,selected=-9;unsigned floats=0;
 bool mutate=false;
 FieldPositionOperands operands() {
  return {this,[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'a',a});return f.memory.at(a);},
   [](void* p,EventEntry& e,int32_t raw){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'v',uint32_t(raw)});++f.floats;
    if(f.mutate && f.floats==1){e.arguments=0x9000;f.mario=0x4000;f.memory[0x104]=44;}
    if(f.mutate && f.floats==2)assert(f.memory.at(0x108c)==bits(11));
    return float(raw);}};
 }
 FieldPositionWorld world() {
  return {this,[](void* p){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'m',f.mario});return f.mario;},
   [](void* p){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'i',uintptr_t(f.current)});return f.current;},
   [](void* p,int32_t n){auto& f=*static_cast<Fixture*>(p);f.selected=n;f.trace.push_back({'p',uintptr_t(n)});return f.party;},
   [](void* p,int32_t n)->uintptr_t{assert(n==4);static_cast<Fixture*>(p)->trace.push_back({'c',4});return 0x3000;},
   [](void* p,uintptr_t a){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'r',a});return f.memory.at(a);},
   [](void* p,uintptr_t a,uint32_t word){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'w',a});f.memory[a]=word;},
   [](void* p){static_cast<Fixture*>(p)->trace.push_back({'R',0});},
   [](void* p){static_cast<Fixture*>(p)->trace.push_back({'S',0});}};
 }
};
int main() {
 Fixture f;f.memory={{0x100,11},{0x104,22},{0x108,33}};f.mutate=true;
 EventEntry e;e.arguments=0x100;e.locals.fill(9);e.localFlags.fill(77);const auto old=e;
 assert(fieldMarioSetPositionEvent(e,f.operands(),f.world())==2);
 assert(f.memory.at(0x108c)==bits(11) && f.memory.at(0x1090)==bits(44) && f.memory.at(0x1094)==bits(33) && !f.memory.count(0x408c));
 assert(e.locals==old.locals && e.localFlags==old.localFlags);
 assert((f.trace==std::vector<std::pair<char,uintptr_t>>{{'m',0x1000},{'a',0x100},{'v',11},{'w',0x108c},{'a',0x104},{'v',44},{'w',0x1090},{'a',0x108},{'v',33},{'w',0x1094}}));
 Fixture party;party.current=-1;party.party=0;party.memory[0x100]=0;e.arguments=0x100;auto noFloat=party.operands();noFloat.scalar=nullptr;
 assert(fieldPartySetPositionEvent(e,noFloat,party.world())==2 && !party.floats && party.selected==-1 && party.memory.size()==1);
 Fixture literal;literal.current=0;literal.memory={{0x100,uint32_t(-30000000)},{0x104,10},{0x108,20},{0x10c,30}};
 assert(fieldPartySetPositionEvent(e,literal.operands(),literal.world())==2 && literal.selected==1 && literal.floats==3);
 assert(literal.memory.at(0x2058)==bits(10) && literal.memory.at(0x205c)==bits(20) && literal.memory.at(0x2060)==bits(30));
 for(const auto& item:literal.trace)assert(item.first!='i'); // Selector is raw, not resolved.
 Fixture camera;camera.memory={{0x108c,bits(1.5f)},{0x1090,bits(2.25f)},{0x1094,bits(-3.f)}};
 assert(fieldCameraRoadResetEvent(camera.world())==2);
 assert(camera.memory.at(0x3094)==bits(1.5f) && camera.memory.at(0x3098)==bits(2.26f) && camera.memory.at(0x309c)==bits(-3.f));
 assert(camera.memory.at(0x30b0)==camera.memory.at(0x3098));
 assert(camera.memory.at(0x10ec)==bits(1.5f) && camera.memory.at(0x10f0)==bits(2.25f) && camera.memory.at(0x10fc)==bits(2.25f));
 assert((camera.trace==std::vector<std::pair<char,uintptr_t>>{{'m',0x1000},{'c',4},
  {'r',0x108c},{'w',0x3094},{'r',0x1090},{'w',0x3098},{'r',0x1094},{'w',0x309c},
  {'r',0x3094},{'w',0x30ac},{'r',0x3098},{'w',0x30b0},{'r',0x309c},{'w',0x30b4},
  {'r',0x108c},{'r',0x1090},{'w',0x10ec},{'w',0x10f0},{'r',0x1094},{'w',0x10f4},
  {'r',0x108c},{'r',0x1090},{'w',0x10f8},{'w',0x10fc},{'r',0x1094},{'w',0x1100},{'R',0},{'S',0}}));
 // Actual native field owners: position changes current root, not floor state;
 // reset touches road/shift work and retained targets, not view blends/clocks.
 Game game(false);game.groundHeight=1;game.progress.elevation=1;game.groundMetadataHeight=99;
 const auto publication=fieldBeroStartPublication({150,225,-300});assert(publication);game.fieldBeroStart=publication;
 auto world=game.fieldPositionWorld();const auto mario=world.mario(world.user);
 world.write(world.user,mario+0x90,bits(400));assert(game.groundHeight==1 && game.progress.elevation==1 && game.groundMetadataHeight==99);
 assert(scalar(world.read(world.user,mario+0x90))==400);
 auto& parties=const_cast<FieldPartyRegistry&>(game.fieldPartyState());assert(parties.allocateFirst()==0 && parties.allocateFirst()==1);
 for(auto& actor:parties.actors){actor->word(0,0x12345678);actor->scalar(0x10c,123);}
 game.fieldCameraEvents.view.state=3;game.fieldCameraEvents.target.state=2;game.fieldCameraEvents.dolly=42;
 game.cameraRoadWork.current.active=true;game.cameraRoadWork.current.road=3;game.cameraRoadWork.progress=.7f;
 game.fieldCameraCandidateActive=true;game.fieldCameraCandidateShiftRate=22;
 game.fieldCameraShift.current=50;game.fieldCameraShift.previousMilliseconds=123456;
 assert(game.applyFieldBeroStart() && game.fieldBeroStartApplied && game.progress.position.x==1.5f && game.progress.position.z==-3);
 assert(game.groundHeight==1 && game.progress.elevation==1 && game.groundMetadataHeight==99);
 for(const auto& actor:parties.actors)assert(actor->scalar(0x58)==150 && actor->scalar(0x5c)==225 && actor->scalar(0x60)==-300 && actor->word(0)==0x12345678 && actor->scalar(0x10c)==123);
 assert(game.marioCamera.retained.y==225 && game.marioCameraCollision.y==225 && game.fieldCamera.point.y==225.01f && game.fieldCameraCollisionTarget.y==225.01f);
 assert(!game.cameraRoadWork.current.active && !game.cameraRoadWork.current.road && game.cameraRoadWork.progress==0);
 assert(!game.fieldCameraCandidateActive && game.fieldCameraCandidateShiftRate==0 && game.fieldCameraShift.current==0 && game.fieldCameraShift.previousMilliseconds==123456);
 assert(game.fieldCameraEvents.view.state==3 && game.fieldCameraEvents.target.state==2 && game.fieldCameraEvents.dolly==42);
 game.fieldCameraShift.current=25;assert(game.applyFieldBeroStart() && game.fieldCameraShift.current==25); // No replay over later writers.
 Game unknown(false);unknown.fieldBeroStart=publication;unknown.fieldPartyCurrentIndex.reset();unknown.fieldCameraShift.current=12;
 assert(!unknown.applyFieldBeroStart() && !unknown.fieldBeroStartApplied && unknown.fieldCameraShift.current==12 && unknown.progress.position.x!=1.5f);
}
