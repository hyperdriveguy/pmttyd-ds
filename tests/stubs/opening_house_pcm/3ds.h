#pragma once
#include <cstddef>
#include <cstdint>
enum {NDSP_WBUF_FREE,NDSP_WBUF_QUEUED,NDSP_WBUF_PLAYING,NDSP_WBUF_DONE};
enum {NDSP_INTERP_LINEAR,NDSP_FORMAT_MONO_PCM16};
struct ndspWaveBuf {int16_t* data_pcm16=nullptr;uint32_t nsamples=0;bool looping=false;uint8_t status=NDSP_WBUF_FREE;};
void* linearAlloc(size_t);
void linearFree(void*);
void DSP_FlushDataCache(const void*,size_t);
void ndspChnWaveBufClear(int);
void ndspChnSetInterp(int,int);
void ndspChnSetRate(int,float);
void ndspChnSetFormat(int,int);
void ndspChnSetMix(int,float*);
void ndspChnWaveBufAdd(int,ndspWaveBuf*);
