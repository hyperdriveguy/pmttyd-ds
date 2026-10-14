#include "game.hpp"
#include "event_manager.hpp"
#include <cassert>
using namespace ttyd;
int main(){
 assert(loadFieldBeroInfoAssets("romfs"));FieldBeroInfo info;info.synchronize(2);EventEntry event;
 for(unsigned n=0;n<16;++n)event.locals[n]=int32_t(400+n);
 event.localFlags={1,2,3};const auto untouched=event;
 assert(!info.intoInfo(event) && event.locals==untouched.locals && !info.publication());
 assert(info.publishIncoming("w_bero",0) && info.intoInfo(event));
 assert(event.locals[1]==0 && event.locals[2]==2 && uint32_t(event.locals[3])==FieldBeroInfo::IncomingAddress && event.locals[4]==0 && event.locals[5]==6);
 const auto position=uint32_t(event.locals[6]);uint32_t word=0;
 assert(info.readWord(position,word) && word==100000);
 assert(uint32_t(event.locals[7])!=0x80416060 && uint32_t(event.locals[8])==0xffff0000);
 assert(event.locals[0]==untouched.locals[0] && event.localFlags==untouched.localFlags);
 for(unsigned n=9;n<16;++n)assert(event.locals[n]==untouched.locals[n]);
 uint8_t byte=0;assert(info.readByte(uint32_t(event.locals[3]),byte) && byte=='w');
 assert(info.publishIncoming("dokan",0) && info.intoInfo(event));
 assert(event.locals[1]==1 && event.locals[2]==2 && event.locals[4]==2 && event.locals[5]==9 && uint32_t(event.locals[7])==0x80416060);
 assert(info.publishIncoming("missing",0) && info.intoInfo(event));
 assert(event.locals[1]==0 && event.locals[4]==0xf00 && event.locals[5]==10000 && uint32_t(event.locals[6])==0x8030cd84);
 assert(uint32_t(event.locals[3])==FieldBeroInfo::IncomingAddress);
 assert(info.publishIncoming("",2) && info.intoInfo(event));
 assert(event.locals[1]==0 && event.locals[4]==0 && event.locals[5]==6 && uint32_t(event.locals[6])==position && uint32_t(event.locals[3])!=FieldBeroInfo::IncomingAddress);
 assert(info.publishIncoming("",0) && info.intoInfo(event));assert(event.locals[4]==0xf00 && event.locals[5]==10000);
 info.synchronize(0);assert(!info.known() && !info.publication());info.synchronize(2);assert(!info.known());
 assert(info.publishRoadIncoming(1215808,0));assert(info.publication()->at(1)==2);
 assert(loadFieldBeroInfoAssets("romfs"));assert(!info.known() && !info.publication());info.synchronize(2);assert(!info.known());
 // Native successful room arrival now publishes the actual original into_info
 // outputs; checkpoint restoration alone has no incoming-entry provenance.
 assert(loadWarps("romfs/rooms.bin") && loadGround("romfs/gor_02.ground") && loadCollision("romfs/gor_02.walls") && loadHead("romfs/gor_02.head"));
 Game game(false);game.progress.room=0;game.progress.originalStory=23;
 // Existing first road is original plaza east -> east w_bero.
 game.warpRequest=0;assert(game.finishWarp());assert(game.progress.room==2 && game.fieldBeroInfo.known());
 const auto published=game.fieldBeroInfo.publication();assert(published && (*published)[0]==0 && (*published)[1]==2 && (*published)[4]==6);
 assert(uint32_t((*published)[5])==position && uint32_t((*published)[7])==0xffff0000);
 const auto save=game.progress;game.restore(save);assert(!game.fieldBeroInfo.known() && !game.fieldBeroInfo.publication());
}
