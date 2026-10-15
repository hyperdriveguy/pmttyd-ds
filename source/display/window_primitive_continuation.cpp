#include "window_primitive_continuation.hpp"
#include <cstring>
namespace ttyd {
namespace {
float scalar(uint32_t x){float f;std::memcpy(&f,&x,4);return f;}
bool same(const WindowPrimitiveServices&a,const WindowPrimitiveServices&b){return a.user==b.user&&a.current==b.current&&a.word==b.word&&a.request==b.request&&a.matrix==b.matrix;}
bool equal(float a,float b){return std::memcmp(&a,&b,4)==0;}
struct Invoke{bool& b;explicit Invoke(bool&v):b(v){b=true;}~Invoke(){b=false;}};
}
// pc is original source instruction identity, f[] its binary32 registers.
// Entry f30/f31=x/y,f27/f28=width/height,f29=border; later f2/f3 are right/
// bottom,f4/f5 inner left/right,f6/f7 inner top/bottom before scratch reuse.
struct WindowPrimitiveCursor::State {
 WindowPrimitiveServices owner;const uint32_t* color=nullptr;uint32_t type=0,argumentType=0,camera=0,bits=0,constantColor=0,loadedColor=0,texture=0;
 uint32_t pc=0x8007b4a8,pending=0;bool waiting=false,mathWaiting=false,done=false,poisoned=false;
 std::array<float,5> input{};std::array<float,32> f{};
 WindowPrimitiveRequest request;BattleAudienceMatrixRequest math;BattleActMatrix local{},view{};unsigned viewIndex=0;
};
WindowPrimitiveCursor::WindowPrimitiveCursor()=default;WindowPrimitiveCursor::~WindowPrimitiveCursor()=default;
bool WindowPrimitiveCursor::reset(){if(invoking_)return false;state_.reset();return true;}
uint32_t WindowPrimitiveCursor::pending()const{return state_?state_->pending:0;}
// Retain exact source loads/arithmetic/FIFO sequence rather than precomputing
// nine quads. Five float args compare by bits (including signed zero/NaNs);
// observed owner loss poisons until reset, and reentry/reset is refused.
bool resumeOriginalWindowPrimitive(uint32_t type,const uint32_t& color,float x,float y,float width,float height,float border,const WindowPrimitiveServices&s,WindowPrimitiveCursor&cursor){
 if(cursor.invoking_)return false;
 Invoke invoking(cursor.invoking_);
 const std::array<float,5> inputs{x,y,width,height,border};
 if(!cursor.state_){cursor.state_=std::make_unique<WindowPrimitiveCursor::State>();auto&c=*cursor.state_;c.owner=s;c.color=&color;c.type=type;c.argumentType=type;c.input=inputs;c.f[30]=x;c.f[31]=y;c.f[27]=width;c.f[28]=height;c.f[29]=border;}
 auto&c=*cursor.state_;if(!same(s,c.owner)||c.color!=&color||c.argumentType!=type)return false;
 for(unsigned i=0;i<5;++i)if(!equal(c.input[i],inputs[i]))return false;
 auto current=[&](){if(c.poisoned)return false;if(!s.current||!s.current(s.user)){c.poisoned=true;return false;}return true;};
 auto read=[&](uint32_t address,uint32_t&out){c.pending=address;if(!s.word)return false;auto v=s.word(s.user,address);if(!current()||!v)return false;out=*v;return true;};
 auto call=[&](uint32_t fn,uint32_t site,std::vector<uint32_t> ints=std::vector<uint32_t>{},std::vector<float> reals=std::vector<float>{},std::optional<uint32_t> rgba=std::nullopt,std::optional<BattleActMatrix> matrix=std::nullopt)->std::optional<uint32_t>{
  if(!c.waiting){c.request={};c.request.function=fn;c.request.callsite=site;c.request.integers=std::move(ints);c.request.reals=std::move(reals);c.request.color=rgba;c.request.matrix=matrix;c.waiting=true;}
  c.pending=fn;if(!s.request)return {};auto v=s.request(s.user,c.request);if(!current()||!v)return {};c.waiting=false;return v;
 };
 // Each FIFO float owns its own retained store at cc008000. Advance source
 // PC only after acknowledgment; a later hold never repeats an earlier store.
 auto emit=[&](uint32_t site,float f){if(!c.waiting){c.request={};c.request.kind=WindowPrimitiveRequestKind::FifoFloat;c.request.callsite=site;c.request.reals={f};c.waiting=true;}c.pending=0xcc008000;if(!s.request)return false;auto v=s.request(s.user,c.request);if(!current()||!v)return false;c.waiting=false;return true;};
 auto math=[&](uint32_t fn){if(!c.mathWaiting){c.math={};c.math.function=fn;if(fn==0x8029dd64){c.math.left=c.view;c.math.right=c.local;}c.mathWaiting=true;}c.pending=fn;if(!s.matrix||!s.matrix(s.user,c.math)||!current())return false;c.local=c.math.output;c.mathWaiting=false;return true;};
 while(current()){
 switch(c.pc){
 case 0x8007b4a8:{const auto v=call(0x8000f2e4,0x8007b4a8);if(!v)return false;c.camera=*v;c.pc=0x8007b4ac;break;}
 // Actual window completion ea1c=0 is cached no-draw success AFTER camera
 // query. Low16 nonzero type normalizes0; texture indexing retains source mask.
 case 0x8007b4ac:if(!read(0x8041ea1c,c.bits))return false;if(!c.bits){c.done=true;c.pc=0x8007ba94;break;}if(c.type&0xffff)c.type=0;c.pc=0x8007b4cc;break;
 case 0x8007b4cc:if(!call(0x802b5c8c,0x8007b4cc,{0}))return false;c.pc=0x8007b4d4;break;
 case 0x8007b4d4:if(!call(0x802b940c,0x8007b4d4,{1}))return false;c.pc=0x8007b4ec;break;
 case 0x8007b4ec:if(!call(0x802b8d60,0x8007b4ec,{7,0,0,7,0}))return false;c.pc=0x8007b500;break;
 case 0x8007b500:if(!call(0x802b932c,0x8007b500,{1,4,5,7}))return false;c.pc=0x8007b510;break;
 case 0x8007b510:if(!call(0x802b93d8,0x8007b510,{1,7,0}))return false;c.pc=0x8007b514;break;
 case 0x8007b514:if(!read(0x80420538,c.bits))return false;c.f[1]=scalar(c.bits);c.pc=0x8007b51c;break;
 case 0x8007b51c:if(!read(0x80420528,c.constantColor))return false;c.pc=0x8007b534;break;
 case 0x8007b534:if(!call(0x802b8ff4,0x8007b534,{0},{c.f[1],c.f[1],c.f[1],c.f[1]},c.constantColor))return false;c.pc=0x8007b538;break;
 // Direct POS9/UV13, VAT0 float4 XY/ST, texgen matrix60; no color attribute.
 // One TEV stage uses constant C0 times texture color/alpha, not font material.
 case 0x8007b538:if(!call(0x802b4520,0x8007b538,{}))return false;c.pc=0x8007b544;break;
 case 0x8007b544:if(!call(0x802b3c08,0x8007b544,{9,1}))return false;c.pc=0x8007b550;break;
 case 0x8007b550:if(!call(0x802b3c08,0x8007b550,{13,1}))return false;c.pc=0x8007b568;break;
 case 0x8007b568:if(!call(0x802b4558,0x8007b568,{0,9,1,4,0}))return false;c.pc=0x8007b580;break;
 case 0x8007b580:if(!call(0x802b4558,0x8007b580,{0,13,1,4,0}))return false;c.pc=0x8007b59c;break;
 case 0x8007b59c:if(!call(0x802b4e04,0x8007b59c,{0,1,4,60,0,125}))return false;c.pc=0x8007b5a4;break;
 case 0x8007b5a4:if(!call(0x802b6f3c,0x8007b5a4,{0}))return false;c.pc=0x8007b5ac;break;
 case 0x8007b5ac:if(!call(0x802b5084,0x8007b5ac,{1}))return false;c.pc=0x8007b5b4;break;
 case 0x8007b5b4:if(!call(0x802b8fcc,0x8007b5b4,{1}))return false;c.pc=0x8007b5c8;break;
 case 0x8007b5c8:if(!call(0x802b8e30,0x8007b5c8,{0,0,0,255}))return false;c.pc=0x8007b5e4;break;
 case 0x8007b5e4:if(!call(0x802b89e8,0x8007b5e4,{0,0,0,0,1,0}))return false;c.pc=0x8007b600;break;
 case 0x8007b600:if(!call(0x802b8a50,0x8007b600,{0,0,0,0,1,0}))return false;c.pc=0x8007b618;break;
 case 0x8007b618:if(!call(0x802b8960,0x8007b618,{0,15,2,8,15}))return false;c.pc=0x8007b630;break;
 case 0x8007b630:if(!call(0x802b89a4,0x8007b630,{0,7,1,4,7}))return false;c.pc=0x8007b640;break;
 case 0x8007b640:if(!call(0x802b8c98,0x8007b640,{0,0,0}))return false;c.pc=0x8007b648;break;
 case 0x8007b648:if(!call(0x802ba39c,0x8007b648,{0}))return false;c.pc=0x8007b64c;break;
 // Borrowed caller color is loaded only here; later waits retain this copy.
 // SDK identity then view*local captures12 words from original camera+11c.
 case 0x8007b64c:c.loadedColor=*c.color;c.pc=0x8007b65c;break;
 case 0x8007b65c:if(!call(0x802b8ab8,0x8007b65c,{1},{},c.loadedColor))return false;c.pc=0x8007b664;break;
 case 0x8007b664:if(!math(0x8029dd04))return false;c.pc=0x8007b668;break;
 case 0x8007b668:while(c.viewIndex<12){if(!read(c.camera+0x11c+4*c.viewIndex,c.bits))return false;c.view[c.viewIndex++]=scalar(c.bits);}c.pc=0x8007b674;break;
 case 0x8007b674:if(!math(0x8029dd64))return false;c.pc=0x8007b680;break;
 case 0x8007b680:if(!call(0x802ba2fc,0x8007b680,{0},{},{},c.local))return false;c.pc=0x8007b688;break;
 case 0x8007b688:if(!call(0x802ba39c,0x8007b688,{0}))return false;c.pc=0x8007b68c;break;
 // Fresh ea24 points to original32-byte GX objects; type offset is
 // (type<<5)&001fffe0. It is not an atlas index or a texture-ready substitute.
 case 0x8007b68c:if(!read(0x8041ea24,c.texture))return false;c.texture+=((c.type<<5)&0x001fffe0);c.pc=0x8007b69c;break;
 case 0x8007b69c:if(!call(0x802b7880,0x8007b69c,{c.texture,0}))return false;c.pc=0x8007b6ac;break;
 // Begin QUADS128,VAT0,36 vertices. Following180 individual FIFO floats
 // are XYZ+UV for nine cells. Every lfs of538/540/544/548 reloads source
 // 0/one-third/two-thirds/1; retain these interleavings and binary32 operations.
 // No border clamp,mesh replacement or original SDK numeric parity is implied.
 case 0x8007b6ac:if(!call(0x802b5a6c,0x8007b6ac,{128,0,36}))return false;c.pc=0x8007b6b4;break;
 case 0x8007b6b4: c.f[2]=c.f[30]+c.f[27];c.pc=0x8007b6b8;break;
 case 0x8007b6b8: if(!emit(0x8007b6b8,c.f[30]))return false;c.pc=0x8007b6bc;break;
 case 0x8007b6bc: c.f[3]=c.f[31]-c.f[28];c.pc=0x8007b6c0;break;
 case 0x8007b6c0: c.f[4]=c.f[30]+c.f[29];c.pc=0x8007b6c4;break;
 case 0x8007b6c4: if(!read(0x80420540,c.bits))return false;c.f[1]=scalar(c.bits);c.pc=0x8007b6c8;break;
 case 0x8007b6c8: if(!emit(0x8007b6c8,c.f[31]))return false;c.pc=0x8007b6cc;break;
 case 0x8007b6cc: if(!read(0x80420538,c.bits))return false;c.f[0]=scalar(c.bits);c.pc=0x8007b6d0;break;
 case 0x8007b6d0: c.f[6]=c.f[31]-c.f[29];c.pc=0x8007b6d4;break;
 case 0x8007b6d4: c.f[5]=c.f[2]-c.f[29];c.pc=0x8007b6d8;break;
 case 0x8007b6d8: if(!emit(0x8007b6d8,c.f[0]))return false;c.pc=0x8007b6dc;break;
 case 0x8007b6dc: c.f[7]=c.f[29]+c.f[3];c.pc=0x8007b6e0;break;
 case 0x8007b6e0: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b6e4;break;
 case 0x8007b6e4: if(!read(0x80420544,c.bits))return false;c.f[0]=scalar(c.bits);c.pc=0x8007b6e8;break;
 case 0x8007b6e8: if(!emit(0x8007b6e8,c.f[8]))return false;c.pc=0x8007b6ec;break;
 case 0x8007b6ec: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b6f0;break;
 case 0x8007b6f0: if(!emit(0x8007b6f0,c.f[8]))return false;c.pc=0x8007b6f4;break;
 case 0x8007b6f4: if(!emit(0x8007b6f4,c.f[4]))return false;c.pc=0x8007b6f8;break;
 case 0x8007b6f8: if(!emit(0x8007b6f8,c.f[31]))return false;c.pc=0x8007b6fc;break;
 case 0x8007b6fc: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b700;break;
 case 0x8007b700: if(!emit(0x8007b700,c.f[8]))return false;c.pc=0x8007b704;break;
 case 0x8007b704: if(!emit(0x8007b704,c.f[1]))return false;c.pc=0x8007b708;break;
 case 0x8007b708: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b70c;break;
 case 0x8007b70c: if(!emit(0x8007b70c,c.f[8]))return false;c.pc=0x8007b710;break;
 case 0x8007b710: if(!emit(0x8007b710,c.f[4]))return false;c.pc=0x8007b714;break;
 case 0x8007b714: if(!emit(0x8007b714,c.f[6]))return false;c.pc=0x8007b718;break;
 case 0x8007b718: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b71c;break;
 case 0x8007b71c: if(!emit(0x8007b71c,c.f[8]))return false;c.pc=0x8007b720;break;
 case 0x8007b720: if(!emit(0x8007b720,c.f[1]))return false;c.pc=0x8007b724;break;
 case 0x8007b724: if(!emit(0x8007b724,c.f[1]))return false;c.pc=0x8007b728;break;
 case 0x8007b728: if(!emit(0x8007b728,c.f[30]))return false;c.pc=0x8007b72c;break;
 case 0x8007b72c: if(!emit(0x8007b72c,c.f[6]))return false;c.pc=0x8007b730;break;
 case 0x8007b730: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b734;break;
 case 0x8007b734: if(!emit(0x8007b734,c.f[8]))return false;c.pc=0x8007b738;break;
 case 0x8007b738: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b73c;break;
 case 0x8007b73c: if(!emit(0x8007b73c,c.f[8]))return false;c.pc=0x8007b740;break;
 case 0x8007b740: if(!emit(0x8007b740,c.f[1]))return false;c.pc=0x8007b744;break;
 case 0x8007b744: if(!emit(0x8007b744,c.f[4]))return false;c.pc=0x8007b748;break;
 case 0x8007b748: if(!emit(0x8007b748,c.f[31]))return false;c.pc=0x8007b74c;break;
 case 0x8007b74c: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b750;break;
 case 0x8007b750: if(!emit(0x8007b750,c.f[8]))return false;c.pc=0x8007b754;break;
 case 0x8007b754: if(!emit(0x8007b754,c.f[1]))return false;c.pc=0x8007b758;break;
 case 0x8007b758: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b75c;break;
 case 0x8007b75c: if(!emit(0x8007b75c,c.f[8]))return false;c.pc=0x8007b760;break;
 case 0x8007b760: if(!emit(0x8007b760,c.f[5]))return false;c.pc=0x8007b764;break;
 case 0x8007b764: if(!emit(0x8007b764,c.f[31]))return false;c.pc=0x8007b768;break;
 case 0x8007b768: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b76c;break;
 case 0x8007b76c: if(!emit(0x8007b76c,c.f[8]))return false;c.pc=0x8007b770;break;
 case 0x8007b770: if(!emit(0x8007b770,c.f[0]))return false;c.pc=0x8007b774;break;
 case 0x8007b774: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b778;break;
 case 0x8007b778: if(!emit(0x8007b778,c.f[8]))return false;c.pc=0x8007b77c;break;
 case 0x8007b77c: if(!emit(0x8007b77c,c.f[5]))return false;c.pc=0x8007b780;break;
 case 0x8007b780: if(!emit(0x8007b780,c.f[6]))return false;c.pc=0x8007b784;break;
 case 0x8007b784: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b788;break;
 case 0x8007b788: if(!emit(0x8007b788,c.f[8]))return false;c.pc=0x8007b78c;break;
 case 0x8007b78c: if(!emit(0x8007b78c,c.f[0]))return false;c.pc=0x8007b790;break;
 case 0x8007b790: if(!emit(0x8007b790,c.f[1]))return false;c.pc=0x8007b794;break;
 case 0x8007b794: if(!emit(0x8007b794,c.f[4]))return false;c.pc=0x8007b798;break;
 case 0x8007b798: if(!emit(0x8007b798,c.f[6]))return false;c.pc=0x8007b79c;break;
 case 0x8007b79c: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b7a0;break;
 case 0x8007b7a0: if(!emit(0x8007b7a0,c.f[8]))return false;c.pc=0x8007b7a4;break;
 case 0x8007b7a4: if(!emit(0x8007b7a4,c.f[1]))return false;c.pc=0x8007b7a8;break;
 case 0x8007b7a8: if(!emit(0x8007b7a8,c.f[1]))return false;c.pc=0x8007b7ac;break;
 case 0x8007b7ac: if(!emit(0x8007b7ac,c.f[5]))return false;c.pc=0x8007b7b0;break;
 case 0x8007b7b0: if(!emit(0x8007b7b0,c.f[31]))return false;c.pc=0x8007b7b4;break;
 case 0x8007b7b4: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b7b8;break;
 case 0x8007b7b8: if(!emit(0x8007b7b8,c.f[8]))return false;c.pc=0x8007b7bc;break;
 case 0x8007b7bc: if(!emit(0x8007b7bc,c.f[0]))return false;c.pc=0x8007b7c0;break;
 case 0x8007b7c0: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b7c4;break;
 case 0x8007b7c4: if(!emit(0x8007b7c4,c.f[8]))return false;c.pc=0x8007b7c8;break;
 case 0x8007b7c8: if(!emit(0x8007b7c8,c.f[2]))return false;c.pc=0x8007b7cc;break;
 case 0x8007b7cc: if(!emit(0x8007b7cc,c.f[31]))return false;c.pc=0x8007b7d0;break;
 case 0x8007b7d0: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b7d4;break;
 case 0x8007b7d4: if(!emit(0x8007b7d4,c.f[8]))return false;c.pc=0x8007b7d8;break;
 case 0x8007b7d8: if(!read(0x80420548,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b7dc;break;
 case 0x8007b7dc: if(!emit(0x8007b7dc,c.f[8]))return false;c.pc=0x8007b7e0;break;
 case 0x8007b7e0: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b7e4;break;
 case 0x8007b7e4: if(!emit(0x8007b7e4,c.f[8]))return false;c.pc=0x8007b7e8;break;
 case 0x8007b7e8: if(!emit(0x8007b7e8,c.f[2]))return false;c.pc=0x8007b7ec;break;
 case 0x8007b7ec: if(!emit(0x8007b7ec,c.f[6]))return false;c.pc=0x8007b7f0;break;
 case 0x8007b7f0: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b7f4;break;
 case 0x8007b7f4: if(!emit(0x8007b7f4,c.f[8]))return false;c.pc=0x8007b7f8;break;
 case 0x8007b7f8: if(!read(0x80420548,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b7fc;break;
 case 0x8007b7fc: if(!emit(0x8007b7fc,c.f[8]))return false;c.pc=0x8007b800;break;
 case 0x8007b800: if(!emit(0x8007b800,c.f[1]))return false;c.pc=0x8007b804;break;
 case 0x8007b804: if(!emit(0x8007b804,c.f[5]))return false;c.pc=0x8007b808;break;
 case 0x8007b808: if(!emit(0x8007b808,c.f[6]))return false;c.pc=0x8007b80c;break;
 case 0x8007b80c: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b810;break;
 case 0x8007b810: if(!emit(0x8007b810,c.f[8]))return false;c.pc=0x8007b814;break;
 case 0x8007b814: if(!emit(0x8007b814,c.f[0]))return false;c.pc=0x8007b818;break;
 case 0x8007b818: if(!emit(0x8007b818,c.f[1]))return false;c.pc=0x8007b81c;break;
 case 0x8007b81c: if(!emit(0x8007b81c,c.f[30]))return false;c.pc=0x8007b820;break;
 case 0x8007b820: if(!emit(0x8007b820,c.f[6]))return false;c.pc=0x8007b824;break;
 case 0x8007b824: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b828;break;
 case 0x8007b828: if(!emit(0x8007b828,c.f[8]))return false;c.pc=0x8007b82c;break;
 case 0x8007b82c: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b830;break;
 case 0x8007b830: if(!emit(0x8007b830,c.f[8]))return false;c.pc=0x8007b834;break;
 case 0x8007b834: if(!emit(0x8007b834,c.f[1]))return false;c.pc=0x8007b838;break;
 case 0x8007b838: if(!emit(0x8007b838,c.f[4]))return false;c.pc=0x8007b83c;break;
 case 0x8007b83c: if(!emit(0x8007b83c,c.f[6]))return false;c.pc=0x8007b840;break;
 case 0x8007b840: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b844;break;
 case 0x8007b844: if(!emit(0x8007b844,c.f[8]))return false;c.pc=0x8007b848;break;
 case 0x8007b848: if(!emit(0x8007b848,c.f[1]))return false;c.pc=0x8007b84c;break;
 case 0x8007b84c: if(!emit(0x8007b84c,c.f[1]))return false;c.pc=0x8007b850;break;
 case 0x8007b850: if(!emit(0x8007b850,c.f[4]))return false;c.pc=0x8007b854;break;
 case 0x8007b854: if(!emit(0x8007b854,c.f[7]))return false;c.pc=0x8007b858;break;
 case 0x8007b858: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b85c;break;
 case 0x8007b85c: if(!emit(0x8007b85c,c.f[8]))return false;c.pc=0x8007b860;break;
 case 0x8007b860: if(!emit(0x8007b860,c.f[1]))return false;c.pc=0x8007b864;break;
 case 0x8007b864: if(!emit(0x8007b864,c.f[0]))return false;c.pc=0x8007b868;break;
 case 0x8007b868: if(!emit(0x8007b868,c.f[30]))return false;c.pc=0x8007b86c;break;
 case 0x8007b86c: if(!emit(0x8007b86c,c.f[7]))return false;c.pc=0x8007b870;break;
 case 0x8007b870: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b874;break;
 case 0x8007b874: if(!emit(0x8007b874,c.f[8]))return false;c.pc=0x8007b878;break;
 case 0x8007b878: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b87c;break;
 case 0x8007b87c: if(!emit(0x8007b87c,c.f[8]))return false;c.pc=0x8007b880;break;
 case 0x8007b880: if(!emit(0x8007b880,c.f[0]))return false;c.pc=0x8007b884;break;
 case 0x8007b884: if(!emit(0x8007b884,c.f[4]))return false;c.pc=0x8007b888;break;
 case 0x8007b888: if(!emit(0x8007b888,c.f[6]))return false;c.pc=0x8007b88c;break;
 case 0x8007b88c: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b890;break;
 case 0x8007b890: if(!emit(0x8007b890,c.f[8]))return false;c.pc=0x8007b894;break;
 case 0x8007b894: if(!emit(0x8007b894,c.f[1]))return false;c.pc=0x8007b898;break;
 case 0x8007b898: if(!emit(0x8007b898,c.f[1]))return false;c.pc=0x8007b89c;break;
 case 0x8007b89c: if(!emit(0x8007b89c,c.f[5]))return false;c.pc=0x8007b8a0;break;
 case 0x8007b8a0: if(!emit(0x8007b8a0,c.f[6]))return false;c.pc=0x8007b8a4;break;
 case 0x8007b8a4: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b8a8;break;
 case 0x8007b8a8: if(!emit(0x8007b8a8,c.f[8]))return false;c.pc=0x8007b8ac;break;
 case 0x8007b8ac: if(!emit(0x8007b8ac,c.f[0]))return false;c.pc=0x8007b8b0;break;
 case 0x8007b8b0: if(!emit(0x8007b8b0,c.f[1]))return false;c.pc=0x8007b8b4;break;
 case 0x8007b8b4: if(!emit(0x8007b8b4,c.f[5]))return false;c.pc=0x8007b8b8;break;
 case 0x8007b8b8: if(!emit(0x8007b8b8,c.f[7]))return false;c.pc=0x8007b8bc;break;
 case 0x8007b8bc: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b8c0;break;
 case 0x8007b8c0: if(!emit(0x8007b8c0,c.f[8]))return false;c.pc=0x8007b8c4;break;
 case 0x8007b8c4: if(!emit(0x8007b8c4,c.f[0]))return false;c.pc=0x8007b8c8;break;
 case 0x8007b8c8: if(!emit(0x8007b8c8,c.f[0]))return false;c.pc=0x8007b8cc;break;
 case 0x8007b8cc: if(!emit(0x8007b8cc,c.f[4]))return false;c.pc=0x8007b8d0;break;
 case 0x8007b8d0: if(!emit(0x8007b8d0,c.f[7]))return false;c.pc=0x8007b8d4;break;
 case 0x8007b8d4: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b8d8;break;
 case 0x8007b8d8: if(!emit(0x8007b8d8,c.f[8]))return false;c.pc=0x8007b8dc;break;
 case 0x8007b8dc: if(!emit(0x8007b8dc,c.f[1]))return false;c.pc=0x8007b8e0;break;
 case 0x8007b8e0: if(!emit(0x8007b8e0,c.f[0]))return false;c.pc=0x8007b8e4;break;
 case 0x8007b8e4: if(!emit(0x8007b8e4,c.f[5]))return false;c.pc=0x8007b8e8;break;
 case 0x8007b8e8: if(!emit(0x8007b8e8,c.f[6]))return false;c.pc=0x8007b8ec;break;
 case 0x8007b8ec: if(!read(0x80420538,c.bits))return false;c.f[8]=scalar(c.bits);c.pc=0x8007b8f0;break;
 case 0x8007b8f0: if(!emit(0x8007b8f0,c.f[8]))return false;c.pc=0x8007b8f4;break;
 case 0x8007b8f4: if(!emit(0x8007b8f4,c.f[0]))return false;c.pc=0x8007b8f8;break;
 case 0x8007b8f8: if(!emit(0x8007b8f8,c.f[1]))return false;c.pc=0x8007b8fc;break;
 case 0x8007b8fc: if(!emit(0x8007b8fc,c.f[2]))return false;c.pc=0x8007b900;break;
 case 0x8007b900: if(!emit(0x8007b900,c.f[6]))return false;c.pc=0x8007b904;break;
 case 0x8007b904: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b908;break;
 case 0x8007b908: if(!emit(0x8007b908,c.f[6]))return false;c.pc=0x8007b90c;break;
 case 0x8007b90c: if(!read(0x80420548,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b910;break;
 case 0x8007b910: if(!emit(0x8007b910,c.f[6]))return false;c.pc=0x8007b914;break;
 case 0x8007b914: if(!emit(0x8007b914,c.f[1]))return false;c.pc=0x8007b918;break;
 case 0x8007b918: if(!emit(0x8007b918,c.f[2]))return false;c.pc=0x8007b91c;break;
 case 0x8007b91c: if(!emit(0x8007b91c,c.f[7]))return false;c.pc=0x8007b920;break;
 case 0x8007b920: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b924;break;
 case 0x8007b924: if(!emit(0x8007b924,c.f[6]))return false;c.pc=0x8007b928;break;
 case 0x8007b928: if(!read(0x80420548,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b92c;break;
 case 0x8007b92c: if(!emit(0x8007b92c,c.f[6]))return false;c.pc=0x8007b930;break;
 case 0x8007b930: if(!emit(0x8007b930,c.f[0]))return false;c.pc=0x8007b934;break;
 case 0x8007b934: if(!emit(0x8007b934,c.f[5]))return false;c.pc=0x8007b938;break;
 case 0x8007b938: if(!emit(0x8007b938,c.f[7]))return false;c.pc=0x8007b93c;break;
 case 0x8007b93c: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b940;break;
 case 0x8007b940: if(!emit(0x8007b940,c.f[6]))return false;c.pc=0x8007b944;break;
 case 0x8007b944: if(!emit(0x8007b944,c.f[0]))return false;c.pc=0x8007b948;break;
 case 0x8007b948: if(!emit(0x8007b948,c.f[0]))return false;c.pc=0x8007b94c;break;
 case 0x8007b94c: if(!emit(0x8007b94c,c.f[30]))return false;c.pc=0x8007b950;break;
 case 0x8007b950: if(!emit(0x8007b950,c.f[7]))return false;c.pc=0x8007b954;break;
 case 0x8007b954: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b958;break;
 case 0x8007b958: if(!emit(0x8007b958,c.f[6]))return false;c.pc=0x8007b95c;break;
 case 0x8007b95c: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b960;break;
 case 0x8007b960: if(!emit(0x8007b960,c.f[6]))return false;c.pc=0x8007b964;break;
 case 0x8007b964: if(!emit(0x8007b964,c.f[0]))return false;c.pc=0x8007b968;break;
 case 0x8007b968: if(!emit(0x8007b968,c.f[4]))return false;c.pc=0x8007b96c;break;
 case 0x8007b96c: if(!emit(0x8007b96c,c.f[7]))return false;c.pc=0x8007b970;break;
 case 0x8007b970: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b974;break;
 case 0x8007b974: if(!emit(0x8007b974,c.f[6]))return false;c.pc=0x8007b978;break;
 case 0x8007b978: if(!emit(0x8007b978,c.f[1]))return false;c.pc=0x8007b97c;break;
 case 0x8007b97c: if(!emit(0x8007b97c,c.f[0]))return false;c.pc=0x8007b980;break;
 case 0x8007b980: if(!emit(0x8007b980,c.f[4]))return false;c.pc=0x8007b984;break;
 case 0x8007b984: if(!emit(0x8007b984,c.f[3]))return false;c.pc=0x8007b988;break;
 case 0x8007b988: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b98c;break;
 case 0x8007b98c: if(!emit(0x8007b98c,c.f[6]))return false;c.pc=0x8007b990;break;
 case 0x8007b990: if(!emit(0x8007b990,c.f[1]))return false;c.pc=0x8007b994;break;
 case 0x8007b994: if(!read(0x80420548,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b998;break;
 case 0x8007b998: if(!emit(0x8007b998,c.f[6]))return false;c.pc=0x8007b99c;break;
 case 0x8007b99c: if(!emit(0x8007b99c,c.f[30]))return false;c.pc=0x8007b9a0;break;
 case 0x8007b9a0: if(!emit(0x8007b9a0,c.f[3]))return false;c.pc=0x8007b9a4;break;
 case 0x8007b9a4: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b9a8;break;
 case 0x8007b9a8: if(!emit(0x8007b9a8,c.f[6]))return false;c.pc=0x8007b9ac;break;
 case 0x8007b9ac: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b9b0;break;
 case 0x8007b9b0: if(!emit(0x8007b9b0,c.f[6]))return false;c.pc=0x8007b9b4;break;
 case 0x8007b9b4: if(!read(0x80420548,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b9b8;break;
 case 0x8007b9b8: if(!emit(0x8007b9b8,c.f[6]))return false;c.pc=0x8007b9bc;break;
 case 0x8007b9bc: if(!emit(0x8007b9bc,c.f[4]))return false;c.pc=0x8007b9c0;break;
 case 0x8007b9c0: if(!emit(0x8007b9c0,c.f[7]))return false;c.pc=0x8007b9c4;break;
 case 0x8007b9c4: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b9c8;break;
 case 0x8007b9c8: if(!emit(0x8007b9c8,c.f[6]))return false;c.pc=0x8007b9cc;break;
 case 0x8007b9cc: if(!emit(0x8007b9cc,c.f[1]))return false;c.pc=0x8007b9d0;break;
 case 0x8007b9d0: if(!emit(0x8007b9d0,c.f[0]))return false;c.pc=0x8007b9d4;break;
 case 0x8007b9d4: if(!emit(0x8007b9d4,c.f[5]))return false;c.pc=0x8007b9d8;break;
 case 0x8007b9d8: if(!emit(0x8007b9d8,c.f[7]))return false;c.pc=0x8007b9dc;break;
 case 0x8007b9dc: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b9e0;break;
 case 0x8007b9e0: if(!emit(0x8007b9e0,c.f[6]))return false;c.pc=0x8007b9e4;break;
 case 0x8007b9e4: if(!emit(0x8007b9e4,c.f[0]))return false;c.pc=0x8007b9e8;break;
 case 0x8007b9e8: if(!emit(0x8007b9e8,c.f[0]))return false;c.pc=0x8007b9ec;break;
 case 0x8007b9ec: if(!emit(0x8007b9ec,c.f[5]))return false;c.pc=0x8007b9f0;break;
 case 0x8007b9f0: if(!emit(0x8007b9f0,c.f[3]))return false;c.pc=0x8007b9f4;break;
 case 0x8007b9f4: if(!read(0x80420538,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007b9f8;break;
 case 0x8007b9f8: if(!emit(0x8007b9f8,c.f[6]))return false;c.pc=0x8007b9fc;break;
 case 0x8007b9fc: if(!emit(0x8007b9fc,c.f[0]))return false;c.pc=0x8007ba00;break;
 case 0x8007ba00: if(!read(0x80420548,c.bits))return false;c.f[6]=scalar(c.bits);c.pc=0x8007ba04;break;
 case 0x8007ba04: if(!emit(0x8007ba04,c.f[6]))return false;c.pc=0x8007ba08;break;
 case 0x8007ba08: if(!emit(0x8007ba08,c.f[4]))return false;c.pc=0x8007ba0c;break;
 case 0x8007ba0c: if(!emit(0x8007ba0c,c.f[3]))return false;c.pc=0x8007ba10;break;
 case 0x8007ba10: if(!read(0x80420538,c.bits))return false;c.f[4]=scalar(c.bits);c.pc=0x8007ba14;break;
 case 0x8007ba14: if(!emit(0x8007ba14,c.f[4]))return false;c.pc=0x8007ba18;break;
 case 0x8007ba18: if(!emit(0x8007ba18,c.f[1]))return false;c.pc=0x8007ba1c;break;
 case 0x8007ba1c: if(!read(0x80420548,c.bits))return false;c.f[1]=scalar(c.bits);c.pc=0x8007ba20;break;
 case 0x8007ba20: if(!emit(0x8007ba20,c.f[1]))return false;c.pc=0x8007ba24;break;
 case 0x8007ba24: if(!emit(0x8007ba24,c.f[5]))return false;c.pc=0x8007ba28;break;
 case 0x8007ba28: if(!emit(0x8007ba28,c.f[7]))return false;c.pc=0x8007ba2c;break;
 case 0x8007ba2c: if(!read(0x80420538,c.bits))return false;c.f[1]=scalar(c.bits);c.pc=0x8007ba30;break;
 case 0x8007ba30: if(!emit(0x8007ba30,c.f[1]))return false;c.pc=0x8007ba34;break;
 case 0x8007ba34: if(!emit(0x8007ba34,c.f[0]))return false;c.pc=0x8007ba38;break;
 case 0x8007ba38: if(!emit(0x8007ba38,c.f[0]))return false;c.pc=0x8007ba3c;break;
 case 0x8007ba3c: if(!emit(0x8007ba3c,c.f[2]))return false;c.pc=0x8007ba40;break;
 case 0x8007ba40: if(!emit(0x8007ba40,c.f[7]))return false;c.pc=0x8007ba44;break;
 case 0x8007ba44: if(!read(0x80420538,c.bits))return false;c.f[1]=scalar(c.bits);c.pc=0x8007ba48;break;
 case 0x8007ba48: if(!emit(0x8007ba48,c.f[1]))return false;c.pc=0x8007ba4c;break;
 case 0x8007ba4c: if(!read(0x80420548,c.bits))return false;c.f[1]=scalar(c.bits);c.pc=0x8007ba50;break;
 case 0x8007ba50: if(!emit(0x8007ba50,c.f[1]))return false;c.pc=0x8007ba54;break;
 case 0x8007ba54: if(!emit(0x8007ba54,c.f[0]))return false;c.pc=0x8007ba58;break;
 case 0x8007ba58: if(!emit(0x8007ba58,c.f[2]))return false;c.pc=0x8007ba5c;break;
 case 0x8007ba5c: if(!emit(0x8007ba5c,c.f[3]))return false;c.pc=0x8007ba60;break;
 case 0x8007ba60: if(!read(0x80420538,c.bits))return false;c.f[1]=scalar(c.bits);c.pc=0x8007ba64;break;
 case 0x8007ba64: if(!emit(0x8007ba64,c.f[1]))return false;c.pc=0x8007ba68;break;
 case 0x8007ba68: if(!read(0x80420548,c.bits))return false;c.f[1]=scalar(c.bits);c.pc=0x8007ba6c;break;
 case 0x8007ba6c: if(!emit(0x8007ba6c,c.f[1]))return false;c.pc=0x8007ba70;break;
 case 0x8007ba70: if(!read(0x80420548,c.bits))return false;c.f[1]=scalar(c.bits);c.pc=0x8007ba74;break;
 case 0x8007ba74: if(!emit(0x8007ba74,c.f[1]))return false;c.pc=0x8007ba78;break;
 case 0x8007ba78: if(!emit(0x8007ba78,c.f[5]))return false;c.pc=0x8007ba7c;break;
 case 0x8007ba7c: if(!emit(0x8007ba7c,c.f[3]))return false;c.pc=0x8007ba80;break;
 case 0x8007ba80: if(!read(0x80420538,c.bits))return false;c.f[1]=scalar(c.bits);c.pc=0x8007ba84;break;
 case 0x8007ba84: if(!emit(0x8007ba84,c.f[1]))return false;c.pc=0x8007ba88;break;
 case 0x8007ba88: if(!emit(0x8007ba88,c.f[0]))return false;c.pc=0x8007ba8c;break;
 case 0x8007ba8c: if(!read(0x80420548,c.bits))return false;c.f[0]=scalar(c.bits);c.pc=0x8007ba90;break;
 case 0x8007ba90: if(!emit(0x8007ba90,c.f[0]))return false;c.pc=0x8007ba94;break;
 case 0x8007ba94:c.done=true;c.pending=0;return true;
 default:return false;
 }
 }
 return false;
}
}
