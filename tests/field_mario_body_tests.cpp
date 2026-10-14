#include "game.hpp"
#include "field_area_event.hpp"
#include "animation_group_prefetch.hpp"
#include <cassert>
#include <cstring>
using namespace ttyd;
namespace {bool close(float a,float b){return std::fabs(a-b)<1e-6f;}uint32_t bits(float v){uint32_t out;std::memcpy(&out,&v,4);return out;}}
int main(){
 Game game(false);const auto& body=game.fieldMarioBodyState();assert(body.form==0 && body.width==20 && body.height==37 && body.walkBase==1 && body.dashBase==2.25f && body.multiplier==1);
 auto world=game.fieldMarioPreparationWorld();const auto& s=world.storage;const auto mario=s.mario(s.user);
 game.fieldCameraFlags=0xa5000034;game.fieldCameraDisplayFlags=0x44020080;game.fieldCameraEvents.view.state=3;game.fieldCameraShift.previousMilliseconds=777;
 assert(fieldMarioBackgroundEvent(false,world)==2 && !(game.fieldMarioBattleState().flags&0x2000000));
 assert(game.fieldCameraFlags==0xa5000024 && game.fieldCameraDisplayFlags==0x44000080 && game.fieldCameraEvents.view.state==3 && game.fieldCameraShift.previousMilliseconds==777);
 assert(fieldMarioBackgroundEvent(true,world)==2 && (game.fieldMarioBattleState().flags&0x2000000));
 assert(game.fieldCameraFlags==0xa5000034 && game.fieldCameraDisplayFlags==0x44020080);
 auto rates=game.fieldMovementRates();assert(close(rates.walk,.005f) && close(rates.dash,.01125f));
 FieldMovement movement;assert(close(movement.grounded(1,0,rates).x,.01125f));assert(close(movement.groundedStick(33,44,rates).x,.003f));
 movement.grounded(1,0,rates);assert(movement.airborne(1,0,false,true,body.airCap()).x>.01125f && close(body.airCap(),.0225f));
 assert(close(body.walkThreshold(),.01f));FieldJump ceiling;ceiling.motion=3;ceiling.takeoffY=1;float proposed=.5f;assert(!ceiling.prepareCeiling(proposed,.0075f,body.walkThreshold()) && ceiling.motion==1);
 // Actual Game movement reads background rates each visit, rather than a cached
 // second speed owner. Original direction-rate composition remains identical.
 clearCollision();Game ordinary(false),background(false);ordinary.progress.hp=background.progress.hp=7;
 assert(fieldMarioBackgroundEvent(true,background.fieldMarioPreparationWorld())==2);
 const auto x0=ordinary.progress.position.x,b0=background.progress.position.x;
 Input input;input.x=1;ordinary.tick(input);background.tick(input);
 assert(close((ordinary.progress.position.x-x0)*.5f,background.progress.position.x-b0));
 assert(fieldMarioBackgroundEvent(false,background.fieldMarioPreparationWorld())==2);
 const auto before=background.progress.position.x;ordinary.tick(input);background.tick(input);
 assert(close(ordinary.progress.position.x-x0-(before-b0)*2,background.progress.position.x-before));
 // Dynamic height reaches overhead contact and correction, with actual object
 // triangles rather than only checking a stored scalar.
 clearCollision();std::vector<FieldObjectTriangle> roof(1);roof[0].vertices={-1,.45f,-1,1,.45f,-1,0,.45f,1};roof[0].owner=77;
 auto low=searchFieldHead({0,0},{1,0},0,0,.1f,nullptr,false,&roof,.37f,20);
 auto tall=searchFieldHead({0,0},{1,0},0,0,.1f,nullptr,false,&roof,.5f,20);
 assert(!low.contact && low.clearance>0 && tall.contact && tall.objectKey==77 && tall.correction<0);
 // Live body fields feed real source-unit storage; retained speeds are distinct.
 s.write(s.user,mario+0x184,bits(1.5f));s.write(s.user,mario+0x188,bits(3));assert(body.walkBase==1.5f && body.dashBase==3 && close(game.fieldMovementRates().walk,.0075f) && close(body.airCap(),.03f));
 assert(fieldMarioBackgroundEvent(false,world)==2 && body.walkBase==1 && body.dashBase==2.25f);
 const auto prior=s.read(s.user,mario);s.write(s.user,mario,prior|0x4000000);const auto camera=game.fieldCameraFlags;
 assert(fieldMarioBackgroundEvent(true,world)==0 && game.fieldCameraFlags==camera && s.read(s.user,mario)==(prior|0x4000000));s.write(s.user,mario,prior);
 // Unchanged original parent/child progresses through native Game background
 // ownership and retains its source key hold at the actual party frontier.
 assert(loadFieldAreaEventAssets("romfs") && loadFieldMapFlagAssets("romfs") && loadFieldBeroInfoAssets("romfs"));
 FieldAreaEvent area;assert(area.begin(2,0));FieldAreaGpState state{};std::memcpy(state.bytes.data()+0x11c-FieldAreaGpState::Begin,"w_bero",7);
 state.bytes[0x176-FieldAreaGpState::Begin]=1;state.bytes[0x177-FieldAreaGpState::Begin]=146;assert(area.appendAreaInit(state));
 FieldMapFlags map;map.synchronize(2);FieldBeroInfo info;info.synchronize(2);FieldBeroExec exec;AnimationGroupPrefetch cache;assert(cache.open("romfs/field-prefetch"));
 FieldAreaEventServices services;services.map=&map;services.beroInfo=&info;services.beroExec=&exec;services.prefetch=&cache;services.mario=world;
 for(unsigned t=1;t<40 && area.pendingFunction!=0x800e3058;++t)area.tick(t*17,services);
 assert(!area.failed && area.pendingFunction==0x800e3058 && area.liveEventCount()==2 && game.fieldMarioBattleState().keyDepth==1 && game.fieldKeysOff);
 assert(!(game.fieldCameraFlags&0x10) && !(game.fieldCameraDisplayFlags&0x20000) && !(game.fieldMarioBattleState().flags&0x2000000));
}
