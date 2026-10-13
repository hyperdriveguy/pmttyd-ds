#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
namespace ttyd {
// Native framebuffer adapter, not a GameCube GX capture implementation.
// Same RGBA8 byte format on both sides; the SDK transfer detiles the sideways
// 240x400 render buffer. Source386x480 crop124/0 becomes native193x240 at110/0.
inline size_t bookCaptureTexel(unsigned x,unsigned y){
 unsigned morton=0;for(unsigned bit=0;bit<3;++bit){morton|=((x>>bit)&1)<<(2*bit);morton|=((y>>bit)&1)<<(2*bit+1);}
 return ((y/8)*32+x/8)*64+morton;
}
inline bool packOpeningBookCapture(const uint8_t* sideways,size_t sourceBytes,uint8_t* tiled,size_t targetBytes){
 if(!sideways || !tiled || sourceBytes<240*400*4 || targetBytes<256*256*4)return false;
 for(unsigned y=0;y<256;++y)for(unsigned x=0;x<256;++x){
  const unsigned screenX=110+std::min(x,192u),screenY=std::min(y,239u);
  const size_t source=(screenX*240+screenY)*4;
  const size_t target=bookCaptureTexel(x,255-y)*4;
  std::memcpy(tiled+target,sideways+source,4);
 }
 return true;
}
}
