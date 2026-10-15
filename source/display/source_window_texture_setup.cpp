#include "source_window_texture_setup.hpp"
namespace ttyd {namespace {
bool same(const SourceWindowTextureSetupServices&a,const SourceWindowTextureSetupServices&b){
 const auto&x=a.objects.gx;const auto&y=b.objects.gx;
 return x.user==y.user&&x.lifetime==y.lifetime&&!x.lifetime.owner_before(y.lifetime)&&!y.lifetime.owner_before(x.lifetime)&&x.current==y.current&&x.word==y.word&&x.byte==y.byte&&x.store==y.store&&x.clear==y.clear&&x.truncate==y.truncate&&a.objects.half==b.objects.half&&a.objects.doubleWord==b.objects.doubleWord&&a.objects.subtractSingle==b.objects.subtractSingle&&a.request==b.request;
}
struct Running{bool&v;explicit Running(bool&b):v(b){v=true;}~Running(){v=false;}};
}
std::optional<uint8_t> SourceWindowTexturePath::byte(uint32_t at)const{const auto i=cells.find(at);return i==cells.end()?std::nullopt:std::optional<uint8_t>(i->second);}
struct SourceWindowTextureSetupCursor::State {
 SourceWindowTextureSetupServices s;uint32_t function=0,input=0,pc=0,pending=0;
 uint32_t value=0,palette=0,handle=0,rounded=0,root=0,count=0,index=0,offset=0,table=0,header=0,flags=0,object=0;
 bool invalid=false;SourceWindowTexturePath path;std::optional<SourceWindowTextureSetupCall> call;
 SourceWindowTextureSetupCursor unpack;SourcePaletteTextureObjectCursor tex;
 bool valid(){if(invalid)return false;if(!s.objects.gx.current||!s.objects.gx.current(s.objects.gx.user))invalid=true;return !invalid;}
 bool read(uint32_t site,uint32_t at,uint32_t&out,bool byte=false){pending=site;if(!valid())return false;std::optional<uint32_t>v;if(byte){if(!s.objects.gx.byte)return false;const auto b=s.objects.gx.byte(s.objects.gx.user,at);if(b)v=*b;}else{if(!s.objects.gx.word)return false;v=s.objects.gx.word(s.objects.gx.user,at);}if(!valid()||!v)return false;out=*v;return true;}
 bool write(uint32_t site,uint32_t at,uint32_t size,uint32_t v){pending=site;if(!valid()||!s.objects.gx.store)return false;const bool ok=s.objects.gx.store(s.objects.gx.user,at,size,v);return valid()&&ok;}
 bool invoke(uint32_t fn,uint32_t site,std::initializer_list<uint32_t>args,uint32_t&out,bool destination=false,bool sourcePath=false){
  pending=fn;if(!valid()||!s.request)return false;if(!call){SourceWindowTextureSetupCall q;q.function=fn;q.callsite=site;q.count=uint8_t(args.size());unsigned i=0;for(auto a:args)q.arguments[i++]=a;q.destination=destination?&path:nullptr;q.path=sourcePath?&path:nullptr;call=q;}
  const auto v=s.request(s.objects.gx.user,*call);if(!valid()||!v)return false;out=*v;call.reset();return true;
 }
};
SourceWindowTextureSetupCursor::SourceWindowTextureSetupCursor()=default;SourceWindowTextureSetupCursor::~SourceWindowTextureSetupCursor()=default;
bool SourceWindowTextureSetupCursor::active()const{return bool(state_);}bool SourceWindowTextureSetupCursor::stale()const{return state_&&state_->invalid;}
uint32_t SourceWindowTextureSetupCursor::pending()const{return state_?state_->pending:0;}
uint32_t SourceWindowTextureSetupCursor::childPending()const{return !state_?0:state_->unpack.active()?state_->unpack.pending():state_->tex.childPending()?state_->tex.childPending():state_->tex.pending();}
bool SourceWindowTextureSetupCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumeSourceWindowTextureEntry(uint32_t fn,uint32_t input,const SourceWindowTextureSetupServices&s,SourceWindowTextureSetupCursor&cursor){
 if(cursor.invoking_||!s.objects.gx.lifetime||!s.objects.gx.current||(fn!=0x8007fbd8&&fn!=0x8007fcf0&&fn!=0x80037b6c))return false;
 if(cursor.state_){const auto&c=*cursor.state_;if(c.function!=fn||c.input!=input||!same(c.s,s))return false;}
 Running running(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<SourceWindowTextureSetupCursor::State>();auto&c=*cursor.state_;c.s=s;c.function=fn;c.input=input;c.pc=fn==0x8007fbd8?0:fn==0x8007fcf0?100:200;c.palette=input;}
 auto&c=*cursor.state_;
#define R(P,S,A,D,N) case P:if(!c.read(S,A,c.D))return false;c.pc=N;continue
#define B(P,S,A,D,N) case P:if(!c.read(S,A,c.D,true))return false;c.pc=N;continue
#define W(P,S,A,Z,V,N) case P:if(!c.write(S,A,Z,V))return false;c.pc=N;continue
#define C(P,F,S,A,D,N) case P:if(!c.invoke(F,S,A,c.D))return false;c.pc=N;continue
 for(;;){if(!c.valid())return false;switch(c.pc){
 W(0,0x8007fc00,0x8041ea18,4,0,1);
 W(1,0x8007fc04,0x8041ea1c,4,0,2);
 case 2:if(!c.invoke(0x800782e4,0x8007fc08,{0x802c3080,0,0},c.palette))return false;c.pc=3;continue;
 W(3,0x8007fc10,0x8041ea18,4,c.palette,4);
 case 4:c.pc=c.palette?30:5;continue;
 C(5,0x80007388,0x8007fc18,{},root,6);
 case 6:if(!c.invoke(0x8026a0b8,0x8007fc30,{0x802c3094,c.root},c.value,true))return false;c.pc=7;continue;
 case 7:if(!c.invoke(0x80144e88,0x8007fc40,{2,0},c.handle,false,true))return false;c.pc=8;continue;
 case 8:if(!c.invoke(0x80144dcc,0x8007fc48,{c.handle},c.value))return false;c.rounded=(c.value+31)&0xffffffe0u;c.pc=9;continue;
 case 9:if(!c.invoke(0x800300f0,0x8007fc5c,{0,c.rounded},c.palette))return false;c.pc=10;continue;
 W(10,0x8007fc60,0x8041ea18,4,c.palette,11);
 W(11,0x8007fc70,c.handle+0x6c,4,c.handle,12);
 R(12,0x8007fc7c,0x8041ea18,palette,13);
 case 13:if(!c.invoke(0x80144de4,0x8007fc80,{c.handle,c.palette,c.rounded,0,0x8007fcf0},c.value))return false;c.pc=900;continue;
 case 30:c.pending=0x80037b6c;if(!resumeSourceUnpackTexturePalette(c.palette,c.s,c.unpack)||!c.valid())return false;c.pc=31;continue;
 R(31,0x8007fc8c,0x8041ea18,palette,32);
 R(32,0x8007fc94,c.palette+4,count,33);
 case 33:if(!c.invoke(0x800300f0,0x8007fc9c,{0,c.count<<5},c.object))return false;c.pc=34;continue;
 W(34,0x8007fca0,0x8041ea24,4,c.object,35);
 case 35:c.index=c.offset=0;c.pc=36;continue;
 R(36,0x8007fcc4,0x8041ea18,palette,37);
 R(37,0x8007fccc,c.palette+4,count,38);
 case 38:c.pc=c.index<c.count?39:900;continue;
 R(39,0x8007fcb0,0x8041ea24,object,40);
 case 40:c.pending=0x80037a80;if(!resumeSourcePaletteTextureObject(c.palette,c.object+c.offset,c.index,c.s.objects,c.tex)||!c.valid())return false;c.offset+=32;++c.index;c.pc=36;continue;
 // Genuine DVD callback: first argument unused; second argument+2c is read first.
 R(100,0x8007fd04,c.input+0x2c,handle,101);
 R(101,0x8007fd08,0x8041ea18,palette,102);
 case 102:c.pending=0x80037b6c;if(!resumeSourceUnpackTexturePalette(c.palette,c.s,c.unpack)||!c.valid())return false;c.pc=103;continue;
 case 103:if(!c.invoke(0x80144dd4,0x8007fd14,{c.handle},c.value))return false;c.pc=104;continue;
 R(104,0x8007fd18,0x8041ea18,palette,105);
 R(105,0x8007fd20,c.palette+4,count,106);
 case 106:if(!c.invoke(0x800300f0,0x8007fd28,{0,c.count<<5},c.object))return false;c.pc=107;continue;
 W(107,0x8007fd2c,0x8041ea24,4,c.object,108);
 case 108:c.index=c.offset=0;c.pc=109;continue;
 R(109,0x8007fd50,0x8041ea18,palette,110);
 R(110,0x8007fd58,c.palette+4,count,111);
 case 111:c.pc=c.index<c.count?112:114;continue;
 R(112,0x8007fd3c,0x8041ea24,object,113);
 case 113:c.pending=0x80037a80;if(!resumeSourcePaletteTextureObject(c.palette,c.object+c.offset,c.index,c.s.objects,c.tex)||!c.valid())return false;c.offset+=32;++c.index;c.pc=109;continue;
 W(114,0x8007fd68,0x8041ea1c,4,1,900);
 // Whole UnpackTexPalette: source pointer threshold, all late alias reads,
 // per-header unpack flags, U16 loop comparison and wrapping offset mask.
 R(200,0x80037b80,c.palette,value,201);
 case 201:c.pc=c.value==0x0020af30?203:202;continue;
 case 202:if(!c.invoke(0x80297894,0x80037ba8,{0x802c0148,0x24,0x802c0158},c.value))return false;c.pc=203;continue;
 R(203,0x80037bac,c.palette+8,table,204);
 case 204:c.pc=c.table>0x80000000u?900:205;continue;
 W(205,0x80037bc4,c.palette+8,4,c.table+c.palette,206);
 case 206:c.index=0;c.pc=230;continue;
 R(207,0x80037bcc,c.palette+8,table,208);
 case 208:c.offset=(c.index<<3)&0x0007fff8;c.pc=209;continue;
 R(209,0x80037bd4,c.table+c.offset,header,210);
 case 210:c.pc=c.header?211:220;continue;
 W(211,0x80037be4,c.table+c.offset,4,c.header+c.palette,212);
 R(212,0x80037be8,c.palette+8,table,213);
 R(213,0x80037bec,c.table+c.offset,header,214);
 B(214,0x80037bf0,c.header+0x23,flags,215);
 case 215:c.pc=c.flags?220:216;continue;
 R(216,0x80037bfc,c.header+8,value,217);
 W(217,0x80037c08,c.header+8,4,c.value+c.palette,218);
 R(218,0x80037c0c,c.palette+8,table,219);
 R(219,0x80037c10,c.table+c.offset,header,2191);
 W(2191,0x80037c14,c.header+0x23,1,1,220);
 R(220,0x80037c18,c.palette+8,table,221);
 case 221:c.offset+=4;c.pc=222;continue;
 R(222,0x80037c20,c.table+c.offset,header,223);
 case 223:c.pc=c.header?224:229;continue;
 W(224,0x80037c30,c.table+c.offset,4,c.header+c.palette,225);
 R(225,0x80037c34,c.palette+8,table,226);
 R(226,0x80037c38,c.table+c.offset,header,227);
 B(227,0x80037c3c,c.header+2,flags,228);
 case 228:c.pc=c.flags?229:2281;continue;
 R(2281,0x80037c48,c.header+8,value,2282);
 W(2282,0x80037c54,c.header+8,4,c.value+c.palette,2283);
 R(2283,0x80037c58,c.palette+8,table,2284);
 R(2284,0x80037c5c,c.table+c.offset,header,2285);
 W(2285,0x80037c60,c.header+2,1,1,229);
 case 229:++c.index;c.pc=230;continue;
 R(230,0x80037c68,c.palette+4,count,231);
 case 231:c.pc=uint32_t(uint16_t(c.index))<c.count?207:900;continue;
 case 900:cursor.state_.reset();return true;
 default:return false;
 }}
#undef R
#undef B
#undef W
#undef C
}
bool resumeSourceWindowTextureSetup(const SourceWindowTextureSetupServices&s,SourceWindowTextureSetupCursor&c){return resumeSourceWindowTextureEntry(0x8007fbd8,0,s,c);}
bool resumeSourceWindowTextureCompletion(uint32_t second,const SourceWindowTextureSetupServices&s,SourceWindowTextureSetupCursor&c){return resumeSourceWindowTextureEntry(0x8007fcf0,second,s,c);}
bool resumeSourceUnpackTexturePalette(uint32_t p,const SourceWindowTextureSetupServices&s,SourceWindowTextureSetupCursor&c){return resumeSourceWindowTextureEntry(0x80037b6c,p,s,c);}
}
