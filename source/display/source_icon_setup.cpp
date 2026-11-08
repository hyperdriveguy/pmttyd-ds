#include "source_icon_setup.hpp"
namespace ttyd {namespace {
constexpr uint32_t WorkBankSlots=32,WorkEntryBytes=56;
struct Guard{bool&v;explicit Guard(bool&x):v(x){v=true;}~Guard(){v=false;}};
bool same(const SourceIconSetupServices&a,const SourceIconSetupServices&b){
 const auto&x=a.common.objects.gx;const auto&y=b.common.objects.gx;
 return a.request==b.request&&a.common.request==b.common.request&&x.user==y.user&&x.lifetime==y.lifetime&&!x.lifetime.owner_before(y.lifetime)&&!y.lifetime.owner_before(x.lifetime)&&x.current==y.current&&x.word==y.word&&x.byte==y.byte&&x.store==y.store&&x.clear==y.clear&&x.truncate==y.truncate&&a.common.objects.half==b.common.objects.half&&a.common.objects.doubleWord==b.common.objects.doubleWord&&a.common.objects.subtractSingle==b.common.objects.subtractSingle;
}
}
struct SourceIconSetupCursor::State {
 SourceIconSetupServices s;uint32_t function=0,input=0,pc=0,pending=0,value=0,count=0,heap=0,root=0,handle=0,rounded=0,palette=0;
 bool invalid=false;SourceIconPath path;std::optional<SourceIconSetupCall>held;SourceWindowTextureSetupCursor unpack;
 bool valid(){if(invalid)return false;const auto&g=s.common.objects.gx;if(!g.current||!g.current(g.user))invalid=true;return !invalid;}
 bool read(uint32_t site,uint32_t a,uint32_t&out){pending=site;const auto&g=s.common.objects.gx;if(!valid()||!g.word)return false;const auto v=g.word(g.user,a);if(!valid()||!v)return false;out=*v;return true;}
 bool store(uint32_t site,uint32_t a,uint32_t v){pending=site;const auto&g=s.common.objects.gx;if(!valid()||!g.store)return false;const bool ok=g.store(g.user,a,4,v);return valid()&&ok;}
 bool call(uint32_t fn,uint32_t site,std::initializer_list<uint32_t>args,uint32_t&out,bool destination=false,bool sourcePath=false){
  pending=fn;if(!valid()||!s.request)return false;if(!held){SourceIconSetupCall q;q.function=fn;q.callsite=site;q.count=uint8_t(args.size());unsigned i=0;for(auto v:args)q.arguments[i++]=v;q.destination=destination?&path:nullptr;q.path=sourcePath?&path:nullptr;held=q;}
  const auto v=s.request(s.common.objects.gx.user,*held);if(!valid()||!v)return false;out=*v;held.reset();return true;
 }
};
SourceIconSetupCursor::SourceIconSetupCursor()=default;SourceIconSetupCursor::~SourceIconSetupCursor()=default;
bool SourceIconSetupCursor::active()const{return bool(state_);}bool SourceIconSetupCursor::stale()const{return state_&&state_->invalid;}uint32_t SourceIconSetupCursor::pending()const{return state_?state_->pending:0;}uint32_t SourceIconSetupCursor::childPending()const{return state_&&state_->unpack.active()?state_->unpack.pending():0;}bool SourceIconSetupCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumeSourceIconSetupEntry(uint32_t fn,uint32_t input,const SourceIconSetupServices&s,SourceIconSetupCursor&cursor){
 const auto&g=s.common.objects.gx;if(cursor.invoking_||!g.lifetime||!g.current||(fn!=0x8001b698&&fn!=0x8001b810&&fn!=0x8001b83c))return false;
 if(cursor.state_&&(cursor.state_->function!=fn||cursor.state_->input!=input||!same(cursor.state_->s,s)))return false;
 Guard guard(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<SourceIconSetupCursor::State>();auto&c=*cursor.state_;c.s=s;c.function=fn;c.input=input;c.pc=fn==0x8001b698?0:fn==0x8001b810?100:200;}
 auto&c=*cursor.state_;
#define W(P,S,A,V,N) case P:if(!c.store(S,A,V))return false;c.pc=N;continue
#define R(P,S,A,D,N) case P:if(!c.read(S,A,c.D))return false;c.pc=N;continue
 for(;;){if(!c.valid())return false;switch(c.pc){
 W(0,0x8001b6b8,0x8041e640,WorkBankSlots,1);
 case 1:if(!c.call(0x800300f0,0x8001b6bc,{0,WorkBankSlots*WorkEntryBytes},c.heap))return false;c.pc=2;continue;
 R(2,0x8001b6c0,0x8041e640,count,3);
 W(3,0x8001b6c8,0x8041e644,c.heap,4);
 case 4:if(!c.call(0x800050b4,0x8001b6d4,{c.heap,0,c.count*WorkEntryBytes},c.value))return false;c.pc=5;continue;
 W(5,0x8001b6e4,0x8041e648,WorkBankSlots,6);
 case 6:if(!c.call(0x800300f0,0x8001b6ec,{0,WorkBankSlots*WorkEntryBytes},c.heap))return false;c.pc=7;continue;
 R(7,0x8001b6f0,0x8041e648,count,8);
 W(8,0x8001b6f8,0x8041e64c,c.heap,9);
 case 9:if(!c.call(0x800050b4,0x8001b700,{c.heap,0,c.count*WorkEntryBytes},c.value))return false;c.pc=10;continue;
 W(10,0x8001b708,0x8041e630,0,11);W(11,0x8001b70c,0x8041e634,0,12);W(12,0x8001b710,0x8041e638,0,13);W(13,0x8001b714,0x8041e63c,0,14);
 case 14:if(!c.call(0x80007388,0x8001b718,{},c.root))return false;c.pc=15;continue;
 case 15:if(!c.call(0x8026a0b8,0x8001b730,{0x802bf858,c.root},c.value,true))return false;c.pc=16;continue;
 case 16:if(!c.call(0x80144e88,0x8001b740,{2,0},c.handle,false,true))return false;c.pc=c.handle?17:22;continue;
 case 17:if(!c.call(0x80144dcc,0x8001b74c,{c.handle},c.value))return false;c.rounded=(c.value+31)&0xffffffe0u;c.pc=18;continue;
 case 18:if(!c.call(0x800300f0,0x8001b760,{0,c.rounded},c.heap))return false;c.pc=19;continue;
 W(19,0x8001b764,0x8041e630,c.heap,20);W(20,0x8001b774,c.handle+0x6c,c.handle,21);
 // Original palette pointer is refreshed AFTER userdata store.
 R(21,0x8001b780,0x8041e630,palette,211);
 case 211:if(!c.call(0x80144de4,0x8001b784,{c.handle,c.palette,c.rounded,0,0x8001b83c},c.value))return false;c.pc=22;continue;
 case 22:if(!c.call(0x80007388,0x8001b788,{},c.root))return false;c.pc=23;continue;
 case 23:if(!c.call(0x8026a0b8,0x8001b7a0,{0x802bf864,c.root},c.value,true))return false;c.pc=24;continue;
 case 24:if(!c.call(0x80144e88,0x8001b7b0,{2,0},c.handle,false,true))return false;c.pc=c.handle?25:30;continue;
 case 25:if(!c.call(0x80144dcc,0x8001b7bc,{c.handle},c.value))return false;c.rounded=(c.value+31)&0xffffffe0u;c.pc=26;continue;
 case 26:if(!c.call(0x800300f0,0x8001b7d0,{0,c.rounded},c.heap))return false;c.pc=27;continue;
 W(27,0x8001b7d4,0x8041e634,c.heap,28);W(28,0x8001b7e4,c.handle+0x6c,c.handle,29);
 R(29,0x8001b7f0,0x8041e634,palette,291);
 case 291:if(!c.call(0x80144de4,0x8001b7f4,{c.handle,c.palette,c.rounded,0,0x8001b810},c.value))return false;c.pc=30;continue;
 R(100,0x8001b81c,c.input+0x2c,handle,101);
 case 101:if(!c.call(0x80144dd4,0x8001b820,{c.handle},c.value))return false;c.pc=102;continue;
 W(102,0x8001b828,0x8041e63c,1,30);
 R(200,0x8001b84c,c.input+0x2c,handle,201);
 R(201,0x8001b850,0x8041e630,palette,202);
 case 202:c.pending=0x80037b6c;if(!resumeSourceUnpackTexturePalette(c.palette,c.s.common,c.unpack)||!c.valid())return false;c.pc=203;continue;
 case 203:if(!c.call(0x80144dd4,0x8001b85c,{c.handle},c.value))return false;c.pc=204;continue;
 W(204,0x8001b864,0x8041e638,1,30);
 case 30:cursor.state_.reset();return true;
 default:return false;
 }}
#undef W
#undef R
}
bool resumeSourceIconSetup(const SourceIconSetupServices&s,SourceIconSetupCursor&c){return resumeSourceIconSetupEntry(0x8001b698,0,s,c);}
bool resumeSourceIconBinCompletion(uint32_t p,const SourceIconSetupServices&s,SourceIconSetupCursor&c){return resumeSourceIconSetupEntry(0x8001b810,p,s,c);}
bool resumeSourceIconTplCompletion(uint32_t p,const SourceIconSetupServices&s,SourceIconSetupCursor&c){return resumeSourceIconSetupEntry(0x8001b83c,p,s,c);}
}
