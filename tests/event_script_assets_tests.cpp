#include "event_script_assets.hpp"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <unistd.h>
using namespace ttyd;
namespace {
void word(std::vector<uint8_t>& bytes,size_t at,uint32_t value) {for(unsigned i=0;i<4;++i)bytes[at+i]=uint8_t(value>>(24-8*i));}
void save(const char* path,const std::vector<uint8_t>& bytes) {auto* f=std::fopen(path,"wb");assert(f);assert(std::fwrite(bytes.data(),1,bytes.size(),f)==bytes.size());std::fclose(f);}
}
int main(int argc,char** argv) {
    if(argc==2) {
        EventScriptAssets original;assert(original.load(argv[1]));uint32_t p=0x80355cf0,w=0;unsigned commands=0;
        while(p<0x80355cf0+0xf0c) {assert(original.readWord(p,w));const uint32_t count=w>>16,op=w&0xffff;assert(count<=255 && op<=118);p+=4+4*count;++commands;}
        assert(p==0x80355cf0+0xf0c && commands==268 && original.readWord(p,w)); // Real terminal scan trailing word.
        assert(original.readWord(0x80355cf0,w) && w==0x0005005b);
        assert(original.readWord(0x80416bd0,w) && w==2 && original.readWord(0x80416bd4,w) && w==1 && original.readWord(0x80416bd8,w));
        std::cout<<"original Jump asset reader passed268 commands and trailing read\n";return 0;
    }
    char path[]="/tmp/pm-event-scripts-XXXXXX";const int fd=mkstemp(path);assert(fd>=0);close(fd);
    std::vector<uint8_t> packed(68,0);const char magic[]="PMEVT001";for(unsigned i=0;i<8;++i)packed[i]=magic[i];word(packed,8,2);
    word(packed,12,0x80300000);word(packed,16,16);word(packed,20,36);
    word(packed,24,0xfffffff0);word(packed,28,16);word(packed,32,52);
    word(packed,36,0x00020032);word(packed,40,0xfe363c80);word(packed,44,0x12345678);word(packed,48,1);
    word(packed,52,0x80001200);word(packed,56,0xabcdef01);word(packed,60,0x23456789);word(packed,64,0xdeadbeef);save(path,packed);
    EventScriptAssets assets;uint32_t w=77;uint8_t b=88;assert(!assets.readWord(0x80300000,w) && w==77);assert(assets.load(path) && assets.segments().size()==2);
    assert(assets.readWord(0x80300004,w) && w==0xfe363c80 && assets.readByte(0x80300007,b) && b==0x80);
    assert(assets.readWord(0x80300005,w) && w==0x363c8012); // Big-endian unaligned mapped bytes.
    assert(assets.readWord(0xfffffffc,w) && w==0xdeadbeef && assets.readByte(0xffffffff,b) && b==0xef);
    assert(!assets.readWord(0xfffffffd,w) && w==0xdeadbeef && !assets.readWord(0x8030000d,w) && !assets.readByte(0x80300010,b));
    for(int scenario=0;scenario<9;++scenario) {
        auto bad=packed;
        switch(scenario) {case 0:bad[0]='X';break;case 1:word(bad,8,0xffffffff);break;case 2:word(bad,20,35);break;case 3:word(bad,24,0x80300008);break;case 4:word(bad,28,20);break;case 5:word(bad,12,0x80300001);break;case 6:word(bad,16,0);break;case 7:bad.pop_back();break;case 8:bad.push_back(0);break;}
        save(path,bad);assert(!assets.load(path));assert(assets.readWord(0x80300004,w) && w==0xfe363c80); // Failure preserves previous valid pack.
    }
    auto adjacent=packed;word(adjacent,24,0x80300010);save(path,adjacent);assert(assets.load(path));
    assert(assets.readWord(0x8030000f,w) && w==0x01800012); // A mapped word can straddle adjacent original-address spans.
    assert(!assets.load(nullptr));unlink(path);assert(!assets.load(path));
    std::cout<<"event script asset tests passed\n";
}
