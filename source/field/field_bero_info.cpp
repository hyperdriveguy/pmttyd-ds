#include "field_bero_info.hpp"
#include "field_map_flags.hpp"
#include "field_bero_exec.hpp"
#include "event_script_assets.hpp"
#include "event_manager.hpp"
#include "event_address_space.hpp"
#include <cstdio>
#include <cstring>
#include <unordered_map>
#include <vector>
namespace ttyd {
struct FieldBeroInfoProgram {
 struct Table {uint32_t address;std::vector<uint32_t> nodes;};
 EventScriptAssets data;
 uint32_t fallback=0,defaultEvent=0,defaultOut=0,prefixes=0;
 std::unordered_map<uint32_t,Table> tables;
 bool text(uint32_t address,std::string& out) const {
  out.clear();for(unsigned n=0;n<64;++n){uint8_t byte=0;if(!data.readByte(address+n,byte))return false;if(!byte)return true;out.push_back(char(byte));}return false;
 }
};
namespace {std::shared_ptr<const FieldBeroInfoProgram> assets;uint64_t generation=0;
int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
bool loadFieldBeroInfoAssets(const char* root) {
 if(!root)return false;
 auto next=std::make_shared<FieldBeroInfoProgram>();const std::string base=root;
 if(!next->data.load((base+"/bero-info.pmevt").c_str()))return false;
 FILE* f=std::fopen((base+"/bero-info.bin").c_str(),"rb");if(!f)return false;
 char magic[8];uint32_t count=0;
 bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMBRIO02",8) && std::fread(&count,4,1,f)==1 && count && count<=64 &&
  std::fread(&next->fallback,4,1,f)==1 && std::fread(&next->defaultEvent,4,1,f)==1 && std::fread(&next->prefixes,4,1,f)==1 && std::fread(&next->defaultOut,4,1,f)==1;
 for(unsigned n=0;ok && n<count;++n) {
  uint32_t room=0,number=0;FieldBeroInfoProgram::Table t{};
  ok=std::fread(&room,4,1,f)==1 && std::fread(&t.address,4,1,f)==1 && !(t.address&3) && std::fread(&number,4,1,f)==1 && number && number<=16;
  if(!ok)break;
  t.nodes.resize(number);ok=std::fread(t.nodes.data(),4,number,f)==number;
  for(unsigned e=0;ok && e<number;++e) {
   uint32_t word=0;std::string name;
   for(unsigned w=0;ok && w<15;++w)ok=next->data.readWord(t.address+e*60+w*4,word);
   ok=ok && next->data.readWord(t.address+e*60,word) && word && next->text(word,name) && !name.empty();
   for(unsigned p=0;p<e;++p)if(t.nodes[e] && t.nodes[e]==t.nodes[p])ok=false;
  }
  uint32_t sentinel=1;ok=ok && next->data.readWord(t.address+number*60,sentinel) && !sentinel && next->tables.emplace(room,std::move(t)).second;
 }
 uint32_t word=0;for(unsigned w=0;ok && w<15;++w)ok=next->data.readWord(next->fallback+4*w,word);
 for(unsigned n=0;ok && n<8;++n){std::string text;ok=next->data.readWord(next->prefixes+4*n,word) && next->text(word,text) && !text.empty();}
 ok=ok && next->data.readWord(next->defaultEvent,word) && next->data.readWord(next->defaultOut,word) &&
  next->data.readWord(0x8030f060,word) && word==0x0001005b && next->data.readWord(0x8030f064,word) && word==0x8007a214 &&
  next->data.readWord(0x8030f2d0,word) && word==2 && next->data.readWord(0x8030f2d4,word) && word==1 && next->data.readWord(0x8030f2d8,word) && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
 assets=std::move(next);++generation;return true;
}
void FieldBeroInfo::invalidate(){program_.reset();room_=UINT32_MAX;incomingKnown_=false;registered_=false;entries_={};switches_={};incoming_.clear();publication_.reset();}
void FieldBeroInfo::synchronize(uint32_t room) {
 if(room_==room && generation_==generation)return;
 invalidate();room_=room;generation_=generation;
 if(assets && assets->tables.count(room))program_=assets;
}
bool FieldBeroInfo::registered() const{return program_ && generation_==generation && registered_;}
bool FieldBeroInfo::initialize(const FieldMapFlags& map,FieldBeroExec& exec) {
 if(registered())return true;
 const auto address=tableAddress();return address && registerOriginal(*address,map,exec);
}
std::optional<uint32_t> FieldBeroInfo::tableAddress() const {
 if(!program_ || generation_!=generation)return {};
 return program_->tables.at(room_).address;
}
bool FieldBeroInfo::registerOriginal(uint32_t address,const FieldMapFlags& map,FieldBeroExec& exec) {
 if(tableAddress()!=std::optional<uint32_t>(address) || map.room()!=room_)return false;
 const auto& table=program_->tables.at(room_);
 // Missing/flag80 names would invoke the still-unbound original map-error ring.
 // Hold that diagnostic domain; ordinary validated names perform the real stores.
 for(unsigned n=0;n<table.nodes.size();++n){uint32_t pointer=0;std::string name;
  if(!program_->data.readWord(table.address+60*n,pointer) || !program_->text(pointer,name) || map.hitSourceNode(name)!=std::optional<uint32_t>(table.nodes[n]))return false;
 }
 entries_={};for(unsigned n=0;n<table.nodes.size();++n){entries_[n]=table.address+60*n;switches_[n]=-1;}
 exec.afterInfo();registered_=true;return true;
}
std::optional<float> FieldBeroInfo::length(int32_t index) const {
 if(!program_ || generation_!=generation || index<0)return std::nullopt;
 const auto& t=program_->tables.at(room_);if(uint32_t(index)>=t.nodes.size())return std::nullopt;
 uint32_t word=0,kind=0;if(!program_->data.readWord(t.address+60*uint32_t(index)+24,word))return std::nullopt;
 if(word!=UINT32_MAX)return float(signedWord(word));
 if(!program_->data.readWord(t.address+60*uint32_t(index)+4,kind))return std::nullopt;
 kind=(kind>>16)&0xfff;return kind==0 || kind==3 || kind==0xf00?30.f:60.f;
}
bool FieldBeroInfo::infoByIndex(EventEntry& event,int32_t index) const {
 if(!registered() || index<0 || uint32_t(index)>=program_->tables.at(room_).nodes.size() || !switches_[unsigned(index)])return false;
 const auto address=entries_[unsigned(index)];uint32_t name=0,kind=0,direction=0,entry=0,caseType=0,exit=0,map=0,destination=0;
 if(!readWord(address,name) || !readWord(address+4,kind) || !readWord(address+8,direction) || !readWord(address+28,entry) || !readWord(address+32,caseType) || !readWord(address+36,exit) || !readWord(address+40,map) || !readWord(address+44,destination))return false;
 kind>>=16;
 if(direction==20000){direction=10000;if((kind&0xfff)<=1){std::string text;if(!program_->text(name,text))return false;direction=8;
  for(unsigned n=0;n<8;++n){uint32_t pointer=0;std::string prefix;if(!readWord(program_->prefixes+4*n,pointer) || !program_->text(pointer,prefix))return false;if(text.compare(0,prefix.size(),prefix)==0){direction=n;break;}}
 }}
 if(caseType==UINT32_MAX)switch(kind&0xfff){case 0xf00:caseType=0;break;case 0:caseType=10;break;case 1:caseType=9;break;case 2:caseType=4;break;case 3:caseType=7;break;}
 const uint32_t words[]={name,kind,direction,address+12,entry?entry:program_->defaultEvent,0xffff0000,uint32_t(*switches_[unsigned(index)]),caseType,exit?exit:program_->defaultOut,0xffff0000,map,destination};
 for(unsigned n=0;n<12;++n)event.locals[n+3]=signedWord(words[n]);
 return true;
}
bool FieldBeroInfo::known() const{return program_ && generation_==generation && incomingKnown_;}
bool FieldBeroInfo::importScripts(EventAddressSpace& memory) const{return program_ && generation_==generation && memory.importScripts(program_->data);}
bool FieldBeroInfo::publishIncoming(const std::string& name,uint32_t flags) {
 if(!program_ || generation_!=generation || name.size()>=64 || name.find('\0')!=std::string::npos)return false;
 incoming_=name;flags_=flags;incomingKnown_=true;EventEntry event;
 if(!intoInfo(event)){incomingKnown_=false;publication_.reset();return false;}
 std::array<int32_t,8> words;for(unsigned n=0;n<8;++n)words[n]=event.locals[n+1];publication_=words;return true;
}
bool FieldBeroInfo::publishRoadIncoming(uint32_t sourceNode,uint32_t flags) {
 if(!program_ || generation_!=generation || !sourceNode)return false;
 const auto& t=program_->tables.at(room_);
 for(unsigned n=0;n<t.nodes.size();++n)if(t.nodes[n]==sourceNode){uint32_t pointer=0;std::string name;return program_->data.readWord(t.address+n*60,pointer) && program_->text(pointer,name) && publishIncoming(name,flags);}
 return false;
}
bool FieldBeroInfo::intoInfo(EventEntry& event,uint32_t incomingAddress) const {
 if(!known())return false;
 const auto& t=program_->tables.at(room_);uint32_t selected=program_->fallback,index=0;
 if(!incoming_.empty())for(unsigned n=0;n<t.nodes.size();++n){uint32_t pointer=0;std::string name;if(!program_->data.readWord(t.address+n*60,pointer) || !program_->text(pointer,name))return false;if(name==incoming_){selected=t.address+n*60;index=n;break;}}
 const bool special=(flags_&2) && incoming_.empty();if(special){selected=t.address;index=0;}
 uint32_t name=0,kind=0,direction=0,callback=0;
 if(!program_->data.readWord(selected,name) || !program_->data.readWord(selected+4,kind) || !program_->data.readWord(selected+8,direction) || !program_->data.readWord(selected+28,callback))return false;
 kind>>=16;
 if(direction==20000) {
  direction=10000;
  if((kind&0xfff)<=1){std::string text;if(!program_->text(name,text))return false;direction=8;
   for(unsigned n=0;n<8;++n){uint32_t pointer=0;std::string prefix;if(!program_->data.readWord(program_->prefixes+4*n,pointer) || !program_->text(pointer,prefix))return false;if(text.compare(0,prefix.size(),prefix)==0){direction=n;break;}}
  }
 }
 const uint32_t values[]={index,uint32_t(t.nodes.size()),special?name:incomingAddress,kind,direction,selected+12,callback?callback:program_->defaultEvent,0xffff0000};
 for(unsigned n=0;n<8;++n)event.locals[n+1]=signedWord(values[n]);
 return true;
}
bool FieldBeroInfo::readByte(uint32_t address,uint8_t& value) const {
 if(!program_ || generation_!=generation)return false;
 if(incomingKnown_ && address>=IncomingAddress && uint64_t(address)<=uint64_t(IncomingAddress)+incoming_.size()){value=address==IncomingAddress+incoming_.size()?0:uint8_t(incoming_[address-IncomingAddress]);return true;}
 return program_->data.readByte(address,value);
}
bool FieldBeroInfo::readWord(uint32_t address,uint32_t& value) const {uint32_t next=0;for(unsigned n=0;n<4;++n){uint8_t byte=0;if(!readByte(address+n,byte))return false;next=next<<8|byte;}value=next;return true;}
}
