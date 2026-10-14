#include "game.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <unistd.h>
using namespace ttyd;
int main(){
 assert(loadFieldBeroInfoAssets("romfs") && loadFieldMapFlagAssets("romfs"));FieldBeroInfo info;info.synchronize(2);EventEntry event;
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
 // Original get_info validation uses names that ignore flag1 but exclude80.
 FieldMapFlags map;map.synchronize(2);FieldBeroExec exec;exec.unknown();
 assert(info.currentNumber()==0);info.setCurrentNumber(-7);
 assert(map.hitEnabled("w_bero",false));assert(map.hitSourceNode("w_bero")==1215808);
 assert(map.hitFlags("w_bero",0x80,true));assert(!map.hitSourceNode("w_bero"));
 assert(!info.initialize(map,exec) && !exec.read() && !info.registered());
 assert(map.hitFlags("w_bero",0x80,false) && info.initialize(map,exec));assert(exec.read()==0 && info.registered() && info.currentNumber()==-7);
 for(unsigned n=0;n<16;++n)event.locals[n]=int32_t(400+n);
 assert(info.infoByIndex(event,0));assert(event.locals[4]==0 && event.locals[5]==6 && event.locals[9]==-1 && event.locals[10]==10 && uint32_t(event.locals[11])==0x80416074);
 assert(uint32_t(event.locals[12])==0xffff0000 && event.locals[0]==400 && event.locals[1]==401 && event.locals[2]==402 && event.locals[15]==415);
 assert(info.infoByIndex(event,1) && event.locals[4]==2 && event.locals[5]==9 && event.locals[9]==-1 && event.locals[10]==6); // Source pipe explicitly selects case6.
 assert(info.length(0)==30.f && info.length(1)==60.f && !info.length(-1) && !info.length(2));
 assert(!info.infoByIndex(event,-1) && !info.infoByIndex(event,2));
 exec.onOff(0,8);assert(info.initialize(map,exec) && exec.read()==8); // No replay over later real Bero writers.
 // Native successful room arrival now publishes the actual original into_info
 // outputs; checkpoint restoration alone has no incoming-entry provenance.
 assert(loadWarps("romfs/rooms.bin") && loadGround("romfs/gor_02.ground") && loadCollision("romfs/gor_02.walls") && loadHead("romfs/gor_02.head"));
 Game game(false);game.progress.room=0;game.progress.originalStory=23;
 // Existing first road is original plaza east -> east w_bero.
 game.warpRequest=0;assert(game.finishWarp());assert(game.progress.room==2 && game.fieldBeroInfo.known());
 assert(game.fieldBeroInfo.registered() && game.beroExec->read()==0 && game.fieldBeroInfo.currentNumber()==0);
 const auto published=game.fieldBeroInfo.publication();assert(published && (*published)[0]==0 && (*published)[1]==2 && (*published)[4]==6);
 assert(uint32_t((*published)[5])==position && uint32_t((*published)[7])==0xffff0000);
 const auto originalEnd=game.progress.position;game.beginRoadArrival();const auto originalStart=game.progress.position;
 const auto save=game.progress;game.restore(save);assert(!game.fieldBeroInfo.known() && !game.fieldBeroInfo.publication() && !game.fieldBeroInfo.registered() && !game.fieldBeroInfo.currentNumber());
 // Isolated authored-length variant proves the actual road endpoint consumes
 // the table field while preserving the source fixed30-unit start radius.
 char temporary[]="/tmp/ttyd-bero-length-XXXXXX";assert(mkdtemp(temporary));const std::filesystem::path fixture(temporary);
 std::filesystem::copy_file("romfs/bero-info.bin",fixture/"bero-info.bin");
 std::ifstream input("romfs/bero-info.pmevt",std::ios::binary);std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(input)),{});
 auto read=[&](size_t at){return uint32_t(bytes[at])<<24|uint32_t(bytes[at+1])<<16|uint32_t(bytes[at+2])<<8|bytes[at+3];};
 bool changed=false;const auto lengthAddress=position+12;
 for(uint32_t n=0;n<read(8);++n){const auto a=read(12+12*n),size=read(16+12*n),off=read(20+12*n);if(a<=lengthAddress && lengthAddress+4<=a+size){const auto at=off+lengthAddress-a;bytes[at]=bytes[at+1]=bytes[at+2]=0;bytes[at+3]=45;changed=true;break;}}
 assert(changed);{std::ofstream out(fixture/"bero-info.pmevt",std::ios::binary);out.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());}
 assert(loadFieldBeroInfoAssets(temporary));Game variant(false);variant.progress.room=0;variant.progress.originalStory=23;variant.warpRequest=0;assert(variant.finishWarp());
 const auto variantEnd=variant.progress.position;variant.beginRoadArrival();
 assert(variant.progress.position.x==originalStart.x && variant.progress.position.z==originalStart.z);
 assert(std::fabs(variantEnd.x-originalEnd.x-.15f)<.00001f && variantEnd.z==originalEnd.z);
 assert(variant.fieldBeroInfo.length(0)==45.f);assert(loadFieldBeroInfoAssets("romfs"));std::filesystem::remove_all(fixture);
}
