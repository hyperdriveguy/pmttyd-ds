#include "source_mario_dvd_root.hpp"
#include <cassert>
#include <fstream>
#include <vector>
using namespace ttyd;
namespace {
uint32_t word(const std::vector<uint8_t>&b,unsigned a){assert(a+4<=b.size());return uint32_t(b[a])<<24|uint32_t(b[a+1])<<16|uint32_t(b[a+2])<<8|b[a+3];}
}
int main(){
 assert(sourceMarioDvdRoot(OriginalMarioDvdSourceR2)==0x8041f494);
 for(uint32_t input:{0u,1u,0x7fabu,0x7facu,0x7fadu,0x80000000u,0xffffffffu}){
  const auto expected=uint32_t((uint64_t(input)+0xffff8054ULL)&0xffffffffULL);
  assert(sourceMarioDvdRoot(input)==expected);
 }
 // Pointer zero is an actual arithmetic result, not a missing-service result.
 assert(sourceMarioDvdRoot(0x7fac)==0);
 std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);
 auto raw=[&](uint32_t at,unsigned n){std::vector<uint8_t>b(n);iso.seekg(at);iso.read(reinterpret_cast<char*>(b.data()),n);assert(iso);return b;};
 assert(raw(0,8)==std::vector<uint8_t>({'G','8','M','E','0','1',0,0}));const auto dol=word(raw(0x420,4),0);const auto h=raw(dol,256);
 auto source=[&](uint32_t at,unsigned n){std::vector<uint8_t>result;for(unsigned i=0;i<18;++i){const auto base=word(h,0x48+4*i),length=word(h,0x90+4*i);if(base<=at&&uint64_t(at)+n<=uint64_t(base)+length){assert(result.empty());result=raw(dol+word(h,4*i)+at-base,n);}}assert(result.size()==n);return result;};
 const auto init=source(0x80005418,8);assert(word(init,0)==0x3c408042&&word(init,4)==0x60427440);const auto r2=((word(init,0)&65535)<<16)|(word(init,4)&65535);assert(r2==OriginalMarioDvdSourceR2);
 const auto body=source(0x80007388,8);assert(word(body,0)==0x38628054&&word(body,4)==0x4e800020);
 assert(source(sourceMarioDvdRoot(r2),2)==std::vector<uint8_t>({'.',0}));
 // Native callers still consume original format/name bytes from their actual
 // source mapping; the arithmetic getter neither reads nor recreates strings.
 for(const auto&span:{std::pair<uint32_t,const char*>{0x802c3094,"%s/f/msgWindow.tpl"},{0x802c2ba0,"%s/f/%s.bfn"},{0x802c2bac,"%s/font/%s.bfn"}}){
  std::vector<uint8_t>expected;for(const char*p=span.second;*p;++p)expected.push_back(uint8_t(*p));expected.push_back(0);assert(source(span.first,unsigned(expected.size()))==expected);
 }
 return 0;
}
