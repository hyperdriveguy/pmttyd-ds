#include "source_palette_texture_object.hpp"
namespace ttyd {namespace {
bool same(const SourceGxTextureObjectServices&a,const SourceGxTextureObjectServices&b){return a.user==b.user&&a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.current==b.current&&a.word==b.word&&a.byte==b.byte&&a.store==b.store&&a.clear==b.clear&&a.truncate==b.truncate;}
bool same(const SourcePaletteTextureObjectServices&a,const SourcePaletteTextureObjectServices&b){return same(a.gx,b.gx)&&a.half==b.half&&a.doubleWord==b.doubleWord&&a.subtractSingle==b.subtractSingle;}
struct Running{bool&value;explicit Running(bool&v):value(v){value=true;}~Running(){value=false;}};
}
struct SourcePaletteTextureObjectCursor::State {
 SourcePaletteTextureObjectServices s;uint32_t palette=0,object=0,index=0,offset=0,table=0,header=0,phase=0,pending=0,min=0,max=0;
 uint64_t conversion=0;bool invalid=false;
 SourceGxTextureObjectInput init,lod;SourceGxTextureObjectCursor child;
 bool valid(){if(invalid)return false;if(!s.gx.current||!s.gx.current(s.gx.user))invalid=true;return !invalid;}
 bool word(uint32_t site,uint32_t address,uint32_t&out){pending=site;if(!valid()||!s.gx.word)return false;const auto v=s.gx.word(s.gx.user,address);if(!valid()||!v)return false;out=*v;return true;}
 bool byte(uint32_t site,uint32_t address,uint32_t&out){pending=site;if(!valid()||!s.gx.byte)return false;const auto v=s.gx.byte(s.gx.user,address);if(!valid()||!v)return false;out=*v;return true;}
 bool half(uint32_t site,uint32_t address,uint32_t&out){pending=site;if(!valid()||!s.half)return false;const auto v=s.half(s.gx.user,address);if(!valid()||!v)return false;out=*v;return true;}
 bool subtract(uint32_t site,uint32_t value,uint32_t&out){pending=site;if(!valid()||!s.subtractSingle)return false;const auto v=s.subtractSingle(s.gx.user,0x4330000000000000ULL|value,conversion);if(!valid()||!v)return false;out=*v;return true;}
};
SourcePaletteTextureObjectCursor::SourcePaletteTextureObjectCursor()=default;
SourcePaletteTextureObjectCursor::~SourcePaletteTextureObjectCursor()=default;
bool SourcePaletteTextureObjectCursor::active()const{return bool(state_);}
bool SourcePaletteTextureObjectCursor::stale()const{return state_&&state_->invalid;}
uint32_t SourcePaletteTextureObjectCursor::pending()const{return state_?state_->pending:0;}
uint32_t SourcePaletteTextureObjectCursor::childPending()const{return state_?state_->child.pending():0;}
bool SourcePaletteTextureObjectCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumeSourcePaletteTextureObject(uint32_t palette,uint32_t object,uint32_t index,const SourcePaletteTextureObjectServices&s,SourcePaletteTextureObjectCursor&cursor){
 if(cursor.invoking_||!s.gx.lifetime||!s.gx.current)return false;
 if(cursor.state_){const auto&c=*cursor.state_;if(c.palette!=palette||c.object!=object||c.index!=index||!same(c.s,s))return false;}
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<SourcePaletteTextureObjectCursor::State>();auto&c=*cursor.state_;c.s=s;c.palette=palette;c.object=object;c.index=index;c.offset=index<<3;c.init.integers[0]=c.lod.integers[0]=object;}
 auto&c=*cursor.state_;
#define WORD(P,S,A,D,N) case P:if(!c.word(S,A,D))return false;c.phase=N;continue
#define BYTE(P,S,A,D,N) case P:if(!c.byte(S,A,D))return false;c.phase=N;continue
#define HALF(P,S,A,D,N) case P:if(!c.half(S,A,D))return false;c.phase=N;continue
 for(;;){if(!c.valid())return false;switch(c.phase){
 WORD(0,0x80037aa0,palette+8,c.table,1);
 WORD(1,0x80037aa4,c.table+c.offset,c.header,2);
 BYTE(2,0x80037aa8,c.header+33,c.min,3);
 BYTE(3,0x80037aac,c.header+34,c.max,4);
 WORD(4,0x80037ac4,c.header+8,c.init.integers[1],5);
 HALF(5,0x80037acc,c.header+2,c.init.integers[2],6);
 HALF(6,0x80037ad0,c.header,c.init.integers[3],7);
 WORD(7,0x80037ad4,c.header+4,c.init.integers[4],8);
 WORD(8,0x80037ad8,c.header+12,c.init.integers[5],9);
 WORD(9,0x80037adc,c.header+16,c.init.integers[6],10);
 case 10:c.pending=0x802b72b0;c.init.integers[7]=c.min!=c.max;if(!resumeSourceGxTextureObject(c.pending,c.init,c.s.gx,c.child))return false;if(!c.valid())return false;c.phase=11;continue;
 WORD(11,0x80037ae4,c.table+c.offset,c.header,12);
 BYTE(12,0x80037af4,c.header+33,c.min,13);
 BYTE(13,0x80037afc,c.header+34,c.max,14);
 case 14:{c.pending=0x80037b0c;if(!c.s.doubleWord)return false;const auto value=c.s.doubleWord(c.s.gx.user,0x802c0140);if(!c.valid()||!value)return false;c.conversion=*value;c.phase=15;continue;}
 case 15:if(!c.subtract(0x80037b1c,c.min,c.lod.reals[0]))return false;c.phase=16;continue;
 WORD(16,0x80037b20,c.header+20,c.lod.integers[1],17);
 WORD(17,0x80037b28,c.header+24,c.lod.integers[2],18);
 WORD(18,0x80037b30,c.header+28,c.lod.reals[2],19);
 case 19:if(!c.subtract(0x80037b34,c.max,c.lod.reals[1]))return false;c.phase=20;continue;
 BYTE(20,0x80037b38,c.header+32,c.lod.integers[4],21);
 case 21:c.pending=0x802b7544;if(!resumeSourceGxTextureObject(c.pending,c.lod,c.s.gx,c.child))return false;if(!c.valid())return false;cursor.state_.reset();return true;
 default:return false;
 }}
#undef WORD
#undef BYTE
#undef HALF
}
bool SourcePrivateTextureObject::bind(uint32_t address,std::shared_ptr<const void> lifetime,void*user,bool(*current)(void*)){
 if(lifetime_||invalid_||invoking_||!lifetime||!current||uint64_t(address)+32>0x100000000ULL)return false;
 address_=address;lifetime_=std::move(lifetime);user_=user;current_=current;return live();
}
bool SourcePrivateTextureObject::owns(uint32_t address,uint32_t size)const{return lifetime_&&address>=address_&&uint64_t(address)+size<=uint64_t(address_)+32;}
bool SourcePrivateTextureObject::overlaps(uint32_t address,uint32_t size)const{return lifetime_&&size&&uint64_t(address)<uint64_t(address_)+32&&uint64_t(address)+size>address_;}
bool SourcePrivateTextureObject::live()const{if(invalid_||invoking_||!lifetime_||!current_)return false;Running running(invoking_);if(!current_(user_))invalid_=true;return !invalid_;}
std::optional<uint32_t> SourcePrivateTextureObject::read(uint32_t address,uint32_t size)const{
 if((size!=1&&size!=2&&size!=4)||!owns(address,size)||!live())return {};
 uint32_t value=0;for(uint32_t i=0;i<size;++i){const auto b=bytes_[address-address_+i];if(!b)return {};value=(value<<8)|*b;}return value;
}
bool SourcePrivateTextureObject::store(uint32_t address,uint32_t size,uint32_t value){
 if((size!=1&&size!=2&&size!=4)||!owns(address,size)||!live())return false;
 for(uint32_t i=0;i<size;++i)bytes_[address-address_+i]=uint8_t(value>>(8*(size-i-1)));
 return true;
}
std::optional<std::array<uint32_t,8>> SourcePrivateTextureObject::words()const{
 std::array<uint32_t,8> result{};for(unsigned i=0;i<8;++i){const auto v=read(address_+4*i,4);if(!v)return {};result[i]=*v;}return result;
}
void SourcePrivateTextureObject::retire(){invalid_=true;}
}
