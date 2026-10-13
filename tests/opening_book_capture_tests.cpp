#include "opening_book_capture.hpp"
#include <array>
#include <cassert>
#include <vector>
using namespace ttyd;
int main(){
 std::vector<uint8_t> source(240*400*4),target(256*256*4);
 for(unsigned x=0;x<400;++x)for(unsigned y=0;y<240;++y){const auto i=(x*240+y)*4;source[i]=uint8_t(x);source[i+1]=uint8_t(x>>8);source[i+2]=uint8_t(y);source[i+3]=uint8_t(x^y);}
 assert(!packOpeningBookCapture(nullptr,source.size(),target.data(),target.size()));assert(!packOpeningBookCapture(source.data(),source.size()-1,target.data(),target.size()));assert(!packOpeningBookCapture(source.data(),source.size(),target.data(),target.size()-1));
 assert(packOpeningBookCapture(source.data(),source.size(),target.data(),target.size()));
 // Independently enumerate8x8 bit-interleaving, including every tile boundary,
 // source screen rotation, content crop and edge padding, without reusing helper.
 std::vector<bool> visited(256*256);
 for(unsigned tileY=0;tileY<32;++tileY)for(unsigned tileX=0;tileX<32;++tileX)for(unsigned code=0;code<64;++code){
  unsigned x=tileX*8+(code&1)+((code&4)>>1)+((code&16)>>2);
  unsigned storageY=tileY*8+((code&2)>>1)+((code&8)>>2)+((code&32)>>3),y=255-storageY;
  const unsigned sx=110+std::min(x,192u),sy=std::min(y,239u);const size_t out=((tileY*32+tileX)*64+code)*4;
  assert(!visited[y*256+x]);visited[y*256+x]=true;
  assert(target[out]==uint8_t(sx) && target[out+1]==uint8_t(sx>>8) && target[out+2]==uint8_t(sy) && target[out+3]==uint8_t(sx^sy));
 }
 assert(bookCaptureTexel(0,0)==0 && bookCaptureTexel(7,7)==63 && bookCaptureTexel(8,0)==64 && bookCaptureTexel(0,8)==2048);
}
