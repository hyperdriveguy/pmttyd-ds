#include "animation_source_render.hpp"
#include <initializer_list>
namespace ttyd {
namespace {
using A=AnimationRenderArgument;using K=AnimationRenderKind;
A imm(uint32_t x){return {K::Scalar,x};}A guest(uint32_t x){return {K::Guest,x};}
A mat(uint32_t x){return {K::Matrix,x};}A vec(uint32_t x){return {K::Vector,x};}A color(uint32_t x){return {K::Color,x};}
bool same(const AnimationSourceRenderServices& a,const AnimationSourceRenderServices& b){return a.user==b.user && a.word==b.word && a.byte==b.byte && a.half==b.half && a.children.user==b.children.user && a.children.call==b.children.call && a.children.fifo==b.children.fifo;}
}
// Fixed renderProc8003c884. W=current work,P=pose,D=MODEL,H=shape,B=draw;
// guest offsets below are hex,strides decimal. Calls carry durable tagged local
// identities,never fabricated guest stack addresses. Only shape is consumed.
bool resumeAnimationSourceRender(uint32_t shape,const AnimationSourceRenderServices& s,AnimationSourceRenderCursor& c,unsigned visits){
 if(!c.active){c=AnimationSourceRenderCursor{};c.active=true;c.input=shape;c.owner=s;}
 else if(shape!=c.input || !same(s,c.owner))return false;
 auto word=[&](uint32_t address,uint32_t& out){auto x=s.word?s.word(s.user,address):std::nullopt;if(!x)return false;out=*x;return true;};
 auto byte=[&](uint32_t address,uint32_t& out,bool sign=false){auto x=s.byte?s.byte(s.user,address):std::nullopt;if(!x)return false;out=sign?uint32_t(int32_t(int8_t(*x))):uint32_t(*x);return true;};
 // Capture complete ABI/return storage once per child; its false may follow
 // retained effects. Earlier parent reads/GPU/FIFO writes cannot be replayed.
 auto call=[&](uint32_t function,std::initializer_list<A> args){if(!c.pending){c.request={};c.request.function=function;c.request.count=args.size();unsigned i=0;for(auto x:args)c.request.arguments[i++]=x;c.result=0;c.pending=true;}if(!s.children.call || !s.children.call(s.children.user,c.request,c.locals,c.result))return false;c.pending=false;return true;};
 auto done=[&](){c.active=false;return true;};
 for(unsigned visit=0;visit<visits;++visit)switch(c.phase){
 // Shape=-1 is the ONLY early empty sentinel. Otherwise current camera,
 // modeling8041e8a0,view Concat,position/normal upload and matrix0 selection
 // precede shape admission. InvXpose status is ignored; optional normal may
 // remain absent/stale,not an invented identity. SDK functions are8029dd64/
 // df28;GX position/normal/current loads are802ba2fc/a34c/a39c.
 case 0:if(c.input==0xffffffffu)return done();c.phase=1;break;
 case 1:if(!call(0x8000f2e4,{}))return false;c.camera=c.result;++c.phase;break;
 case 2:if(!word(0x8041e8a0,c.modeling))return false;++c.phase;break;
 case 3:if(!call(0x8029dd64,{guest(c.camera+0x11c),guest(c.modeling),mat(0)}))return false;++c.phase;break;
 case 4:if(!call(0x802ba2fc,{mat(0),imm(0)}))return false;++c.phase;break;
 case 5:if(!call(0x8029df28,{mat(0),mat(1)}))return false;++c.phase;break;
 case 6:if(!call(0x802ba34c,{mat(1),imm(0)}))return false;++c.phase;break;
 case 7:if(!call(0x802ba39c,{imm(0)}))return false;++c.phase;break;
 // Root80415dc0 ->W20 MODEL;D14c shapes stride 168,D19c draws stride 108.
 // H98 first draw,H9c signed count,Ha0 pass,Ha4 cull-table index. Shape
 // position/normal/color bases40/48/50 expand to12/12/4-byte row offsets.
 case 8:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 9:if(!word(c.w+0x20,c.d))return false;++c.phase;break;
 case 10:if(!word(c.d+0x14c,c.h))return false;c.h+=168*c.input;++c.phase;break;
 case 11:if(!word(c.d+0x19c,c.b))return false;++c.phase;break;
 case 12:if(!word(c.h+0x98,c.v))return false;c.b+=108*c.v;++c.phase;break;
 case 13:if(!word(c.h+0xa0,c.mode))return false;++c.phase;break;
 case 14:if(!word(c.h+0x9c,c.drawCount))return false;++c.phase;break;
 case 15:case 16:case 17:if(!word(c.h+0x40+8*(c.phase-15),c.byteBases[c.phase-15]))return false;c.byteBases[c.phase-15]*=c.phase==17?4:12;++c.phase;break;
 // Admission:shape mode 0 with effective pass 2,or exact pass,or either
 // pose alpha bytef3/f7 !=255. Reads short-circuit; mode0 can reload We0.
 // Cull table80415dc8 contains2/1/3/0;no source index clamp before request.
 case 18:c.phase=c.mode==0?19:20;break;
 case 19:if(!word(c.w+0xe0,c.v))return false;c.phase=c.v==2?25:20;break;
 case 20:if(!word(c.w+0xe0,c.v))return false;c.phase=c.v==c.mode?25:21;break;
 case 21:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 22:if(!byte(c.p+0xf3,c.v))return false;c.phase=c.v!=255?25:23;break;
 case 23:if(!byte(c.p+0xf7,c.v))return false;if(c.v==255)return done();c.phase=25;break;
 case 25:if(!word(c.h+0xa4,c.v))return false;++c.phase;break;
 case 26:if(!word(0x80415dc8+4*c.v,c.v))return false;++c.phase;break;
 case 27:if(!call(0x802b5c8c,{imm(c.v)}))return false;++c.phase;break;
 // Original UV capture order is 58,60,68,70,80,78,88,90, interleaved with W/D.
 case 28:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 29:if(!word(c.h+0x58,c.uv[0]))return false;++c.phase;break;
 case 30:if(!word(c.w+0x20,c.d))return false;++c.phase;break;
 case 31:if(!word(c.h+0x60,c.uv[1]))return false;++c.phase;break;
 case 32:if(!word(c.h+0x68,c.uv[2]))return false;++c.phase;break;
 case 33:if(!word(c.d+0x18c,c.base))return false;++c.phase;break;
 case 34:if(!word(c.h+0x70,c.uv[3]))return false;++c.phase;break;
 case 35:if(!word(c.h+0x80,c.uv[5]))return false;++c.phase;break;
 case 36:if(!word(c.h+0x78,c.uv[4]))return false;++c.phase;break;
 case 37:if(!word(c.h+0x88,c.uv[6]))return false;++c.phase;break;
 case 38:if(!word(c.h+0x90,c.uv[7]))return false;for(auto& u:c.uv)u=c.base+8*u;c.phase=90;break;
 case 90:if(int32_t(c.draw)>=int32_t(c.drawCount))return done();c.found1=c.found2=false;c.phase=100;break;
 // Each draw captures default slots before Pe4 guard. Zero admits seed
 // vector802c103c,MultVec in-place,then separate vector1 for Light800208c0.
 // Nonzero Pe4 skips defaults only; later override/rebind/arrays still run.
 case 100:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 101:if(!word(c.b,c.defaultSlots))return false;++c.phase;break;
 case 102:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 103:if(!word(c.p+0xe4,c.v))return false;c.phase=c.v?120:104;break;
 case 104:if(!word(0x8041e8a0,c.modeling))return false;++c.phase;break;
 case 105:case 106:case 107:if(!word(0x802c103c+4*(c.phase-105),c.locals.vectors[0][c.phase-105]))return false;++c.phase;break;
 case 108:if(!call(0x8029e6a4,{guest(c.modeling),vec(0),vec(0)}))return false;c.locals.vectors[1]=c.locals.vectors[0];++c.phase;break;
 case 109:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 110:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 111:if(!word(c.p+0xec,c.v))return false;++c.phase;break;
 case 112:if(!call(0x800208c0,{imm(c.v),vec(1)}))return false;c.phase=200;break;
 // Primary override admission and first-kind captures.
 // ENTIRE P4 word gates both primary scans. Kind1 has priority even with
 // null override;kind2 only follows no kind1 match. Wcc/d0 matrices,W30/34
 // objects,W24/28 smartTexObj operands are separate fresh captures per kind.
 case 120:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 121:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 122:if(!word(c.p+4,c.v))return false;c.phase=c.v?123:420;break;
 case 123:if(!word(c.w+0x20,c.d))return false;++c.phase;break;
 case 124:if(!word(c.w+0xcc,c.matrix))return false;++c.phase;break;
 case 125:if(!word(c.w+0x24,c.objectValue))return false;++c.phase;break;
 case 126:if(!word(c.w+0x30,c.object))return false;++c.phase;break;
 case 127:if(!word(c.d+0x198,c.textureData))return false;++c.phase;break;
 case 128:if(!word(c.d+0x134,c.textureCount))return false;++c.phase;break;
 case 129:if(!word(c.p+0x6c,c.states))return false;++c.phase;break;
 case 130:if(!word(c.d+0x194,c.textures))return false;++c.phase;break;
 case 131:if(!word(c.b,c.slots))return false;c.scan=0;c.scanKind=1;c.scanReturn=132;c.phase=300;break;
 case 132:c.phase=c.found1?350:140;break;
 case 140:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 141:if(!word(c.b,c.slots))return false;++c.phase;break;
 case 142:if(!word(c.w+0x20,c.d))return false;++c.phase;break;
 case 143:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 144:if(!word(c.w+0xd0,c.matrix))return false;++c.phase;break;
 case 145:if(!word(c.w+0x28,c.objectValue))return false;++c.phase;break;
 case 146:if(!word(c.w+0x34,c.object))return false;++c.phase;break;
 case 147:if(!word(c.d+0x198,c.textureData))return false;++c.phase;break;
 case 148:if(!word(c.d+0x134,c.textureCount))return false;++c.phase;break;
 case 149:if(!word(c.p+0x6c,c.states))return false;++c.phase;break;
 case 150:if(!word(c.d+0x194,c.textures))return false;c.scan=0;c.scanKind=2;c.scanReturn=350;c.phase=300;break;
 // Default texture ABI capture is deliberately not a resource snapshot.
 // Default Texture8003dcfc ABI uses current source tables/pose/file refs.
 // Then C2(Pf4),C1(Pf0),flagsPe8 ->LastStageBlend80021860. Fog800217c8
 // compares NEW camera4/current getters;TEV80021b80 uses captured default slots.
 case 200:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 201:if(!word(c.b,c.slots))return false;++c.phase;break;
 case 202:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 203:if(!word(c.w+0x20,c.d))return false;++c.phase;break;
 case 204:if(!word(c.p+0x10,c.index))return false;++c.phase;break;
 case 205:if(!word(c.w,c.base))return false;++c.phase;break;
 case 206:if(!word(c.w+8,c.textureTable))return false;++c.phase;break;
 case 207:if(!word(c.d+0x194,c.textures))return false;++c.phase;break;
 case 208:if(!word(c.base+16*c.index+12,c.v))return false;c.textureTable+=12*c.v;++c.phase;break;
 case 209:if(!word(c.p+0x6c,c.states))return false;++c.phase;break;
 case 210:if(!word(c.d+0x134,c.textureCount))return false;++c.phase;break;
 case 211:if(!word(c.d+0x198,c.textureData))return false;++c.phase;break;
 case 212:if(!call(0x8003dcfc,{imm(c.slots),guest(c.b+0x10),guest(c.textures),guest(c.states),imm(c.textureCount),guest(c.textureData),guest(c.textureTable)}))return false;++c.phase;break;
 case 213:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 214:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 215:if(!word(c.p+0xf4,c.locals.colors[1]))return false;++c.phase;break;
 case 216:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 217:if(!word(c.p+0xf0,c.locals.colors[0]))return false;++c.phase;break;
 case 218:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 219:if(!word(c.p+0xe8,c.v))return false;++c.phase;break;
 case 220:if(!call(0x80021860,{imm(c.v),color(0),color(1)}))return false;++c.phase;break;
 case 221:if(!call(0x8000f2fc,{imm(4)}))return false;c.camera=c.result;++c.phase;break;
 case 222:if(!call(0x8000f2e4,{}))return false;c.phase=c.camera==c.result?223:224;break;
 case 223:if(!call(0x800217c8,{}))return false;++c.phase;break;
 case 224:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 225:if(!word(c.b+8,c.mode))return false;++c.phase;break;
 case 226:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 227:if(!word(0x8041e8a0,c.modeling))return false;++c.phase;break;
 case 228:if(!word(c.p+0xe8,c.v))return false;++c.phase;break;
 case 229:if(!call(0x80021b80,{imm(c.defaultSlots),imm(c.mode),imm(c.v),guest(c.modeling)}))return false;c.phase=120;break;
 // Shared ordered reverse descriptor scan; kind3 is independent kind1 rebind.
 // Reverse slot IDs at B10+4*(N-j-1),base records8 bytes,state24 bytes,
 // texture-data descriptors64 bytes. UNSIGNED frame byte extends base index;
 // signed out-of-range calls printf8026a2a8(format802c1078,FRAME byte),then 0.
 // A first matching kind ends the scan even when no object was bound.
 case 300:if(int32_t(c.scan)>=int32_t(c.slots)){c.phase=c.scanReturn;break;}++c.phase;break;
 case 301:if(!word(c.b+0x10+4*(c.slots-c.scan-1),c.index))return false;++c.phase;break;
 case 302:if(!word(c.textures+8*c.index,c.v))return false;++c.phase;break;
 case 303:if(!byte(c.states+24*c.index,c.frame))return false;c.index=c.v+c.frame;c.phase=int32_t(c.index)<0 || int32_t(c.index)>=int32_t(c.textureCount)?304:305;break;
 case 304:if(!call(0x8026a2a8,{guest(0x802c1078),imm(c.frame)}))return false;c.index=0;++c.phase;break;
 case 305:if(!word(c.textureData+64*c.index+8,c.v))return false;if(c.v!=(c.scanKind==2?2u:1u)){++c.scan;c.phase=300;break;}c.phase=c.object?306:310;break;
 // smartTexObj8002f33c RETURN feeds GXLoadTexObj802b7880. Tex matrix ID
 // 30+3*j indexes 3-row GX slots;60=identity,125=post-identity. LoadTexMtxImm
 //802ba3d0 type1=2x4;TexCoordGen2 802b4e04 uses type1,source4+j,normalize0.
 case 306:if(!call(0x8002f33c,{guest(c.object),imm(c.objectValue)}))return false;c.t=c.result;++c.phase;break;
 case 307:if(!call(0x802b7880,{guest(c.t),imm(c.scan)}))return false;c.phase=c.matrix?308:309;break;
 case 308:if(!call(0x802ba3d0,{guest(c.matrix),imm(30+3*c.scan),imm(1)}))return false;++c.phase;break;
 case 309:if(!call(0x802b4e04,{imm(c.scan),imm(1),imm(c.scan+4),imm(c.matrix?30+3*c.scan:60),imm(0),imm(125)}))return false;++c.phase;break;
 case 310:if(c.scanKind==1)c.found1=true;if(c.scanKind==2)c.found2=true;c.phase=c.scanReturn;break;
 // Found kind with zero suppression flag and B8==0 requests TEV alpha
 // 802b89a4(stage0,ZERO7,ZERO7,ZERO7,RASA5). Not part of materialProc.
 case 350:if(!c.found1 && !c.found2){c.phase=420;break;}++c.phase;break;
 case 351:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 352:if(!word(c.w+(c.found1?0xd8:0xdc),c.v))return false;c.phase=c.v?370:353;break;
 case 353:if(!word(c.b+8,c.v))return false;c.phase=c.v?370:354;break;
 case 354:if(!call(0x802b89a4,{imm(0),imm(7),imm(7),imm(7),imm(5)}))return false;c.phase=370;break;
 // Extra object W38:smart helper return is DISCARDED,unlike primaries.
 // Non-XLU We0!=3 requests Z/Blend/ZComp/Alpha 802b93d8/932c/940c/8d60:
 // LEQUAL3,write1;blendNONE0 factorsSRCALPHA4/INV5;after-texture0;
 // GEQUAL6 threshold128 OR1 NEVER0. No state restoration follows.
 case 370:if(!word(c.b+8,c.v))return false;c.phase=c.v?420:371;break;
 case 371:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 372:if(!word(c.w+0x38,c.extra))return false;c.phase=c.extra?373:420;break;
 case 373:if(!word(c.w+0x2c,c.objectValue))return false;++c.phase;break;
 case 374:if(!call(0x8002f33c,{guest(c.extra),imm(c.objectValue)}))return false;++c.phase;break;
 case 375:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 376:if(!word(c.w+0xe0,c.v))return false;c.phase=c.v==3?381:377;break;
 case 377:if(!call(0x802b93d8,{imm(1),imm(3),imm(1)}))return false;++c.phase;break;
 case 378:if(!call(0x802b932c,{imm(0),imm(4),imm(5),imm(0)}))return false;++c.phase;break;
 case 379:if(!call(0x802b940c,{imm(0)}))return false;++c.phase;break;
 case 380:if(!call(0x802b8d60,{imm(6),imm(128),imm(1),imm(0),imm(0)}))return false;++c.phase;break;
 // Rebind CURRENT selected base object/matrix then extra object/matrixd4.
 // Reached matrices have no null guard. Extra TEV hardcodes 2 stages/mode12.
 case 381:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 382:if(!word(c.w+(c.found1?0x30:0x34),c.object))return false;c.phase=c.object?383:388;break;
 case 383:if(!call(0x802b7880,{guest(c.object),imm(0)}))return false;++c.phase;break;
 case 384:if(!call(0x802b4e04,{imm(0),imm(1),imm(4),imm(30),imm(0),imm(125)}))return false;++c.phase;break;
 case 385:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 386:if(!word(c.w+(c.found1?0xcc:0xd0),c.matrix))return false;++c.phase;break;
 case 387:if(!call(0x802ba3d0,{guest(c.matrix),imm(30),imm(1)}))return false;++c.phase;break;
 case 388:if(!call(0x802b7880,{guest(c.extra),imm(1)}))return false;++c.phase;break;
 case 389:if(!call(0x802b4e04,{imm(1),imm(1),imm(4),imm(33),imm(0),imm(125)}))return false;++c.phase;break;
 case 390:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 391:if(!word(c.w+0xd4,c.matrix))return false;++c.phase;break;
 case 392:if(!call(0x802ba3d0,{guest(c.matrix),imm(33),imm(1)}))return false;++c.phase;break;
 case 393:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 394:if(!word(0x8041e8a0,c.modeling))return false;++c.phase;break;
 case 395:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 396:if(!word(c.p+0xe8,c.v))return false;++c.phase;break;
 case 397:if(!call(0x80021b80,{imm(2),imm(12),imm(c.v),guest(c.modeling)}))return false;c.phase=420;break;
 // Independent flag10 rebind is reached even when P4 disabled both primaries.
 // Independent pose flag10 kind1 rebind runs even when P4 skipped primaries.
 // It needs nonnull W30/Wcc,then performs a NEW scan rather than cached found.
 case 420:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 421:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 422:if(!word(c.p,c.v))return false;c.phase=c.v&0x10?423:500;break;
 case 423:if(!word(c.w+0x30,c.object))return false;c.phase=c.object?424:500;break;
 case 424:if(!word(c.w+0xcc,c.matrix))return false;c.phase=c.matrix?425:500;break;
 case 425:if(!word(c.w+0x20,c.d))return false;++c.phase;break;
 case 426:if(!word(c.w+0x24,c.objectValue))return false;++c.phase;break;
 case 427:if(!word(c.d+0x198,c.textureData))return false;++c.phase;break;
 case 428:if(!word(c.d+0x134,c.textureCount))return false;++c.phase;break;
 case 429:if(!word(c.p+0x6c,c.states))return false;++c.phase;break;
 case 430:if(!word(c.d+0x194,c.textures))return false;++c.phase;break;
 case 431:if(!word(c.b,c.slots))return false;c.scan=0;c.scanKind=3;c.scanReturn=500;c.phase=300;break;
 // Per-draw run records: D150+8*B38,countB3c; offsetsB40/44/48 are
 // index-stream starts,not shape array bases. We8 chooses shared Wf8/fc or
 // current P4c/54 arrays; all address/length arithmetic wraps32.
 case 500:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 501:if(!word(c.b+0x38,c.v))return false;++c.phase;break;
 case 502:if(!word(c.w+0x20,c.d))return false;++c.phase;break;
 case 503:if(!word(c.w+0xe8,c.selected))return false;++c.phase;break;
 case 504:if(!word(c.d+0x150,c.run))return false;c.run+=8*c.v;++c.phase;break;
 case 505:if(!word(c.d+0x164,c.arrayBases[2]))return false;c.arrayBases[2]+=c.byteBases[2];++c.phase;break;
 case 506:if(!word(c.b,c.slots))return false;++c.phase;break;
 case 507:if(!word(c.b+0x3c,c.runCount))return false;++c.phase;break;
 case 508:case 509:case 510:if(!word(c.b+0x40+4*(c.phase-508),c.offsets[c.phase-508]))return false;++c.phase;break;
 case 511:c.phase=c.selected?512:514;break;
 case 512:if(!word(c.w+0xfc,c.arrayBases[1]))return false;++c.phase;break;
 case 513:if(!word(c.w+0xf8,c.arrayBases[0]))return false;c.phase=517;break;
 case 514:if(!word(c.w+0x18,c.p))return false;++c.phase;break;
 case 515:if(!word(c.p+0x4c,c.arrayBases[0]))return false;++c.phase;break;
 case 516:if(!word(c.p+0x54,c.arrayBases[1]))return false;++c.phase;break;
 case 517:c.arrayBases[0]+=c.byteBases[0];c.arrayBases[1]+=c.byteBases[1];++c.phase;break;
 // GX802b4520 ClearVtxDesc,3c08 SetVtxDesc,4558 SetVtxAttrFmt,
 // 4db0 SetArray. Attr9/10/11=position/normal/color0;13+=TEX coordinates.
 // Desc3=index16;types4=float32,5=RGBA8;strides12/12/4 and UV8 bytes.
 case 518:if(!call(0x802b4520,{}))return false;c.attribute=0;++c.phase;break;
 case 519:if(!call(0x802b3c08,{imm(9+c.attribute),imm(3)}))return false;++c.phase;break;
 case 520:if(!call(0x802b4558,{imm(0),imm(9+c.attribute),imm(c.attribute==1?0:1),imm(c.attribute==2?5:4),imm(0)}))return false;++c.phase;break;
 case 521:if(!call(0x802b4db0,{imm(9+c.attribute),guest(c.arrayBases[c.attribute]),imm(c.attribute==2?4:12)}))return false;if(++c.attribute<3)c.phase=519;else{c.attribute=0;c.phase=522;}break;
 case 522:if(int32_t(c.attribute)>=int32_t(c.slots)){c.runIndex=0;c.phase=590;break;}++c.phase;break;
 case 523:if(!call(0x802b3c08,{imm(13+c.attribute),imm(3)}))return false;++c.phase;break;
 case 524:if(!call(0x802b4558,{imm(0),imm(13+c.attribute),imm(1),imm(4),imm(0)}))return false;++c.phase;break;
 // SIGNED UV selector uses reverse order except one-slot case. The native
 // local table has 8 entries:escaped captured selector holds at 526. This is an
 // implementation frontier,not an original bounds guard or repairable reread.
 case 525:if(!byte(c.b+0x30+(c.slots==1?0:c.slots-c.attribute-1),c.selector,true))return false;++c.phase;break;
 case 526:if(c.selector>=8)return false;if(!call(0x802b4db0,{imm(13+c.attribute),guest(c.uv[c.selector]),imm(8)}))return false;++c.attribute;c.phase=522;break;
 case 590:if(int32_t(c.runIndex)>=int32_t(c.runCount)){++c.draw;c.b+=108;c.phase=90;break;}c.phase=c.slots==1?600:650;break;
 // Single UV streams are fixed before Begin, then halfword reads/stores alternate.
 // GXBegin802b5a6c primitivea0=TRIANGLE_FAN,format0,count LOW16.
 // One UV:four streams are fixed BEFORE Begin;lhz at+2 reads low BE half
 // of 4-byte index slots. Alternate each read with one sth to FIFO cc008000.
 // Zero count still calls Begin;held FIFO keeps the already loaded halfword.
 case 600:if(!byte(c.b+0x30,c.selector,true))return false;++c.phase;break;
 case 601:if(!word(c.run,c.first))return false;++c.phase;break;
 case 602:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 603:if(!word(c.b+0x4c+4*c.selector,c.uvFirst))return false;++c.phase;break;
 case 604:if(!word(c.w+0x20,c.d))return false;++c.phase;break;
 case 605:if(!word(c.run+4,c.count))return false;c.count&=0xffff;++c.phase;break;
 case 606:if(!word(c.d+0x158,c.streams[0]))return false;++c.phase;break;
 case 607:if(!word(c.d+0x160,c.streams[1]))return false;++c.phase;break;
 case 608:if(!word(c.d+0x168,c.streams[2]))return false;++c.phase;break;
 case 609:if(!word(c.d+0x16c+4*c.selector,c.streams[3]))return false;for(unsigned i=0;i<4;++i)c.streams[i]+=4*(c.first+(i==3?c.uvFirst:c.offsets[i]))+2;++c.phase;break;
 case 610:if(!call(0x802b5a6c,{imm(0xa0),imm(0),imm(c.count)}))return false;c.vertex=c.lane=0;c.phase=620;break;
 case 620:if(c.vertex>=c.count){c.phase=700;break;}++c.phase;break;
 case 621:{auto x=s.half?s.half(s.user,c.streams[c.lane]+4*c.vertex):std::nullopt;if(!x)return false;c.fifoValue=*x;++c.phase;break;}
 case 622:if(!s.children.fifo || !s.children.fifo(s.children.user,0xcc008000,c.fifoValue))return false;if(++c.lane==4){c.lane=0;++c.vertex;c.phase=620;}else c.phase=621;break;
 // Multi UV captures W once after Begin, even with zero vertices.
 // Multi UV captures W once AFTER Begin,even at zero vertices. D reloads
 // through that W per coordinate;root changes do not redirect it. Common
 // position/normal/color use halfword loads;UV uses full word then LOW16.
 case 650:if(!word(0x80415dc0,c.w))return false;++c.phase;break;
 case 651:if(!word(c.run,c.first))return false;++c.phase;break;
 case 652:if(!word(c.w+0x20,c.d))return false;++c.phase;break;
 case 653:if(!word(c.run+4,c.count))return false;c.count&=0xffff;++c.phase;break;
 case 654:if(!word(c.d+0x158,c.streams[0]))return false;++c.phase;break;
 case 655:if(!word(c.d+0x160,c.streams[1]))return false;++c.phase;break;
 case 656:if(!word(c.d+0x168,c.streams[2]))return false;for(unsigned i=0;i<3;++i)c.streams[i]+=4*(c.first+c.offsets[i])+2;++c.phase;break;
 case 657:if(!call(0x802b5a6c,{imm(0xa0),imm(0),imm(c.count)}))return false;++c.phase;break;
 case 658:if(!word(0x80415dc0,c.postBeginWork))return false;c.vertex=c.lane=0;++c.phase;break;
 case 659:if(c.vertex>=c.count){c.phase=700;break;}++c.phase;break;
 case 660:{auto x=s.half?s.half(s.user,c.streams[c.lane]+4*c.vertex):std::nullopt;if(!x)return false;c.fifoValue=*x;++c.phase;break;}
 case 661:if(!s.children.fifo || !s.children.fifo(s.children.user,0xcc008000,c.fifoValue))return false;if(++c.lane<3)c.phase=660;else{c.coordinate=0;c.phase=662;}break;
 case 662:if(int32_t(c.coordinate)>=int32_t(c.slots)){++c.vertex;c.lane=0;c.phase=659;break;}
  // Original four-coordinate bulk lanes1/2/3 read selector before D; bulk
  // lane0 and every remainder read D before selector. No merged snapshot.
  c.phase=c.coordinate<(c.slots&~3u) && (c.coordinate&3u)?665:663;break;
 case 663:if(!word(c.postBeginWork+0x20,c.d))return false;++c.phase;break;
 case 664:if(!byte(c.b+0x30+c.slots-c.coordinate-1,c.selector,true))return false;c.phase=667;break;
 case 665:if(!byte(c.b+0x30+c.slots-c.coordinate-1,c.selector,true))return false;++c.phase;break;
 case 666:if(!word(c.postBeginWork+0x20,c.d))return false;++c.phase;break;
 case 667:if(!word(c.b+0x4c+4*c.selector,c.uvFirst))return false;++c.phase;break;
 case 668:if(!word(c.d+0x16c+4*c.selector,c.base))return false;++c.phase;break;
 case 669:if(!word(c.base+4*(c.first+c.uvFirst+c.vertex),c.v))return false;c.fifoValue=uint16_t(c.v);++c.phase;break;
 case 670:if(!s.children.fifo || !s.children.fifo(s.children.user,0xcc008000,c.fifoValue))return false;++c.coordinate;c.phase=662;break;
 // 8-byte runs then 108-byte draw records. No GXEnd,state restore,stack pop
 // or whole-lease completion is invented;outer group/Inner still own teardown.
 case 700:c.run+=8;++c.runIndex;c.phase=590;break;
 default:return false;
 }
 return false;
}
}
