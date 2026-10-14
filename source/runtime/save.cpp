#include "save.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <unistd.h>
// Native checkpoint format, not a GameCube memory-card record. Byte offsets
// and version gates are ABI: changing them requires migration, not struct casting.
// Full layout/version/knowledge-bit tables: docs/code/save-format.md.
namespace ttyd {
namespace {
// Four-byte words are little-endian; floats travel as copied binary32 bits.
// Byte shifts are 8 bits and masks are 255, independent of host endianness.
void put(SaveBytes& b, int offset, uint32_t v) { for (int i=0;i<4;++i) b[offset+i]=(v>>(i*8))&255; }
uint32_t get(const SaveBytes& b,int offset) { uint32_t v=0; for (int i=0;i<4;++i) v|=uint32_t(b[offset+i])<<(i*8); return v; }
// FNV-1a/32: offset basis 2166136261, prime 16777619; unsigned overflow
// is deliberate. Checksums cover bytes BEFORE their checksum field.
uint32_t checksum(const SaveBytes& b,int length) { uint32_t v=2166136261u; for(int i=0;i<length;++i) {v^=b[i];v*=16777619u;} return v; }
bool read(const std::string& path, Progress& p) {
    FILE* f=std::fopen(path.c_str(),"rb"); if(!f) return false;
    SaveBytes b{};const size_t size=std::fread(b.data(),1,b.size(),f);
    // v1..2=32 bytes; v3..7=40; v8..9=0x600; v10=0x620. Reject truncation and
    // trailing bytes before decoding the zero-filled largest-version buffer.
    bool ok=(size==32 && (get(b,4)==1 || get(b,4)==2)) || (size==40 && get(b,4)>=3 && get(b,4)<=7) ||
        (size==0x600 && (get(b,4)==8 || get(b,4)==9)) || (size==b.size() && get(b,4)==10);
    ok=ok && std::fgetc(f)==EOF; std::fclose(f);
    return ok && decodeSave(b,p);
}
}
SaveBytes encodeSave(const Progress& p) {
    SaveBytes b{}; if(p.mushrooms<0 || p.mushrooms>99 || p.hp<0 || p.hp>(p.originalPouch?32767:10) || !roomAsset(p.room) || !std::isfinite(p.elevation) || std::fabs(p.elevation)>12.7f ||
        (p.interiorMask && !((p.room==0 && p.interiorMask==1) || (p.room==2 && (p.interiorMask==2 || (p.interiorMask==4 && p.originalStory && *p.originalStory>=23))))))return b;
    const auto& talk=p.originalNpcTalkState;
    // Optional saved switches preserve UNKNOWN separately from known false/zero.
    // Do not infer original quest state from a legacy checkpoint's empty bytes.
    const bool known=talk.quest1420 || talk.flag5405 || talk.flag5406;
    const bool completed=talk.flag5376.has_value();
    if(!talk.trouble.valid())return b;
    const bool trouble=talk.trouble.present() || p.interiorMask==4;
    const bool extended=p.originalPouch || talk.flag5407.has_value() || completed || trouble;
    // Pick the minimum emitted version retaining the supplied optional state:
    // 6=save-block flags, 7=quest guards, 8=pouch/5407, 9=5376 completion guard.
    std::memcpy(b.data(),"PM2D",4); put(b,4,trouble?10:completed?9:extended?8:known?7:6);
    uint32_t x,z; std::memcpy(&x,&p.position.x,4); std::memcpy(&z,&p.position.z,4);
    put(b,8,x); put(b,12,z); put(b,16,p.hp); put(b,20,p.mushrooms);
    // Packed word at 20: mushrooms[7:0], quest1420[15:8]; bits16..18
    // mark quest/5405/5406 known;19/20 are flag values;21/22=5407 known/value;
    // 23=pouch present;24/25=5376 known/value. Higher bits must stay zero.
    if(known || extended)put(b,20,uint32_t(p.mushrooms) | (uint32_t(talk.quest1420.value_or(0))<<8) |
        (uint32_t(talk.quest1420.has_value())<<16) | (uint32_t(talk.flag5405.has_value())<<17) |
        (uint32_t(talk.flag5406.has_value())<<18) | (uint32_t(talk.flag5405.value_or(false))<<19) |
        (uint32_t(talk.flag5406.value_or(false))<<20) | (uint32_t(talk.flag5407.has_value())<<21) |
        (uint32_t(talk.flag5407.value_or(false))<<22) | (uint32_t(bool(p.originalPouch))<<23) |
        (uint32_t(completed)<<24) | (uint32_t(talk.flag5376.value_or(false))<<25));
    // Header bytes24..27: completion, sound, room, signed elevation in tenths.
    // +/-12.7 is the int8 elevation range admitted by this native encoder.
    // HP cap is harness10 without original pouch, signed-halfword32767 with it.
    b[24]=p.crumpDefeated; b[25]=p.sound;b[26]=p.room;b[27]=int8_t(std::round(p.elevation*10));
    if(p.originalStory){put(b,28,uint32_t(*p.originalStory));b[32]=1;}
    b[33]=p.interiorMask;
    b[34]=uint8_t(p.originalFlag1207) | (uint8_t(p.originalFlag233)<<1) | (uint8_t(p.originalFlag236)<<2);
    b[35]=p.originalSaveMode;
    put(b,36,checksum(b,36));
    // Original pouch bytes occupy [40,1532); the extended checksum at1532
    // covers the header, its checksum and the pouch, for 1536 total bytes.
    if(extended) {
        if(p.originalPouch)std::memcpy(b.data()+40,p.originalPouch->bytes.data(),p.originalPouch->bytes.size());
        put(b,1532,checksum(b,1532));
    }
    if(trouble) {
        // v10 appends additional state after the unchanged legacy pouch and
        // checksum. Four named header flags have no duplicate extension bits.
        for(unsigned i=0;i<6;++i) {
            b[1536+i]=uint8_t(talk.trouble.known>>(8*i));
            b[1542+i]=uint8_t(talk.trouble.values>>(8*i));
            b[1549+i]=talk.trouble.bytes[i];
        }
        b[1548]=talk.trouble.byteKnown;
        put(b,1564,checksum(b,1564));
    }
    return b;
}
// Copy shared storage explicitly; copying Progress alone aliases the live pouch.
Progress snapshotSaveProgress(const Progress& p) {
    auto out=p;
    if(p.originalPouch)out.originalPouch=std::make_shared<ProloguePouch>(*p.originalPouch);
    return out;
}
bool decodeSave(const SaveBytes& b, Progress& p) {
    const uint32_t version=get(b,4);
    if(std::memcmp(b.data(),"PM2D",4) || version<1 || version>10 ||
       (version>=3?get(b,36)!=checksum(b,36):get(b,28)!=checksum(b,28)))return false;
    if(version>=8 && get(b,1532)!=checksum(b,1532))return false;
    if(version==10) {
        if(get(b,1564)!=checksum(b,1564))return false;
        for(unsigned i=1555;i<1564;++i)if(b[i])return false;
    } else for(unsigned i=1536;i<b.size();++i)if(b[i])return false;
    if(version>=3 && (b[32]>1 || (version==3 && b[33]) ||
       (version<5?b[34]!=0:version==5?b[34]>1:(b[34]&~7u)!=0) ||
       (version<6 && b[35]) || (!b[32] && get(b,28))))return false;
    // Decode into a candidate and publish only at the final assignment. All
    // failures leave the caller's Progress unchanged; geometry is checked later.
    Progress out; uint32_t x=get(b,8),z=get(b,12);
    const uint32_t packed=get(b,20),mushrooms=version>=7?packed&255u:packed;
    if(version>=7) {
        // Reserved-bit masks depend on version. A value without its knowledge
        // bit is invalid, rather than silently turning unknown state into known.
        if((packed&(version>=9?0xfc000000u:version==8?0xff000000u:0xffe00000u)) || (!(packed&0x10000u) && (packed&0xff00u)) ||
           (!(packed&0x20000u) && (packed&0x80000u)) || (!(packed&0x40000u) && (packed&0x100000u)))return false;
        if(packed&0x10000u)out.originalNpcTalkState.quest1420=uint8_t(packed>>8);
        if(packed&0x20000u)out.originalNpcTalkState.flag5405=(packed&0x80000u)!=0;
        if(packed&0x40000u)out.originalNpcTalkState.flag5406=(packed&0x100000u)!=0;
    }
    if(version>=8) {
        if(!(packed&0x200000u) && (packed&0x400000u))return false;
        if(packed&0x200000u)out.originalNpcTalkState.flag5407=(packed&0x400000u)!=0;
        if(packed&0x800000u) {
            out.originalPouch=std::make_shared<ProloguePouch>();
            std::memcpy(out.originalPouch->bytes.data(),b.data()+40,out.originalPouch->bytes.size());
        } else for(unsigned i=40;i<1532;++i)if(b[i])return false;
    }
    if(version>=9) {
        if(!(packed&0x1000000u) && (packed&0x2000000u))return false;
        if(packed&0x1000000u)out.originalNpcTalkState.flag5376=(packed&0x2000000u)!=0;
    }
    if(version==10) {
        auto& t=out.originalNpcTalkState.trouble;
        for(unsigned i=0;i<6;++i) {
            t.known|=uint64_t(b[1536+i])<<(8*i);
            t.values|=uint64_t(b[1542+i])<<(8*i);
            t.bytes[i]=b[1549+i];
        }
        t.byteKnown=b[1548];if(!t.valid())return false;
    }
    // Native checkpoint sanity bounds: finite X/Z within +/-100, mushrooms<=99,
    // boolean bytes<=1; v1 has neither a room nor an elevation field.
    std::memcpy(&out.position.x,&x,4); std::memcpy(&out.position.z,&z,4);
    if(get(b,16)>(out.originalPouch?32767u:10u) || mushrooms>99 || b[24]>1 || b[25]>1 || (get(b,4)==1 && b[26]) || !roomAsset(b[26]) || (get(b,4)==1 && b[27]) || !std::isfinite(out.position.x) || !std::isfinite(out.position.z) || std::fabs(out.position.x)>100 || std::fabs(out.position.z)>100) return false;
    if(version>=3 && b[32]){const uint32_t word=get(b,28);out.originalStory=word<=0x7fffffff?int32_t(word):int32_t(int64_t(word)-0x100000000LL);}
    // Only existing interior variants are legal: plaza(room0)/mask1 and
    // east(room2)/mask2, and v10 Trouble Center/mask4 after story23. These are native packaged-room IDs, not REL addresses.
    if(version>=4) {
        out.interiorMask=b[33];
        if(out.interiorMask && !((b[26]==0 && out.interiorMask==1) || (b[26]==2 && (out.interiorMask==2 || (version==10 && out.interiorMask==4 && out.originalStory && *out.originalStory>=23)))))return false;
    }
    if(version>=5)out.originalFlag1207=(b[34]&1)!=0;
    if(version>=6) {
        out.originalFlag233=(b[34]&2)!=0;out.originalFlag236=(b[34]&4)!=0;
        out.originalSaveMode=b[35];
    }
    out.room=b[26];out.elevation=int8_t(b[27])/10.f;out.hp=get(b,16); out.mushrooms=mushrooms; out.crumpDefeated=b[24];out.sound=b[25];p=out;return true;
}
LoadResult loadSave(const std::string& path,Progress& p) {
    if(read(path,p)) return LoadResult::Loaded;
    if(read(path+".bak",p)) return LoadResult::Recovered;
    return access(path.c_str(),F_OK)==0 || access((path+".bak").c_str(),F_OK)==0 ? LoadResult::Invalid : LoadResult::Missing;
}
bool writeSave(const std::string& path,const Progress& p) {
    const auto b=encodeSave(p); Progress checked;
    if(!decodeSave(b,checked)) return false;
    // Commit protocol: validate encoded bytes, write/flush/close .tmp, move a
    // valid prior main to .bak, then rename .tmp to main. Failed final rename
    // can leave only .bak; loadSave tries it. No full power-loss guarantee is made.
    const std::string tmp=path+".tmp",bak=path+".bak";
    FILE* f=std::fopen(tmp.c_str(),"wb"); if(!f) return false;
    const size_t size=get(b,4)==10?b.size():get(b,4)>=8?0x600:40;
    bool ok=std::fwrite(b.data(),1,size,f)==size;
    if(std::fflush(f)!=0) ok=false;
    // libctru's filesystem backend closes/commits files; fsync is used on hosts.
#ifndef __3DS__
    if(fsync(fileno(f))!=0) ok=false;
#endif
    if(std::fclose(f)!=0) ok=false;
    if(!ok) {std::remove(tmp.c_str());return false;}
    Progress existing;
    if(read(path,existing)) {
        if(std::rename(path.c_str(),bak.c_str())!=0) {std::remove(tmp.c_str());return false;}
    }
    if(std::rename(tmp.c_str(),path.c_str())!=0) {std::remove(tmp.c_str());return false;}
    return true;
}
}
