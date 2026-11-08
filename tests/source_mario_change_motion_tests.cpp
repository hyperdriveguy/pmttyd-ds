#include "source_mario_change_motion.hpp"
#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t MP=0x8041e900,A=0x6000,B=0x7000,T=0x80310030;
struct World {
 std::shared_ptr<const void> life=std::make_shared<int>(1);std::map<uint32_t,uint32_t>w;std::map<uint32_t,uint16_t>h;
 std::vector<std::string>trace;size_t index=0,stall=~size_t(0);int waits=0;bool alive=true,late=false,swap=false,reenter=false;
 SourceMarioChangeMotion*active=nullptr;SourceMarioStayPost*stay=nullptr;
 World(){w[MP]=A;w[A]=0;w[A+4]=0x08000004;w[A+0xc]=8;h[A+0x2e]=0;h[A+0x30]=0;w[A+0x27c]=w[A+0x280]=0xffffffff;w[B+0x27c]=w[B+0x280]=0xffffffff;for(unsigned i=0;i<32;++i)w[T+12*i+8]=i==0?0x800a6b10:0;}
 bool admit(){if(reenter){reenter=false;if(active){assert(!active->reset());assert(!active->resume(1,services()));}if(stay){assert(!stay->reset());assert(!stay->resume(services()));}}if(index==stall&&waits++<2)return false;++index;return true;}
 static bool current(void*p){return static_cast<World*>(p)->alive;}
 static std::optional<uint32_t>word(void*p,uint32_t a){auto&x=*static_cast<World*>(p);if(!x.admit())return {};if(!x.w.count(a))return {};auto v=x.w[a];x.trace.push_back("R"+std::to_string(a)+":"+std::to_string(v));return v;}
 static std::optional<uint16_t>half(void*p,uint32_t a){auto&x=*static_cast<World*>(p);if(!x.admit())return {};if(!x.h.count(a))return {};auto v=x.h[a];x.trace.push_back("H"+std::to_string(a)+":"+std::to_string(v));return v;}
 static bool store(void*p,uint32_t a,uint32_t v){auto&x=*static_cast<World*>(p);if(!x.admit())return false;x.w[a]=v;x.trace.push_back("W"+std::to_string(a)+":"+std::to_string(v));if(x.late&&a==A+4&&!(v&0x08000000))x.w[a]|=0x40000000;return true;}
 static bool storeHalf(void*p,uint32_t a,uint16_t v){auto&x=*static_cast<World*>(p);if(!x.admit())return false;x.h[a]=v;x.trace.push_back("S"+std::to_string(a)+":"+std::to_string(v));if(x.late&&a==A+0x2e)x.h[A+0x30]=3;return true;}
 static bool call(void*p,const SourceMarioMotionCall&q){auto&x=*static_cast<World*>(p);if(!x.admit())return false;x.trace.push_back("C"+std::to_string(q.function)+":"+std::to_string(q.callsite)+":"+std::to_string(q.argument));if(x.swap){x.w[MP]=B;x.w[B+0x280]=88;}x.w[A+0xc]|=0x80;return true;}
 SourceMarioMotionServices services(){return {life,this,current,word,half,store,storeHalf,call};}
};
void refMotion(World&x,uint32_t requested){auto p=*World::word(&x,MP);if(*World::word(&x,p)&0x10000000)return;auto old=*World::half(&x,p+0x30);if(old==16||old==17){auto f=*World::word(&x,p+4);if(f&0x08000000)World::store(&x,p+4,f&~0x08000000);f=*World::word(&x,p+4);if(f&4)World::store(&x,p+4,f&~4u);}auto mot=*World::half(&x,p+0x2e);World::storeHalf(&x,p+0x30,mot);World::storeHalf(&x,p+0x2e,uint16_t(requested));auto entry=T+12u**World::half(&x,p+0x30);auto fn=*World::word(&x,entry+8);if(fn)World::call(&x,{fn,0x800945f4,entry});auto f=*World::word(&x,p+0xc);World::store(&x,p+0xc,f|1);}
void refStay(World&x){auto p=*World::word(&x,MP);auto m=*World::half(&x,p+0x2e);bool special=m==19||m==20;for(unsigned o:{0x27cu,0x280u}){if(special&&o==0x280)break;p=*World::word(&x,MP);auto h=*World::word(&x,p+o);if(h!=0xffffffff){World::call(&x,{0x800d95f8,special?0x800a6b9cu:o==0x27c?0x800a6b50u:0x800a6b74u,h});World::store(&x,p+o,0xffffffff);}}}
void parity(){for(unsigned previous:{0u,16u,17u,18u})for(unsigned motion:{0u,1u,2u,19u,20u})for(unsigned requested:{0u,1u,2u,0x12345u})for(bool late:{false,true}){
 World r;r.h[A+0x30]=previous;r.h[A+0x2e]=motion;r.late=late;refMotion(r,requested);for(size_t wait=0;wait<=r.index;++wait){World w;w.h[A+0x30]=previous;w.h[A+0x2e]=motion;w.late=late;w.stall=wait;SourceMarioChangeMotion c;w.active=&c;w.reenter=true;unsigned n=0;while(!c.resume(requested,w.services()))assert(++n<10);assert(w.trace==r.trace&&w.w==r.w&&w.h==r.h);auto count=w.index;assert(c.resume(requested,w.services())&&w.index==count);assert(c.reset());}}
 for(unsigned motion:{0u,1u,19u,20u})for(uint32_t handle:{0xffffffffu,0u,42u})for(bool swap:{false,true}){World r;r.h[A+0x2e]=motion;r.w[A+0x27c]=r.w[A+0x280]=handle;r.swap=swap;refStay(r);for(size_t wait=0;wait<=r.index;++wait){World w;w.h[A+0x2e]=motion;w.w[A+0x27c]=w.w[A+0x280]=handle;w.swap=swap;w.stall=wait;SourceMarioStayPost c;w.stay=&c;w.reenter=true;unsigned n=0;while(!c.resume(w.services()))assert(++n<10);assert(w.trace==r.trace&&w.w==r.w&&w.h==r.h);assert(c.reset());}}
}
void admission(){World w;w.w[A]=0x10000000;auto s=w.services();s.half=nullptr;s.storeWord=nullptr;s.storeHalf=nullptr;s.call=nullptr;SourceMarioChangeMotion no;assert(no.resume(0,s));assert(w.trace.size()==2);
 World h;h.stall=3;SourceMarioChangeMotion c;auto services=h.services();assert(!c.resume(1,services));auto count=h.index;assert(!c.resume(2,services)&&h.index==count);auto replaced=services;replaced.call=nullptr;assert(!c.resume(1,replaced)&&h.index==count);int borrowed=0;replaced=services;replaced.lifetime=std::shared_ptr<const void>(services.lifetime.get(),[&](const void*){++borrowed;});assert(!c.resume(1,replaced)&&h.index==count);h.alive=false;assert(!c.resume(1,services));h.alive=true;assert(!c.resume(1,services)&&h.index==count);assert(!c.reset());
 World p;auto ps=p.services();ps.call=nullptr;SourceMarioChangeMotion post;assert(!post.resume(1,ps)&&post.pending()==0x800a6b10);assert(p.h[A+0x2e]==1&&p.w[A+0xc]==8);
 World q;q.w[A+0x27c]=12;auto qs=q.services();qs.call=nullptr;SourceMarioStayPost stay;assert(!stay.resume(qs)&&stay.pending()==0x800d95f8&&q.w[A+0x27c]==12);
}
void realTableComposition(){World w;w.w[A+0x27c]=w.w[A+0x280]=0xffffffff;SourceMarioChangeMotion parent;SourceMarioStayPost child;
 struct Context {World*w;SourceMarioStayPost*child;};Context context{&w,&child};auto s=w.services();
 // Native TEST composition routes the actual table child to its whole owner;
 // all guest state and table operands remain the real semantic fixture above.
 struct Bridge{static bool current(void*p){return static_cast<Context*>(p)->w->alive;}static std::optional<uint32_t>word(void*p,uint32_t a){return World::word(static_cast<Context*>(p)->w,a);}static std::optional<uint16_t>half(void*p,uint32_t a){return World::half(static_cast<Context*>(p)->w,a);}static bool store(void*p,uint32_t a,uint32_t v){return World::store(static_cast<Context*>(p)->w,a,v);}static bool storeHalf(void*p,uint32_t a,uint16_t v){return World::storeHalf(static_cast<Context*>(p)->w,a,v);}static bool call(void*p,const SourceMarioMotionCall&q){auto&x=*static_cast<Context*>(p);assert(q.function==0x800a6b10&&q.argument==T);return x.child->resume(x.w->services());}};
 s.user=&context;s.current=Bridge::current;s.word=Bridge::word;s.half=Bridge::half;s.storeWord=Bridge::store;s.storeHalf=Bridge::storeHalf;s.call=Bridge::call;
 assert(parent.resume(1,s)&&child.complete());assert(w.h[A+0x30]==0&&w.h[A+0x2e]==1&&w.w[A+0xc]==9);
 const auto reference=w.trace;const auto count=w.index;
 for(size_t wait=0;wait<=count;++wait){World held;held.stall=wait;SourceMarioChangeMotion outer;SourceMarioStayPost inner;Context ctx{&held,&inner};auto bound=s;bound.user=&ctx;bound.lifetime=held.life;unsigned tries=0;while(!outer.resume(1,bound))assert(++tries<10);assert(inner.complete()&&held.trace==reference);}

}
}
int main(){parity();admission();realTableComposition();std::cout<<"Whole ChgMot/StayPost differential traces/every wait twice, fresh table/flags, typed Off and identity/reentry pass\n";return 0;}
