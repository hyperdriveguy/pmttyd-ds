#include <memory>
#include "native_source_font_memory.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <unistd.h>
using namespace ttyd;
namespace {
void provision(EventAddressSpace&vm,uint8_t pattern){for(auto span:NativeSourceFontMemory::StateSpans)assert(vm.registerRam(span.first,std::vector<uint8_t>(span.second,pattern)));}
uint32_t word(EventAddressSpace&vm,uint32_t at){uint32_t v;assert(vm.readWord(at,v));return v;}
}
int main(int argc,char**argv){
 char path[]="/tmp/ttyd-font-test-XXXXXX";int fd=mkstemp(path);assert(fd>=0);close(fd);
 if(argc==1){std::ifstream meta("romfs/font.json");std::string json((std::istreambuf_iterator<char>(meta)),{});auto at=json.find("source_offset");assert(at!=std::string::npos);at=json.find(':',at)+1;std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);iso.seekg(std::streamoff(std::stoull(json.substr(at))));std::vector<uint8_t>raw(66560);iso.read(reinterpret_cast<char*>(raw.data()),raw.size());assert(iso);FILE*out=std::fopen(path,"wb");assert(out&&std::fwrite(raw.data(),raw.size(),1,out)==1);std::fclose(out);}
 auto file=NativeSourceFontFile::load(argc>1?argv[1]:path);assert(file&&file->bytes().size()==66560&&file->bytes()[77]==1);assert(!NativeSourceFontFile::load(nullptr)&&!NativeSourceFontFile::load("/no/source/font"));
 auto changed=file->bytes();changed[77]=9;FILE*out=std::fopen(path,"wb");assert(out&&std::fwrite(changed.data(),changed.size(),1,out)==1);std::fclose(out);assert(!NativeSourceFontFile::load(path));out=std::fopen(path,"wb");assert(out&&std::fwrite(file->bytes().data(),100,1,out)==1);std::fclose(out);assert(!NativeSourceFontFile::load(path)&&!unlink(path));
 constexpr uint32_t Heap=0x89000000;auto owner=NativeSourceFontMemory::create();assert(!owner->captured()&&owner->owns(0x8041e978)&&!owner->word(0x8041e978));
 EventNativeRuntime first(0x81000000,{}),second(0x82000000,{});provision(first.memory,0x12);provision(second.memory,0x34);
 auto a=owner->bind(first.memory);assert(a&&owner->captured()&&owner->word(0x8041e978)==0x12121212);assert(!owner->bind(first.memory));
 auto b=owner->bind(second.memory);assert(b&&word(second.memory,0x8041e978)==0x12121212);assert(second.memory.writeWord(0x8041e9ac,0x89abcdef));assert(word(first.memory,0x8041e9ac)==0x89abcdef&&owner->word(0x8041e9ac)==0x89abcdef);
 auto native=std::make_shared<std::vector<uint8_t>>(96,0xcc);std::weak_ptr<const void> retained=native;assert(owner->adoptAllocation(Heap,native->data(),96,native));assert(!owner->adoptAllocation(Heap+16,native->data()+16,16,native));assert(!owner->adoptAllocation(Heap+4096,native->data()+16,16,native));assert(!owner->adoptAllocation(0xfffffffc,native->data(),16,native));
 assert(owner->owns(Heap+95)&&owner->owns(Heap-1,2)&&!owner->byte(Heap));uint8_t value;assert(!first.memory.readByte(Heap,value));
 std::vector<uint8_t>actual{0x46,0x4f,0x4e,0x54,0x00,0x01,0x00,0x00};assert(owner->installRead(Heap,actual));assert(!owner->installRead(Heap,actual));assert(owner->word(Heap)==0x464f4e54&&word(first.memory,Heap)==0x464f4e54&&word(second.memory,Heap)==0x464f4e54);assert(!owner->byte(Heap+8)&&!first.memory.readByte(Heap+8,value)&&(*native)[8]==0xcc);
 assert(second.memory.writeByte(Heap+1,'X')&&owner->byte(Heap+1)=='X');assert(!owner->storeWord(Heap+6,123)&&owner->half(Heap+6)==0);assert(!owner->word(0xfffffffe));
 native.reset();assert(!retained.expired());b.reset();assert(word(second.memory,0x8041e978)==0x34343434&&!second.memory.readByte(Heap,value));assert(word(first.memory,0x8041e9ac)==0x89abcdef);
 a.reset();assert(word(first.memory,0x8041e978)==0x12121212&&!first.memory.readByte(Heap,value));assert(owner->word(Heap)==0x46584e54);owner.reset();assert(retained.expired());
 // No BSS/default state is created when any actual source span is missing.
 {EventNativeRuntime vm(0x83000000,{});auto memory=NativeSourceFontMemory::create();assert(vm.memory.registerRam(0x8041e978,std::vector<uint8_t>(0x44,0x56)));assert(!memory->bind(vm.memory)&&!memory->captured()&&!memory->word(0x8041e978)&&word(vm.memory,0x8041e978)==0x56565656);}
 // A reserved unknown allocation tail cannot be silently occupied at bind/read admission.
 {EventNativeRuntime vm(0x83000000,{});provision(vm.memory,0x56);auto memory=NativeSourceFontMemory::create();auto backing=std::make_shared<std::vector<uint8_t>>(64,0xcc);assert(memory->adoptAllocation(Heap,backing->data(),64,backing));assert(vm.memory.registerRam(Heap+48,{1}));assert(!memory->bind(vm.memory)&&!memory->captured()&&word(vm.memory,0x8041e978)==0x56565656);}
 {EventNativeRuntime vm(0x83000000,{});provision(vm.memory,0x56);auto memory=NativeSourceFontMemory::create();auto binding=memory->bind(vm.memory);assert(binding);auto backing=std::make_shared<std::vector<uint8_t>>(64,0xcc);assert(memory->adoptAllocation(Heap,backing->data(),64,backing));assert(vm.memory.registerRam(Heap+48,{1}));assert(!memory->installRead(Heap,{1,2,3})&&(*backing)[0]==0xcc&&!memory->byte(Heap));}
}
