#include "animation_scratch_memory.hpp"
#include "animation_startup_process.hpp"
#include "animation_draw_traversal.hpp"
#include "animation_texture.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
float real(EventAddressSpace& m,uint32_t a){const uint32_t v=word(m,a);float f;std::memcpy(&f,&v,4);return f;}
void real(EventAddressSpace& m,uint32_t a,float f){uint32_t v;std::memcpy(&v,&f,4);assert(m.writeWord(a,v));}
uint8_t byte(EventAddressSpace& m,uint32_t a){uint8_t v=0;assert(m.readByte(a,v));return v;}
void close(float a,float b){assert(std::isfinite(a) && std::isfinite(b));if(std::fabs(a-b)>0.0001f){std::fprintf(stderr,"mismatch %.9g vs %.9g\n",double(a),double(b));assert(false);}}
void compare(const ActorAnimationSample& a,const ActorAnimationSample& b){
 assert(a.positions.size()==b.positions.size() && a.normals.size()==b.normals.size() && a.nodes.size()==b.nodes.size());
 for(size_t i=0;i<a.positions.size();++i)for(unsigned j=0;j<3;++j)close(a.positions[i][j],b.positions[i][j]);
 for(size_t i=0;i<a.normals.size();++i)for(unsigned j=0;j<3;++j)close(a.normals[i][j],b.normals[i][j]);
 for(size_t i=0;i<a.nodes.size();++i)close(a.nodes[i],b.nodes[i]);
 assert(a.textures==b.textures && a.visibility==b.visibility);
 for(size_t i=0;i<a.surfaces.size();++i)for(unsigned j=0;j<5;++j)close(a.surfaces[i].transform[j],b.surfaces[i].transform[j]);
}
}
void runAnimationScratchMemoryFixture(const char* root){
 auto pool=std::make_shared<BattlePosePool>();auto files=NativeFileCache::create();assert(stagePrologueAnimationFiles(*files,root));
 bool yield=true;assert(files->yieldProvider(&yield,[](void* u){return *static_cast<bool*>(u);}));
 EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
 auto process=AnimationStartupProcess::cold(files,pool,platform,(std::string(root)+"/animation-boot/memory.pmevt").c_str(),0x81000000,0x82000000,12*1024*1024);assert(process && process->advance());auto& m=process->vm().memory;
 assert(m.registerRam(0x81800000,std::vector<uint8_t>(256)) && m.registerRam(0x800000f8,{0x09,0xa7,0xec,0x80}));
 const char* stems[]={"field-player-a_mario","field-player-a_mario_r","field-player-e_mario","trio-attack-mario","field-player-c_pkr","field-native-c_kuri_h"};
 for(unsigned h=0;h<6;++h){
  const auto prefix=std::string(root)+"/"+stems[h];auto r=loadBattlePoseResource((prefix+".pose-directory.bin").c_str(),(prefix+".pmanim").c_str());assert(r);
  for(unsigned i=0;i<=r->modelName.size();++i)assert(m.writeByte(0x81800000+i,i<r->modelName.size()?uint8_t(r->modelName[i]):0));
  assert(process->enter(0x81800000,0,r)==int32_t(h));auto* p=pool->pose(h);assert(p && p->sourceBuffers && !p->timestamp());
  BattlePosePool detached;auto d=detached.enter(r,0,{});assert(d);
  // Forward, same-key reverse fraction, backwards key and reset use the same
  // real authoring data. Native vectors never supply the source scratch input.
  for(uint64_t ms:{0ULL,17ULL,83ULL,167ULL,500ULL,333ULL,317ULL,0ULL}){assert(pool->main(h,{ms,ms}) && detached.main(*d,{ms,ms}));compare(p->shape,detached.pose(*d)->shape);}
 }
 auto* p=pool->pose(4);assert(pool->selectName(4,"PKR_W_1",true,{}) && pool->main(4,{117,117}));
 const auto modelReferences=pool->models()[p->word(0x10)].references();assert(!pool->release(4) && pool->models()[p->word(0x10)].references()==modelReferences && p->sourceBuffers);
 const uint32_t model=word(m,word(m,pool->models()[p->word(0x10)].file()+0xa0)),dir=word(m,model+0x1ac),t=word(m,dir+64*p->word(0x14)+0x3c),control=word(m,t+0x24),keys=word(m,t+0x28);
 const uint32_t posCache=p->word(0x48),posOut=p->word(0x4c),nodeOut=p->word(0x64),tex=p->word(0x6c);
 assert(posCache!=posOut && p->word(0x58)==p->word(0x5c) && tex==p->word(0x68));
 const float original=real(m,posCache),outputBefore=real(m,posOut),fraction=p->scalar(0x30);real(m,posCache,original+7);
 assert(pool->main(4,{117,117}) && p->scalar(0x30)==fraction);close(real(m,posOut),p->shape.positions[0][0]);close(real(m,posOut),outputBefore+7);
 const auto sampled=p->shape.positions[0];real(m,posOut,sampled[0]+9);
 const auto live=animationOutputVector(*p,false,0);assert(live && (*live)[0]==sampled[0]+9 && p->shape.positions[0]==sampled);
 real(m,nodeOut,123);assert(animationOutputNode(*p,0)==123 && p->shape.nodes[0]!=123);
 assert(m.writeByte(p->word(0x5c),0));assert(animationOutputByte(*p,false,0)==0);
 // Source texture updates touch selector,+4,+8 only. Cache padding, scale and
 // rotation survive same-key sampling; consumers see live VM writes immediately.
 assert(m.writeByte(tex+1,0xa5) && m.writeByte(tex+2,0x5a) && m.writeByte(tex+3,0x3c));real(m,tex+12,1.25f);real(m,tex+16,.75f);real(m,tex+20,8);
 assert(pool->main(4,{117,117}) && byte(m,tex+1)==0xa5 && byte(m,tex+2)==0x5a && byte(m,tex+3)==0x3c);
 const auto transform=animationOutputTransform(*p,0);assert(transform && (*transform)[2]==1.25f && (*transform)[3]==.75f && (*transform)[4]==8);
 assert(pool->main(4,{167,167}) && byte(m,tex+1)==0xa5 && byte(m,tex+2)==0x5a && byte(m,tex+3)==0x3c);
 assert(real(m,tex+12)==1.25f && real(m,tex+16)==.75f && real(m,tex+20)==8);
 // A held traversal captures its current node, while the following child sees
 // the current visibility bytes rather than the earlier decoded shape snapshot.
 const auto& raw=*p->resource->draw;const uint32_t rootGroup=raw.groups()-1;const auto rootNode=raw.group(rootGroup);
 for(uint32_t i=0;i<raw.word(0x13c);++i)assert(m.writeByte(p->word(0x5c)+i,0));p->shape.visibility[rootNode.visibility]=1;
 BattlePoseInnerContext context;context.handle=4;context.resource=p->resource;context.model={1,0,0,0,0,1,0,0,0,0,1,0};
 unsigned renders=0;AnimationDrawTraversalServices services;services.user=&renders;services.render=[](void* u,const AnimationShapeRequest&,uint32_t&){++*static_cast<unsigned*>(u);return true;};
 AnimationDrawTraversalCursor traversal;assert(resumeAnimationDrawTraversal(*pool,context,services,traversal) && renders==0);
 // Resume an already captured node matrix child without re-reading P64.
 assert(pool->selectName(4,"PKR_W_1",true,{}) && pool->main(4,{117,117}));
 struct Held {unsigned calls=0;bool hold=true;} held;
 services.user=&held;services.matrix=[](void* u,BattleAudienceMatrixRequest& r){auto& h=*static_cast<Held*>(u);++h.calls;if(h.hold)return false;BattleAudienceAuxServices s;bindAudienceAuxMathAdapter(s);return s.matrix(nullptr,r);};
 services.render=[](void*,const AnimationShapeRequest&,uint32_t& pending){pending=0xdead1234;return false;};traversal={};
 assert(!resumeAnimationDrawTraversal(*pool,context,services,traversal) && traversal.frames.back().node.active);
 const uint32_t savedNode=p->word(0x64);p->word(0x64,0);held.hold=false;
 assert(!resumeAnimationDrawTraversal(*pool,context,services,traversal) && traversal.pending==0xdead1234 && held.calls>1);p->word(0x64,savedNode);
 // Large signed previous indices safely choose the original rebuild branch.
 p->word(0x3c,0x7fffffff);assert(pool->main(4,{117,117}) && p->word(0x3c)<word(m,t+8));
 // The raw loaded timeline, not converted controls, drives time publications.
 const uint32_t loop=word(m,control),oldEnd=word(m,control+8);assert(m.writeWord(control,0));real(m,control+8,5);
 assert(pool->selectName(4,"PKR_W_1",true,{}) && pool->main(4,{500,500}) && p->scalar(0x24)==5);
 assert(m.writeWord(control,loop) && m.writeWord(control+8,oldEnd));
 assert(pool->selectName(4,"PKR_W_1",true,{}) && pool->main(4,{117,117}));
 // Extend the actual authored loop tail to enter current>next: continuous
 // outputs survive even when the cumulative caches rebuild through the end.
 const uint32_t nk=word(m,t+8);const float lastTime=real(m,keys+44*(nk-1));real(m,control+8,lastTime+10);
 const uint64_t seamMs=uint64_t(std::ceil(double(lastTime)*1000.0/60.0))+17;
 real(m,posOut,101);real(m,p->word(0x54),103);real(m,nodeOut,102);p->word(0x3c,0xffffffff);
 assert(pool->main(4,{seamMs,seamMs}) && p->word(0x28)==nk-1 && p->word(0x2c)==0);
 assert(real(m,posOut)==101 && real(m,p->word(0x54))==103 && real(m,nodeOut)==102);
 assert(m.writeWord(control+8,oldEnd) && pool->selectName(4,"PKR_W_1",true,{}) && pool->main(4,{117,117}));
 // Live cubic tangent changes reach BE node OUTPUT, not just a detached sample.
 bool cubic=false;const uint32_t next=p->word(0x2c),key=keys+44*next,n=word(m,key+0x28),first=word(m,key+0x24),stream=word(m,t+0x3c);uint32_t target=0;
 for(uint32_t i=0;i<n;++i){const uint32_t r=stream+4*(first+i);target+=byte(m,r);const int a=int8_t(byte(m,r+2)),b=int8_t(byte(m,r+3));if(b<=-89 || b>=89 || a<-89 || a>88)continue;
  const uint32_t address=0x803d9580+4*uint32_t(a+89),saved=word(m,address);const float before=real(m,p->word(0x64)+4*target);real(m,address,real(m,address)+3);
  assert(pool->main(4,{117,117}) && real(m,p->word(0x64)+4*target)!=before);assert(m.writeWord(address,saved) && pool->main(4,{117,117}));close(real(m,p->word(0x64)+4*target),before);cubic=true;break;
 }
 assert(cubic);
 // File ownership alone is insufficient when actual VM backing disappears.
 const uint32_t work=word(m,0x80415dc0),auxEntry=word(m,work+0x100),aux=word(m,word(m,auxEntry+0xa0));
 const uint32_t base=word(m,model+0x154),oldCache=word(m,posCache);assert(files->ownsPayload(aux,12*uint32_t(p->shape.positions.size())) && m.unregisterExternalRam(aux));
 assert(m.writeWord(model+0x154,aux));p->word(0x3c,0xffffffff);
 assert(!pool->main(4,{117,117}) && word(m,posCache)==oldCache);assert(m.writeWord(model+0x154,base) && pool->main(4,{117,117}));
 // Layout admission holds without touching cache/output or silently using the
 // converted vector fallback. A later restoration continues the same source RAM.
 const uint32_t savedOut=p->word(0x4c);p->word(0x4c,p->word(0x48));const uint32_t before=word(m,p->word(0x48));assert(!pool->main(4,{117,117}) && !animationOutputVector(*p,false,0) && word(m,p->word(0x48))==before);p->word(0x4c,savedOut);
 // Dropping a standalone context must freeze while its last shared owner is
 // already destructing; weak_ptr::lock cannot be used by that destructor.
 const auto originalProvider=p->sourceBuffers;
 auto separate=AnimationScratchMemory::bind(pool,files,m,*process->animationMemory());assert(separate);
 p->sourceBuffers=separate->pose(4,p->nativeGeneration);auto expired=p->sourceBuffers;real(m,posOut,89);separate.reset();
 assert(!p->sourceBuffers && p->shape.positions[0][0]==89 && !expired->node(*p,0));p->sourceBuffers=originalProvider;
 // Failed retirement leaves a dead view. A later unrelated context cannot
 // adopt it implicitly, even if the allocator reuses the old owner's address.
 separate=AnimationScratchMemory::bind(pool,files,m,*process->animationMemory());p->sourceBuffers=separate->pose(4,p->nativeGeneration);expired=p->sourceBuffers;
 p->word(0x64,0);separate.reset();p->word(0x64,savedNode);assert(p->sourceBuffers==expired && !expired->node(*p,0));
 for(unsigned i=0;i<16;++i){auto nextOwner=AnimationScratchMemory::bind(pool,files,m,*process->animationMemory());assert(nextOwner);nextOwner.reset();assert(p->sourceBuffers==expired && !expired->node(*p,0));}
 p->sourceBuffers=originalProvider;
 // Copy freezes the current source cache and output; its tangent/state/resources
 // are independent of later VM writes. Retirement freezes every valid pose.
 real(m,posOut,91);BattlePosePool copy(*pool);auto* q=copy.pose(4);assert(q && !q->sourceBuffers && q->shape.positions[0][0]==91);real(m,posOut,92);assert(q->shape.positions[0][0]==91 && animationOutputVector(*p,false,0)->at(0)==92);
 auto stale=p->sourceBuffers;process.reset();assert(!p->sourceBuffers && p->shape.positions[0][0]==92 && !stale->node(*p,0));
 assert(pool->main(4,{117,117}) && copy.main(4,{117,117}));
}
int main(){runAnimationScratchMemoryFixture("romfs");}
